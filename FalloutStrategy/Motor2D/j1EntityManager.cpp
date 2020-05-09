#include "j1EntityManager.h"
#include "j1App.h"
#include "p2Log.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Collision.h"
#include "j1Window.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Audio.h"
#include "DynamicEntity.h"
#include "StaticEntity.h"
#include "j1Player.h"
#include "brofiler/Brofiler/Brofiler.h"
#include "MenuManager.h"
#include "j1Console.h"
#include "AI_Manager.h"
#include "AI_Player.h"
#include "j1Pathfinding.h"
#include "Troop.h"
#include "Animal.h"
#include "Gatherer.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

j1EntityManager::j1EntityManager(){
	name = ("entities");

	blocked_movement = false;	
	
	for (int faction = VAULT; faction < NO_FACTION; faction++)
	{
		for (int type = MELEE; type <= BASE; type++)
		{
			reference_entities[faction][type] = nullptr;
		}
	}
}

j1EntityManager::~j1EntityManager(){}

j1Entity* j1EntityManager::CreateEntity(Faction faction, EntityType type, int position_x, int position_y, GenericPlayer* owner){
	BROFILER_CATEGORY("EntityCreation", Profiler::Color::Linen)

	//if (!owner) owner = App->scene->players[faction];

	j1Entity* entity = nullptr;

	switch (type)
	{
	case MELEE:
		iPoint available_tile = FindSpawnPoint(position_x, position_y);
		entity = new Troop(MELEE, faction, available_tile, owner);
		if (owner) {
			owner->troops.push_back((Troop*)entity);
			owner->melees++;
		}
		break;
	case RANGED:
		iPoint available_tile = FindSpawnPoint(position_x, position_y);
		entity = new Troop(RANGED, faction, available_tile, owner);
		if (owner) {
			owner->troops.push_back((Troop*)entity);
			owner->rangeds++;
		}
		break;
	case GATHERER:
		iPoint available_tile = FindSpawnPoint(position_x, position_y);
		entity = new Gatherer(faction, available_tile, owner);
		if (owner) {
			owner->gatherers_vector.push_back((Gatherer*)entity);
			owner->gatherers++;
		}
		break;
	case BASE:
		entity = new StaticEntity(faction, BASE, { position_x, position_y }, owner);
		if (owner) {
			owner->base = (StaticEntity*)entity;
		}
		break;
	case LABORATORY:
		entity = new StaticEntity(faction, LABORATORY, { position_x, position_y }, owner);
		if (owner) {
			owner->laboratory = (StaticEntity*)entity;
		}
		break;
	case BARRACK:
		entity = new StaticEntity(faction, BIGHORNER, { position_x, position_y }, owner);
		if (owner) {
			if(!owner->barrack[0])
				owner->barrack[0] = (StaticEntity*)entity;
			else if (!owner->barrack[1])
				owner->barrack[1] = (StaticEntity*)entity;
		}
		break;
	case BIGHORNER:
		entity = new Animal(BIGHORNER, { position_x, position_y });
		break;
	case BRAHAM:
		entity = new Animal(BRAHAM, { position_x, position_y });
		break;
	case DEATHCLAW:
		break;
	case MR_HANDY:
		entity = new Troop(MR_HANDY, faction, { position_x, position_y }, owner);
		if (owner) {
			owner->troops.push_back((Troop*)entity);
		}
		break;
	default:
		break;
	}

	if ((type == MELEE) || (type == RANGED) || (type == GATHERER) || (type == BIGHORNER) || (type == BRAHAM) || (type == DEATHCLAW) || (type == MR_HANDY)) {

		//If there's another unit in that tile, we find a new spawn point
				
		entity = new DynamicEntity(faction, type, { position_x, position_y }, owner);
	
		if ((faction != ANIMALS) && (type != MR_HANDY))
			entity->reference_entity = reference_entities[faction][type];
		else if (type == BIGHORNER)
			entity->reference_entity = reference_bighroner;
		else if (type == BRAHAM)
			entity->reference_entity = reference_braham;
		else if (type == DEATHCLAW)
			entity->reference_entity = reference_deathclaw;
		else if (type == MR_HANDY)
			entity->reference_entity = reference_MrHandy;
		
		if (entity->reference_entity != nullptr){
			occupied_tiles[entity->current_tile.x][entity->current_tile.y] = true;
			entities.push_back(entity);
			entity->LoadReferenceData();
		}
	}
	else if ((type == BASE) || (type == LABORATORY) || (type == BARRACK))
	{
		entity = new StaticEntity(faction, type, {position_x, position_y},owner);
		entity->reference_entity = reference_entities[faction][type];
		
		if (entity != NULL)
		{
			if (entity->reference_entity != nullptr) {
				entities.push_back(entity);
				entity->LoadReferenceData();
			}

			//Render building
			entity->render_position = { (int)(entity->position.x - 0.5f * entity->sprite_size),(int)(entity->position.y - entity->sprite_size * 0.75) };

			//TODO: delete all this
			//Add spawn position for units
			if (faction == GHOUL) {
				if (type == BASE)
					entity->render_position += App->map->MapToWorld(20, 12);
				else if (type == BARRACK)
					entity->render_position += App->map->MapToWorld(20, 13);
				else if (type == LABORATORY)
					entity->render_position += App->map->MapToWorld(20, 11);
			}
			else if(faction == VAULT){
				if (type == BASE)
					entity->render_position += App->map->MapToWorld(9, 5);
				else if (type == BARRACK)
					entity->render_position += App->map->MapToWorld(8, 5);
				else if (type == LABORATORY)
					entity->render_position += App->map->MapToWorld(7, 3);
			}
			else if (faction == MUTANT) {
				if (type == BASE)
					entity->render_position += App->map->MapToWorld(10, 6);
				else if (type == BARRACK)
					entity->render_position += App->map->MapToWorld(9, 7);
				else if (type == LABORATORY)
					entity->render_position += App->map->MapToWorld(9, 6);
			}
			else if (faction == BROTHERHOOD) {
				if (type == BASE)
					entity->render_position += App->map->MapToWorld(10, 6);
				else if (type == BARRACK)
					entity->render_position += App->map->MapToWorld(10, 7);
				else if (type == LABORATORY)
					entity->render_position += App->map->MapToWorld(10, 6);
			}

			//Spawn position is just below render position
			entity->spawnPosition = { App->map->WorldToMap(entity->render_position.x, entity->render_position.y) };
		}
	}

	//assign entity to owner
	if (owner) {
		switch (entity->type)
		{
		case MELEE:
			owner->troops.push_back((DynamicEntity*)entity);
			owner->melees++;
			break;
		case RANGED:
			owner->troops.push_back((DynamicEntity*)entity);
			owner->rangeds++;
			break;
		case GATHERER:
			owner->gatherers_vector.push_back((DynamicEntity*)entity);
			owner->gatherers++;
			break;
		case BASE:
			owner->base = (StaticEntity*)entity;
			break;
		case BARRACK:
			if (owner->barrack[0] == nullptr) owner->barrack[0] = (StaticEntity*)entity;
			else if (owner->barrack[1] == nullptr) owner->barrack[1] = (StaticEntity*)entity;
			break;
		case LABORATORY:
			if (owner->laboratory == nullptr) owner->laboratory = (StaticEntity*)entity;
			break;
		default:
			break;
		}

	}

	return entity;
}

bool j1EntityManager::Awake(pugi::xml_node& config){
	bool ret = true;
	config_data = config;

	radar_cost = config.child("radar").attribute("cost").as_int();
	mr_handy_cost = config.child("mr_handy").attribute("cost").as_int();

	RandomFactions();
	LoadUpgradeCosts(config);
	LoadUnitCosts();

	return ret;
}

bool j1EntityManager::Start() {
	BROFILER_CATEGORY("EntitiesStart", Profiler::Color::Linen)
	bool ret = true;
	
	App->console->CreateCommand("destroy_all_entities", "remove all dynamic entities", (j1Module*)this);

	loading_reference_entities = true;
	loading_faction = VAULT;
	loading_entity = MELEE;

	for (int y = 0; y < 150; y++)
	{
		for (int x = 0; x < 150; x++)
		{
			occupied_tiles[x][y] = false;
		}
	}

	//automatic entities loading
	for (int faction = VAULT; faction < NO_FACTION; faction++)
	{
		for (int type = MELEE; type < BIGHORNER; type++)
		{
			reference_entities[faction][type] = nullptr;
			reference_entities[faction][type] = CreateEntity((Faction)faction, (EntityType)type, faction, type);
		}
	}

	reference_bighroner = (DynamicEntity*)CreateEntity(NO_FACTION, BIGHORNER, NO_FACTION, BIGHORNER);
	reference_braham = (DynamicEntity*)CreateEntity(NO_FACTION, BRAHAM, NO_FACTION, BRAHAM);
	reference_deathclaw = (DynamicEntity*)CreateEntity(NO_FACTION, DEATHCLAW, NO_FACTION, DEATHCLAW);
	reference_MrHandy = (DynamicEntity*)CreateEntity(NO_FACTION, MR_HANDY, NO_FACTION, MR_HANDY);

	LoadReferenceEntityData();

	showing_building_menu = false;

	sort_timer.Start();
	load_timer.Start();

	return ret;
}

bool j1EntityManager::CleanUp()
{
	bool ret = true;

	for (int faction = VAULT; faction < NO_FACTION; faction++)
	{
		for (int type = MELEE; type <= BASE; type++)
		{
			if (reference_entities[faction][type] != nullptr) {
				App->tex->UnLoad(reference_entities[faction][type]->texture);
				delete reference_entities[faction][type];
				reference_entities[faction][type] = nullptr;
			}
		}
	}

	for (int i = 0; i < entities.size(); i++)
	{
		delete entities[i];
		entities[i] = nullptr;
	}

	entities.clear();

	for (int j = 0; j < resource_buildings.size(); j++)
	{
		delete resource_buildings[j];
		resource_buildings[j] = nullptr;
	}
	resource_buildings.clear();

	return ret;
}

bool j1EntityManager::PreUpdate() {
	bool ret = true;

	for (int i = 0; i < entities.size(); i++)
	{
		//delete entities to destroy
		if (entities[i]->to_delete)
		{
			entities[i]->owner->DeleteEntity(entities[i]);
			if (!entities[i]->is_dynamic)App->scene->CheckWinner();
			delete entities[i];
			entities[i] = nullptr;
			entities.erase(entities.begin() + i);
		}
	}

	return ret;
}

bool j1EntityManager::Update(float dt)
{
	BROFILER_CATEGORY("EntitiesUpdate", Profiler::Color::GreenYellow)
	bool ret = true;

	//load all textures and animations on the go
	if (loading_reference_entities) {
		ret = LoadReferenceEntityAnimations();
	}

	if (!App->isPaused)
	{
		for (int i = 0; i < entities.size(); i++)
		{
			if(!entities[i]->to_delete)
				entities[i]->Update(dt);
		}
	}

	return ret;
}

bool j1EntityManager::PostUpdate()
{
	BROFILER_CATEGORY("EntitiesPostUpdate", Profiler::Color::Orange)
	bool ret = true;
	SDL_Rect tex_rect = {128,0,64,64 };
	iPoint tex_position;

	if (!loading_reference_entities)
	{
		if (App->render->debug) {
			//resource buildings debug
			for (int i = 0; i < resource_buildings.size(); i++)
			{
				for (int j = 0; j < resource_buildings[i]->tiles.size(); j++)
				{
					SDL_Rect rect = { 128,0,64,64 };
					tex_position = App->map->MapToWorld(resource_buildings[i]->tiles[j].x, resource_buildings[i]->tiles[j].y);

					if ((tex_position.x + rect.w > -(App->render->camera.x))
						&& (tex_position.x < -App->render->camera.x + App->render->camera.w)
						&& (tex_position.y + rect.h > -(App->render->camera.y))
						&& (tex_position.y < (-App->render->camera.y + App->render->camera.h)))
						{
							App->render->Blit(App->render->debug_tex, tex_position.x, tex_position.y, &rect);
						}
				}
			}
		}

		if (App->player->selected_entity != nullptr)
		{
			StaticEntity* static_entity = (StaticEntity*)App->player->selected_entity;
			//Create HUD for the building
			switch (static_entity->faction) {
			case GHOUL:
				if (static_entity->type == BASE) {

					if (!showing_building_menu) {
						App->menu_manager->CreateMenuFaction(Menu::BUI_BASES, FACTION::GHOUL, BUILDING_TYPE::BASE);
						showing_building_menu = true;
					}

				}
				else if (static_entity->type == BARRACK) {

					if (!showing_building_menu) {
						App->menu_manager->CreateMenuFaction(Menu::BUI_BASES, FACTION::GHOUL, BUILDING_TYPE::BARRACK);
						showing_building_menu = true;
					}

				}
				else if (static_entity->type == LABORATORY) {

					if (!showing_building_menu) {
						App->menu_manager->CreateMenuFaction(Menu::BUI_BASES, FACTION::GHOUL, BUILDING_TYPE::LAB);
						showing_building_menu = true;
					}

				}
				break;
			case BROTHERHOOD:
				if (static_entity->type == BASE) {

					if (!showing_building_menu) {
						App->menu_manager->CreateMenuFaction(Menu::BUI_BASES, FACTION::BROTHERHOOD, BUILDING_TYPE::BASE);
						showing_building_menu = true;
					}
				}
				else if (static_entity->type == BARRACK) {

					if (!showing_building_menu) {
						App->menu_manager->CreateMenuFaction(Menu::BUI_BASES, FACTION::BROTHERHOOD, BUILDING_TYPE::BARRACK);
						showing_building_menu = true;

					}
				}
				else if (static_entity->type == LABORATORY) {

					if (!showing_building_menu) {
						App->menu_manager->CreateMenuFaction(Menu::BUI_BASES, FACTION::BROTHERHOOD, BUILDING_TYPE::LAB);
						showing_building_menu = true;
					}
				}
				break;

			case VAULT:

				if (static_entity->type == BASE) {

					if (!showing_building_menu) {
						App->menu_manager->CreateMenuFaction(Menu::BUI_BASES, FACTION::VAULT, BUILDING_TYPE::BASE);
						showing_building_menu = true;
					}
				}
				else if (static_entity->type == BARRACK) {

					if (!showing_building_menu) {
						App->menu_manager->CreateMenuFaction(Menu::BUI_BASES, FACTION::VAULT, BUILDING_TYPE::BARRACK);
						showing_building_menu = true;
					}

				}
				else if (static_entity->type == LABORATORY) {

					if (!showing_building_menu) {
						App->menu_manager->CreateMenuFaction(Menu::BUI_BASES, FACTION::VAULT, BUILDING_TYPE::LAB);
						showing_building_menu = true;
					}

				}
				break;
			case MUTANT:
				if (static_entity->type == BASE) {

					if (!showing_building_menu) {
						App->menu_manager->CreateMenuFaction(Menu::BUI_BASES, FACTION::SUPERMUTANT, BUILDING_TYPE::BASE);
						showing_building_menu = true;
					}

				}
				else if (static_entity->type == BARRACK) {

					if (!showing_building_menu) {
						App->menu_manager->CreateMenuFaction(Menu::BUI_BASES, FACTION::SUPERMUTANT, BUILDING_TYPE::BARRACK);
						showing_building_menu = true;
					}

				}
				else if (static_entity->type == LABORATORY) {
					if (!showing_building_menu) {
						App->menu_manager->CreateMenuFaction(Menu::BUI_BASES, FACTION::SUPERMUTANT, BUILDING_TYPE::LAB);
						showing_building_menu = true;
					}
				}
				break;
			}
		}
		
		if (sort_timer.Read() > 500) {
			BubbleSortEntities();
			sort_timer.Start();
		}

		for (int i = 0; i < entities.size(); i++)
		{
			//camera culling
			if ((entities[i]->position.x + entities[i]->sprite_size * 0.5f > -App->render->camera.x)
				&& (entities[i]->position.x - entities[i]->sprite_size * 0.5f < -App->render->camera.x + App->render->camera.w)
				&& (entities[i]->position.y + entities[i]->sprite_size * 0.25f > -App->render->camera.y)
				&& (entities[i]->position.y - entities[i]->sprite_size * 0.25f < -App->render->camera.y + App->render->camera.h)) {

				//sort and blit entities
				entities[i]->PostUpdate();
			}
		}

		if (App->render->debug) {
			iPoint occuppied_tile = { -1,-1 };
			for (int y = 0; y < 150; y++)
			{
				for (int x = 0; x < 150; x++)
				{
					if (occupied_tiles[x][y]) {
						occuppied_tile = App->map->MapToWorld(x, y);
						App->render->DrawQuad({ occuppied_tile.x + HALF_TILE,occuppied_tile.y + HALF_TILE,8,8 }, 155, 155, 155, 255);
					}
				}
			}
		}
	}

	return ret;
}


bool j1EntityManager::LoadReferenceEntityAnimations() {
	bool ret = true;

	if (load_timer.Read() > 100) {

		if (loading_entity == BIGHORNER) {
			loading_entity = MELEE;
			loading_faction++;
		}

		if (loading_faction == ANIMALS) {
			if (loading_entity == 1) 
				ret = reference_bighroner->LoadAnimations();
			else if (loading_entity == 2)
				ret = reference_braham->LoadAnimations();
			else if (loading_entity == 3) {
				reference_deathclaw->LoadAnimations();
				reference_MrHandy->LoadAnimations();

				for (int faction = VAULT; faction < ANIMALS; faction++)
				{
					reference_entities[faction][BARRACK]->texture = reference_entities[faction][BASE]->texture;
					reference_entities[faction][LABORATORY]->texture = reference_entities[faction][BASE]->texture;
				}

				load_timer.Start();
				loading_reference_entities = false;
			}
			loading_entity++;
		}
		else if (loading_faction != NO_FACTION)
		{
			ret = reference_entities[loading_faction][loading_entity]->LoadAnimations();
			loading_entity++;
		}
	}

	return ret;
}

bool j1EntityManager::LoadReferenceEntityData() {
	bool ret = true;
	pugi::xml_document entities_file;
	pugi::xml_node entities_node;
	pugi::xml_parse_result result = entities_file.load_file("entities.xml");

	if (result == NULL)
		LOG("Could not load map xml file entities.xml. pugi error: %s", result.description());
	else
		 entities_node = entities_file.child("entities");

	pugi::xml_node type_node = entities_node.first_child();
	pugi::xml_node faction_node;
	Faction faction = NO_FACTION;
	EntityType type = NO_TYPE;

	//load dynamic entities
	faction_node = type_node.first_child();
	while (type_node != nullptr) {
		while (faction_node != nullptr)
		{
			std::string faction_string = std::string(faction_node.name());

			//check faction
			if (faction_string == "vault") faction = VAULT;
			else if (faction_string == "brotherhood") faction = BROTHERHOOD;
			else if (faction_string == "mutants") faction = MUTANT;
			else if (faction_string == "ghouls") faction = GHOUL;
			else if (faction_string == "animals") faction = ANIMALS;
			else if (faction_string == "mr_handy") faction = NO_FACTION;

			pugi::xml_node entity_node = faction_node.first_child();
			while (entity_node != nullptr)
			{
				std::string type_string = std::string(entity_node.name());

				//check type
				if (type_string == "melee") type = MELEE;
				else if (type_string == "ranged") type = RANGED;
				else if (type_string == "gatherer") type = GATHERER;
				else if (type_string == "base") type = BASE;
				else if (type_string == "barrack") type = BARRACK;
				else if (type_string == "laboratory") type = LABORATORY;
				else if (type_string == "bighorner") type = BIGHORNER;
				else if (type_string == "braham") type = BRAHAM;
				else if (type_string == "deathclaw") type = DEATHCLAW;
				else if (type_string == "mr_handy") type = MR_HANDY;

				//load attributes
				int health = entity_node.attribute("health").as_int();
				int damage = entity_node.attribute("damage").as_int();
				int speed = entity_node.attribute("speed").as_int();

				//load into reference entities
				if ((faction != ANIMALS)&&(faction != NO_FACTION)) {
					reference_entities[faction][type]->max_health = health;
					reference_entities[faction][type]->damage = damage;
					reference_entities[faction][type]->speed = { (float)speed, (float)speed * 0.5f};
				}
				else if (type != MR_HANDY)
				{
					DynamicEntity* animal = nullptr;
					if (type == BIGHORNER) animal = reference_bighroner;
					else if (type == BRAHAM) animal = reference_braham;
					else if (type == DEATHCLAW) animal = reference_deathclaw;

					animal->max_health = health;
					animal->damage = damage;
					animal->speed = { (float)speed, (float)speed * 0.5f };
					animal->resource_collected = entity_node.attribute("resource_quantity").as_int();
				}
				else
				{
					DynamicEntity* mr_handy;
					mr_handy = reference_MrHandy;
					mr_handy->max_health = health;
					mr_handy->speed = { (float)speed, (float)speed * 0.5f };
					mr_handy->damage = damage;
				}

				entity_node = entity_node.next_sibling();
			}
			faction_node = faction_node.next_sibling();
		}
		type_node = type_node.next_sibling();
		faction_node = type_node.first_child();
	}

	return ret;
}

void j1EntityManager::DestroyEntity(j1Entity* entity) { entity->to_delete = true;}

void j1EntityManager::DestroyAllEntities() {
	for (int i = 0; i < entities.size(); i++)
	{
		entities[i]->to_delete = true;
	}
}

j1Entity* j1EntityManager::FindEntityByTile(iPoint tile) {
	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i]->is_dynamic)
		{
			if (entities[i]->current_tile == tile)
				return entities[i];
		}
		else
		{
			StaticEntity* static_entity = (StaticEntity*)entities[i];
			for (int j = 0; j < static_entity->tiles.size(); j++)
			{
				if (static_entity->tiles[j] == tile)
					return entities[i];
			}
		}
	}
	return nullptr;
}

ResourceBuilding* j1EntityManager::FindResourceBuildingByTile(iPoint tile) {
	for (int i = 0; i < resource_buildings.size(); i++)
	{
		for (int j = 0; j < resource_buildings[i]->tiles.size(); j++)
		{
			if (resource_buildings[i]->tiles[j] == tile)
				return resource_buildings[i];
		}
	}
	return nullptr;
}

iPoint j1EntityManager::ClosestTile(iPoint position, std::vector<iPoint> entity_tiles) const {
	iPoint pivot = entity_tiles[0];
	for (int i = 0; i < entity_tiles.size(); i++)
	{
		if (position.DistanceManhattan(entity_tiles[i]) < position.DistanceManhattan(pivot))
			pivot = entity_tiles[i];
	}
	return pivot;
}

iPoint j1EntityManager::FindFreeAdjacentTile(iPoint origin, iPoint destination) {
	int max = 1;
	iPoint possible_tile;
	iPoint closest_possible_tile = {-1,-1};
	int distance_to_origin = 100000;
	int distance_to_destination = 100000;

	while (max < 5) {
		for (int y = -max; y < max; y++)
		{
			for (int x = -max; x < max; x++)
			{
				if (x != 0 && y != 0) {
					possible_tile.x = destination.x + x;
					possible_tile.y = destination.y + y;

					if ((!occupied_tiles[possible_tile.x][possible_tile.y])&&(App->pathfinding->IsWalkable(possible_tile))) {
						if ((possible_tile.DistanceManhattan(origin) <= distance_to_origin)&& (possible_tile.DistanceManhattan(destination) <= distance_to_destination)) {
								distance_to_origin = possible_tile.DistanceManhattan(origin);
								distance_to_destination = possible_tile.DistanceManhattan(destination);
								closest_possible_tile = possible_tile;
							}
		
					}
				}
			}
		}

		if (closest_possible_tile != iPoint(-1, -1))
			return closest_possible_tile;

		max++;
	}
	return possible_tile;
}

ResourceBuilding* j1EntityManager::GetClosestResourceBuilding(iPoint current_position) {
	ResourceBuilding* closest_building = nullptr;
	int min_distance = 1000;
	for (int i = 0; i < resource_buildings.size(); i++)
	{
		if (resource_buildings[i]->quantity > 0)
		{
			int building_distance = ClosestTile(current_position, resource_buildings[i]->tiles).DistanceManhattan(current_position);
			if (building_distance < min_distance) {
				closest_building = resource_buildings[i];
				min_distance = building_distance;
			}
		}
	}

	return closest_building;
}

void j1EntityManager::BubbleSortEntities() {
	BROFILER_CATEGORY("BubbleSortEntities", Profiler::Color::Blue)
	int i, j;
	int n = entities.size();
	for (i = 0; i < n - 1; i++) {
		for (j = 0; j < n - i - 1; j++) {
		  if (entities[j]->position.y > entities[j + 1]->position.y)
			std::swap(entities[j], entities[j + 1]);
		}
	}
}

void j1EntityManager::RandomFactions() {
	Faction faction = static_cast<Faction>(rand() % GHOUL);

	//Initialize at { 0,1,2,3 }
	for(int i = 0; i < 4; i++)
		randomFaction[i] = i;
	
	//Randomize faction order
	//std::random_shuffle(&randomFaction[0], &randomFaction[3]);

	srand(time(NULL));

	int temp = 0;
	int randomIndex = 0;

	for (int i = 0; i < 4; i++) {
		randomIndex = rand() % 4;
		temp = randomFaction[i];
		randomFaction[i] = randomFaction[randomIndex];
		randomFaction[randomIndex] = temp;
	}

	
	for (int i = 0; i < 4; i++)
		LOG("faction %i", randomFaction[i]);
}

void j1EntityManager::OnCommand(std::vector<std::string> command_parts) {
	if (command_parts[0] == "destroy_all_entities") {
		for (int i = 0; i < entities.size(); i++)
		{
			if (entities[i]->is_dynamic)
				entities[i]->to_delete = true;
		}
	}
}

void j1EntityManager::LoadUpgradeCosts(pugi::xml_node& config)
{
	pugi::xml_node boost_node = config.first_child().first_child();
	Faction faction = NO_FACTION;
	std::string faction_name;

	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 4; i++) {
			if (i == 0)faction = VAULT;
			else if (i == 1)faction = BROTHERHOOD;
			else if (i == 2)faction = MUTANT;
			else if (i == 3)faction = GHOUL;

			int caps_cost = boost_node.attribute("cost").as_int();
			int upgrade_time = boost_node.attribute("upgrade_time").as_int();
			int cost_increment = boost_node.attribute("cost_increment").as_int();

			if (j == 0) { //BASE upgrades
				float storage_increment = boost_node.attribute("storage_increment").as_float();
				float gatherer_capacity = boost_node.attribute("gatherer_capacity").as_float();

				base_resource_limit[faction] = { faction, RESOURCES_LIMIT, 0, caps_cost, cost_increment, upgrade_time, storage_increment };
				gatherer_resource_limit[faction] = { faction, GATHERER_CAPACITY, 0, caps_cost, cost_increment, upgrade_time, gatherer_capacity };
			}
			else if (j == 1) {//LABORATORY upgrades
				float health_increment = boost_node.attribute("health_increment").as_float();
				float creation_time = boost_node.attribute("creation_time").as_float();

				units_health[faction] = { faction, UNITS_HEALTH, 0, caps_cost, cost_increment, upgrade_time, health_increment };
				units_creation_time[faction] = { faction, CREATION_TIME, 0, caps_cost, cost_increment, upgrade_time, creation_time };
			}
			else if (j == 2) {//BARRACK upgrades
				float damage_increment = boost_node.attribute("damage_increment").as_float();
				float speed_increment = boost_node.attribute("speed_increment").as_float();

				units_damage[faction] = { faction, UNITS_DAMAGE, 0, caps_cost, cost_increment, upgrade_time, damage_increment };
				units_speed[faction] = { faction, UNITS_SPEED, 0, caps_cost, cost_increment, upgrade_time, speed_increment };
			}
		}
		boost_node = boost_node.next_sibling();
	}
}

void j1EntityManager::LoadUnitCosts() {
	//Loads the cost of spawning units and creating upgrades from XML file
	
	pugi::xml_document entities_file;
	pugi::xml_node entities_node;
	pugi::xml_parse_result result = entities_file.load_file("entities.xml");

	if (result == NULL)
		LOG("Could not load map xml file entities.xml. pugi error: %s", result.description());
	else
		entities_node = entities_file.child("entities");

	pugi::xml_node type_node = entities_node.first_child();
	pugi::xml_node faction_node;
	Faction faction = NO_FACTION;
	EntityType type = NO_TYPE;

	//load unit costs
	faction_node = type_node.first_child();
	while (type_node != nullptr) {
		while (faction_node != nullptr)
		{
			std::string faction_string = std::string(faction_node.name());

			//check faction
			if (faction_string == "vault") faction = VAULT;
			else if (faction_string == "brotherhood") faction = BROTHERHOOD;
			else if (faction_string == "mutants") faction = MUTANT;
			else if (faction_string == "ghouls") faction = GHOUL;

			pugi::xml_node entity_node = faction_node.first_child();
			while (entity_node != nullptr)
			{
				std::string type_string = std::string(entity_node.name());

				//check type
				if (type_string == "melee") type = MELEE;
				else if (type_string == "ranged") type = RANGED;
				else if (type_string == "gatherer") type = GATHERER;

				//load attributes
				int food = entity_node.child("cost").attribute("food").as_int();
				int water = entity_node.child("cost").attribute("water").as_int();
				int time = entity_node.child("cost").attribute("time").as_int();

				//load into unit_data
				unit_data[faction][type] = { water, food, time };

				entity_node = entity_node.next_sibling();
			}
			faction_node = faction_node.next_sibling();
		}
		type_node = type_node.next_sibling();
		faction_node = type_node.first_child();
	}
}

ResourceBuilding* j1EntityManager::CreateResourceSpot(int position_x, int position_y, Resource resource_type, int resource_quantity) {
	ResourceBuilding* resource_spot = new ResourceBuilding();
	resource_spot->resource_type = resource_type;
	resource_spot->quantity = resource_quantity;
	resource_spot->tiles.push_back({ position_x, position_y });
	resource_buildings.push_back(resource_spot);

	return resource_spot;
}

void j1EntityManager::DestroyResourceSpot(ResourceBuilding* resource_spot) {
	for (int i = 0; i < resource_buildings.size(); i++)
	{
		if (resource_spot == resource_buildings[i]) {
			delete resource_buildings[i];
			resource_buildings[i] = nullptr;
			resource_buildings.erase(resource_buildings.begin() + i);
		}
	}
}

iPoint j1EntityManager::FindSpawnPoint(int position_x, int position_y) {
	if (occupied_tiles[position_x][position_y]) {
		//There's another unit, let's find a new spawn point
		bool spawnPointFound = false;

		while (occupied_tiles[position_x][position_y]) {
			for (int k = 0; k < 10; k++) {
				for (int i = 0; i <= 5; i++) {
					if (spawnPointFound == false) {
						if (!occupied_tiles[position_x - i][position_y + k]) {
							position_x -= i;
							position_y += k;
							spawnPointFound = true;
						}
					}
				}
				if (spawnPointFound == false) {
					for (int j = 0; j <= 5; j++) {
						if (spawnPointFound == false) {
							if (!occupied_tiles[position_x + k][position_y - j]) {
								position_y -= j;
								position_x += k;
								spawnPointFound = true;
							}
						}
					}
				}
				//First line completed. Next look for spawn points in the next line
			}
			//We didn't find a free spawn point, so we spawn in the same tile as other unit
			break;
		}
	}
	return iPoint(position_x, position_y);
}

bool j1EntityManager::CreateReferenceEntities() {

}