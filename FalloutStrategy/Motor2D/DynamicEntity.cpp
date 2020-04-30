#include "DynamicEntity.h"
#include "j1Map.h"
#include "j1App.h"
#include "j1Scene.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1Pathfinding.h"
#include "j1Textures.h"
#include "j1EntityManager.h"
#include "j1Player.h"
#include "j1Input.h"
#include "AI_Player.h"
#include "StaticEntity.h"
#include <iostream>
#include <string>
#include "SDL_mixer/include/SDL_mixer.h"

DynamicEntity::DynamicEntity(Faction g_faction, EntityType g_type, iPoint g_current_tile, GenericPlayer* g_owner) : j1Entity(), resource_collected(0) {

	switch (g_type)
	{
	case MELEE:
		range = 1;
		is_agressive = true;
		break;
	case RANGED:
		range = 3;
		is_agressive = true;
		break;
	case GATHERER:
		range = 1;
		is_agressive = false;
		break;
	case BIGHRONER:
		range = 1;
		is_agressive = false;
		break;
	case BRAHAM:
		range = 1;
		is_agressive = false;
		break;
	case DEATHCLAW:
		range = 1;
		is_agressive = true;
		break;
	default:
		break;
	}

	owner = g_owner;
	type = g_type;
	faction = g_faction;
	current_tile = g_current_tile;

	position = App->map->fMapToWorld(current_tile.x, current_tile.y);
	position.x += HALF_TILE;
	position.y += HALF_TILE;

	is_dynamic = true;

	state = IDLE;
	direction = BOTTOM_RIGHT;

	target_entity = nullptr;
	resource_building = nullptr;
	attacking_entity = nullptr;

	action_time = 3.0f;
	target_tile = { -1,-1 };
	speed = { 0, 0 };
	sprite_size = 128;

	detection_radius = 6;
	detection_timer.Start();
}

DynamicEntity::~DynamicEntity() {
	target_entity = nullptr;
	resource_building = nullptr;
	reference_entity = nullptr;
	owner = nullptr;
	attacking_entity = nullptr;
	current_animation = nullptr;
	texture = nullptr;
	path_to_target.clear();
}

bool DynamicEntity::Update(float dt) {

	Mix_AllocateChannels(25);
	j1Entity* enemy_in_range = nullptr;

	switch (state)
	{
	case IDLE:
		/*
		if (node_path.size() > 0) {
			target_tile = node_path.back();
			node_path.pop_back();
			PathfindToPosition(target_tile);
		}

		if ((attacking_entity != nullptr)&&(attacking_entity->state != DIE)) {
			if (is_agressive){
				target_entity = attacking_entity;
				if (current_tile.DistanceManhattan(attacking_entity->current_tile) > range) {
				PathfindToPosition(attacking_entity->current_tile);
				}
				else {
					attacking_entity->attacking_entity = this;
					state = ATTACK;
					UpdateTile();
				}
			}
		}

		if (detection_timer.Read() > 250) {
			j1Entity* enemy_in_range = DetectEntitiesInRange();

			if (enemy_in_range != nullptr) {
				if (is_agressive) {
					if (target_entity == nullptr) {
						target_entity = enemy_in_range;
						PathfindToPosition(target_entity->current_tile);
					}
				}
				else {
					Flee();
				}
			}

			detection_timer.Start();
		}
		*/
		
		enemy_in_range = DetectEntitiesInRange();

		if (enemy_in_range) {
			if (is_agressive) {
				if ((target_entity == nullptr)||(target_entity == enemy_in_range)) {
					target_entity = enemy_in_range;
					if ((current_tile.DistanceNoSqrt(enemy_in_range->current_tile) > range) && (target_entity->current_tile != target_tile))
						PathfindToPosition(enemy_in_range->current_tile);
					else {
						next_tile = current_tile;
						target_tile = current_tile;
						UpdateTile();
						path_to_target.clear();
					}
				}
			}
			else {
				Flee();
			}
		}
		
		break;

	case WALK:
		//if the entitiy is about to reach it's target tile
		/*
		if (current_tile.LinealDistance(target_tile) <= range) {
			//we reach the destination and there is an entity in it
			//ranged and melee
			if (is_agressive) {
				if ((target_entity != nullptr)&&(current_tile.LinealDistance(target_entity->current_tile) <= range))
				{
					//enemy target
					if ((faction != target_entity->faction)&&(node_path.size() == 0)) {
						state = ATTACK;
						target_entity->attacking_entity = this;
						Attack();
					}
					//ally
					else if (next_tile == target_tile)
					{
						state = IDLE;
						next_tile = current_tile;
						path_to_target.clear();
						UpdateTile();
					}
				}
			}
			//gatherer
			else {
				if (next_tile == target_tile) {
					//gather
					if ((resource_building != nullptr) && (resource_collected < storage_capacity)) {
						state = GATHER;
						timer.Start();
					}

					//give gathered resources
					else if ((resource_collected > 0) && (target_entity != nullptr) && (target_entity->volume < target_entity->storage_capacity)) {
						StoreGatheredResources();

						//go back to resource building to get more resources
						if (resource_building->quantity > 0) {
							PathfindToPosition(App->entities->ClosestTile(current_tile, resource_building->tiles));
							state = WALK;
						}
						//find another building
						else
						{
							resource_building = App->entities->GetClosestResourceBuilding(current_tile);
							//if there is at least a resource building left, go there
							if (resource_building != nullptr) {
								PathfindToPosition(App->entities->ClosestTile(current_tile, resource_building->tiles));
								state = WALK;
							}
							//if there are no resource buildings left
							else
							{
								state = IDLE;
							}
						}
					}
					
					if (target_entity != nullptr) {
						state = IDLE;
					}
				}
			}
		}
		*/


		if (current_tile.DistanceNoSqrt(target_tile) <= range) {
			if (is_agressive) {
				if (target_entity != nullptr) {
					if ((target_entity->current_tile == target_tile)|| (current_tile.DistanceManhattan(target_entity->current_tile))) {
					state = ATTACK;
					path_to_target.clear();
					UpdateTile();
					}
				}
			}
		}
		/*
		if ((target_entity != nullptr) && (current_tile.DistanceManhattan(target_entity->current_tile) <= range) && (is_agressive)) {
			state = ATTACK;
			path_to_target.clear();
			UpdateTile();
		}
		*/	

		Move(dt);

		SpatialAudio(position.x, position.y, faction, state, type);

		break;

	case ATTACK:

		if (timer.ReadSec() > action_time) {
			Attack();
		}

		SpatialAudio(position.x, position.y, faction, state, type);

		break;
	case GATHER:
		if (timer.ReadSec() > action_time) {
			Gather();
			state = WALK;
		}
		break;

	case HIT:
		current_animation = &animations[HIT][direction];
		if (current_animation->Finished()) {
			if ((attacking_entity != nullptr) && (is_agressive)) {
				state = ATTACK;
			}
			else {
				state = IDLE;
			}
		}

		SpatialAudio(position.x, position.y, faction, state, type);

		break;
	case DIE:
		if (!delete_timer.Started()) {
			delete_timer.Start();
			direction = TOP_LEFT;
		}

		if (delete_timer.ReadSec() > 5) {
			to_delete = true;
			App->entities->occupied_tiles[current_tile.x][current_tile.y] = false;
			attacking_entity->state = IDLE;
		}
		
		SpatialAudio(position.x, position.y, faction, state, type);

		break;
	default:
		break;
	}

	//Group Movement Request
	if (this->info.current_group != nullptr)
	{
		if (info.current_group->IsGroupLead(this)) {
			if (this->faction == App->player->faction)
				if (App->input->GetKey(SDL_SCANCODE_M) == KEY_UP)
					LOG("Calling group movement!");
					info.current_group->CheckForMovementRequest(App->player->Map_mouseposition, dt);
			/*
			else {
				AI_Player* ai_owner = ((AI_Player*)this->owner);
				// -- Group leader owns any other faction, then store path nodes into a vector to reach enemy base.
				if (target_tile == iPoint(-1,-1)) {

					target_tile = ai_owner->path_to_enemy_base.back();

					if (!App->pathfinding->IsWalkable(target_tile)) {
						LOG("Invalid node");
						target_tile = App->pathfinding->ExpandTile(target_tile);
						LOG("New node position x: %i y: %i", target_tile.x, target_tile.y);
					}
					ai_owner->goal_tile_set = false;
					LOG("Starting position x: %i y: %i", target_tile.x, target_tile.y);
				}
				else if (TargetTileReached(target_tile) == true)
				{
					ai_owner->path_to_enemy_base.pop_back();
					target_tile = ai_owner->path_to_enemy_base.back();

					if (!App->pathfinding->IsWalkable(target_tile))
					{
						LOG("Invalid node");
						target_tile = App->pathfinding->ExpandTile(target_tile);
					}

					LOG("New node position x: %i y: %i", target_tile.x, target_tile.y);

					//if last node is reached go for the base
					if (ai_owner->path_to_enemy_base.size() <= 0) { 
						target_tile = ai_owner->target_player->base->current_tile; 
					}

					ai_owner->goal_tile_set = false;
				}

				// -- Make a movement request each node, when reached we proceed to reach next one until we finish all node list.
				this->info.current_group->CheckForMovementRequest(target_tile, dt);
			}
			*/
		}
		/*
		//if about to reach the base pathfind to attack it
		if (this->faction != App->player->faction) {
			if (((AI_Player*)owner)->path_to_enemy_base.size() == 0) {
				info.current_group->removeUnit(this);
				info.current_group = nullptr;
				target_tile = ((AI_Player*)owner)->target_player->base->current_tile;
				PathfindToPosition(target_tile);
				state = WALK;
				target_entity = ((AI_Player*)owner)->target_player->base;
			}
		}
		*/
	}

	//save dt for animations
	last_dt = dt;

	return true;
}

bool DynamicEntity::PostUpdate() {

	//tile debug
	SDL_Rect tile_rect;
	iPoint tile_tex_position = App->map->MapToWorld(current_tile.x, current_tile.y);

	//debug
	if (App->render->debug) 
	{
		//pathfinding debug
		for (uint j = 0; j < path_to_target.size(); ++j) {
			iPoint pos = App->map->MapToWorld(path_to_target[j].x, path_to_target[j].y);
			tile_rect = { 192, 0, 64,64 };
			App->render->Blit(App->render->debug_tex, pos.x, pos.y, &tile_rect);
		}

		//ally 
		if (faction == App->player->faction) tile_rect = { 0,0,64,64 };
		//enemy
		else tile_rect = { 64,0,64,64 };
		if(App->player->selected_entity != this)
			App->render->Blit(App->render->debug_tex, tile_tex_position.x, tile_tex_position.y, &tile_rect);
	}

	//selected entity
	if (App->player->selected_entity == this || this->info.IsSelected)
	{
		tile_rect = { 320,0,64,64 };
		//blit tile
		App->render->Blit(App->render->debug_tex, tile_tex_position.x, tile_tex_position.y, &tile_rect);
	}

	if (direction >= NO_DIRECTION) {
		if (last_direction < NO_DIRECTION)
			direction = last_direction;
		else 
			direction = BOTTOM_RIGHT;
	}

	current_animation = &animations[state][direction];

	//Render character
	render_position = { (int)(position.x - sprite_size * 0.5f), (int)(position.y - 1.82f * TILE_SIZE)};
	App->render->Blit(reference_entity->texture, render_position.x, render_position.y, &current_animation->GetCurrentFrame(last_dt));

	if (App->render->debug) 
	{
		App->render->DrawQuad({ (int)position.x - 2, (int)position.y - 2 , 4,4 }, 255, 0, 0, 255);
		App->render->DrawQuad({ (int)(next_tile_rect.x), (int)(next_tile_rect.y), next_tile_rect.w, next_tile_rect.h }, 0, 255, 0, 255);
	}

	//Health Bar
	SDL_Rect background_bar = { position.x - HALF_TILE * 0.75f, position.y - TILE_SIZE * 1.5f, 50, 4 };
	SDL_Rect foreground_bar = { position.x - HALF_TILE * 0.75f, position.y - TILE_SIZE * 1.5f, (float)current_health/max_health * 50, 4 };
	SDL_Rect frame = { position.x - HALF_TILE * 0.75f - 1, position.y - TILE_SIZE * 1.5f - 1, 52, 6};
	if (foreground_bar.w < 0) foreground_bar.w = 0;
	App->render->DrawQuad(background_bar, 55, 55, 55, 255);
	App->render->DrawQuad(foreground_bar, 0, 255, 0, 255);
	App->render->DrawQuad(frame, 155, 155, 155, 185, false);

	return true;
}

void DynamicEntity::Move(float dt) {

	if (path_to_target.size() > 0) {

		// -- Get next tile center
		next_tile_position = App->map->MapToWorld(next_tile.x, next_tile.y);
		next_tile_rect = { next_tile_position.x + HALF_TILE - 5, next_tile_position.y + HALF_TILE -3, 10, 10 };

		last_direction = direction;
		direction = GetDirectionToGo(next_tile_rect);

		switch (direction)
		{
		case NO_DIRECTION:
			if (next_tile != target_tile)
			{
				//current_tile = path_to_target.front();
				if (path_to_target.size() > 1)
				{
					next_tile = path_to_target[1];
					
					if (App->entities->occupied_tiles[next_tile.x][next_tile.y]) {
						PathfindToPosition(target_tile);
						return;
					}
					
				}
				path_to_target.erase(path_to_target.begin());
			}
			else if (node_path.size() == 0)
			{
				path_to_target.clear();
				state = IDLE;
			}

			//node movement 
			else if ((node_path.size() > 0)) {
				if (next_tile == target_tile) {
					node_path.pop_back();

					//if we have reached the final node pathfind to target building
					if (node_path.size() == 0)
					{
						if (target_entity != nullptr) {
							target_tile = App->entities->ClosestTile(current_tile, ((StaticEntity*)target_entity)->tiles);
						}
					}
					//if not we keep following the path 
					else {
						target_tile = node_path.back();
					}

					PathfindToPosition(target_tile);
				}
			}

			direction = last_direction;
			UpdateTile();
			break;

		case TOP_LEFT:
			position.x -= speed.x * dt;
			position.y -= speed.y * dt;
			break;
		case TOP_RIGHT:
			position.x += speed.x * dt;
			position.y -= speed.y * dt;
			break;
		case BOTTOM_LEFT:
			position.x -= speed.x * dt;
			position.y += speed.y * dt;
			break;
		case BOTTOM_RIGHT:
			position.x += speed.x * dt;
			position.y += speed.y * dt;
			break;
		default:
			break;
		}
	}
	else
	{
		UpdateTile();
		direction = last_direction;
		state = IDLE;
	}

	UpdateTile();
}

void DynamicEntity::Attack() {

	timer.Start();

	//damage unit if god_mode isn't activated
	if ((target_entity->faction == App->player->faction) && (App->player->god_mode))
		return;

	if (target_entity != nullptr) {
		target_entity->current_health -= damage;

		if (target_entity->is_dynamic) {
			DynamicEntity* dynamic_target = (DynamicEntity*)target_entity;

			if(dynamic_target->attacking_entity == nullptr)
				target_entity->attacking_entity = this;

			path_to_target.clear();
			dynamic_target->state = HIT;
		}
	}

	//Change animation directions to fit
	if (target_entity->is_dynamic) {
		DynamicEntity* dynamic_target = (DynamicEntity*)target_entity;

		if ((current_tile.x > target_entity->current_tile.x) && (current_tile.y == target_entity->current_tile.y)) {
			direction = TOP_LEFT;
			dynamic_target->direction = BOTTOM_RIGHT;
		}
		else if ((current_tile.x == target_entity->current_tile.x) && (current_tile.y > target_entity->current_tile.y)) {
			direction = TOP_RIGHT;
			dynamic_target->direction = BOTTOM_LEFT;
		}
		else if ((current_tile.x == target_entity->current_tile.x) && (current_tile.y < target_entity->current_tile.y)) {
			direction = BOTTOM_LEFT;
			dynamic_target->direction = TOP_RIGHT;
		}
		else if ((current_tile.x < target_entity->current_tile.x) && (current_tile.y == target_entity->current_tile.y)) {
			direction = BOTTOM_RIGHT;
			dynamic_target->direction = TOP_LEFT;
		}

		//Kill enemy
		if (target_entity->current_health <= 0) {
			dynamic_target->state = DIE;
			target_entity = nullptr;
			path_to_target.clear();
			state = IDLE;
		}
	}
	else
	{
		//Destroy building
		StaticEntity* static_target = (StaticEntity*)target_entity;
		if (target_entity->current_health <= 0) {
			static_target->state = DIE;
			attacking_entity = nullptr;
			target_entity = nullptr;
			path_to_target.clear();
			state = IDLE;
		}
	}
}

void DynamicEntity::Gather() {
	uint resource = resource_building->quantity - (resource_building->quantity - damage);

	resource_building->quantity -= resource;
	resource_collected += resource;
	resource_type = resource_building->resource_type;

	StaticEntity* base = owner->base;
	if (base != nullptr) {
		PathfindToPosition(App->entities->ClosestTile(current_tile, base->tiles));
		target_entity = base;
	}
}

void DynamicEntity::StoreGatheredResources() {
	target_entity->volume += resource_collected;

	//update owner resources
	if (resource_type == Resource::CAPS) owner->caps += resource_collected;
	else if (resource_type == Resource::WATER) owner->water += resource_collected;
	else if (resource_type == Resource::FOOD) owner->food += resource_collected;

	if (owner == App->player)
		App->player->UpdateResourceData(resource_type, resource_collected);

	resource_collected = 0;
	target_entity = nullptr;
}

void DynamicEntity::Flee() {
	int minimum_distance = 5;
	int max_distance = 10;
	int range_separation = max_distance - minimum_distance;
	int x, y;
	int x_sign, y_sign;
	iPoint possible_tile;

	do
	{
		srand(time(NULL));
		x = rand() % range_separation;
		y = rand() % range_separation;

		if (rand() % 2 == 0) x_sign = -1;
		else x_sign = 1;

		if (rand() % 2 == 1) y_sign = 1;
		else y_sign = -1;
		
		possible_tile.x = current_tile.x + (minimum_distance + x) * x_sign;
		possible_tile.y = current_tile.y + (minimum_distance + y) * y_sign;

	} while (App->entities->occupied_tiles[possible_tile.x][possible_tile.y]);

	PathfindToPosition(possible_tile);
}

void DynamicEntity::PathfindToPosition(iPoint destination) {

	//if the tile is in the map but it's not walkable
	if (!App->pathfinding->IsWalkable(destination)) 
		destination = App->pathfinding->FindNearestWalkableTile(current_tile, destination);

	if (App->entities->occupied_tiles[destination.x][destination.y])
		destination = App->entities->FindFreeAdjacentTile(current_tile, destination);

	target_tile = destination;

	if (!App->pathfinding->IsWalkable(current_tile)) 
		next_tile = App->pathfinding->FindWalkableAdjacentTile(current_tile);
	
	if (App->pathfinding->CreatePath(current_tile, destination) == -2) 
		node_path = App->pathfinding->CreateNodePath(current_tile, destination);
	

	path_to_target.clear();
	path_to_target = App->pathfinding->GetLastPath();

	state = WALK;

	if (path_to_target.size() > 0) {
		next_tile = path_to_target.front();
	}
}

bool DynamicEntity::LoadAnimations() {
	bool ret = true;
	char* faction_char = { "NoFaction" };
	char* type_char = { "NoType" };
	float speed_reducer = 0.065f;

	//entity faction
	if (faction == VAULT) faction_char = "VaultDwellers";
	else if (faction == BROTHERHOOD) faction_char = "Brotherhood";
	else if (faction == MUTANT) faction_char = "SuperMutant";
	else if (faction == GHOUL) faction_char = "Ghouls";
	else if (faction == ANIMALS) faction_char = "Animals";

	//entity type
	if (type == MELEE) type_char = "Melee";
	else if (type == RANGED) type_char = "Ranged";
	else if (type == GATHERER) type_char = "Gatherer";
	else if (type == BIGHRONER) type_char = "Bighorner";
	else if (type == BRAHAM) type_char = "Braham";
	else if (type == DEATHCLAW) type_char = "Deathclaw";
	
	std::string file = std::string("Assets/textures/characters/").append(faction_char).append("/").append(faction_char).append("_").append(type_char);
	std::string animation_path = file;
	animation_path.append(".tmx");
	std::string texture_path = file;
	texture_path.append(".png");

	pugi::xml_document animation_file;
	pugi::xml_parse_result result = animation_file.load_file(animation_path.c_str());
	texture = App->tex->Load(texture_path.c_str());

	if (result == NULL)
	{
		LOG("Could not load animation tmx file %s. pugi error: %s", file.c_str(), result.description());
		ret = false;
	}

	int tile_width = animation_file.child("map").child("tileset").attribute("tilewidth").as_int();
	int tile_height = animation_file.child("map").child("tileset").attribute("tileheight").as_int();
	int columns = animation_file.child("map").child("tileset").attribute("columns").as_int();
	int firstgid = animation_file.child("map").child("tileset").attribute("firstgid").as_int();
	sprite_size = tile_width;
	int id, tile_id;
	float speed;

	pugi::xml_node animation = animation_file.child("map").child("tileset").child("tile");
	pugi::xml_node frame = animation.child("animation").child("frame");

	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;

	while (animation != nullptr)
	{
		std::string animation_direction = std::string(animation.child("properties").child("property").attribute("value").as_string());
		std::string animation_name = std::string(animation.child("properties").child("property").attribute("name").as_string());
		int direction = TOP_RIGHT;
		State state = IDLE;
		bool loop = true;

		//animation
		if (animation_name == "idle") state = IDLE;
		else if (animation_name == "walk") state = WALK;
		else if (animation_name == "attack") state = ATTACK;
		else if (animation_name == "gather") state = GATHER;
		else if (animation_name == "hit") {
			state = HIT;
			loop = false;}
		else if (animation_name == "die") {
			state = DIE;
			loop = false; }

		//animation direction
		if (animation_direction == "top_left") direction = TOP_LEFT;
		else if (animation_direction == "top_right") direction = TOP_RIGHT;
		else if (animation_direction == "bottom_left") direction = BOTTOM_LEFT;
		else if (animation_direction == "bottom_right") direction = BOTTOM_RIGHT;

		id = animation.attribute("id").as_int();

		while (frame != nullptr) {
			tile_id = frame.attribute("tileid").as_int();
			speed = frame.attribute("duration").as_int() * speed_reducer;
			rect.x = rect.w * ((tile_id) % columns);
			rect.y = rect.h * ((tile_id) / columns);
			animations[state][direction].PushBack(rect, speed);
			frame = frame.next_sibling();
		}
		animations[state][direction].loop = loop;

		animation = animation.next_sibling();
		frame = animation.child("animation").child("frame");
	}

	return ret;
}

bool DynamicEntity::LoadReferenceData() {
	bool ret = true;
	DynamicEntity* dynamic_reference = (DynamicEntity*)reference_entity;

	//load animations
	for (int i = 0; i < NO_STATE; i++)
	{
		for (int j = 0; j < NO_DIRECTION; j++)
		{
			animations[i][j] = dynamic_reference->animations[i][j];
		}
	}

	//load property data
	current_health = max_health = reference_entity->max_health;
	storage_capacity= damage = reference_entity->damage;
	speed = reference_entity->speed;
	sprite_size = reference_entity->sprite_size;

	return ret;
}

Direction DynamicEntity::GetDirectionToGo(SDL_Rect next_tile_rect) const {

	if ((floor(position.x) > ceil(next_tile_rect.x)) && (floor(position.x) < ceil(next_tile_rect.x + next_tile_rect.w))
		&& (ceil(position.y) > floor(next_tile_rect.y)) && (floor(position.y) < ceil(next_tile_rect.y + next_tile_rect.h))) {
		return Direction::NO_DIRECTION;
	}
	if ((ceil(position.x) > floor(next_tile_rect.x + next_tile_rect.w * 0.5f)) && (ceil(position.y) > floor(next_tile_rect.y + next_tile_rect.h * 0.5f))) {
		return Direction::TOP_LEFT;
	}
	else if ((ceil(position.x) > floor(next_tile_rect.x + next_tile_rect.w * 0.5f)) && (floor(position.y) < ceil(next_tile_rect.y + next_tile_rect.h * 0.5f))) {
		return Direction::BOTTOM_LEFT;
	}
	else if ((floor(position.x) < ceil(next_tile_rect.x + next_tile_rect.w * 0.5f)) && (ceil(position.y) > floor(next_tile_rect.y + next_tile_rect.h * 0.5f))) {
		return Direction::TOP_RIGHT;
	}
	else if ((floor(position.x) < ceil(next_tile_rect.x + next_tile_rect.w * 0.5f)) && (floor(position.y) < ceil(next_tile_rect.y + next_tile_rect.h * 0.5f))) {
		return Direction::BOTTOM_RIGHT;
	}
}

void DynamicEntity::UpdateTile() {
	App->entities->occupied_tiles[current_tile.x][current_tile.y] = false;
	current_tile = App->map->WorldToMap(position.x, position.y);
	App->entities->occupied_tiles[current_tile.x][current_tile.y] = true;
}

j1Entity* DynamicEntity::DetectEntitiesInRange() {
	iPoint checked_tile = { -1,-1 };
	j1Entity* detected_entity = nullptr;
	j1Entity* closest_enemy = nullptr;

	entities_in_range.clear();

	for (int y = -detection_radius; y < detection_radius; y++)
	{
		for (int x = -detection_radius; x < detection_radius; x++)
		{
			if ((x != 0) && (y != 0)) {
				checked_tile.x = current_tile.x + x;
				checked_tile.y = current_tile.y + y;

				if (App->entities->occupied_tiles[checked_tile.x][checked_tile.y]) {
					detected_entity = App->entities->FindEntityByTile(checked_tile);
					entities_in_range.push_back(detected_entity);

					if ((detected_entity != nullptr)&&(detected_entity->faction != faction))
						if (closest_enemy == nullptr) {
							closest_enemy = detected_entity;
						}
						else {
							if (detected_entity->current_tile.DistanceManhattan(current_tile) < closest_enemy->current_tile.DistanceManhattan(current_tile)) {
								closest_enemy = detected_entity;
							}
						}
				}
			}
		}
	}
	return closest_enemy;
}

// --- UnitInfo Constructors and Destructor ---

UnitInfo::UnitInfo() {}

UnitInfo::~UnitInfo() {}

UnitInfo::UnitInfo(const UnitInfo& info) : color(info.color) {}
