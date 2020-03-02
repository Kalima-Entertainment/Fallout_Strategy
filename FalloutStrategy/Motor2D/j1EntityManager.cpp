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
#include "j1Entity.h"
#include "DynamicEntity.h"
#include "StaticEntity.h"
//#include "brofiler/Brofiler/Brofiler.h"


j1EntityManager::j1EntityManager(){
	name.create("entities");
}


j1EntityManager::~j1EntityManager(){
}

j1Entity* j1EntityManager::CreateDynamicEntity(Faction faction, Troop troop, int position_x, int position_y){
	//BROFILER_CATEGORY("EntityCreation", Profiler::Color::Linen)
	//static_assert(EntityType::UNKNOWN == 4, "code needs update");

	j1Entity* entity = nullptr;
	switch (troop)
	{
	case Troop::MELEE:
		entity = new DynamicEntity(faction, Troop::MELEE);
		if		(faction == Faction::VAULT)			entity->reference_entity = reference_vault_melee;
		else if (faction == Faction::BROTHERHOOD)	entity->reference_entity = reference_brotherhood_melee;
		else if (faction == Faction::MUTANT)		entity->reference_entity = reference_mutant_melee;
		else if (faction == Faction::GHOUL)			entity->reference_entity = reference_ghoul_melee;
		break;
	case Troop::RANGED:
		entity = new DynamicEntity(faction, Troop::RANGED);
		if		(faction == Faction::VAULT)			entity->reference_entity = reference_vault_range;
		else if (faction == Faction::BROTHERHOOD)	entity->reference_entity = reference_brotherhood_range;
		else if (faction == Faction::MUTANT)		entity->reference_entity = reference_mutant_range;
		else if (faction == Faction::GHOUL)			entity->reference_entity = reference_ghoul_range;
		break;
	case Troop::GATHERER:
		entity = new DynamicEntity(faction, Troop::GATHERER);
		if		(faction == Faction::VAULT)			entity->reference_entity = reference_vault_gatherer;
		else if (faction == Faction::BROTHERHOOD)	entity->reference_entity = reference_brotherhood_gatherer;
		else if (faction == Faction::MUTANT)		entity->reference_entity = reference_mutant_gatherer;
		else if (faction == Faction::GHOUL)			entity->reference_entity = reference_ghoul_gatherer;
		break;
	default:
		break;
	}

	if (entity != NULL)
	{
		entity->faction = faction;
		entity->position.x = entity->initialPosition.x = position_x;
		entity->position.y = entity->initialPosition.y = position_y;

		if (entity->reference_entity != nullptr)
		{
			entity->LoadReferenceData();
		}
	}
	   
	if (entity != nullptr) 
		entities.push_back(entity);

	return entity;
}

j1Entity* j1EntityManager::CreateStaticEntity(Faction faction, BuildingType buiding_type, int position_x, int position_y) {
	//BROFILER_CATEGORY("EntityCreation", Profiler::Color::Linen)
	//static_assert(EntityType::UNKNOWN == 4, "code needs update");
	j1Entity* entity = nullptr;

	switch (buiding_type)
	{
	case BuildingType::BASE:
		break;
	case BuildingType::WAREHOUSE:
		break;
	case BuildingType::BARRACK:
		break;
	default:
		break;
	}

	if (entity != NULL)
	{
		entity->faction = faction;
		entity->position.x = entity->initialPosition.x = position_x;
		entity->position.y = entity->initialPosition.y = position_y;
	}

	if (entity != nullptr)
		entities.push_back(entity);
	return entity;
}

void j1EntityManager::DestroyEntity(j1Entity* entity){
	//BROFILER_CATEGORY("EntityDestruction", Profiler::Color::Orange)
	/*
		p2List_item<j1Entity*>* item;

	if (entity != nullptr) {
		item = entities.At(entities.find(entity));
		if (entity->collider != nullptr)
		{
			entity->collider->to_delete = true;
			entity->collider = nullptr;
		}
		entities.del(item);;
	}
	*/
}

void j1EntityManager::DestroyAllEntities() {
	/*
	for (auto item = entities.begin(); item != entities.end(); ++item)
	{
		DestroyEntity(item->data);
	}
	*/
}

bool j1EntityManager::Awake(pugi::xml_node& config){
	bool ret = true;

	config_data = config;

	return ret;
}

bool j1EntityManager::Start()
{
	bool ret = true;
	//create reference entities

	//load all textures
	//Vault Dwellers
	reference_vault_melee = CreateDynamicEntity(VAULT, Troop::MELEE, 10, 10);
	reference_vault_range = CreateDynamicEntity(VAULT, Troop::RANGED, 10, 20);
	reference_vault_gatherer = CreateDynamicEntity(VAULT, Troop::GATHERER, 70, 30);

	reference_vault_melee->LoadAnimations("VaultDwellers/Vault_Dweller_Melee");
	reference_vault_range->LoadAnimations("VaultDwellers/Vault_Dweller_Ranged");
	reference_vault_gatherer->LoadAnimations("VaultDwellers/Vault_Dweller_Gatherer");

	//Brotherhood
	reference_brotherhood_melee = CreateDynamicEntity(BROTHERHOOD, Troop::MELEE, 40, 40);
	reference_brotherhood_range = CreateDynamicEntity(BROTHERHOOD, Troop::RANGED, 50, 50);
	reference_brotherhood_gatherer = CreateDynamicEntity(BROTHERHOOD, Troop::GATHERER, 60, 60);

	reference_brotherhood_melee->LoadAnimations("Brotherhood/Brotherhood_melee");
	reference_brotherhood_range->LoadAnimations("Brotherhood/Brotherhood_Ranged");
	reference_brotherhood_gatherer->LoadAnimations("Brotherhood/Brotherhood_gatherer");

	//Super Mutants
	reference_mutant_melee = CreateDynamicEntity(MUTANT, Troop::MELEE, 70, 70);
	reference_mutant_range = CreateDynamicEntity(MUTANT, Troop::RANGED, 80, 80);
	reference_mutant_gatherer = CreateDynamicEntity(MUTANT, Troop::GATHERER, 90, 90);

	reference_mutant_melee->LoadAnimations("SuperMutant/SuperMutant_Mele");
	reference_mutant_range->LoadAnimations("SuperMutant/SuperMutant_Ranged");
	reference_mutant_gatherer->LoadAnimations("SuperMutant/SuperMutant_Gatherer");

	//Ghouls
	reference_ghoul_melee = CreateDynamicEntity(GHOUL, Troop::MELEE, 100, 100);
	reference_ghoul_range = CreateDynamicEntity(GHOUL, Troop::RANGED, 110, 110);
	reference_ghoul_gatherer = CreateDynamicEntity(GHOUL, Troop::GATHERER, 120, 120);

	reference_ghoul_melee->LoadAnimations("Ghouls/Ghouls_Melee");
	reference_ghoul_range->LoadAnimations("Ghouls/Ghouls_Ranged");
	reference_ghoul_gatherer->LoadAnimations("Ghouls/Ghouls_Gatherer");

	return ret;
}

bool j1EntityManager::CleanUp()
{
	bool ret = true;
	/*
	App->tex->UnLoad(trap_texture);
	trap_texture = nullptr;

	App->tex->UnLoad(reference_walking_enemy->texture);
	reference_walking_enemy->texture = nullptr;

	App->tex->UnLoad(reference_flying_enemy->texture);
	reference_flying_enemy->texture = nullptr;

	App->tex->UnLoad(reference_walking_enemy2->texture);
	reference_walking_enemy2->texture = nullptr;

	//destroy all entities
	for (p2List_item<j1Entity*>* entity = entities.start; entity != nullptr; entity = entity->next)
	{
		entity->data->DestroyEntity(entity->data);
	}
	*/
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
	for (int i = REFERENCE_ENTITIES; i < entities.size(); i++)
	{
		entities[i]->PostUpdate();
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