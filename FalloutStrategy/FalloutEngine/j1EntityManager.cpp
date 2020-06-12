#include "j1EntityManager.h"
#include "j1App.h"
#include "p2Log.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"
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
#include "Deathclaw.h"
#include "GenericPlayer.h"
#include "j1Entity.h"
#include "AssetsManager.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "j1Gui.h"
#include "ParticleSystem.h"
#include "Emiter.h"

j1EntityManager::j1EntityManager(){

	name.assign("entities");

	for (int i = 0; i < 4;i++) {
		randomFaction[i] = 0;
	}

	loading_faction = VAULT;
	loading_entity = MELEE;
	blocked_movement = false;
	showing_building_menu = false;
	blood = nullptr;
	smoke = nullptr;
	hit = nullptr;
	entities_loaded = false;

	mr_handy_cost = NULL;
	mr_handy_time = NULL;

	no_reso.x = 2757;
	no_reso.y = 3086;
	no_reso.w = 106;
	no_reso.h = 104;


	radar_cost = NULL;
}

j1EntityManager::~j1EntityManager(){}

bool j1EntityManager::Awake(pugi::xml_node& config){
	bool ret = true;
	config_data = config;

	radar_cost = config.child("radar").attribute("cost").as_int();
	mr_handy_cost = config.child("mr_handy").attribute("cost").as_int();

	LOG("%i",mr_handy_cost);

	mr_handy_time = config.child("mr_handy").attribute("time").as_int();

	pugi::xml_node animation_node = config.child("animation_files");
	std::string base_folder = animation_node.attribute("base_folder").as_string();
	pugi::xml_node file_node = animation_node.child("file");

	for(int i = 0; i < REFERENCE_ENTITIES; i++)
	{
		texture_folders[i] = base_folder;
		texture_folders[i].append(file_node.attribute("folder").as_string());
		tmx_files[i] = file_node.attribute("tmx").as_string();
		file_node = file_node.next_sibling();
		reference_entities[i] = nullptr;
	}

	for(int y = 0; y < 150; y++)
	{
		for(int x = 0; x < 150; x++)
		{
			occupied_tiles[x][y] = false;
		}
	}

	RandomFactions();
	LoadUpgradeCosts(config);
	LoadUnitCosts();

	return ret;
}

bool j1EntityManager::Start() {
	BROFILER_CATEGORY("EntitiesStart", Profiler::Color::Linen)
	bool ret = true;

	App->console->CreateCommand("destroy_all_entities", "remove all dynamic entities", dynamic_cast<j1Module*>(this));

	loading_reference_entities = true;
	loading_faction = VAULT;
	loading_entity = MELEE;

	background_health_bar = { 0,0,0,0 };
	foreground_health_bar = { 0,0,0,0 };
	frame_quad = { 0,0,0,0 };

	

	for(int y = 0; y < 150; y++)
	{
		for(int x = 0; x < 150; x++)
		{
			occupied_tiles[x][y] = false;
		}
	}

	//automatic entities loading
	int i = 0;
	for(int faction = VAULT; faction < NO_FACTION; faction++)
	{
		for(int type = MELEE; type < BIGHORNER; type++)
		{
			reference_entities[i] = CreateEntity((Faction)faction, (EntityType)type, faction, type);
			i++;
		}
	}
	reference_entities[i++] = CreateEntity(NO_FACTION, BIGHORNER, NO_FACTION, BIGHORNER);
	reference_entities[i++] = CreateEntity(NO_FACTION, BRAHAM, NO_FACTION, BRAHAM);
	reference_entities[i++] = CreateEntity(NO_FACTION, DEATHCLAW, NO_FACTION, DEATHCLAW);
	reference_entities[i++] = CreateEntity(NO_FACTION, MR_HANDY, NO_FACTION, MR_HANDY);

	LoadReferenceEntityData();

	loading_entity = 0;
	showing_building_menu = false;
	loading_reference_entities = true;

	sort_timer.Start();
	load_timer.Start();

	// -- Loading Particle textures
	blood = App->tex->Load("Assets/textures/particles/blood.png");
	smoke = App->tex->Load("Assets/textures/particles/smoke.png");
	hit = App->tex->Load("Assets/textures/particles/HitParticle.png");

	life_bars = App->tex->Load("Assets/textures/life_bar/bars.png");

	return ret;
}

bool j1EntityManager::CleanUp()
{
	bool ret = true;

	// -- Reference entities
	for(int i = 0; i < REFERENCE_ENTITIES; i++)
	{
		if (reference_entities[i] != nullptr) {
			App->tex->UnLoad(reference_entities[i]->texture);
			delete reference_entities[i];
			reference_entities[i] = nullptr;
		}
	}

	// -- Entities
	for(size_t i = 0; i < entities.size(); i++)
	{
		if (entities[i] != nullptr) {
			delete entities[i];
			entities[i] = nullptr;
		}
	}

	entities.clear();

	// -- Particles
	for(size_t i = 0; i < particles.size(); i++)
	{
		if (particles[i] != nullptr) {
			delete particles[i];
			particles[i] = nullptr;
		}
	}

	particles.clear();
	App->tex->UnLoad(blood);
	App->tex->UnLoad(smoke);
	App->tex->UnLoad(hit);

	blood = nullptr;
	smoke = nullptr;
	hit = nullptr;

	// -- Buildings
	for(size_t j = 0; j < resource_buildings.size(); j++)
	{
		delete resource_buildings[j];
		resource_buildings[j] = nullptr;
	}
	resource_buildings.clear();

	return ret;
}

bool j1EntityManager::PreUpdate() {
	bool ret = true;

	for(size_t i = 0; i < entities.size(); i++)
	{
	}

	return ret;
}

bool j1EntityManager::Update(float dt)
{
	BROFILER_CATEGORY("EntitiesUpdate", Profiler::Color::GreenYellow)
	bool ret = true;

	//load all textures and animations on the go
	if (loading_reference_entities) {
		if (load_timer.Read() > 100) {
			if (loading_entity < REFERENCE_ENTITIES) {
				reference_entities[loading_entity]->LoadAnimations(texture_folders[loading_entity].c_str(), tmx_files[loading_entity].c_str());
				loading_entity++;
				load_timer.Start();
			}
			else {
				loading_reference_entities = false;
			}
		}
	}
	else if (!App->isPaused)
	{
		for(size_t i = 0; i < entities.size(); i++)
		{
			if(!entities[i]->to_delete)
				entities[i]->Update(dt);
			
		}

		if (sort_timer.Read() > 500) {
			BubbleSortEntities();
			sort_timer.Start();
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
			for(size_t i = 0; i < resource_buildings.size(); i++)
			{
				for(size_t j = 0; j < resource_buildings[i]->tiles.size(); j++)
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
				if (resource_buildings[i]->quantity <= 0) {
					SDL_Rect rect2 = { 2757,3086,106,104 };
					App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), tex_position.x, tex_position.y, &rect2);
				}

			}
		}

		if ((App->player->selected_entity != nullptr)&&(!App->player->selected_entity->is_dynamic))
		{
			StaticEntity* static_entity = dynamic_cast<StaticEntity*>(App->player->selected_entity);
			//Create HUD for the building
			switch (static_entity->faction) {
			case VAULT:
				if (static_entity->type == BASE) {

					if (!showing_building_menu) {
						App->menu_manager->CreateMenuFaction(Menu::BUI_BASES, VAULT, BASE);
						showing_building_menu = true;
					}
				}
				else if (static_entity->type == BARRACK) {

					if (!showing_building_menu) {
						App->menu_manager->CreateMenuFaction(Menu::BUI_BASES, VAULT, BARRACK);
						showing_building_menu = true;
					}

				}
				else if (static_entity->type == LABORATORY) {

					if (!showing_building_menu) {
						App->menu_manager->CreateMenuFaction(Menu::BUI_BASES, VAULT, LABORATORY);
						showing_building_menu = true;
					}

				}
				break;
			case BROTHERHOOD:
				if (static_entity->type == BASE) {

					if (!showing_building_menu) {
						App->menu_manager->CreateMenuFaction(Menu::BUI_BASES, BROTHERHOOD, BASE);
						showing_building_menu = true;
					}
				}
				else if (static_entity->type == BARRACK) {

					if (!showing_building_menu) {
						App->menu_manager->CreateMenuFaction(Menu::BUI_BASES, BROTHERHOOD, BARRACK);
						showing_building_menu = true;

					}
				}
				else if (static_entity->type == LABORATORY) {

					if (!showing_building_menu) {
						App->menu_manager->CreateMenuFaction(Menu::BUI_BASES, BROTHERHOOD, LABORATORY);
						showing_building_menu = true;
					}
				}
				break;

			case MUTANT:
				if (static_entity->type == BASE) {

					if (!showing_building_menu) {
						App->menu_manager->CreateMenuFaction(Menu::BUI_BASES, MUTANT, BASE);
						showing_building_menu = true;
					}

				}
				else if (static_entity->type == BARRACK) {

					if (!showing_building_menu) {
						App->menu_manager->CreateMenuFaction(Menu::BUI_BASES, MUTANT, BARRACK);
						showing_building_menu = true;
					}

				}
				else if (static_entity->type == LABORATORY) {
					if (!showing_building_menu) {
						App->menu_manager->CreateMenuFaction(Menu::BUI_BASES, MUTANT, LABORATORY);
						showing_building_menu = true;
					}
				}
				break;

			case GHOUL:
				if (static_entity->type == BASE) {

					if (!showing_building_menu) {
						App->menu_manager->CreateMenuFaction(Menu::BUI_BASES, GHOUL, BASE);
						showing_building_menu = true;
					}

				}
				else if (static_entity->type == BARRACK) {

					if (!showing_building_menu) {
						App->menu_manager->CreateMenuFaction(Menu::BUI_BASES, GHOUL, BARRACK);
						showing_building_menu = true;
					}

				}
				else if (static_entity->type == LABORATORY) {

					if (!showing_building_menu) {
						App->menu_manager->CreateMenuFaction(Menu::BUI_BASES, GHOUL, LABORATORY);
						showing_building_menu = true;
					}

				}
				break;
			}
		}
		
		for(size_t i = 0; i < entities.size(); i++)
		{
			//camera culling
			if ((entities[i]->position.x + entities[i]->sprite_size * 0.5f > -App->render->camera.x)
				&& (entities[i]->position.x - entities[i]->sprite_size * 0.5f < -App->render->camera.x + App->render->camera.w)
				&& (entities[i]->position.y + entities[i]->sprite_size * 0.25f > -App->render->camera.y)
				&& (entities[i]->position.y - entities[i]->sprite_size * 0.25f < -App->render->camera.y + App->render->camera.h)) {

				//sort and blit entities
				entities[i]->PostUpdate();
			}

			//delete entities to destroy
			if (entities[i]->to_delete)
			{
				if (entities[i]->owner != nullptr)
					entities[i]->owner->DeleteEntity(entities[i]);

				if ((!entities[i]->is_dynamic) && (!App->IsLoading()))
					App->scene->CheckWinner();

				delete entities[i];
				entities[i] = nullptr;
				entities.erase(entities.begin() + i);
			}
		}

		/*
		//occupied tiles
		if (App->render->debug) {
			iPoint occuppied_tile = { -1,-1 };
			for(int y = 0; y < 150; y++)
			{
				for(int x = 0; x < 150; x++)
				{
					if (occupied_tiles[x][y]) {
						occuppied_tile = App->map->MapToWorld(x, y);
						App->render->DrawQuad({ occuppied_tile.x + HALF_TILE,occuppied_tile.y + HALF_TILE,8,8 }, 255, 155, 155, 175);
					}
				}
			}
		}
		*/
		
	}

	return ret;
}

j1Entity* j1EntityManager::CreateEntity(Faction faction, EntityType type, int position_x, int position_y, GenericPlayer* owner) {
	BROFILER_CATEGORY("EntityCreation", Profiler::Color::Linen)

	j1Entity* entity = nullptr;
	iPoint available_tile;
	bool default_case = false;

	switch (type)
	{
	case MELEE:
		available_tile = FindSpawnPoint(position_x, position_y);
		entity = new Troop(MELEE, faction, available_tile, owner);
		if (owner) {
			owner->troops.push_back(dynamic_cast<Troop*>(entity));
			owner->melees++;
		}
		entity->reference_entity = reference_entities[GetReferenceEntityID(faction, MELEE)];
		break;

	case RANGED:
		available_tile = FindSpawnPoint(position_x, position_y);
		entity = new Troop(RANGED, faction, available_tile, owner);
		if (owner) {
			owner->troops.push_back(dynamic_cast<Troop*>(entity));
			owner->rangeds++;
		}
		entity->reference_entity = reference_entities[GetReferenceEntityID(faction, RANGED)];
		break;

	case GATHERER:
		available_tile = FindSpawnPoint(position_x, position_y);
		entity = new Gatherer(faction, available_tile, owner);
		if (owner) {
			owner->gatherers_vector.push_back(dynamic_cast<Gatherer*>(entity));
			owner->gatherers++;
		}
		entity->reference_entity = reference_entities[GetReferenceEntityID(faction, GATHERER)];
		break;

	case BASE:
		entity = new StaticEntity(faction, BASE, { position_x, position_y }, owner);
		if (owner) {
			owner->base = dynamic_cast<StaticEntity*>(entity);
		}
		entity->reference_entity = reference_entities[GetReferenceEntityID(faction, BASE)];
		break;

	case LABORATORY:
		entity = new StaticEntity(faction, LABORATORY, { position_x, position_y }, owner);
		if (owner) {
			owner->laboratory = dynamic_cast<StaticEntity*>(entity);
		}
		entity->reference_entity = reference_entities[GetReferenceEntityID(faction, LABORATORY)];
		break;

	case BARRACK:
		entity = new StaticEntity(faction, BARRACK, { position_x, position_y }, owner);
		if (owner) {
			if (!owner->barrack[0])
				owner->barrack[0] = dynamic_cast<StaticEntity*>(entity);
			else if (!owner->barrack[1])
				owner->barrack[1] = dynamic_cast<StaticEntity*>(entity);
		}
		entity->reference_entity = reference_entities[GetReferenceEntityID(faction, BARRACK)];
		break;

	case BIGHORNER:
		entity = new Animal(BIGHORNER, { position_x, position_y });
		entity->reference_entity = reference_entities[GetReferenceEntityID(NO_FACTION, BIGHORNER)];
		break;

	case BRAHAM:
		entity = new Animal(BRAHAM, { position_x, position_y });
		entity->reference_entity = reference_entities[GetReferenceEntityID(NO_FACTION, BRAHAM)];
		break;

	case DEATHCLAW:
		entity = new Deathclaw({ position_x, position_y });
		entity->reference_entity = reference_entities[GetReferenceEntityID(NO_FACTION, DEATHCLAW)];
		break;

	case MR_HANDY:
		entity = new Troop(MR_HANDY, faction, { position_x, position_y }, owner);
		if (owner) {
			owner->troops.push_back(dynamic_cast<Troop*>(entity));
		}
		entity->reference_entity = reference_entities[GetReferenceEntityID(NO_FACTION, MR_HANDY)];
		break;
	default:
		default_case = true; //Potential bug. If the program comes here and this variable didn't exist -> It would crash
		break;
	}

	if (default_case == false && entity->reference_entity != nullptr) {
		if(entity->is_dynamic)
			occupied_tiles[entity->current_tile.x][entity->current_tile.y] = true;

		entities.push_back(entity);
		entity->LoadDataFromReference();
	}

	return entity;
}

bool j1EntityManager::LoadReferenceEntityAnimations() {
	bool ret = true;

	return ret;
}

bool j1EntityManager::LoadReferenceEntityData() {
	bool ret = true;
	pugi::xml_document entities_file;
	pugi::xml_node entities_node;

	char* buffer;
	int bytesFile = App->assetManager->Load("entities.xml", &buffer);
	pugi::xml_parse_result result = entities_file.load_buffer(buffer, bytesFile);
	RELEASE_ARRAY(buffer);

	if (result == NULL)
		LOG("Could not load map xml file entities.xml. pugi error: %s", result.description());
	else
		 entities_node = entities_file.child("entities");

	pugi::xml_node class_node = entities_node.first_child();
	pugi::xml_node type_node;
	pugi::xml_node faction_node;
	Faction faction = NO_FACTION;
	//EntityType type = NO_TYPE;
	std::string faction_name;
	std::string type_name;

	while (class_node)
	{
		faction_node = class_node.first_child();

		while (faction_node)
		{
			faction_name = faction_node.name();

			//faction
			if (faction_name == "vault")
				faction = VAULT;
			else if (faction_name == "brotherhood")
				faction = BROTHERHOOD;
			else if (faction_name == "mutants")
				faction = MUTANT;
			else if (faction_name == "ghouls")
				faction = GHOUL;

			type_node = faction_node.first_child();

			while (type_node)
			{
				type_name = type_node.name();

				//type
				if (type_name == "melee") {
					reference_entities[GetReferenceEntityID(faction, MELEE)]->LoadReferenceData(type_node);
				}
				else if (type_name == "ranged") {
					reference_entities[GetReferenceEntityID(faction, RANGED)]->LoadReferenceData(type_node);
				}
				else if (type_name == "gatherer") {
					reference_entities[GetReferenceEntityID(faction, GATHERER)]->LoadReferenceData(type_node);
				}
				else if (type_name == "base") {
					reference_entities[GetReferenceEntityID(faction, BASE)]->LoadReferenceData(type_node);
				}
				else if (type_name == "laboratory") {
					reference_entities[GetReferenceEntityID(faction, LABORATORY)]->LoadReferenceData(type_node);
				}
				else if (type_name == "barrack") {
					reference_entities[GetReferenceEntityID(faction, BARRACK)]->LoadReferenceData(type_node);
				}
				else if (type_name == "bighorner") {
					reference_entities[GetReferenceEntityID(NO_FACTION, BIGHORNER)]->LoadReferenceData(type_node);
				}
				else if (type_name == "braham") {
					reference_entities[GetReferenceEntityID(NO_FACTION, BRAHAM)]->LoadReferenceData(type_node);
				}
				else if (type_name == "deathclaw") {
					reference_entities[GetReferenceEntityID(NO_FACTION, DEATHCLAW)]->LoadReferenceData(type_node);
				}
				else if (type_name == "mr_handy") {
					reference_entities[GetReferenceEntityID(NO_FACTION, MR_HANDY)]->LoadReferenceData(type_node);
				}

				type_node = type_node.next_sibling();
			}

			faction_node = faction_node.next_sibling();
		}

		class_node = class_node.next_sibling();
	}


	return ret;
}

void j1EntityManager::DestroyEntity(j1Entity* entity) { entity->to_delete = true;}

void j1EntityManager::DestroyAllEntities() {
	for(size_t i = 0; i < entities.size(); i++)
	{
		entities[i]->to_delete = true;
	}
}

void j1EntityManager::DestroyAllEntitiesNow() {
	for(size_t i = 0; i < entities.size(); i++)
	{
		delete entities[i];
		entities[i] = nullptr;
	}
	entities.clear();
}

j1Entity* j1EntityManager::FindEntityByTile(iPoint tile, j1Entity* entity_to_skip) {
	for(size_t i = 0; i < entities.size(); i++)
	{
		if (entities[i] != entity_to_skip) {
			if (entities[i]->is_dynamic)
			{
				if (entities[i]->current_tile == tile)
					return entities[i];
			}
			else
			{
				StaticEntity* static_entity = dynamic_cast<StaticEntity*>(entities[i]);
				for (size_t j = 0; j < static_entity->tiles.size(); j++)
				{
					if (static_entity->tiles[j] == tile)
						return entities[i];
				}
			}
		}
	}
	return nullptr;
}

ResourceBuilding* j1EntityManager::FindResourceBuildingByTile(iPoint tile) {
	for(size_t i = 0; i < resource_buildings.size(); i++)
	{
		for(size_t j = 0; j < resource_buildings[i]->tiles.size(); j++)
		{
			if (resource_buildings[i]->tiles[j] == tile)
				return resource_buildings[i];
		}
	}
	return nullptr;
}

iPoint j1EntityManager::ClosestTile(iPoint position, std::vector<iPoint> entity_tiles) {
	iPoint pivot = entity_tiles[0];
	for(size_t i = 0; i < entity_tiles.size(); i++)
	{
		if (position.DistanceManhattan(entity_tiles[i]) < position.DistanceManhattan(pivot))
			pivot = entity_tiles[i];
	}
	return pivot;
}

iPoint j1EntityManager::FindClosestFreeTile(iPoint origin, iPoint destination) {
	int max = 1;
	iPoint possible_tile;
	iPoint closest_possible_tile = {-1,-1};
	int distance_to_origin = 100000;
	int distance_to_destination = 100000;

	while (max < 10) {
		for(int y = -max; y <= max; y++)
		{
			for(int x = -max; x <= max; x++)
			{
				if (x != 0 || y != 0) {
					possible_tile.x = destination.x + x;
					possible_tile.y = destination.y + y;

					if ((!occupied_tiles[possible_tile.x][possible_tile.y])&&(App->pathfinding->IsWalkable(possible_tile))) {
						if ((possible_tile.DistanceManhattan(destination) < distance_to_destination)&&(possible_tile.DistanceManhattan(origin) <= distance_to_origin)) {
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

iPoint j1EntityManager::FindFreeAdjacentTile(iPoint origin, iPoint destination) {
	iPoint closest_adjacent_tile = { -1,-1 };
	iPoint adjacent_tiles[4];
	int distance = 10000;

	//east
	adjacent_tiles[0] = { destination.x - 1, destination.y };
	//north
	adjacent_tiles[1] = { destination.x , destination.y -1 };
	//east
	adjacent_tiles[2] = { destination.x + 1, destination.y };
	//south
	adjacent_tiles[3] = { destination.x, destination.y + 1 };

	for (int i = 0; i < 4; i++)
	{
		if ((occupied_tiles[adjacent_tiles[i].x][adjacent_tiles[i].y] == false) && (adjacent_tiles[i].DistanceManhattan(origin) < distance))
			closest_adjacent_tile = adjacent_tiles[i];
	}

	return closest_adjacent_tile;
}

ResourceBuilding* j1EntityManager::GetClosestResourceBuilding(iPoint current_position, Resource resource_type) {
	ResourceBuilding* closest_building = nullptr;
	int min_distance = 10000;

	for(size_t i = 0; i < resource_buildings.size(); i++)
	{
		if ((resource_buildings[i]->quantity > 0)&&((resource_type == Resource::NO_TYPE)||(resource_buildings[i]->resource_type == resource_type)))
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

bool j1EntityManager::IsTileOccupied(iPoint position) {
	return occupied_tiles[position.x][position.y];
}

bool j1EntityManager::IsTileInPositionOccupied(fPoint position) {
	iPoint tile = App->map->fWorldToMap(position.x, position.y);
	return occupied_tiles[tile.x][tile.y];
}

void j1EntityManager::BubbleSortEntities() {
	BROFILER_CATEGORY("BubbleSortEntities", Profiler::Color::Blue)
	int n = entities.size();
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - i - 1; j++) {
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

	srand((unsigned int)time(NULL));

	for(int i = 0; i < 4; i++) {

		int randomIndex = rand() % 4;
		int temp = randomFaction[i];
		randomFaction[i] = randomFaction[randomIndex];
		randomFaction[randomIndex] = temp;
	}


	for(int i = 0; i < 4; i++)
		LOG("faction %i", randomFaction[i]);
}

void j1EntityManager::OnCommand(std::vector<std::string> command_parts) {
	if (command_parts[0] == "destroy_all_entities") {
		for(size_t i = 0; i < entities.size(); i++)
		{
			if (entities[i]->is_dynamic)
				entities[i]->to_delete = true;
		}
	}
}

void j1EntityManager::LoadUpgradeCosts(pugi::xml_node& config)
{
	pugi::xml_node boost_node = config.child("boost").first_child();
	Faction faction = NO_FACTION;
	std::string faction_name;

	for(int j = 0; j < 3; j++) {
		for(int i = 0; i < 4; i++) {
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
	
	char* buffer;
	int bytesFile = App->assetManager->Load("entities.xml", &buffer);
	pugi::xml_parse_result result = entities_file.load_buffer(buffer, bytesFile);
	RELEASE_ARRAY(buffer);

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
			else if (faction_string == "no_faction") faction = NO_FACTION;

			pugi::xml_node entity_node = faction_node.first_child();
			while (entity_node != nullptr)
			{
				std::string type_string = std::string(entity_node.name());

				if (faction != NO_FACTION) { //Load costs only for players
				//check type
					if (type_string == "melee") type = MELEE;
					else if (type_string == "ranged") type = RANGED;
					else if (type_string == "gatherer") type = GATHERER;
					//load attributes
					int food = entity_node.child("cost").attribute("food").as_int();
					int water = entity_node.child("cost").attribute("water").as_int();
					int time = entity_node.child("cost").attribute("time").as_int();

					if (time != 0) //Bug fix
						unit_data[faction][type] = { water, food, time }; //load into unit_data
				}

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
	for(size_t i = 0; i < resource_buildings.size(); i++)
	{
		if (resource_spot == resource_buildings[i]) {
			delete resource_buildings[i];
			resource_buildings[i] = nullptr;
			resource_buildings.erase(resource_buildings.cbegin() + i);
		}
	}
}

// Load Game State
bool j1EntityManager::Load(pugi::xml_node& data)
{
	DestroyAllEntitiesNow();
	App->ai_manager->CleanUp();
	App->ai_manager->Start();
	RestartOccupiedTiles();

	std::string player_faction = "NO_FACTION_ASSIGNED";
	Faction faction = NO_FACTION;
	std::string type_name = "NO_TYPE";
	std::string faction_name = "NO_FACTION";
	EntityType type = NO_TYPE;
	fPoint position = { -1.0f,-1.0f };
	iPoint current_tile = { 0,0 }, target_tile = { 0,0 };
	GenericPlayer* owner = nullptr;

	player_faction = data.child("player").attribute("player_faction").as_string();

	if (player_faction == "brotherhood") { App->player->faction = BROTHERHOOD; }
	else if (player_faction == "supermutant") { App->player->faction = MUTANT; }
	else if (player_faction == "ghoul") { App->player->faction = GHOUL; }
	else if (player_faction == "vault") { App->player->faction = VAULT; }


	pugi::xml_node iterator = data.first_child();
	while (iterator)
	{
		int current_health = 0;
		int upgrade_gath = 0;
		int upgrade_base = 0;
		int upgrade_health = 0;
		int upgrade_creat = 0;
		int upgrade_dama = 0;
		int upgrade_speed = 0;
		int width = 0, height = 0;

		bool dynamic_entity = true;

		type_name = iterator.attribute("type").as_string();
		faction_name = iterator.attribute("faction").as_string();

		if (type_name == "melee") { type = MELEE;}
		else if (type_name == "ranged") { type = RANGED;}
		else if (type_name == "gatherer") { type = GATHERER;}
		else if (type_name == "base") {
			type = BASE;
			dynamic_entity = false;
		}
		else if (type_name == "laboratory") {
			type = LABORATORY;
			dynamic_entity = false;
		}
		else if (type_name == "barrack") {
			type = BARRACK;
			dynamic_entity = false;
		}
		else if (type_name == "bighorner") { type = BIGHORNER; }
		else if (type_name == "braham") { type = BRAHAM; }
		else if (type_name == "deathclaw") { type = DEATHCLAW; }
		else if (type_name == "mr_handy") { type = MR_HANDY; }

		if (faction_name == "brotherhood") { faction = BROTHERHOOD; }
		else if (faction_name == "supermutant"){ faction = MUTANT; }
		else if (faction_name == "vault"){ faction = VAULT; }
		else if (faction_name == "ghoul") { faction = GHOUL; }
		else if (faction_name == "no_faction") { faction = NO_FACTION; }


		if (type_name == "base") {
			upgrade_gath =iterator.attribute("level_gatherer_resource_limit").as_int();
			upgrade_base = iterator.attribute("level_base_resource_limit").as_int();
		}
		else if (type_name == "laboratory") {
			upgrade_health = iterator.attribute("level_units_health").as_int();
			upgrade_creat = iterator.attribute("level_units_creation_time").as_int();
		}
		else if (type_name == "barrack") {
			upgrade_dama = iterator.attribute("level_units_damage").as_int();
			upgrade_speed = iterator.attribute("level_units_speed").as_int();
		}

		position.x = iterator.attribute("position_x").as_float();
		position.y = iterator.attribute("position_y").as_float();
		current_tile.x = iterator.attribute("current_tile_x").as_int();
		current_tile.y = iterator.attribute("current_tile_y").as_int();
		current_health = iterator.attribute("current_health").as_int();

		LOG("%f %f %i %i", position.x, position.y , current_tile.x, current_tile.y);
		if (dynamic_entity == false) {

			//create building
			StaticEntity* entity = dynamic_cast<StaticEntity*>(App->entities->CreateEntity(faction, type, current_tile.x, current_tile.y, App->scene->players[faction]));

			iPoint tile = current_tile;
			if (type_name == "base") {
				entity = App->scene->players[faction]->base;
				width = height = 4;
			}
			else if (type_name == "barrack") {
				if (App->scene->players[faction]->barrack[0] == nullptr) {
					App->scene->players[faction]->barrack[0] = entity;
				}
				else {
					App->scene->players[faction]->barrack[1] = entity;
				}
				width = 2;
				height = 4;
			}
			else if (type_name == "laboratory") {
				entity = App->scene->players[faction]->laboratory;
				width = height = 3;
			}

			for(int y = 0; y < height; y++)
			{
				for(int x = 0; x < width; x++)
				{
					tile.x = current_tile.x + x;
					tile.y = current_tile.y + y;
					entity->tiles.push_back(tile);
				}
			}

			entity->CalculateRenderAndSpawnPositions();

		}
		else if(dynamic_entity){

			if (faction_name == "no_faction") {
				CreateEntity(faction, type, current_tile.x, current_tile.y);
			}
			else {
				DynamicEntity* entity = dynamic_cast<DynamicEntity*>(CreateEntity(faction, type, current_tile.x, current_tile.y, App->scene->players[faction]));

			}

		}


		iterator = iterator.next_sibling();
	}



	LOG("%i", entities.size());

	return true;
}

// Save Game State
bool j1EntityManager::Save(pugi::xml_node& data) const
{
	for(size_t i = 0; i < entities.size(); i++)
	{
		pugi::xml_node entities_pugi = data.append_child("entity");
		entities_pugi.append_attribute("number") = i;

		if (entities[i]->type == MELEE) { entities_pugi.append_attribute("type") = "melee"; }
		else if (entities[i]->type == RANGED) { entities_pugi.append_attribute("type") = "ranged"; }
		else if (entities[i]->type == GATHERER) { entities_pugi.append_attribute("type") = "gatherer"; }
		else if (entities[i]->type == BASE) { entities_pugi.append_attribute("type") = "base"; }
		else if (entities[i]->type == LABORATORY) { entities_pugi.append_attribute("type") = "laboratory"; }
		else if (entities[i]->type == BARRACK) { entities_pugi.append_attribute("type") = "barrack"; }
		else if (entities[i]->type == BIGHORNER) { entities_pugi.append_attribute("type") = "bighorner"; }
		else if (entities[i]->type == BRAHAM) { entities_pugi.append_attribute("type") = "braham"; }
		else if (entities[i]->type == DEATHCLAW) { entities_pugi.append_attribute("type") = "deathclaw"; }
		else if (entities[i]->type == MR_HANDY) { entities_pugi.append_attribute("type") = "mr_handy"; }

		if (entities[i]->faction == BROTHERHOOD) { entities_pugi.append_attribute("faction") = "brotherhood"; }
		else if (entities[i]->faction == MUTANT) { entities_pugi.append_attribute("faction") = "supermutant"; }
		else if (entities[i]->faction == VAULT) { entities_pugi.append_attribute("faction") = "vault"; }
		else if (entities[i]->faction == GHOUL) { entities_pugi.append_attribute("faction") = "ghoul"; }
		else if (entities[i]->faction == NO_FACTION) { entities_pugi.append_attribute("faction") = "no_faction"; }



		entities_pugi.append_attribute("position_x") = entities[i]->position.x;
		entities_pugi.append_attribute("position_y") = entities[i]->position.y;
		entities_pugi.append_attribute("current_tile_x") = entities[i]->current_tile.x;
		entities_pugi.append_attribute("current_tile_y") = entities[i]->current_tile.y;
		entities_pugi.append_attribute("current_health") = entities[i]->current_health;


		if (entities[i]->type == BASE) {
			entities_pugi.append_attribute("level_gatherer_resource_limit") = App->entities->gatherer_resource_limit[entities[i]->faction].upgrade_num;
			entities_pugi.append_attribute("level_base_resource_limit") = App->entities->base_resource_limit[entities[i]->faction].upgrade_num;
		}

		if (entities[i]->type == LABORATORY) {
		entities_pugi.append_attribute("level_units_health") = App->entities->units_health[entities[i]->faction].upgrade_num;
		entities_pugi.append_attribute("level_units_creation_time") = App->entities->units_creation_time[entities[i]->faction].upgrade_num;
		}

		if (entities[i]->type == BARRACK) {
		entities_pugi.append_attribute("level_units_damage") = App->entities->units_damage[entities[i]->faction].upgrade_num;
		entities_pugi.append_attribute("level_units_speed") = App->entities-> units_speed[entities[i]->faction].upgrade_num;
		}

	}

	pugi::xml_node player_pugi = data.append_child("player");

	if (App->player->faction == BROTHERHOOD) { player_pugi.append_attribute("player_faction") = "brotherhood"; }
	else if (App->player->faction == MUTANT) { player_pugi.append_attribute("player_faction") = "supermutant"; }
	else if (App->player->faction == VAULT) { player_pugi.append_attribute("player_faction") = "vault"; }
	else if (App->player->faction == GHOUL) { player_pugi.append_attribute("player_faction") = "ghoul"; }

	LOG("%i", entities.size());

	return true;
}

iPoint j1EntityManager::FindSpawnPoint(int position_x, int position_y) {
	if (occupied_tiles[position_x][position_y]) {
		//There's another unit, let's find a new spawn point
		bool spawnPointFound = false;

		while (occupied_tiles[position_x][position_y]) {
			for(int k = 0; k < 10; k++) {
				for(int i = 0; i <= 5; i++) {
					if (spawnPointFound == false) {
						if (!occupied_tiles[position_x - i][position_y + k]) {
							position_x -= i;
							position_y += k;
							spawnPointFound = true;
						}
					}
				}
				if (spawnPointFound == false) {
					for(int j = 0; j <= 5; j++) {
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

int j1EntityManager::GetReferenceEntityID(Faction faction, EntityType type) {

	switch (faction)
	{
	case VAULT:
		if (type == MELEE) return 0;
		else if (type == RANGED) return 1;
		else if (type == GATHERER) return 2;
		else if (type == BASE) return 3;
		else if (type == LABORATORY) return 4;
		else if (type == BARRACK) return 5;
		break;
	case BROTHERHOOD:
		if (type == MELEE) return 6;
		else if (type == RANGED) return 7;
		else if (type == GATHERER) return 8;
		else if (type == BASE) return 9;
		else if (type == LABORATORY) return 10;
		else if (type == BARRACK) return 11;
		break;
	case MUTANT:
		if (type == MELEE) return 12;
		else if (type == RANGED) return 13;
		else if (type == GATHERER) return 14;
		else if (type == BASE) return 15;
		else if (type == LABORATORY) return 16;
		else if (type == BARRACK) return 17;
		break;
	case GHOUL:
		if (type == MELEE) return 18;
		else if (type == RANGED) return 19;
		else if (type == GATHERER) return 20;
		else if (type == BASE) return 21;
		else if (type == LABORATORY) return 22;
		else if (type == BARRACK) return 23;
		break;
	case NO_FACTION:
		if (type == BIGHORNER) return 24;
		else if (type == BRAHAM) return 25;
		else if (type == DEATHCLAW) return 26;
		else if (type == MR_HANDY) return 27;
		break;
	default:
		break;
	}
}

void j1EntityManager::SpawnAnimals() {

	int randomAnimal = 0;

	for(int i = 0; i < 15; i++)
	{
		srand((unsigned int)time(NULL)+i);
		randomAnimal = rand() % 2;

		if(randomAnimal == 1)
		{
			if(FindEntityByTile(App->map->spawnPoint[i]) == nullptr)
			{
				CreateEntity(NO_FACTION, BIGHORNER, App->map->spawnPoint[i].x, App->map->spawnPoint[i].y);
			}

		}
		else
		{
			if (FindEntityByTile(App->map->spawnPoint[i]) == nullptr)
			{
				CreateEntity(NO_FACTION, BRAHAM, App->map->spawnPoint[i].x, App->map->spawnPoint[i].y);
			}
		}
	}
}

ParticleSystem* j1EntityManager::CreateParticle(fPoint pos) {
	ParticleSystem* particleSystem = new ParticleSystem(pos.x, pos.y);

	j1Entity* ret = (j1Entity*)particleSystem;
	ret->to_delete = false;
	particles.push_back(ret);

	return particleSystem;
}

void j1EntityManager::DeleteParticles(){
	for(size_t i = 0; i < particles.size(); i++)
	{
		delete particles[i];
		particles[i] = nullptr;
	}
	entities.clear();
}

void j1EntityManager::ReleaseParticle(ParticleSystem* particle) {

	/*for(int i = 0; i < particles.size(); i++)
	{
		Deleting particles from vector
		if (particles[i] == particle)
		{
			delete entities[i];
			entities[i] = nullptr;
			entities.erase(entities.begin() + i);
		}
	}*/
}

void j1EntityManager::RestartOccupiedTiles() {
	for(int y = 0; y < 150; y++)
	{
		for(int x = 0; x < 150; x++)
		{
			occupied_tiles[x][y] = false;
		}
	}
}
