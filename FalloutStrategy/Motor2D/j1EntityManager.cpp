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

//#include "brofiler/Brofiler/Brofiler.h"

j1EntityManager::j1EntityManager(){
	name.create("entities");

	selected_unit_tex = nullptr;
	blocked_movement = false;
	total_entities = 0;
}

j1EntityManager::~j1EntityManager(){}

j1Entity* j1EntityManager::CreateEntity(Faction faction, EntityType type, int position_x, int position_y){
	//BROFILER_CATEGORY("EntityCreation", Profiler::Color::Linen)
	//static_assert(EntityType::UNKNOWN == 4, "code needs update");

	j1Entity* entity = nullptr;

	if ((type == MELEE) || (type == RANGED) || (type == GATHERER)) {
		entity = new DynamicEntity(faction, type);
		entity->is_dynamic = true;
	}
	else
	{
		entity = new StaticEntity(faction, type);
	}

	entity->reference_entity = reference_entities[faction][type];

	if (entity != NULL)
	{
		entity->faction = faction;
		entity->current_tile.x = position_x;
		entity->current_tile.y = position_y;

		entity->position = App->map->fMapToWorld(entity->current_tile.x, entity->current_tile.y);
		entity->position.x += 32;
		entity->position.y += 32;

		if (entity->reference_entity != nullptr)
		{
			entity->LoadReferenceData();
		}
	}
	   
	if (entity != nullptr) {
		entities.push_back(entity);
		total_entities++;
	}

	return entity;
}

void j1EntityManager::DestroyEntity(j1Entity* entity) {
	delete entity;
	total_entities--;
}

void j1EntityManager::DestroyAllEntities() {
	for (int i = REFERENCE_ENTITIES; i < total_entities; i++)
	{
		entities[i]->to_destroy = true;
	}
}

j1Entity* j1EntityManager::FindEntityByTile(iPoint tile) {
	for (int i = REFERENCE_ENTITIES; i < entities.size(); i++)
	{
		if (entities[i]->current_tile == tile)
		{
			return entities[i];
		}
	}
	return nullptr;
}

bool j1EntityManager::Awake(pugi::xml_node& config){
	bool ret = true;

	config_data = config;

	//manual entities loading
	/*
	//Load reference data
	//Vault Dwellers
	reference_entities[VAULT][MELEE] = CreateEntity(VAULT, MELEE, 0, 0);
	reference_entities[VAULT][RANGED] = CreateEntity(VAULT, RANGED, 1, 0);
	reference_entities[VAULT][GATHERER] = CreateEntity(VAULT, GATHERER, 2, 0);
	//TODO: Add static entities 
	reference_entities[VAULT][BASE] = CreateEntity(VAULT, BASE, 3, 0);

	//Brotherhood
	reference_entities[BROTHERHOOD][MELEE] = CreateEntity(BROTHERHOOD, MELEE, 4, 4);
	reference_entities[BROTHERHOOD][RANGED] = CreateEntity(BROTHERHOOD, RANGED, 5, 5);
	reference_entities[BROTHERHOOD][GATHERER] = CreateEntity(BROTHERHOOD, GATHERER, 6, 6);

	//Super Mutants
	reference_entities[MUTANT][MELEE] = CreateEntity(MUTANT, MELEE, 7, 7);
	reference_entities[MUTANT][RANGED] = CreateEntity(MUTANT, RANGED, 8, 8);
	reference_entities[MUTANT][GATHERER] = CreateEntity(MUTANT, GATHERER, 9, 9);

	//Ghouls
	reference_entities[GHOUL][MELEE] = CreateEntity(GHOUL, MELEE, 10, 10);
	reference_entities[GHOUL][RANGED] = CreateEntity(GHOUL, RANGED, 11, 11);
	reference_entities[GHOUL][GATHERER] = CreateEntity(GHOUL, GATHERER, 12, 12);
	*/

	//automatic entities loading
	for (int faction = VAULT; faction < NO_FACTION; faction++)
	{
		for (int type = MELEE; type < NO_TYPE; type++)
		{
			reference_entities[faction][type] = CreateEntity((Faction)faction, (EntityType)type, faction, type);
		}
	}

	ret = LoadReferenceEntityData();

	return ret;
}

bool j1EntityManager::Start()
{
	bool ret = true;
	//create reference entities

	//load all textures

	//Vault Dwellers
	reference_entities[VAULT][MELEE]->LoadAnimations("VaultDwellers/Vault_Dweller_Melee");
	reference_entities[VAULT][RANGED]->LoadAnimations("VaultDwellers/Vault_Dweller_Ranged");
	reference_entities[VAULT][GATHERER]->LoadAnimations("VaultDwellers/Vault_Dweller_Gatherer");
	reference_entities[VAULT][BASE]->LoadAnimations("VaultDwellers/Vault_Dweller_Base");

	//Brotherhood
	//reference_entities[BROTHERHOOD][MELEE]->LoadAnimations("Brotherhood/Brotherhood_melee");
	//reference_entities[BROTHERHOOD][RANGED]->LoadAnimations("Brotherhood/Brotherhood_Ranged");
	//reference_entities[BROTHERHOOD][GATHERER]->LoadAnimations("Brotherhood/Brotherhood_gatherer");

	//Super Mutants
	//reference_entities[MUTANT][MELEE]->LoadAnimations("SuperMutant/SuperMutant_Mele");
	reference_entities[MUTANT][RANGED]->LoadAnimations("SuperMutant/SuperMutant_Ranged");
	//reference_entities[MUTANT][GATHERER]->LoadAnimations("SuperMutant/SuperMutant_Gatherer");

	//Ghouls
	//reference_entities[GHOUL][MELEE]->LoadAnimations("Ghouls/Ghouls_Melee");
	//reference_entities[GHOUL][RANGED]->LoadAnimations("Ghouls/Ghouls_Ranged");
	//reference_entities[GHOUL][GATHERER]->LoadAnimations("Ghouls/Ghouls_Gatherer");
	//reference_entities[GHOUL][BASE]->LoadAnimations("Ghouls/Ghouls_Base");
	reference_entities[GHOUL][BASE]->LoadAnimations("Ghouls/Ghouls_Buildings");

	debug_tex = App->tex->Load("maps/meta.png");
	selected_unit_tex = App->tex->Load("maps/meta2.png");

	return ret;
}

bool j1EntityManager::CleanUp()
{
	bool ret = true;

	for (int i = 0; i < REFERENCE_ENTITIES; i++)
	{
		if (i < REFERENCE_ENTITIES)
		{
			App->tex->UnLoad(entities[i]->texture);
		}
		delete entities[i];
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
	//BROFILER_CATEGORY("EntitiesUpdate", Profiler::Color::Bisque)
	bool ret = true;

	for (int i = REFERENCE_ENTITIES; i < entities.size(); i++)
	{
		entities[i]->Update(dt);
	}
	return ret;
}

bool j1EntityManager::PostUpdate()
{
	//BROFILER_CATEGORY("EntitiesPostUpdate", Profiler::Color::Bisque)
	bool ret = true;
	SDL_Rect tex_rect = {64,0,64,64 };
	iPoint tex_position;

	if (App->player->selected_entity != nullptr)
	{
		tex_position = App->map->MapToWorld(App->player->selected_entity->current_tile.x, App->player->selected_entity->current_tile.y);
		App->render->Blit(selected_unit_tex, tex_position.x, tex_position.y, &tex_rect);
	}

	for (int i = REFERENCE_ENTITIES; i < total_entities; i++)
	{
		if (entities[i]->to_destroy)
		{
			entities.erase(entities.begin() + i);
		}
		else
		{
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

	pugi::xml_node dynamic_node = entities_node.child("dynamic");
	pugi::xml_node faction_node;
	Faction faction = NO_FACTION;
	EntityType type = NO_TYPE;

	//load dynamic entities
	faction_node = dynamic_node.first_child();
	while (faction_node != nullptr)
	{
		p2SString faction_string(faction_node.name());

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
			p2SString type_string(entity_node.name());

			//check type
			if (type_string == "melee")
				type = MELEE;
			else if (type_string == "ranged")
				type = RANGED;
			else if (type_string == "gatherer")
				type = GATHERER;
			
			//load attributes
			int health = entity_node.attribute("health").as_int();
			int damage = entity_node.attribute("damage").as_int();
			int speed = entity_node.attribute("speed").as_int();

			//load into reference entities
			reference_entities[faction][type]->max_health = health;
			reference_entities[faction][type]->damage = damage;
			reference_entities[faction][type]->speed.x = speed * 0.01;
			reference_entities[faction][type]->speed.y = speed * 0.005f;

			entity_node = entity_node.next_sibling();
		}
		faction_node = faction_node.next_sibling();
	}

	return ret;
}

/*
void j1EntityManager::RellocateEntities() {
	for (p2List_item<j1Entity*>* entity = entities.start; entity != nullptr; entity = entity->next)
	{
		entity->data->position = entity->data->initialPosition;
		entity->data->going_after_player = false;
	}
}

bool j1EntityManager::Load(pugi::xml_node& data)
{
	bool ret = true;
	/*
	pugi::xml_node entity_node = data.first_child();

	DestroyAllEntities();

	while (entity_node != nullptr)
	{
		p2SString entity_name(entity_node.name());
		int x_position = entity_node.attribute("position_x").as_int();
		int y_position = entity_node.attribute("position_y").as_int();

		if (entity_name == "player") {
			player->position.x = x_position;
			player->position.y = y_position;
		}

		if (entity_name == "walking_enemy") 
			CreateEntity(EntityType::WALKING_ENEMY, x_position, y_position);

		if (entity_name == "flying_enemy") 
			CreateEntity(EntityType::FLYING_ENEMY, x_position, y_position);

		if (entity_name == "walking_enemy2")
			CreateEntity(EntityType::WALKING_ENEMY2, x_position, y_position);

		entity_node = entity_node.next_sibling();
	}
	return ret;
}


bool j1EntityManager::Save(pugi::xml_node& data) const
{
	bool ret = true;
	p2List_item<j1Entity*>* item;
	
	for (item = entities.start; item != nullptr; item = item->next)
	{
		pugi::xml_node child = data.append_child(item->data->name.GetString());
		child.append_attribute("position_x") = item->data->position.x;
		child.append_attribute("position_y") = item->data->position.y;
	}
	
	return ret;
}


bool j1EntityManager::CheckpointSave() {
	bool ret = true;
	LOG("Checkpoint triggered");
	
	// xml object were we will store all data
	pugi::xml_document data;
	pugi::xml_node root;
	pugi::xml_node entities_node;

	root = data.append_child("Checkpoint");
	ret = App->render->Save(root.append_child("render"));
	entities_node = root.append_child("entities");

		p2List_item<j1Entity*>* entity;
		for (entity = entities.start; entity != nullptr; entity = entity->next)
		{
			pugi::xml_node child = entities_node.append_child(entity->data->name.GetString());
			if (entity->data == player)
			{
				child.append_attribute("position_x") = entity->data->position.x;
				child.append_attribute("position_y") = entity->data->position.y;
			}
			else
			{
				child.append_attribute("position_x") = entity->data->initialPosition.x;
				child.append_attribute("position_y") = entity->data->initialPosition.y;
			}
		}

	if (ret == true)
	{
		data.save_file(App->checkpoint_save.GetString());
		LOG("... finished saving", );
	}
	
	data.reset();
	return ret;
}

bool j1EntityManager::CheckpointLoad()
{
	bool ret = true;

	pugi::xml_document doc;
	pugi::xml_node root;

	DestroyAllEntities();

	pugi::xml_parse_result result = doc.load_file(App->checkpoint_save.GetString());

	if (result == NULL) LOG("Error loading checkpoint data from %s", App->checkpoint_save.GetString());

	else {
		root = doc.first_child();
		App->render->camera.x = root.child("render").child("camera").attribute("x").as_int();
		App->render->camera.y = root.child("render").child("camera").attribute("y").as_int();

		pugi::xml_node entity_node = root.child("entities").first_child();

		while (entity_node != nullptr)
		{
			p2SString entity_name(entity_node.name());
			int x_position = entity_node.attribute("position_x").as_int();
			int y_position = entity_node.attribute("position_y").as_int();

			if (entity_name == "player") {
				player->position.x = x_position;
				player->position.y = y_position;
				player->collider->SetPos(player->position.x, player->position.y);
			}

			if (entity_name == "walking_enemy")
				CreateEntity(EntityType::WALKING_ENEMY, x_position, y_position);

			if (entity_name == "flying_enemy")
				CreateEntity(EntityType::FLYING_ENEMY, x_position, y_position);

			if (entity_name == "walking_enemy2")
				CreateEntity(EntityType::WALKING_ENEMY2, x_position, y_position);

			entity_node = entity_node.next_sibling();
		}
	}
	return ret;
}
*/