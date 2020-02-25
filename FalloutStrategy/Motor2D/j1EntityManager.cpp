#include "j1EntityManager.h"
#include "j1App.h"
#include "j1Player.h"
#include "p2Log.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Collision.h"
#include "j1Window.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Audio.h"
#include "j1UI.h"
#include "j1Particles.h"
#include "j1WalkingEnemy.h"
#include "j1WalkingEnemy2.h"
#include "j1FlyingEnemy.h"
#include "j1Trap.h"
#include "brofiler/Brofiler/Brofiler.h"


j1EntityManager::j1EntityManager(){
	name.create("entities");
}


j1EntityManager::~j1EntityManager(){
}

j1Entity* j1EntityManager::CreateEntity(EntityType type, int position_x, int position_y){
	BROFILER_CATEGORY("EntityCreation", Profiler::Color::Linen)
	//static_assert(EntityType::UNKNOWN == 4, "code needs update");
	j1Entity* entity = nullptr;
	switch (type)
	{
	case EntityType::PLAYER:
		entity = new j1Player();
		break;
	case EntityType::WALKING_ENEMY:
		entity = new j1WalkingEnemy();
		break;
	case EntityType::WALKING_ENEMY2:
		entity = new j1WalkingEnemy2();
		break;
	case EntityType::FLYING_ENEMY:
		entity = new j1FlyingEnemy();
		break;
	case EntityType::TRAP:
		entity = new j1Trap();
		break;
	case EntityType::PARTICLES:
		break;
	case EntityType::UNKNOWN:
		break;
	default:
		break;
	}

	entity->position.x = entity->initialPosition.x = position_x;
	entity->position.y = entity->initialPosition.y = position_y;

	if (entity != nullptr) entities.add(entity);

	return entity;
}

void j1EntityManager::DestroyEntity(j1Entity* entity){
	BROFILER_CATEGORY("EntityDestruction", Profiler::Color::Orange)
	p2List_item<j1Entity*>* item;

	if (entity != nullptr) {
		item = entities.At(entities.find(entity));
		if (entity->collider != nullptr)
		{
			entity->collider->to_delete = true;
			entity->collider = nullptr;
		}
		if (entity->raycast != nullptr) {
			entity->raycast->to_delete = true;
			entity->raycast = nullptr;
		}
		entities.del(item);;
	}
}

void j1EntityManager::DestroyAllEntities() {
	p2List_item<j1Entity*>* item;

	for (item = entities.start; item != nullptr; item = item->next)
	{
		if (item->data != player) {
			DestroyEntity(item->data);
		}
	}
}

bool j1EntityManager::Awake(pugi::xml_node& config){
	bool ret = true;

	config_data = config;

	gravity = config.child("gravity").attribute("value").as_int();
	max_falling_speed = config.child("max_falling_speed").attribute("value").as_int();

	//player creation
	player = new j1Player();
	player->Awake(config.child("player"));
	entities.add(player);

	//reference walking enemy
	reference_walking_enemy = new j1WalkingEnemy();
	reference_walking_enemy->Awake(config.child("walking_enemy"));

	//reference walking enemy2
	reference_walking_enemy2 = new j1WalkingEnemy2();
	reference_walking_enemy2->Awake(config.child("walking_enemy2"));

	//reference flying enemy
	reference_flying_enemy = new j1FlyingEnemy();
	reference_flying_enemy->Awake(config.child("flying_enemy"));

	return ret;
}

bool j1EntityManager::Start()
{
	bool ret = true;

	player->Start();
	reference_walking_enemy->texture = App->tex->Load("sprites/characters/sheet_hero_idle.png");
	reference_flying_enemy->texture = App->tex->Load("sprites/characters/Sprite_bat.png");
	reference_walking_enemy2->texture = App->tex->Load("sprites/characters/Minotaur - Sprite Sheet.png");

	for (p2List_item<j1Entity*>* entity = entities.start; entity != nullptr; entity = entity->next)
	{
		if (entity->data->type == EntityType::WALKING_ENEMY){
			entity->data->texture = reference_walking_enemy->texture;}
		if (entity->data->type == EntityType::FLYING_ENEMY){
			entity->data->texture = reference_flying_enemy->texture; }
		if (entity->data->type == EntityType::WALKING_ENEMY2) {
			entity->data->texture = reference_walking_enemy2->texture;
		}
	}

	return ret;
}

bool j1EntityManager::CleanUp()
{
	bool ret = true;

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

	return ret;
}

j1Entity* j1EntityManager::getPlayer() {
	for (p2List_item<j1Entity*>* item = entities.start; item != nullptr; item = item->next)
	{
		if (item->data == player) return item->data;
	}
}

bool j1EntityManager::PreUpdate()
{
	BROFILER_CATEGORY("EntitiesPreUpdate", Profiler::Color::Bisque)
	bool ret = true;
	player->PreUpdate();
	return ret;
}

bool j1EntityManager::Update(float dt)
{
	BROFILER_CATEGORY("EntitiesUpdate", Profiler::Color::Bisque)
		bool ret = true;

	if ((!App->pause)&&(!blocked_movement)){
		for (p2List_item<j1Entity*>* entity = entities.start; entity != nullptr; entity = entity->next)
		{
			entity->data->Update(dt);
		}
	}
	accumulated_time += dt;
	//LOG("Accumulated time: %f", accumulated_time);

	/*if (entity != nullptr)
		{
			if (entity->data == player) {
				entity->data->Update(dt);
			}
			else if (accumulated_time > time_between_updates) {
				entity->data->Update(dt);
				accumulated_time = 0;
			}
	}*/
	return ret;
}

bool j1EntityManager::PostUpdate()
{
	BROFILER_CATEGORY("EntitiesPostUpdate", Profiler::Color::Bisque)
	bool ret = true;
	for (p2List_item<j1Entity*>* entity = entities.start; entity != nullptr; entity = entity->next)
	{
		entity->data->PostUpdate();
	}
	return ret;
}

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