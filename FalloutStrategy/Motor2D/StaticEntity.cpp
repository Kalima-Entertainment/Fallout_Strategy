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
	base_resource_limit[0] = { VAULT,"base_resource_limit", 0, 250, 250};
	base_resource_limit[1] = { BROTHERHOOD,"base_resource_limit", 0, 250, 250 };
	base_resource_limit[2] = { MUTANT,"base_resource_limit", 0, 250, 250 };
	base_resource_limit[3] = { GHOUL,"base_resource_limit", 0, 250, 250 };
	gatherer_resource_limit[0] = { VAULT,"gatherer_resource_limit", 0, 250, 250 };
	gatherer_resource_limit[1] = { BROTHERHOOD,"gatherer_resource_limit", 0, 250, 250 };
	gatherer_resource_limit[2] = { MUTANT,"gatherer_resource_limit", 0, 250, 250 };
	gatherer_resource_limit[3] = { GHOUL,"gatherer_resource_limit", 0, 250, 250 };
	units_damage[0] = { VAULT,"units_damage", 0, 350, 250 };
	units_damage[1] = { BROTHERHOOD,"units_damage", 0, 350, 250 };
	units_damage[2] = { MUTANT,"units_damage", 0, 350, 250 };
	units_damage[3] = { GHOUL,"units_damage", 0, 350, 250 };
	units_speed[0] = { VAULT, "units_speed", 0, 350, 250 };
	units_speed[1] = { BROTHERHOOD,"units_speed", 0, 350, 250 };
	units_speed[2] = { MUTANT,"units_speed", 0, 350, 250 };
	units_speed[3] = { GHOUL,"units_speed", 0, 350, 250 };
	units_health[0] = { VAULT,"units_health", 0, 150, 250 };
	units_health[1] = { BROTHERHOOD,"units_health", 0, 150, 250 };
	units_health[2] = { MUTANT,"units_health", 0, 150, 250 };
	units_health[3] = { GHOUL,"units_health", 0, 150, 250 };
	units_creation_time[0] = { VAULT, "units_creation_time", 0, 150, 250 };
	units_creation_time[1] = { BROTHERHOOD,"units_creation_time", 0, 150, 250 };
	units_creation_time[2] = { MUTANT, "units_creation_time", 0, 150, 250 };
	units_creation_time[3] = { GHOUL,"units_creation_time", 0, 150, 250 };

	for (int i = 0; i < 10; i++)
		spawn_stack[i].type = NO_TYPE;
	spawning = false;

	time_left = 0;
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
			chrono.Start();
			spawning = true;
		}
		if (spawning == true) {
			if (chrono.ReadSec() > spawn_stack[0].spawn_seconds) {
				App->entities->CreateEntity(faction, spawn_stack[0].type, spawnPosition.x, spawnPosition.y);
				LOG("Unit Spawned");
				UpdateSpawnStack();
			}
			time_left = spawn_stack[0].spawn_seconds - chrono.ReadSec();
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

	//Health bar stats
	SDL_Rect background_bar = { position.x - HALF_TILE * 0.75f, position.y - TILE_SIZE * 1.5f, 80, 4 };
	SDL_Rect foreground_bar = { position.x - HALF_TILE * 0.75f, position.y - TILE_SIZE * 1.5f, (float)current_health / max_health * background_bar.w, 4 };
	if (foreground_bar.w < 0) foreground_bar.w = 0;
	App->render->DrawQuad(background_bar, 255, 255, 255, 255);
	App->render->DrawQuad(foreground_bar, 230, 165, 30, 255);

	//Spawn bar 
	if (spawning) {
		SDL_Rect spawn_bar_background = { position.x - HALF_TILE * 0.75f, position.y - TILE_SIZE * 1.25f, 80, 4 };
		SDL_Rect spawn_bar_foreground = { position.x - HALF_TILE * 0.75f, position.y - TILE_SIZE * 1.25f, (float)time_left / spawn_stack[0].spawn_seconds * spawn_bar_background.w, 4 };
		App->render->DrawQuad(spawn_bar_background, 150, 150, 150, 255);
		App->render->DrawQuad(spawn_bar_foreground, 230, 165, 30, 255);
	}

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
			int cost = base_resource_limit[faction].first_price + (base_resource_limit[faction].price_increment * base_resource_limit[faction].upgrade_num);

			if (App->player->caps >= cost) {
				storage_capacity += (int)storage_capacity * 0.3;

				if (storage_capacity > max_capacity)
					storage_capacity = max_capacity;

				App->player->UpdateResourceData(Resource::CAPS, -cost);
				LOG("Resource Limit Upgraded. New limit is: %i", storage_capacity);

				base_resource_limit[faction].upgrade_num++;
			}
		}
	}
	else if (upgrade_name == "gatherer_resource_limit") {
		int cost = gatherer_resource_limit[faction].first_price + (gatherer_resource_limit[faction].price_increment * gatherer_resource_limit[faction].upgrade_num);

		if (App->player->caps >= cost) {
			for (int i = 0; i < App->entities->entities.size(); i++) {
				if(App->entities->entities[i]->faction = faction)
					if (App->entities->entities[i]->type == GATHERER ) 
						App->entities->entities[i]->damage += (int)(App->entities->entities[i]->damage * 0.5);	
			}
			//Pay the price
			App->player->UpdateResourceData(Resource::CAPS, -cost);
			LOG("Gatherer Resource Limit Upgraded");

			gatherer_resource_limit[faction].upgrade_num++;
		}
	}
	else if (upgrade_name == "units_damage") {
		int cost = units_damage[faction].first_price + (units_damage[faction].price_increment * units_damage[faction].upgrade_num);
		
		if (App->player->caps >= cost) {
			for (int i = 0; i < App->entities->entities.size(); i++) {
				if (App->entities->entities[i]->faction = faction)
					if (App->entities->entities[i]->type == MELEE || App->entities->entities[i]->type == RANGED)
						App->entities->entities[i]->damage += (int)(App->entities->entities[i]->damage * 0.15);
			}
			//Pay the price
			App->player->UpdateResourceData(Resource::CAPS, -cost);
			LOG("Units Damage Upgraded");

			units_damage[faction].upgrade_num++;
		}
	}
	else if (upgrade_name == "units_speed") {
		int cost = units_speed[faction].first_price + (units_speed[faction].price_increment * units_speed[faction].upgrade_num);

		if (App->player->caps >= cost) {
			for (int i = 0; i < App->entities->entities.size(); i++) {
				if (App->entities->entities[i]->faction = faction) {
					App->entities->entities[i]->speed.x += App->entities->entities[i]->speed.x * 0.15;
					App->entities->entities[i]->speed.y += App->entities->entities[i]->speed.y * 0.15;
				}				
			}
			//Pay the price
			App->player->UpdateResourceData(Resource::CAPS, -cost);
			LOG("Units Speed Upgraded");

			units_speed[faction].upgrade_num++;
		}
	}
	else if (upgrade_name == "units_health") {
		int cost = units_health[faction].first_price + (units_health[faction].price_increment * units_health[faction].upgrade_num);

		if (App->player->caps >= cost) {
			for (int i = 0; i < App->entities->entities.size(); i++) {
				if (App->entities->entities[i]->faction = faction)
					if (App->entities->entities[i]->type == MELEE || App->entities->entities[i]->type == RANGED) {
						App->entities->entities[i]->max_health += (int)(App->entities->entities[i]->max_health * 0.15);
						App->entities->entities[i]->current_health += (int)(App->entities->entities[i]->max_health * 0.15);
					}						
			}
			units_health[faction].upgrade_num++;
		}
	}
	else if (upgrade_name == "units_creation_time") {
		int cost = units_creation_time[faction].first_price + (units_creation_time[faction].price_increment * units_creation_time[faction].upgrade_num);

		if (App->player->caps >= cost) {
			for (int i = 0; i < 12; i++) 
				App->entities->unit_data[i].spawn_seconds -= App->entities->unit_data[i].spawn_seconds * 0.05;			

			units_creation_time[faction].upgrade_num++;
		}
	}
}

void StaticEntity::SpawnUnit(EntityType type) {

	int cost_water, cost_meat;
	int spawn_seconds;
	//Look for that unit data (spawn_seconds and cost)

	for (int j = 0; j < 12; j++) {
		if (App->entities->unit_data[j].faction == faction)
			if (App->entities->unit_data[j].type == type) {
				cost_water = App->entities->unit_data[j].cost_water;
				cost_meat = App->entities->unit_data[j].cost_meat;
				spawn_seconds = App->entities->unit_data[j].spawn_seconds;				

				if (App->player->god_mode == true) {
					cost_meat = 0;
					cost_water = 0;
					spawn_seconds = 1;
				}
					

				if (App->player->water >= cost_water && App->player->food > cost_meat) {
					//Substract resources
					App->player->UpdateResourceData(Resource::WATER, -cost_water);
					App->player->UpdateResourceData(Resource::FOOD, -cost_meat);

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
				break;
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