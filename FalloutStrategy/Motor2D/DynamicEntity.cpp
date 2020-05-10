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
#include "FoWManager.h"

DynamicEntity::DynamicEntity(Faction g_faction, EntityType g_type, iPoint g_current_tile, GenericPlayer* g_owner) : j1Entity() {
	/*
	switch (g_type)
	{
	case MELEE:
		range = 1;
		is_agressive = true;
		break;
	case RANGED:
		range = 6;
		is_agressive = true;
		break;
	case GATHERER:
		range = 1;
		is_agressive = false;
		break;
	case BIGHORNER:
		range = 1;
		is_agressive = false;
		action_time = 20;
		break;
	case BRAHAM:
		range = 1;
		is_agressive = false;
		action_time = 20;
		break;
	case DEATHCLAW:
		range = 1;
		is_agressive = true;
		break;
	case MR_HANDY:
		range = 3;
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

	target_tile = { -1,-1 };
	speed = { 0, 0 };
	sprite_size = 128;

	detection_radius = 6;
	action_timer.Start();
	detection_timer.Start();	

	//Fog Of War
	if (App->render->fog_of_war) {
		if (this->faction == App->player->faction) {
			//Player
			visionEntity = App->fowManager->CreateFoWEntity({ this->current_tile.x, this->current_tile.y }, true);
			visionEntity->SetNewVisionRadius(3);
		}
		else {
			//Enemy
			visionEntity = App->fowManager->CreateFoWEntity({ this->current_tile.x, this->current_tile.y }, false);
			visionEntity->SetNewVisionRadius(3);
		}
		visionEntity->SetNewPosition(App->map->MapToWorld(this->current_tile.x, this->current_tile.y));
	}	
	*/
}

DynamicEntity::~DynamicEntity() {
	target_entity = nullptr;
	reference_entity = nullptr;
	owner = nullptr;
	attacking_entity = nullptr;
	current_animation = nullptr;
	texture = nullptr;
	path_to_target.clear();
}

/*
bool DynamicEntity::Update(float dt) {

	Mix_AllocateChannels(25);
	j1Entity* enemy_in_range = nullptr;

	switch (state)
	{
	case IDLE:
		
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
						state = ATTACK;
						//action_timer.Start();
					}
				}
			}
			else if ((enemy_in_range->type != BRAHAM)&&(enemy_in_range->type != BIGHORNER)) {
				Flee();
				action_timer.Start();
			}
		}
		else if ((type == BRAHAM)||(type  == BIGHORNER))
		{
			if (action_timer.ReadSec() > action_time) {
				Flee();
				action_timer.Start();
			}
		}		

		break;

	case WALK:

		Move(dt);

		if (current_tile.DistanceNoSqrt(target_tile) <= range) {
			if (is_agressive) {
				if (target_entity != nullptr) {
					if ((target_entity->current_tile == target_tile) || (current_tile.DistanceNoSqrt(target_entity->current_tile) <= range)) {
					state = ATTACK;
					path_to_target.clear();
					UpdateTile();
					action_timer.Start();
					target_entity->state = HIT;
					}
				}
			}
			else {
				if (type == GATHERER) {
					if (next_tile == target_tile) {
						//gather
						if (((resource_building != nullptr) && (resource_collected < storage_capacity))
						  ||((resource_collected > 0) && (target_entity != nullptr))) {
							state = GATHER;
							action_timer.Start();
						}
					}
				}
			}
		}

		SpatialAudio(position.x, position.y, faction, state, type);

		break;

	case ATTACK:
		if (target_entity == nullptr)
			state = IDLE;
		else if (target_entity->current_tile.DistanceNoSqrt(current_tile) > range) {
			PathfindToPosition(target_entity->current_tile);
		}
		else if (action_timer.ReadSec() > action_time) {
			Attack();
		}

		SpatialAudio(position.x, position.y, faction, state, type);

		break;
	case GATHER:
		if (action_timer.ReadSec() > action_time) {
			//collect resources
			if (resource_collected < storage_capacity) {
				Gather();
				state = WALK;
			}
			else {
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
					else {
						state = IDLE;
					}
				}
			}
		}
		break;

	case HIT:
		current_animation = &animations[HIT][direction];

		if (current_animation->Finished()) {
			current_animation->Reset();
			if (attacking_entity != nullptr) {
				if (is_agressive) {
					state = ATTACK;
				}
				else {
					Flee();
				}
			}
			else 
			{
				state = IDLE;
			}
		}

		SpatialAudio(position.x, position.y, faction, state, type);

		break;
	case DIE:
		direction = TOP_LEFT;
		if (!delete_timer.Started()) {
			delete_timer.Start();
			direction = TOP_LEFT;

			if (faction == ANIMALS) {
				resource_building = App->entities->CreateResourceSpot(current_tile.x, current_tile.y, resource_type, resource_collected);
				App->entities->occupied_tiles[current_tile.x][current_tile.y] = false;
				current_tile = { -1,-1 };
				next_tile = {-1,-1};
				if (attacking_entity->owner->is_ai) {
					((AI_Player*)attacking_entity->owner)->GatherFood(resource_building);
					attacking_entity = nullptr;
				}
			}
		}

		if (faction != ANIMALS) {
			if (delete_timer.ReadSec() > 4) {
				to_delete = true;
				App->entities->occupied_tiles[current_tile.x][current_tile.y] = false;
				attacking_entity->state = IDLE;
			}
		}
		else {
			if (resource_building->quantity <= 0) {
				to_delete = true;
				App->entities->DestroyResourceSpot(resource_building);
			}
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
				info.current_group->CheckForMovementRequest(App->player->Map_mouseposition, dt);
		}
	}

	//save dt for animations
	last_dt = dt;

	return true;
}
*/

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
		next_tile_rect = { next_tile_position.x + HALF_TILE - 4, next_tile_position.y + HALF_TILE -2, 8, 8 };

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
					}					
				}
				path_to_target.erase(path_to_target.begin());
			}
			else if (node_path.size() == 0)
			{
				//path_to_target.clear();
				state = IDLE;
				direction = last_direction;
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

		if(App->render->fog_of_war)
			visionEntity->SetNewPosition(App->map->MapToWorld(this->current_tile.x, this->current_tile.y));
	}
	else
	{
		UpdateTile();
		direction = last_direction;
		state = IDLE;
	}

	UpdateTile();
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

	} while ((App->entities->occupied_tiles[possible_tile.x][possible_tile.y])&&(!App->pathfinding->IsWalkable(possible_tile)));

	PathfindToPosition(possible_tile);
}

void DynamicEntity::PathfindToPosition(iPoint destination) {
	
	UpdateTile();

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

bool DynamicEntity::LoadAnimations(const char* folder, const char* file_name) {
	bool ret = true;
	float speed_reducer = 0.065f;
	
	std::string tmx = std::string(folder).append(file_name);

	pugi::xml_document animation_file;
	pugi::xml_parse_result result = animation_file.load_file(tmx.c_str());

	std::string image_path = std::string(folder).append(animation_file.child("map").child("tileset").child("image").attribute("source").as_string());
	texture = App->tex->Load(image_path.c_str());

	if (result == NULL)
	{
		LOG("Could not load animation tmx file %s. pugi error: %s", tmx, result.description());
		ret = false;
	}
	else {
		LOG("Successfully loaded %s", tmx);
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
		if (animation_name == "idle") 
			state = IDLE;
		else if (animation_name == "walk") 
			state = WALK;
		else if (animation_name == "attack") 
			state = ATTACK;
		else if (animation_name == "gather") 
			state = GATHER;
		else if (animation_name == "hit") {
			state = HIT;
			loop = false;
		}
		else if (animation_name == "die") {
			state = DIE;
			loop = false; 
		}

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

Direction DynamicEntity::GetDirectionToGo(SDL_Rect next_tile_rect) const {

	if ((ceil(position.x) > floor(next_tile_rect.x)) && (floor(position.x) < ceil(next_tile_rect.x + next_tile_rect.w))
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
