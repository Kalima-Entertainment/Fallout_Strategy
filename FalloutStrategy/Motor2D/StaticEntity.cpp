#include "StaticEntity.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Player.h"
#include "j1Input.h"
#include "SDL_mixer/include/SDL_mixer.h"

StaticEntity::StaticEntity(Faction g_faction, EntityType g_type) {
	type = g_type;
	faction = g_faction;
	state = WAIT;
	storage_capacity = 1000;
	max_capacity = 3000;
	render_texture_pos = { 0,0 };

	//Initialize upgrades
	base_resource_limit = { "base_resource_limit", 0, 250, 250};
	gatherer_resource_limit = { "gatherer_resource_limit", 0, 0, 0 };
	units_damage = { "units_damage", 0, 350, 250 };
	units_speed = { "units_speed", 0, 350, 250 };
	units_health = { "units_health", 0, 150, 250 };
	units_creation_time = { "units_creation_time", 0, 150, 250 };

	for (int i = 0; i < 10; i++)
		spawn_stack[i].type = NO_TYPE;
	spawning = false;
}

StaticEntity::~StaticEntity() {}

bool StaticEntity::Update(float dt) {
	switch (state) {
	case WAIT:
		if (Mix_Playing(6) == 0)
			SpatialAudio(App->audio->factory, 6, position.x, position.y);
		break;
	case WORK:
		break;
	case EXPLODE:
		if (current_animation->Finished())
			to_destroy = true;
		if (Mix_Playing(7) == 0)
			SpatialAudio(App->audio->explode, 7, position.x, position.y);
		break;
	default:
		break;
	}

	//Spawning Units with timer and stack
	{
		if (spawning == false && spawn_stack[0].type != NO_TYPE) {
			spawn_time = std::chrono::steady_clock::now() + std::chrono::seconds(spawn_stack[0].spawn_seconds);
			spawning = true;
			LOG("Chrono started");
		}
		else if (spawning == true) {
			if (std::chrono::steady_clock::now() < spawn_time) {
				LOG("Entity Spawned");
				App->entities->CreateEntity(faction, spawn_stack[0].type, spawnPosition.x, spawnPosition.y);
				UpdateSpawnStack();
			}
		}
	}

	//Interact with the building to spawn units or investigate upgrades
	if (this == App->player->selected_entity) {
		if (type == BASE) {
			//Spawn GATHERER
			if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
				SpawnUnit(GATHERER);
			//Upgrades
			if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
				Upgrade(faction, "base_resource_limit");
			if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
				Upgrade(faction, "gatherer_resource_limit");
		}
		else if (type == BARRACK) {
			//Spawn MELEE
			if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
				SpawnUnit(MELEE);
			//Spawn RANGED
			if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
				SpawnUnit(RANGED);
			//Upgrades
			if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
				Upgrade(faction, "units_damage");
			if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
				Upgrade(faction, "units_speed");
		}
		else if (type == LABORATORY) {
			//Upgrades
			if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
				Upgrade(faction, "units_health");
			if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
				Upgrade(faction, "units_creation_time");
		}		
	}

	last_dt = dt;

	return true;
}

bool StaticEntity::PostUpdate() {
	current_animation = &animations[state];

	//Render building
	render_texture_pos = {(int)(position.x - 0.5f * sprite_size),(int)(position.y - sprite_size * 0.75)};

	App->render->Blit(reference_entity->texture, render_texture_pos.x, render_texture_pos.y, &current_animation->GetCurrentFrame(last_dt));

	if (App->render->debug) 
		App->render->DrawQuad({ (int)render_position.x, render_position.y, 4,4 }, 255, 0, 0, 255); 
	return true;
}

bool StaticEntity::LoadReferenceData() {
	bool ret = true;
	StaticEntity* static_reference = (StaticEntity*)reference_entity;

	//load animations
	for (int i = 0; i < 3; i++)
	{
		animations[i] = static_reference->animations[i];
	}

	//load property data
	current_health = max_health = reference_entity->max_health;
	sprite_size = reference_entity->sprite_size;
	return ret;
}

bool StaticEntity::LoadAnimations() {
	
	bool ret = true;
	char* faction_char = "NoFaction";
	float speed_multiplier = 0.065f;

	if (faction == VAULT)
		faction_char = "VaultDwellers";
	else if (faction == BROTHERHOOD)
		faction_char = "Brotherhood";
	else if (faction == GHOUL)
		faction_char = "Ghouls";
	else if (faction == MUTANT)
		faction_char = "SuperMutant";

	std::string file = std::string("textures/characters/").append(faction_char).append("/").append(faction_char).append("_Buildings.tmx");

	pugi::xml_document animation_file;
	pugi::xml_parse_result result = animation_file.load_file(file.c_str());
	std::string image = std::string(animation_file.child("tileset").child("image").attribute("source").as_string());
	std::string texture_path = std::string("textures/characters/").append(faction_char).append("/").append(faction_char).append("_Buildings.png");

	if (type == BASE)
	{
		this->texture = App->tex->Load(texture_path.c_str());
	}

	if (result == NULL)
	{
		LOG("Could not load animation tmx file %s. pugi error: %s", file, result.description());
		ret = false;
	}

	int tile_width = animation_file.child("map").attribute("tilewidth").as_int();
	int tile_height = animation_file.child("map").attribute("tileheight").as_int();
	int columns = animation_file.child("map").child("tileset").attribute("columns").as_int();
	int firstgid = animation_file.child("map").child("tileset").attribute("firstgid").as_int();
	sprite_size = tile_height;
	int id, tile_id;
	float speed;

	pugi::xml_node animation = animation_file.child("map").child("tileset").child("tile");
	pugi::xml_node frame = animation.child("animation").child("frame");

	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	int i = 0;

	while (animation != nullptr)
	{
		std::string building_type = std::string(animation.child("properties").child("property").attribute("name").as_string());
		std::string animation_name = std::string(animation.child("properties").child("property").attribute("value").as_string());
		StaticState state = NO_STATE;
		EntityType entity_type = BASE;
		bool loop = true;

		//building type
		if (building_type == "base")
			entity_type = BASE;
		else if (building_type == "barrack")
			entity_type = BARRACK;
		else if (building_type == "laboratory")
			entity_type = LABORATORY;

		//animation
		if (animation_name == "idle") {
			state = WAIT;
		}
		else if (animation_name == "work") {
			state = WORK;
			loop = false;
		}
		else if (animation_name == "die") {
			state = EXPLODE;
			loop = false;
		}
		else goto CHANGE_ANIMATION;

		id = animation.attribute("id").as_int();

		if (type == entity_type)
		{
			while (frame != nullptr) {
				tile_id = frame.attribute("tileid").as_int();
				speed = frame.attribute("duration").as_int() * speed_multiplier;
				rect.x = rect.w * ((tile_id) % columns);
				rect.y = rect.h * ((tile_id) / columns);
				animations[state].PushBack(rect, speed);
				frame = frame.next_sibling();
			}
			animations[state].loop = loop;
		}

	CHANGE_ANIMATION: animation = animation.next_sibling();
		frame = animation.child("animation").child("frame");
	}

	return ret;
}

void StaticEntity::Upgrade(Faction faction, std::string upgrade_name) {
	
	if (upgrade_name == "base_resource_limit") {
		if (storage_capacity < max_capacity) {

			int cost = base_resource_limit.first_price + (base_resource_limit.price_increment * base_resource_limit.upgrade_num);

			if (App->player->caps > cost && App->player->water >= cost && App->player->food > cost) {
				storage_capacity += (int)storage_capacity * 0.3;

				if (storage_capacity > max_capacity)
					storage_capacity = max_capacity;

				App->player->UpdateResourceData(Resource::CAPS, -cost);
				App->player->UpdateResourceData(Resource::WATER, -cost);
				App->player->UpdateResourceData(Resource::FOOD, -cost);

				LOG("Resource Limit Upgraded. New limit is: %i", storage_capacity);
			}
		}
	}
	else if (upgrade_name == "gatherer_resource_limit") {
		App->player->UpdateResourceData(Resource::CAPS, 1000);
		App->player->UpdateResourceData(Resource::WATER, 1000);
		App->player->UpdateResourceData(Resource::FOOD, 1000);
	}
	else if (upgrade_name == "units_damage") {
				
	}
	else if (upgrade_name == "units_speed") {

	}
	else if (upgrade_name == "units_health") {

	}
	else if (upgrade_name == "units_creation_time") {

	}
}

void StaticEntity::SpawnUnit(EntityType type) {

	int cost;
	int spawn_seconds;
	//Look for that unit data (spawn_seconds and cost)
	for (int j = 0; j < 12; j++) {
		if (App->entities->unit_data[j].faction == faction)
			if (App->entities->unit_data[j].type == type) {
				cost = App->entities->unit_data[j].cost;
				spawn_seconds = App->entities->unit_data[j].spawn_seconds;
				break;
			}
	}

	if (cost != NULL) { //TODO: Delete this "if" once unit_data[] is filled in EntityManager
		if (App->player->caps > cost && App->player->water >= cost && App->player->food > cost) {
			//Substract resources
			App->player->UpdateResourceData(Resource::CAPS, -cost);
			App->player->UpdateResourceData(Resource::WATER, -cost);
			App->player->UpdateResourceData(Resource::FOOD, -cost);

			//Add to stack
			for (int i = 0; i < 10; i++) {
				if (spawn_stack[i].type == NO_TYPE) {

					spawn_stack[i].type = type;
					spawn_stack[i].spawn_seconds = spawn_seconds;
					LOG("Added to stack. Waiting %i seconds to spawn", spawn_seconds);
					break;
				}
			}
		}
	}	
}

void StaticEntity::UpdateSpawnStack() {
	//First entity in queue has been spawned
	//Now let's start timer for the next entity
	for (int i = 0; i < 9; i++) {
		spawn_stack[i] = spawn_stack[i + 1];
	}
	spawn_stack[9] = { NO_TYPE, 0 };
	spawning = false;
}