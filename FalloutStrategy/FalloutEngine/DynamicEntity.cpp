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

#include "Emiter.h"
#include "ParticleSystem.h"

DynamicEntity::DynamicEntity(Faction g_faction, EntityType g_type, iPoint g_current_tile, GenericPlayer* g_owner) : j1Entity() {
	owner = g_owner;
	type = g_type;
	faction = g_faction;
	current_tile = g_current_tile;

	position = App->map->floatMapToWorld(current_tile.x, current_tile.y);
	position.x += HALF_TILE;
	position.y += HALF_TILE;

	is_dynamic = true;

	state = IDLE;
	direction = BOTTOM_RIGHT;

	target_entity = nullptr;
	//resource_building = nullptr;
	attacking_entity = nullptr;

	target_tile = { -1,-1 };
	speed = { 0, 0 };
	sprite_size = 128;

	detection_radius = 6;
	//action_timer.Start();
	detection_timer.Start();

	DynaParticle = nullptr;
	visionEntity = nullptr;
}

DynamicEntity::DynamicEntity() {
	target_entity = nullptr;
	is_dynamic = true;
	commanded = false;
	state = IDLE;
	direction = TOP_LEFT;
}

DynamicEntity::~DynamicEntity() {
	target_entity = nullptr;
	reference_entity = nullptr;
	owner = nullptr;
	attacking_entity = nullptr;
	current_animation = nullptr;
	texture = nullptr;
	path_to_target.clear();
	entities_in_range.clear();
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

		App->render->DrawQuad({ (int)position.x - 2, (int)position.y - 2 , 4,4 }, 255, 0, 0, 255);
		App->render->DrawQuad({ (int)(next_tile_rect.x), (int)(next_tile_rect.y), next_tile_rect.w, next_tile_rect.h }, 0, 255, 0, 255);
	}

	//selected entity
	if (App->player->selected_entity == this || this->info.IsSelected) {
		tile_rect = { 320,0,64,64 };
		App->render->Blit(App->render->debug_tex, tile_tex_position.x, tile_tex_position.y, &tile_rect);
	}

	if (direction >= NO_DIRECTION) {
		if (last_direction < NO_DIRECTION)
			direction = last_direction;
		else 
			direction = BOTTOM_RIGHT;
	}

	//Health Bar
	background_health_bar = { (int)(position.x - HALF_TILE * 0.75f),(int)(position.y - TILE_SIZE * 1.5f), 50, 4 };
	foreground_health_bar = { (int)(position.x - HALF_TILE * 0.75f),(int)(position.y - TILE_SIZE * 1.5f), (int)(current_health/max_health * 50), 4 };
	if (foreground_health_bar.w < 0)
		foreground_health_bar.w = 0;
	frame_quad = { (int)(position.x - HALF_TILE * 0.75f - 1), (int)(position.y - TILE_SIZE * 1.5f - 1), 52, 6 };

	//Render character
	render_position = { (int)(position.x - sprite_size * 0.5f), (int)(position.y - 1.82f * TILE_SIZE)};
	
	current_animation = &animations[state][direction];

	//Fog Of War Rendering Based
	if(this->current_tile.x >= 0 && this->current_tile.y >= 0)
		if (App->fowManager->GetFoWTileState({ this->current_tile })->tileFogBits != fow_ALL)
		{
			//Character Render
			App->render->Blit(texture, render_position.x, render_position.y, &current_animation->GetCurrentFrame(last_dt));

			//Enemy Health Bar only if visible on fog of war
			App->render->DrawQuad(background_health_bar, 55, 55, 55, 255);
			App->render->DrawQuad(foreground_health_bar, 0, 255, 0, 255);
			App->render->DrawQuad(frame_quad, 155, 155, 155, 185, false);			
		}
		else if ((this->faction == NO_FACTION)||(App->render->debug)) {
			//Animals are also visible on shroud
			if (App->fowManager->GetFoWTileState({ this->current_tile })->tileShroudBits == fow_ALL)
			{
				//Character Render
				App->render->Blit(texture, render_position.x, render_position.y, &current_animation->GetCurrentFrame(last_dt));

				//Enemy Health Bar only if visible on fog of war
				App->render->DrawQuad(background_health_bar, 55, 55, 55, 255);
				App->render->DrawQuad(foreground_health_bar, 0, 255, 0, 255);
				App->render->DrawQuad(frame_quad, 155, 155, 155, 185, false);
			}
		}
	
	return true;
}

void DynamicEntity::Move(float dt) {

	if (path_to_target.size() > 0) {

		// -- Get next tile center
		next_tile_position = App->map->MapToWorld(next_tile.x, next_tile.y);
		next_tile_rect = { next_tile_position.x + HALF_TILE - 4, next_tile_position.y + HALF_TILE -2, 8, 8 };

		last_direction = direction;
		direction = GetDirectionToGo(next_tile_rect);

		fPoint auxPos = position; //We use that variable to optimize Fog Of War code 

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
			//node movement 
			else if ((node_path.size() > 0)) {
				if (next_tile == target_tile) {
					node_path.pop_back();

					//if we have reached the final node pathfind to target building
					if (node_path.size() > 0)
					{
						target_tile = node_path.back();
					}

					PathfindToPosition(target_tile);
				}
			}
			else if (node_path.size() == 0)
			{
				path_to_target.clear();
				state = IDLE;
				direction = last_direction;
				commanded = false;
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

		//Update Fog Of War Position
		if (auxPos != position && visionEntity != NULL)
			visionEntity->SetNewPosition(App->map->MapToWorld(this->current_tile.x, this->current_tile.y));
	}
	else
	{
		UpdateTile();
		direction = last_direction;
		state = IDLE;
		commanded = false;
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

	if (!App->pathfinding->IsWalkable(current_tile))
		destination = App->pathfinding->FindWalkableAdjacentTile(current_tile);

	//if the tile is in the map but it's not walkable
	if (!App->pathfinding->IsWalkable(destination)) 
		destination = App->pathfinding->FindNearestWalkableTile(current_tile, destination);

	if (App->entities->occupied_tiles[destination.x][destination.y])
		destination = App->entities->FindFreeAdjacentTile(current_tile, destination);

	target_tile = destination;
	
	if (current_tile.DistanceManhattan(destination) > DEFAULT_PATH_LENGTH) {
		node_path = App->pathfinding->CreateNodePath(current_tile, destination);

		if (!App->pathfinding->IsWalkable(node_path.back()))
			node_path.back() = App->pathfinding->FindNearestWalkableTile(current_tile, node_path.back());

		App->pathfinding->CreatePath(current_tile, node_path.back());
		target_tile = node_path.back();
	}
	else {
		if (App->pathfinding->CreatePath(current_tile, destination) == -2)
			LOG("No");
	}
	
	path_to_target.clear();
	path_to_target = App->pathfinding->GetLastPath();

	state = WALK;

	if (path_to_target.size() > 0) {
		if (path_to_target.front() == current_tile)
			path_to_target.erase(path_to_target.begin());

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
		//LOG("Successfully loaded %s", tmx);
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

	for (int y = -detection_radius; y <= detection_radius; y++)
	{
		for (int x = -detection_radius; x <= detection_radius; x++)
		{
			if (x != 0 || y != 0) {
				checked_tile.x = current_tile.x + x;
				checked_tile.y = current_tile.y + y;

				if (App->entities->occupied_tiles[checked_tile.x][checked_tile.y]) {

					detected_entity = App->entities->FindEntityByTile(checked_tile);

					if ((detected_entity)&&(detected_entity->is_dynamic) &&(detected_entity->state != DIE)) {
						entities_in_range.push_back(detected_entity);

						if (detected_entity->faction != faction)
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
	}
	return closest_enemy;
}

// --- UnitInfo Constructors and Destructor ---

UnitInfo::UnitInfo() {}

UnitInfo::~UnitInfo() {}

UnitInfo::UnitInfo(const UnitInfo& info) : color(info.color) {}
