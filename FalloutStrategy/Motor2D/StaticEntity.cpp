#include "StaticEntity.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Player.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "SDL_mixer/include/SDL_mixer.h"

StaticEntity::StaticEntity(Faction g_faction, EntityType g_type) {
	type = g_type;
	faction = g_faction;
	state = WAIT;
	storage_capacity = 1000;
	max_capacity = 3000;
	render_texture_pos = { 0,0 };	

	for (int i = 0; i < 10; i++)
		spawn_stack[i].type = NO_TYPE;
	spawning = false;
	upgrading = false;
	want_to_upgrade = false;

	time_left = 0;
}

StaticEntity::~StaticEntity() {
	target_entity = nullptr;
	reference_entity = nullptr;
	owner = nullptr;
	attacking_entity = nullptr;
	current_animation = nullptr;
	texture = nullptr;
	path_to_target.clear();
	tiles.clear();
}

bool StaticEntity::Update(float dt) {

	current_animation = &animations[state];

	switch (state) {
	case WAIT:
		break;
	case WORK:
		break;
	case EXPLODE:
		if (!delete_timer.Started())
			delete_timer.Start();

		if ((delete_timer.ReadSec() > 5)||(current_animation->Finished()))
			to_delete = true;

		if (Mix_Playing(21) == 0)
			SpatialAudio(App->audio->explode, 21, position.x, position.y);

		break;
	default:
		break;
	}

	//Spawning Units with timer and stack
	{
		if (spawning == false && spawn_stack[0].type != NO_TYPE) {
			chrono_spawn.Start();
			spawning = true;
		}
		if (spawning == true) {
			if (chrono_spawn.ReadSec() > spawn_stack[0].spawn_seconds) {
				App->entities->CreateEntity(faction, spawn_stack[0].type, spawnPosition.x, spawnPosition.y, owner);
				//LOG("Unit Spawned");
				UpdateSpawnStack();
			}
			time_left = spawn_stack[0].spawn_seconds - chrono_spawn.ReadSec();
		}
	}
	//Chrono for upgrades
	{
		if (upgrading == false && want_to_upgrade == true) {
			chrono_upgrade.Start();
			upgrading = true;
		}
		if (upgrading == true) {
			//LOG("time remaining %f ", 45 - chrono_upgrade.ReadSec());
			if (chrono_upgrade.ReadSec() > upgrade_stack.upgrade_seconds) {
				if (upgrade_stack.building == BASE) {
					ExecuteUpgrade(upgrade_stack.faction, RESOURCES_LIMIT);
					ExecuteUpgrade(upgrade_stack.faction, GATHERER_CAPACITY);
				}
				else if (upgrade_stack.building == BARRACK) {
					ExecuteUpgrade(upgrade_stack.faction, UNITS_DAMAGE);
					ExecuteUpgrade(upgrade_stack.faction, UNITS_SPEED);
				}
				else if (upgrade_stack.building == LABORATORY) {
					ExecuteUpgrade(upgrade_stack.faction, UNITS_HEALTH);
					ExecuteUpgrade(upgrade_stack.faction, CREATION_TIME);

				}

									

				UpdateUpgradeStack();
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
				Upgrade(App->entities->base_resource_limit[faction]);
			if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
				Upgrade(App->entities->gatherer_resource_limit[faction]);
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
				Upgrade(App->entities->units_damage[faction]);
			if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
				Upgrade(App->entities->units_speed[faction]);
		}
		else if (type == LABORATORY) {
			//Upgrades
			if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
				Upgrade(App->entities->units_health[faction]);
			if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
				Upgrade(App->entities->units_creation_time[faction]);
		}
	}

	last_dt = dt;

	return true;
}

bool StaticEntity::PostUpdate() {
	current_animation = &animations[state];

	SDL_Rect tile_rect = { 256,0,64,64 };
	iPoint tex_position;

	//selected entity debug
	if (App->player->selected_entity == this)
	{
		tile_rect = { 128,0,64,64 };
		for (int i = 0; i < tiles.size(); i++)
		{
			tex_position = App->map->MapToWorld(tiles[i].x, tiles[i].y);
			App->render->Blit(App->render->debug_tex, tex_position.x, tex_position.y, &tile_rect);
		}

	}
	//debug tiles
	else if (App->render->debug) 
	{
		for (int i = 0; i < tiles.size(); i++)
		{
			tex_position = App->map->MapToWorld(tiles[i].x, tiles[i].y);
			App->render->Blit(App->render->debug_tex, tex_position.x, tex_position.y, &tile_rect);
		}
	}

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

	std::string file = std::string("Assets/textures/characters/").append(faction_char).append("/").append(faction_char).append("_Buildings.tmx");

	pugi::xml_document animation_file;
	pugi::xml_parse_result result = animation_file.load_file(file.c_str());
	std::string image = std::string(animation_file.child("tileset").child("image").attribute("source").as_string());
	std::string texture_path = std::string("Assets/textures/characters/").append(faction_char).append("/").append(faction_char).append("_Buildings.png");

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

void StaticEntity::Upgrade(Upgrades_Data upgrades_data) {
	//We substract resources from player and add upgrade to the stack

	if (upgrading == false) {
		if (upgrades_data.upgrade == RESOURCES_LIMIT && App->entities->base_resource_limit[faction].upgrade_num < 4) {
			if (storage_capacity < max_capacity) {
				int cost = App->entities->base_resource_limit[faction].first_price + (App->entities->base_resource_limit[faction].price_increment * App->entities->base_resource_limit[faction].upgrade_num);

				if (owner->caps >= cost) {
					//Pay the price
					if (owner == App->player)
						App->player->UpdateResourceData(Resource::CAPS, -cost);
					else
						owner->caps -= cost;

					upgrade_stack.building = BASE;
					upgrade_stack.faction = faction;
					upgrade_stack.upgrade_seconds = upgrades_data.seconds;
					want_to_upgrade = true;
					LOG("Resource Limit Upgrade started. Waiting %i seconds", upgrade_stack.upgrade_seconds);

					App->entities->base_resource_limit[faction].upgrade_num++;
				}
			}
		}
		else if (upgrades_data.upgrade == GATHERER_CAPACITY && App->entities->gatherer_resource_limit[faction].upgrade_num < 4) {
			int cost = App->entities->gatherer_resource_limit[faction].first_price + (App->entities->gatherer_resource_limit[faction].price_increment * App->entities->gatherer_resource_limit[faction].upgrade_num);

			if (owner->caps >= cost) {
				//Pay the price
				if (owner == App->player)
					App->player->UpdateResourceData(Resource::CAPS, -cost);
				else
					owner->caps -= cost;

				//Upgrade data
				upgrade_stack.building = BASE;
				upgrade_stack.faction = faction;
				upgrade_stack.upgrade_seconds = upgrades_data.seconds;
				want_to_upgrade = true;
				LOG("Gatherer Resource Limit Upgrade started. Waiting %i seconds", upgrade_stack.upgrade_seconds);

				App->entities->gatherer_resource_limit[faction].upgrade_num++;
			}
		}
		else if (upgrades_data.upgrade == UNITS_DAMAGE && App->entities->units_damage[faction].upgrade_num < 4) {
			int cost = App->entities->units_damage[faction].first_price + (App->entities->units_damage[faction].price_increment * App->entities->units_damage[faction].upgrade_num);

			if (owner->caps >= cost) {
				//Pay the price
				if (owner == App->player)
					App->player->UpdateResourceData(Resource::CAPS, -cost);
				else
					owner->caps -= cost;

				//Upgrade data
				upgrade_stack.building = BARRACK;
				upgrade_stack.faction = faction;
				upgrade_stack.upgrade_seconds = upgrades_data.seconds;
				want_to_upgrade = true;
				LOG("Units Damage Upgrade started. Waiting %i seconds", upgrade_stack.upgrade_seconds);

				App->entities->units_damage[faction].upgrade_num++;
			}
		}
		else if (upgrades_data.upgrade == UNITS_SPEED && App->entities->units_speed[faction].upgrade_num < 4) {
			int cost = App->entities->units_speed[faction].first_price + (App->entities->units_speed[faction].price_increment * App->entities->units_speed[faction].upgrade_num);

			if (owner->caps >= cost) {
				//Pay the price
				if (owner == App->player)
					App->player->UpdateResourceData(Resource::CAPS, -cost);
				else
					owner->caps -= cost;

				//Upgrade data
				upgrade_stack.building = BARRACK;
				upgrade_stack.faction = faction;
				upgrade_stack.upgrade_seconds = upgrades_data.seconds;
				want_to_upgrade = true;
				LOG("Units Speed Upgrade started. Waiting %i seconds", upgrade_stack.upgrade_seconds);

				App->entities->units_speed[faction].upgrade_num++;
			}
		}
		else if (upgrades_data.upgrade == UNITS_HEALTH && App->entities->units_health[faction].upgrade_num < 4) {
			int cost = App->entities->units_health[faction].first_price + (App->entities->units_health[faction].price_increment * App->entities->units_health[faction].upgrade_num);

			if (owner->caps >= cost) {
				//Pay the price
				if (owner == App->player)
					App->player->UpdateResourceData(Resource::CAPS, -cost);
				else
					owner->caps -= cost;

				//Upgrade data
				upgrade_stack.building = LABORATORY;
				upgrade_stack.faction = faction;
				upgrade_stack.upgrade_seconds = upgrades_data.seconds;
				want_to_upgrade = true;
				LOG("Units Health Upgrade started. Waiting %i seconds", upgrade_stack.upgrade_seconds);

				App->entities->units_health[faction].upgrade_num++;
			}
		}
		else if (upgrades_data.upgrade == CREATION_TIME && App->entities->units_creation_time[faction].upgrade_num < 4) {
			int cost = App->entities->units_creation_time[faction].first_price + (App->entities->units_creation_time[faction].price_increment * App->entities->units_creation_time[faction].upgrade_num);

			if (owner->caps >= cost) {
				//Pay the price
				if (owner == App->player)
					App->player->UpdateResourceData(Resource::CAPS, -cost);
				else
					owner->caps -= cost;

				//Upgrade data
				upgrade_stack.building = LABORATORY;
				upgrade_stack.faction = faction;
				upgrade_stack.upgrade_seconds = upgrades_data.seconds;
				want_to_upgrade = true;
				LOG("Units Creation Time Upgrade started. Waiting %i seconds", upgrade_stack.upgrade_seconds);

				App->entities->units_creation_time[faction].upgrade_num++;
			}
		}
	}	
}

void StaticEntity::ExecuteUpgrade(Faction faction, Upgrades upgrade_name) {
	//We execute the upgrade as upgrade_seconds have already passed
	if (upgrade_name == RESOURCES_LIMIT) {
		if (storage_capacity < max_capacity) {
			int cost = App->entities->base_resource_limit[faction].first_price + (App->entities->base_resource_limit[faction].price_increment * App->entities->base_resource_limit[faction].upgrade_num);

			storage_capacity += (int)storage_capacity * 0.3;

			if (storage_capacity > max_capacity)
				storage_capacity = max_capacity;

			LOG("Resource Limit Upgraded. New limit is: %i", storage_capacity);
		}
	}
	else if (upgrade_name == GATHERER_CAPACITY) {
		int cost = App->entities->gatherer_resource_limit[faction].first_price + (App->entities->gatherer_resource_limit[faction].price_increment * App->entities->gatherer_resource_limit[faction].upgrade_num);

		//Upgrade gatherers that are currently alive
		for (int i = 0; i < App->entities->entities.size(); i++) {
			if (App->entities->entities[i]->faction == faction)
				if (App->entities->entities[i]->type == GATHERER)
					App->entities->entities[i]->damage += (int)(App->entities->entities[i]->damage * 0.5);
		}
		App->entities->reference_entities[faction][GATHERER]->damage += (App->entities->reference_entities[faction][GATHERER]->damage * 0.5);
		LOG("Gatherer Resource Limit Upgraded");
	}
	else if (upgrade_name == UNITS_DAMAGE) {
		int cost = App->entities->units_damage[faction].first_price + (App->entities->units_damage[faction].price_increment * App->entities->units_damage[faction].upgrade_num);

		//Upgrade melees and ranged that are currently alive
		for (int i = 0; i < App->entities->entities.size(); i++) {
			if (App->entities->entities[i]->faction == faction)
				if (App->entities->entities[i]->type == MELEE || App->entities->entities[i]->type == RANGED)
					App->entities->entities[i]->damage += (int)(App->entities->entities[i]->damage * 0.15);
		}
		LOG("Units Damage Upgraded");
		App->entities->reference_entities[faction][MELEE]->damage += (App->entities->reference_entities[faction][MELEE]->damage * 0.15);
		App->entities->reference_entities[faction][RANGED]->damage += (App->entities->reference_entities[faction][RANGED]->damage * 0.15);
	}
	else if (upgrade_name == UNITS_SPEED) {
		int cost = App->entities->units_speed[faction].first_price + (App->entities->units_speed[faction].price_increment * App->entities->units_speed[faction].upgrade_num);

		//Upgrade units that are currently alive
		for (int i = 0; i < App->entities->entities.size(); i++) {
			if (App->entities->entities[i]->faction == faction) {
				App->entities->entities[i]->speed.x += App->entities->entities[i]->speed.x * 0.15;
				App->entities->entities[i]->speed.y += App->entities->entities[i]->speed.y * 0.15;
			}
		}
		LOG("Units Speed Upgraded");
		App->entities->reference_entities[faction][MELEE]->speed.x += App->entities->reference_entities[faction][MELEE]->speed.x * 0.15;
		App->entities->reference_entities[faction][MELEE]->speed.y += App->entities->reference_entities[faction][MELEE]->speed.y * 0.15;
		App->entities->reference_entities[faction][RANGED]->speed.x += App->entities->reference_entities[faction][RANGED]->speed.x * 0.15;
		App->entities->reference_entities[faction][RANGED]->speed.y += App->entities->reference_entities[faction][RANGED]->speed.y * 0.15;
		App->entities->reference_entities[faction][GATHERER]->speed.x += App->entities->reference_entities[faction][GATHERER]->speed.x * 0.15;
		App->entities->reference_entities[faction][GATHERER]->speed.y += App->entities->reference_entities[faction][GATHERER]->speed.y * 0.15;
	}
	else if (upgrade_name == UNITS_HEALTH) {
		int cost = App->entities->units_health[faction].first_price + (App->entities->units_health[faction].price_increment * App->entities->units_health[faction].upgrade_num);

		//Upgrade melees and ranged that are currently alive
		for (int i = 0; i < App->entities->entities.size(); i++) {
			if (App->entities->entities[i]->faction == faction)
				if (App->entities->entities[i]->type == MELEE || App->entities->entities[i]->type == RANGED) {
					App->entities->entities[i]->max_health += (int)(App->entities->entities[i]->max_health * 0.15);
					App->entities->entities[i]->current_health += (int)(App->entities->entities[i]->max_health * 0.15);
				}
		}
		LOG("Units Health Upgraded");	
		App->entities->reference_entities[faction][MELEE]->max_health += (int)(App->entities->reference_entities[faction][MELEE]->max_health * 0.15);
		App->entities->reference_entities[faction][RANGED]->max_health += (int)(App->entities->reference_entities[faction][RANGED]->max_health * 0.15);
	}
	else if (upgrade_name == CREATION_TIME) {
		int cost = App->entities->units_creation_time[faction].first_price + (App->entities->units_creation_time[faction].price_increment * App->entities->units_creation_time[faction].upgrade_num);

		App->entities->unit_data[faction][GATHERER].spawn_seconds = (floor)(App->entities->unit_data[faction][GATHERER].spawn_seconds * 0.95);
		App->entities->unit_data[faction][MELEE].spawn_seconds = (floor)(App->entities->unit_data[faction][MELEE].spawn_seconds * 0.95);
		App->entities->unit_data[faction][RANGED].spawn_seconds = (floor)(App->entities->unit_data[faction][RANGED].spawn_seconds * 0.95);

		LOG("Units Creation Upgraded Upgraded");	
	}
}

void StaticEntity::SpawnUnit(EntityType type, bool no_cost) {

	int cost_water, cost_meat;
	int spawn_seconds;
	//Look for that unit data (spawn_seconds and cost)

	if (((faction == App->player->faction) && (App->player->god_mode))||(no_cost == true)) {
		cost_water = 0;
		cost_meat = 0;
		spawn_seconds = 0;
	}
	else {
		cost_water = App->entities->unit_data[faction][type].cost_water;
		cost_meat = App->entities->unit_data[faction][type].cost_meat;
		spawn_seconds = App->entities->unit_data[faction][type].spawn_seconds;
	}

	if (owner->water >= cost_water && owner->food > cost_meat) {
		//Substract resources
		if (owner->faction == App->player->faction) {
			App->player->UpdateResourceData(Resource::WATER, -cost_water);
			App->player->UpdateResourceData(Resource::FOOD, -cost_meat);
		}
		else {
			owner->water -= cost_water;
			owner->food -= cost_meat;
		}

		//Add to stack
		for (int i = 0; i < 10; i++) {
			if (spawn_stack[i].type == NO_TYPE) {

				spawn_stack[i].type = type;
				spawn_stack[i].spawn_seconds = spawn_seconds;
				LOG("Unit in queue. Waiting %i seconds to spawn", spawn_seconds);
				break;
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

void StaticEntity::UpdateUpgradeStack() {
	want_to_upgrade = false;
	upgrading = false;
}