#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include <math.h>
#include "j1EntityManager.h"
#include "j1Entity.h"
#include "StaticEntity.h"
#include "brofiler/Brofiler/Brofiler.h"
#include "p2SString.h"
#include "AI_Manager.h"
#include "GenericPlayer.h"
#include "j1Player.h"
#include "AI_Player.h"
#include "j1Scene.h"
#include "j1Pathfinding.h"
#include "FoWManager.h"

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name = ("map");
	spawnCoordinates = 0;
	data.type = MAPTYPE_ISOMETRIC;
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder = (config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	BROFILER_CATEGORY("MapDraw", Profiler::Color::MediumPurple)
	if(map_loaded == false)
		return;

	for(int l = 0; l < MAX_LAYERS; l++)
	{
		MapLayer* layer = &data.layers[l];

		if ((layer->properties.Get("Nodraw") != 0)&&(!App->render->debug))
			continue;

		for(int y = 0; y < data.height; ++y)
		{
			for(int x = 0; x < data.width; ++x)
			{
				int tile_id = layer->Get(x, y);
				if(tile_id > 0)
				{
					iPoint pos = MapToWorld(x, y);

					TileSet* tileset = GetTilesetFromTileId(tile_id);
					SDL_Rect r = tileset->GetTileRect(tile_id);

					//Only render if there is no fog of war at these coordinates
					//if ((App->fowManager->GetFoWTileState({ x, y })->tileFogBits != fow_ALL)||(!App->render->fog_of_war)) {
						//camera culling
						if ((pos.x + r.w + tileset->offset_x > -(App->render->camera.x))
							&& (pos.x < -App->render->camera.x + App->render->camera.w)
							&& (pos.y + r.h > -(App->render->camera.y))
							&& (pos.y + tileset->offset_y < (-App->render->camera.y + App->render->camera.h)))
						{

							App->render->Blit(tileset->texture, pos.x + tileset->offset_x, pos.y + tileset->offset_y, &r);
						}
					//}
				}
			}
		}
	}
}

int Properties::Get(const char* value, int default_value) const
{
	p2List_item<Property*>* item = list.start;

	while(item)
	{
		if(item->data->name == value)
			return item->data->value;
		item = item->next;
	}

	return default_value;
}

TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	TileSet* set = (TileSet*)&data.tilesets[0];
	for (int i = 0; i < MAX_TILESETS; i++){
		set = (TileSet*)&data.tilesets[i];
		if (id < data.tilesets[i + 1].firstgid)
		{
			break;
		}
	}
	return set;
}

iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	if(data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * data.tile_width;
		ret.y = y * data.tile_height;
	}
	else if(data.type == MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - y) * (data.tile_width * 0.5f);
		ret.y = (x + y) * (data.tile_height * 0.5f);
	}
	else
	{
//		LOG("Unknown map type");
		ret.x = x; 
		ret.y = y;
	}

	return ret;
}

fPoint j1Map::fMapToWorld(int x, int y) const
{
	fPoint ret;

	ret.x = (x - y) * (64 * 0.5f);
	ret.y = (x + y) * (32 * 0.5f);

	return ret;
}

iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint ret(0,0);

	if(data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;
	}
	else if(data.type == MAPTYPE_ISOMETRIC)
	{

		float half_width = data.tile_width * 0.5f;
		float half_height = data.tile_height * 0.5f;
		ret.x = int( (x / half_width + y / half_height) / 2) - 1;
		ret.y = int( (y / half_height - (x / half_width)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

iPoint j1Map::fWorldToMap(float x, float y) const
{
	iPoint ret(0, 0);

	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{

		float half_width = data.tile_width * 0.5f;
		float half_height = data.tile_height * 0.5f;
		ret.x = int((x / half_width + y / half_height) / 2) - 1;
		ret.y = int((y / half_height - (x / half_width)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

iPoint j1Map::IsometricWorldToMap(int x, int y) const {
	iPoint map_position;
	map_position.x = x / (data.tile_width * 0.5f);
	map_position.y = y / data.tile_height;
	return map_position;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

// Called before quitting
bool j1Map::CleanUp()
{
	//TODO
	LOG("Unloading map");

	//remove all tilesets
	for (int i = 0; i < MAX_TILESETS; i++)
	{
		App->tex->UnLoad(data.tilesets[i].texture);
	}

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load map modules
bool j1Map::Load(std::string modules[4])
{
	bool ret = true;
	for (int i = 0; i < 4; i++) {
		std::string tmp = folder;
		tmp.append(modules[i].c_str());

		pugi::xml_parse_result result = map_file.load_file(tmp.c_str());

		if(result == NULL)
		{
			LOG("Could not load map xml file %s. pugi error: %s", modules[i].c_str(), result.description());
			ret = false;
		}

		//Tilesets only load from first map
		if (i == 0) {
			// Load general info ----------------------------------------------
			if (ret == true)
			{
				ret = LoadMap();
			}

			// Load all tilesets info ----------------------------------------------
			pugi::xml_node tileset = map_file.child("map").child("tileset");

			for (int t = 0; t < MAX_TILESETS && ret; t++)
			{
				TileSet* set = new TileSet();
				if (ret == true)
				{
					ret = LoadTilesetDetails(tileset, set);
					if (ret == true) {
						data.tilesets[t] = *set;
					}
				}

				if (ret == true)
				{
					ret = LoadTilesetImage(tileset, set);
					if (ret == true) {
						data.tilesets[t] = *set;
					}
				}

				tileset = tileset.next_sibling("tileset");
			}

		}

		// Load layer info ----------------------------------------------
		pugi::xml_node layer = map_file.child("map").child("layer");
		for (int l = 0; l < MAX_LAYERS && ret; l++)
		{
			ret = LoadLayer(layer, &data.layers[l], i);
			layer = layer.next_sibling("layer");
		}

				//Load objectgroup info -------------------------------------

		pugi::xml_node objectgroup;
		objectgroup = map_file.child("map").child("objectgroup");
		{
			if (ret == true)
			{
				ret = LoadObjectGroup(objectgroup,data.objectgroup, i);
			}
		}

		/*
		if (ret == true)
		{
			LOG("Successfully parsed map XML file: %s", modules[i].c_str());
			LOG("width: %d height: %d", data.width, data.height);
			LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

			if (i == 0) {
				for (int t = 0; t < MAX_TILESETS; t++)
				{
					LOG("Tileset ----");
					LOG("name: %s firstgid: %d", data.tilesets[t].name.c_str(), data.tilesets[t].firstgid);
					LOG("tile width: %d tile height: %d", data.tilesets[t].tile_width, data.tilesets[t].tile_height);
					LOG("spacing: %d margin: %d", data.tilesets[t].spacing, data.tilesets[t].margin);
				}
			}

			for (int l = 0; l < MAX_LAYERS; l++)
			{
				LOG("Layer ----");
				LOG("name: %s", data.layers[l].name.c_str());
				LOG("tile width: %d tile height: %d", data.layers[l].width, data.layers[l].height);
			}
			

			LOG("ObjectGroup ----");
			LOG("name: %s", data.objectgroup.name.c_str());
		}
		*/
	}
	map_loaded = ret;
	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int() * 2;
		data.height = map.attribute("height").as_int() * 2;
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		std::string orientation = std::string(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name = std::string(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.c_str(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer, int module_number)
{
	bool ret = true;

	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int() * 2;
	layer->height = node.attribute("height").as_int() * 2;
	LoadProperties(node, layer->properties);
	pugi::xml_node layer_data = node.child("data");

	if(layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
	}

		int first_tile[4];

		first_tile[0] = 0;
		first_tile[1] = 75;
		first_tile[2] = 11250;
		first_tile[3] = 11325;

		int i = first_tile[module_number];
		int iterations = 0;

		for(pugi::xml_node tile = layer_data.child("tile"); tile; tile = tile.next_sibling("tile"))
		{
			layer->data[i] = tile.attribute("gid").as_int(0);
			i++;
			iterations++;
			if (iterations == MODULE_LENGTH) {
				i += MODULE_LENGTH;
				iterations = 0;
			}
		}

	return ret;
}

bool j1Map::LoadObjectGroup(pugi::xml_node& node, ObjectGroup objectgroup, int module_number) {
	bool ret = true;
	objectgroup.name = node.attribute("name").as_string();
	pugi::xml_node object_node = node.child("object");
	SDL_Rect rect = { 0,0,0,0 };

	iPoint offset[4];
	offset[0] = { 0,0 };
	offset[1] = { MODULE_LENGTH * HALF_TILE,  0 };
	offset[2] = { 0, MODULE_LENGTH * HALF_TILE };
	offset[3] = { MODULE_LENGTH * HALF_TILE, MODULE_LENGTH * HALF_TILE };

	if (object_node == NULL)
	{
		LOG("Error loading object group");
		ret = false;
	}

	else
	{
		while (object_node != nullptr)
		{
			std::string object_name = std::string(object_node.attribute("name").as_string());
			pugi::xml_node properties = object_node.child("properties");
			pugi::xml_node property = properties.child("property");

			int x = object_node.attribute("x").as_int() + offset[module_number].x;
			int y = object_node.attribute("y").as_int() + offset[module_number].y;
			int width = object_node.attribute("width").as_int() / (HALF_TILE)+1;
			int height = object_node.attribute("height").as_int() / (HALF_TILE)+1;
			iPoint first_tile_position = { x,y };

			if (object_name == "Resources") {
				ResourceBuilding* resource_building = new ResourceBuilding();

				while (property != nullptr)
				{
					std::string property_name = std::string(property.attribute("name").as_string());

					if (property_name == "Nuka-Cola") {
						resource_building->resource_type = Resource::CAPS;
						resource_building->quantity = property.attribute("value").as_int();
					}
					else if (property_name == "Water") {
						resource_building->resource_type = Resource::WATER;
						resource_building->quantity = property.attribute("value").as_int();
					}
					property = property.next_sibling();
				}
				resource_building->tiles = CalculateArea(first_tile_position, width, height);
				App->entities->resource_buildings.push_back(resource_building);
			}
			else if (object_name == "Static") {
				//create building
				StaticEntity* static_entity;
				iPoint size;

				//Adjust coordinates to tiles
				x /= HALF_TILE;
				y /= HALF_TILE;

				int faction_number = (object_node.child("properties").child("property").attribute("value").as_int());
				Faction building_faction = NO_FACTION;

				building_faction = App->entities->FactionByIndex(App->entities->randomFaction[faction_number]);

				//add tiles and adjust render texture position
				std::string object_type = std::string(object_node.attribute("type").as_string());
				EntityType type = NO_TYPE;
				EntityType dynamic_type = NO_TYPE;

				if (object_type == "Base") {
					type = BASE;
					dynamic_type = GATHERER;
					if (building_faction == GHOUL) {
						x += 3;
						y += 2;
					}else if (building_faction == VAULT) {
						x += 4;
						y += 3;
					}else if (building_faction == MUTANT) {
						x += 3;
						y += 2;
					}else if (building_faction == BROTHERHOOD) {
						x += 3;
						y += 2;
					}
				}
				else if (object_type == "Barrack") {
					type = BARRACK;
					dynamic_type = MELEE;
					if (building_faction == GHOUL) {
						x += 2;
						y += 1;
					}else if (building_faction == VAULT) {
						x += 3;
						y += 3;
					}else if (building_faction == MUTANT) {
						x += 2;
						y += 1;
					}else if (building_faction == BROTHERHOOD) {
						x += 1;
						y += 1;
					}
				}
				else if (object_type == "Laboratory") {
					type = LABORATORY;
					dynamic_type = RANGED;
					if (building_faction == GHOUL) {
						x += 2;
					}else if (building_faction == VAULT) {
						x += 5;
						y += 4;
					}else if (building_faction == MUTANT) {
						x += 3;
						y += 1;
					}else if (building_faction == BROTHERHOOD) {
						x += 2;
						y += 1;
					}
				}

				//create building
				static_entity = (StaticEntity*)App->entities->CreateEntity(building_faction, type, x,y, App->scene->players[building_faction]);
				static_entity->tiles = CalculateArea(first_tile_position, width, height);

				App->entities->CreateEntity(building_faction, dynamic_type, x + 1, y + 1, App->scene->players[building_faction]);

			}
			else if (object_name == "Animal") {
				iPoint position;
				std::string object_type = std::string(object_node.attribute("type").as_string());
				position = App->map->IsometricWorldToMap(x, y);
				if (object_type == "Braham") App->entities->CreateEntity(NO_FACTION, BRAHAM, position.x, position.y);
				else if (object_type == "Bighorner") App->entities->CreateEntity(NO_FACTION, BIGHORNER, position.x, position.y);
				else if (object_type == "SpawnPoint")
				{
					spawnPoint[spawnCoordinates].x = object_node.attribute("x").as_int() + offset[module_number].x;
					spawnPoint[spawnCoordinates].y = object_node.attribute("y").as_int() + offset[module_number].y;
					spawnPoint[spawnCoordinates] = App->map->IsometricWorldToMap(spawnPoint[spawnCoordinates].x, spawnPoint[spawnCoordinates].y);
					LOG("spawnCoordinates = %i / SpawnPoints x = %i y = %i", spawnCoordinates, spawnPoint[spawnCoordinates].x, spawnPoint[spawnCoordinates].y);
					spawnCoordinates++;
				}
			}

			object_node = object_node.next_sibling();
		}
	}
	return ret;
}

// Load a group of properties from a node and fill a list with it
bool j1Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = false;

	pugi::xml_node data = node.child("properties");

	if(data != NULL)
	{
		pugi::xml_node prop;

		for(prop = data.child("property"); prop; prop = prop.next_sibling("property"))
		{
			Properties::Property* p = new Properties::Property();

			p->name = prop.attribute("name").as_string();
			p->value = prop.attribute("value").as_int();

			properties.list.add(p);
		}
	}

	return ret;
}

bool j1Map::CreateWalkabilityMap() const
{
	bool ret = false;

		MapLayer* layer = (MapLayer*)&data.layers[MAX_LAYERS -1];

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int i = (y * layer->width) + x;

				int tile_id = layer->Get(x, y);

				if (tile_id != 0)
				{
					App->pathfinding->SetTileAsUnwalkable(x, y);
				}
			}
		}

	ret = true;

	return ret;
}

std::vector<iPoint> j1Map::CalculateArea(iPoint first_tile_position, int width, int height) {
	bool ret = true;
	std::vector<iPoint> area;

	first_tile_position = IsometricWorldToMap(first_tile_position.x, first_tile_position.y);

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			//get tile position
			iPoint tile_position = { first_tile_position.x + i,first_tile_position.y + j };
			area.push_back(tile_position);

			//set tile as unwalkable
			App->pathfinding->SetTileAsUnwalkable(tile_position.x, tile_position.y);
		}
	}

	return area;
}
