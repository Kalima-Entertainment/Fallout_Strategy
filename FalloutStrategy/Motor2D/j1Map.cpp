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

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name = ("map");
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
	int tile_margin = 3;
	p2List_item<MapLayer*>* item = data.layers.start;

	for (; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		if (layer->properties.Get("Nodraw") != 0)
			continue;

		int total_tiles = 0;
		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int tile_id = layer->Get(x, y);
				if (tile_id > 0)
				{
					TileSet* tileset = GetTilesetFromTileId(tile_id);

					SDL_Rect r = tileset->GetTileRect(tile_id);
					iPoint pos = MapToWorld(x, y);
					//camera culling
					/*if ((pos.x - tileset->offset_x > -(App->render->camera.x + tile_margin * data.tile_width)) && (pos.x < -App->render->camera.x + App->render->camera.w)
						&& (pos.y > -(App->render->camera.y + data.tile_height)) && (pos.y + tileset->offset_y < (-App->render->camera.y + App->render->camera.h + tile_margin * data.tile_height)))
					{*/
					if (App->map->mapcounter == 0)
					{
						App->render->Blit(tileset->texture, pos.x + tileset->offset_x, pos.y + tileset->offset_y, &r);
					}
					if (App->map->mapcounter == 1)
					{
						nummodule = MapToWorld(75, 0);
						App->render->Blit(tileset->texture, pos.x + tileset->offset_x + nummodule.x, pos.y + tileset->offset_y + nummodule.y, &r);
	
					}
					if (App->map->mapcounter == 2)
					{
						nummodule = MapToWorld(-75, -53);
						App->render->Blit(tileset->texture, pos.x + tileset->offset_x + nummodule.x, pos.y + tileset->offset_y + nummodule.y, &r);
					}
					if (App->map->mapcounter >= 3)
					{
						nummodule = MapToWorld(-75, 106);
						App->render->Blit(tileset->texture, pos.x + tileset->offset_x + nummodule.x, pos.y + tileset->offset_y + nummodule.y, &r);
					}
						//total_tiles++;
					/*}*/
				}
			}
		}
		//LOG("Tiles drawn: %i", total_tiles);
		total_tiles = 0;

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
	p2List_item<TileSet*>* item = data.tilesets.start;
	TileSet* set = item->data;

	while(item)
	{
		if(id < item->data->firstgid)
		{
			set = item->prev->data;
			break;
		}
		set = item->data;
		item = item->next;
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
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
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
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// Remove all layers
	p2List_item<MapLayer*>* item2;
	item2 = data.layers.start;

	while(item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.clear();

	// Remove all objects

	p2List_item<ObjectGroup*>* item3;
	item3 = data.objectgroups.start;
	while (item3 != NULL)
	{
		//LOG("Objectgroups releasing");
		RELEASE(item3->data);
		item3 = item3->next;
	}
	data.objectgroups.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const std::string file_name)
{
	bool ret = true;
	std::string tmp = folder;
	tmp.append(file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.c_str());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for(layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if (ret == true) {
			data.layers.add(lay);
		}
	}

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.c_str(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		p2List_item<MapLayer*>* item_layer = data.layers.start;
		while(item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.c_str());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}

		p2List_item<ObjectGroup*>* item_object = data.objectgroups.start;
		while (item_object != NULL)
		{
			ObjectGroup* o = item_object->data;
			LOG("ObjectGroup ----");
			LOG("name: %s", o->name.c_str());
			item_object = item_object->next;
		}
	}

	//Load objectgroup info -------------------------------------

	pugi::xml_node objectgroup;
	for (objectgroup = map_file.child("map").child("objectgroup"); objectgroup && ret; objectgroup = objectgroup.next_sibling("objectgroup"))
	{
		ObjectGroup* set = new ObjectGroup();

		if (ret == true)
		{
			ret = LoadObjectGroup(objectgroup, set);
		}
		data.objectgroups.add(set);
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
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
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

bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	LoadProperties(node, layer->properties);
	pugi::xml_node layer_data = node.child("data");

	if(layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
		RELEASE(layer);
	}
	else
	{
		layer->data = new uint[layer->width*layer->height];
		memset(layer->data, 0, layer->width*layer->height);

		int i = 0;
		for(pugi::xml_node tile = layer_data.child("tile"); tile; tile = tile.next_sibling("tile"))
		{
			layer->data[i++] = tile.attribute("gid").as_int(0);
		}
	}

	return ret;
}

bool j1Map::LoadObjectGroup(pugi::xml_node& node, ObjectGroup* objectgroup) {
	bool ret = true;
	pugi::xml_node object_node = node.child("object");
	SDL_Rect rect = { 0,0,0,0 };
	objectgroup->name = node.attribute("name").as_string();
	uint i = 0u;

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

			int x = object_node.attribute("x").as_int();
			int y = object_node.attribute("y").as_int();
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

				//add tiles
				std::string object_type = std::string(object_node.attribute("type").as_string());
				EntityType type = NO_TYPE;

				if (object_type == "Base") {
					type = BASE;
					x -= 10;
					y -= 5;
				}
				else if (object_type == "Barrack") {
					type = BARRACK;
					x -= 12;
					y -= 6;
				}
				else if (object_type == "Laboratory") {
					type = LABORATORY;
				}

				static_entity = (StaticEntity*)App->entities->CreateEntity(GHOUL, type, x,y);
				static_entity->tiles = CalculateArea(first_tile_position, width, height);
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

bool j1Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
	bool ret = false;
	p2List_item<MapLayer*>* item;
	item = data.layers.start;

	for(item = data.layers.start; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		if(layer->properties.Get("Navigation", 0) == 0)
			continue;

		uchar* map = new uchar[layer->width*layer->height];
		memset(map, 1, layer->width*layer->height);

		for(int y = 0; y < data.height; ++y)
		{
			for(int x = 0; x < data.width; ++x)
			{
				int i = (y*layer->width) + x;

				int tile_id = layer->Get(x, y);
				TileSet* tileset = (tile_id > 0) ? GetTilesetFromTileId(tile_id) : NULL;
				
				if(tileset != NULL)
				{
					map[i] = (tile_id - tileset->firstgid) > 0 ? 0 : 1;
					/*TileType* ts = tileset->GetTileType(tile_id);
					if(ts != NULL)
					{
						map[i] = ts->properties.Get("walkable", 1);
					}*/
				}
			}
		}

		*buffer = map;
		width = data.width;
		height = data.height;
		ret = true;

		break;
	}
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
			iPoint tile_position = { first_tile_position.x + i,first_tile_position.y + j };
			area.push_back(tile_position);
		}
	}

	return area;
}