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
#include "Player.h"
#include "brofiler/Brofiler/Brofiler.h"
#include "MenuManager.h"
#include "j1Console.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

j1EntityManager::j1EntityManager(){
	name = ("entities");

	selected_unit_tex = nullptr;
	blocked_movement = false;

	unit_data[0] = { GHOUL, GATHERER, 40, 0 , 15};
	unit_data[1] = { GHOUL, MELEE,80, 60, 30 };
	unit_data[2] = { GHOUL, RANGED, 80, 80, 40 };
	unit_data[3] = { VAULT, GATHERER, 40, 0, 15 };
	unit_data[4] = { VAULT, MELEE, 60, 60, 30 };
	unit_data[5] = { VAULT, RANGED, 80, 80, 40 };
	unit_data[6] = { MUTANT, GATHERER, 50, 0, 15 };
	unit_data[7] = { MUTANT, MELEE, 80, 100, 30 };
	unit_data[8] = { MUTANT, RANGED, 80, 120, 40 };
	unit_data[9] = { BROTHERHOOD, GATHERER, 50, 0, 15 };
	unit_data[10] = { BROTHERHOOD, MELEE,  100, 80, 30  };
	unit_data[11] = { BROTHERHOOD, RANGED, 100, 100, 40 };

}

j1EntityManager::~j1EntityManager(){}

j1Entity* j1EntityManager::CreateEntity(Faction faction, EntityType type, int position_x, int position_y){
	BROFILER_CATEGORY("EntityCreation", Profiler::Color::Linen)
	//static_assert(EntityType::UNKNOWN == 4, "code needs update");

	j1Entity* entity = nullptr;

	if ((type == MELEE) || (type == RANGED) || (type == GATHERER)) {
		entity = new DynamicEntity(faction, type);
		entity->is_dynamic = true;

		entity->reference_entity = reference_entities[faction][type];

		if (entity != NULL)
		{
			entity->faction = faction;

			//If there's another unit in that tile, we find a new spawn point
			if (FindEntityByTile({ position_x,position_y }) == nullptr) {
				//We can spawn here
				entity->current_tile.x = position_x;
				entity->current_tile.y = position_y;
			}
			else {
				//There's another unit, let's find a new spawn point
				bool spawnPointFound = false;

				while (FindEntityByTile({ position_x,position_y}) != nullptr) {
					for (int k = 0; k < 10; k++) {
						for (int i = 0; i <= 5; i++) {
							if (spawnPointFound == false) {
								if (FindEntityByTile({ position_x - i,position_y + k}) == nullptr) {
									position_x -= i;
									position_y += k;
									entity->current_tile.x = position_x;
									entity->current_tile.y = position_y;
									spawnPointFound = true;
								}
							}
						}
						if (spawnPointFound == false) {
							for (int j = 0; j <= 5; j++) {
								if (spawnPointFound == false) {
									if (FindEntityByTile({ position_x + k,position_y - j }) == nullptr) {
										position_y -= j;
										position_x += k;
										entity->current_tile.x = position_x;
										entity->current_tile.y = position_y;
										spawnPointFound = true;
									}
								}
							}
						}
						//First line completed. Next look for spawn points in the next line
					}
					//We didn't find a free spawn point, so we spawn in the same tile as other unit
					entity->current_tile.x = position_x;
					entity->current_tile.y = position_y;

					break;
				}
			}


			entity->position = App->map->fMapToWorld(entity->current_tile.x, entity->current_tile.y);
			entity->position.x += 32;
			entity->position.y += 32;

			if (entity->reference_entity != nullptr){
				entities.push_back(entity);
				entity->LoadReferenceData();
			}
		}
	}
	else if ((type == BASE) || (type == LABORATORY) || (type == BARRACK))
	{
		entity = new StaticEntity(faction, type);
		entity->is_dynamic = false;
		entity->reference_entity = reference_entities[faction][type];

		if (entity != NULL)
		{
			entity->faction = faction;
			entity->current_tile.x = position_x;
			entity->current_tile.y = position_y;

			entity->position = App->map->fMapToWorld(entity->current_tile.x, entity->current_tile.y);

			if (entity->reference_entity != nullptr) {
				entities.push_back(entity);
				entity->LoadReferenceData();
			}

			//Add spawn position for units
			//TODO: It works for GHOULS, needs to be adapted for all the factions
			if(type == BASE)
				entity->spawnPosition = { entity->current_tile.x + 14, entity->current_tile.y + 9 };
			else if(type == BARRACK)
				entity->spawnPosition = { entity->current_tile.x + 14, entity->current_tile.y + 9 };
		}
	}

	return entity;
}

bool j1EntityManager::Awake(pugi::xml_node& config){
	bool ret = true;

	config_data = config;

	//automatic entities loading
	for (int faction = VAULT; faction < NO_FACTION; faction++)
	{
		for (int type = MELEE; type < NO_TYPE; type++)
		{
			reference_entities[faction][type] = nullptr;
			reference_entities[faction][type] = CreateEntity((Faction)faction, (EntityType)type, faction, type);
		}
	}

	ret = LoadReferenceEntityData();

	return ret;
}

bool j1EntityManager::Start() {
	BROFILER_CATEGORY("EntitiesStart", Profiler::Color::Linen)
	bool ret = true;

	App->console->CreateCommand("destroy_all_entities", "remove all dynamic entities", (j1Module*)this);

	//load all textures and animations
	for (int faction = VAULT; faction < NO_FACTION; faction++)
	{
		for (int type = MELEE; type <= BASE; type++)
		{
			reference_entities[faction][type]->LoadAnimations();
		}

		reference_entities[faction][BARRACK]->texture = reference_entities[faction][BASE]->texture;
		reference_entities[faction][LABORATORY]->texture = reference_entities[faction][BASE]->texture;
	}

	//Ghouls
	//reference_entities[GHOUL][BASE]->LoadAnimations("Ghouls/Ghouls_Base");
	reference_entities[GHOUL][BARRACK]->LoadAnimations();
	reference_entities[GHOUL][BARRACK]->texture = reference_entities[GHOUL][BASE]->texture;

	return ret;
}

bool j1EntityManager::CleanUp()
{
	bool ret = true;

	for (int faction = VAULT; faction < NO_FACTION; faction++)
	{
		for (int type = MELEE; type <= BASE; type++)
		{
			App->tex->UnLoad(reference_entities[faction][type]->texture);
		}
	}

	entities.clear();
	return ret;
}

bool j1EntityManager::PreUpdate()
{
	//BROFILER_CATEGORY("EntitiesPreUpdate", Profiler::Color::Bisque)

	bool ret = true;

	return ret;
}

bool j1EntityManager::Update(float dt)
{
	BROFILER_CATEGORY("EntitiesUpdate", Profiler::Color::GreenYellow)
	bool ret = true;

	for (int i = 0; i < entities.size(); i++)
	{
		entities[i]->Update(dt);
	}

	return ret;
}

bool j1EntityManager::PostUpdate()
{
	BROFILER_CATEGORY("EntitiesPostUpdate", Profiler::Color::Orange)
	bool ret = true;
	SDL_Rect tex_rect = {64,0,64,64 };
	iPoint tex_position;

	//debug kind of entity and path
	if (App->render->debug) {
		for (int i = 0; i < entities.size(); i++)
		{
			if (entities[i]->is_dynamic)
			{
				//Render path
				if (App->render->debug)
				{
					if (entities[i]->path_to_target.size() > 0)
					{
						for (uint j = 0; j < entities[i]->path_to_target.size(); ++j)
						{
							iPoint pos = App->map->MapToWorld(entities[i]->path_to_target[j].x, entities[i]->path_to_target[j].y);
							SDL_Rect debug_rect = { 192, 0, 64,64 };
							App->render->Blit(App->render->debug_tex, pos.x, pos.y, &debug_rect);
						}
					}
				}

				//dynamic entities debug
				//change color depending on if it's an ally or an enemy
				SDL_Rect rect;
				if(App->player->faction == entities[i]->faction ) rect = { 0,0,64,64 };
				else rect = { 64,0,64,64 };

				tex_position = App->map->MapToWorld(entities[i]->current_tile.x, entities[i]->current_tile.y);
				App->render->Blit(App->render->debug_tex, tex_position.x, tex_position.y, &rect);
			}
			else
			{
				StaticEntity* static_entity = (StaticEntity*)entities[i];
				for (int j = 0; j < static_entity->tiles.size(); j++)
				{
					SDL_Rect rect = { 256,0,64,64 };
					tex_position = App->map->MapToWorld(static_entity->tiles[j].x, static_entity->tiles[j].y);
					App->render->Blit(App->render->debug_tex, tex_position.x, tex_position.y, &rect);
				}
			}
		}
		for (int i = 0; i < resource_buildings.size(); i++)
		{
			for (int j = 0; j < resource_buildings[i]->tiles.size(); j++)
			{
				SDL_Rect rect = { 128,0,64,64 };
				tex_position = App->map->MapToWorld(resource_buildings[i]->tiles[j].x, resource_buildings[i]->tiles[j].y);
				App->render->Blit(App->render->debug_tex, tex_position.x, tex_position.y, &rect);
			}
		}
	}

	if (App->player->selected_entity != nullptr)
	{
		//Selected entity is a unit
		if (App->player->selected_entity->is_dynamic == true) {
			tex_position = App->map->MapToWorld(App->player->selected_entity->current_tile.x, App->player->selected_entity->current_tile.y);
			App->render->Blit(App->render->debug_tex, tex_position.x, tex_position.y, &tex_rect);
		}
		//Selected entity is a building
		else {
			StaticEntity* static_entity = (StaticEntity*)App->player->selected_entity;
			for (int j = 0; j < static_entity->tiles.size(); j++)
			{
				tex_position = App->map->MapToWorld(static_entity->tiles[j].x, static_entity->tiles[j].y);
				App->render->Blit(App->render->debug_tex, tex_position.x, tex_position.y, &tex_rect);
			}

			//Create HUD for the building
			switch (static_entity->faction) {
			case GHOUL:
				if (static_entity->type == BASE) {

					if (count == 0) {

						App->menu_manager->CreateGhouls_Base();
						count++;
						LOG("%i", count);
					}

				}
				else if (static_entity->type == BARRACK) {

					if (count == 0) {

						App->menu_manager->CreateGhouls_Barrack();
						count++;
					}

				}
				else if (static_entity->type == LABORATORY) {

					if (count == 0) {
						App->menu_manager->CreateGhouls_Lab();
						count++;
					}

				}
				break;
			case BROTHERHOOD:
				if (static_entity->type == BASE) {

					if (count == 0) {
						count++;
						App->menu_manager->CreateBrotherHood_Base();
					}

				}
				else if (static_entity->type == BARRACK) {

					if (count == 0) {

						count++;
						App->menu_manager->CreateBrotherHood_Barrack();

					}

				}
				else if (static_entity->type == LABORATORY) {

					if (count == 0) {

						count++;
						App->menu_manager->CreateBrotherHood_Lab();

					}
				}
				break;

			case VAULT:

				if (static_entity->type == BASE) {

					if (count == 0) {

						count++;
						App->menu_manager->CreateVault_Base();

					}
				}
				else if (static_entity->type == BARRACK){

					if (count == 0) {

						count++;
						App->menu_manager->CreateVault_Barrack();

					}

				}
				else if (static_entity->type == LABORATORY) {

					if (count == 0) {

						count++;
						App->menu_manager->CreateVault_Lab();

					}

				}
				break;
			case MUTANT:
				if (static_entity->type == BASE) {

					if (count == 0) {

						count++;
						App->menu_manager->CreateSuperMutants_Base();

					}

				}
				else if (static_entity->type == BARRACK) {

					if (count == 0) {

						count++;
						App->menu_manager->CreateSuperMutants_Barrack();

					}

				}
				else if (static_entity->type == LABORATORY) {

					if (count == 0) {

						count++;
						App->menu_manager->CreateSuperMutants_Lab();

					}

				}
				break;
			}
		}
	}

	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i]->to_destroy)
		{
			entities.erase(entities.begin() + i);
		}
		else
		{
			SortEntities();
			entities[i]->PostUpdate();
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
			if (faction_string == "vault")
				faction = VAULT;
			else if (faction_string == "brotherhood")
				faction = BROTHERHOOD;
			else if (faction_string == "mutants")
				faction = MUTANT;
			else if (faction_string == "ghouls")
				faction = GHOUL;

			pugi::xml_node entity_node = faction_node.first_child();
			while (entity_node != nullptr)
			{
				std::string type_string = std::string(entity_node.name());

				//check type
				if (type_string == "melee")
					type = MELEE;
				else if (type_string == "ranged")
					type = RANGED;
				else if (type_string == "gatherer")
					type = GATHERER;
				else if (type_string == "base")
					type = BASE;
				else if (type_string == "barrack")
					type = BARRACK;
				else if (type_string == "laboratory")
					type = LABORATORY;

				//load attributes
				int health = entity_node.attribute("health").as_int();
				int damage = entity_node.attribute("damage").as_int();
				int speed = entity_node.attribute("speed").as_int();

				//load into reference entities
				reference_entities[faction][type]->max_health = health;
				reference_entities[faction][type]->damage = damage;
				reference_entities[faction][type]->speed.x = speed * 1.0f;
				reference_entities[faction][type]->speed.y = speed * 0.5f;

				entity_node = entity_node.next_sibling();
			}
			faction_node = faction_node.next_sibling();
		}
		type_node = type_node.next_sibling();
		faction_node = type_node.first_child();
	}

	return ret;
}

void j1EntityManager::SortEntities() {
	int i, j;
	int n = entities.size();

	for (i = 0; i < n - 1; i++) {
		for (j = 0; j < n - i - 1; j++) {
			if (entities[j]->render_position.y > entities[j + 1]->render_position.y)
				Swap(j, j + 1);
		}
	}
}

void j1EntityManager::Swap(int i, int j)
{
	int temp = i;
	j1Entity* aux = entities[i];
	entities[i] = entities[j];
	entities[j] = aux;
}

void j1EntityManager::DestroyEntity(j1Entity* entity) {
	delete entity;
}

void j1EntityManager::DestroyAllEntities() {
	for (int i = REFERENCE_ENTITIES; i < entities.size(); i++)
	{
		entities[i]->to_destroy = true;
	}
}

j1Entity* j1EntityManager::FindEntityByType(Faction faction, EntityType type) {
	for (int i = 0; i < entities.size(); i++)
	{
		if ((entities[i]->faction == faction)&&(entities[i]->type == type))
		{
			return entities[i];
		}
	}
	return nullptr;
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
				entities[i]->to_destroy = true;
		}
	}
}
