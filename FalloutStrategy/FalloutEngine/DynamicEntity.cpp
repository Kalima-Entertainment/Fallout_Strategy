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
#include "Gatherer.h"
#include <iostream>
#include <string>
#include "SDL_mixer/include/SDL_mixer.h"
#include "FoWManager.h"
#include "AssetsManager.h"

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
	attacking_entity = nullptr;

	target_tile = { -1,-1 };
	speed = { 0, 0 };
	sprite_size = 128;

	detection_radius = 6;
	detection_timer.Start();

	DynaParticle = nullptr;
	visionEntity = nullptr;
}

DynamicEntity::DynamicEntity() {
	target_entity = nullptr;
	target_tile = {-1,-1};
	next_tile = {-1,-1};
	next_tile_position = {-1,-1};
	next_tile_rect = {0,0,0,0};
	detection_radius = 0;
	is_dynamic = true;
	commanded = false;
	is_agressive = false;
	state = IDLE;
	direction = last_direction = TOP_LEFT;
	DynaParticle = nullptr;
	visionEntity = nullptr;
	info.current_group = nullptr;
}

DynamicEntity::~DynamicEntity() {
	target_tile = { -1,-1 };
	next_tile = { -1,-1 };
	next_tile_position = { -1,-1 };
	next_tile_rect = { 0,0,0,0 };
	detection_radius = 0;

	direction = last_direction = TOP_LEFT;

	target_entity = nullptr;
	DynaParticle = nullptr;
	visionEntity = nullptr;
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
	SDL_Rect tile_rect = {0,0,0,0};
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


	//Render character
	render_position = { (int)(position.x - sprite_size * 0.5f), (int)(position.y - 1.82f * TILE_SIZE)};
	
	current_animation = &animations[state][direction];
	
	//Health Bar
	background_health_bar = { (int)(position.x - HALF_TILE * 0.75f),(int)(position.y - TILE_SIZE * 1.25f), 50, 4 };
	foreground_health_bar = { (int)(position.x - HALF_TILE * 0.75f),(int)(position.y - TILE_SIZE * 1.25f), (int)(current_health / max_health * 50), 4 };
	if (foreground_health_bar.w < 0) {
		foreground_health_bar.w = 0;
	}
	frame_quad = { (int)(position.x - HALF_TILE * 0.75f - 1), (int)(position.y - TILE_SIZE * 1.25f - 1), 52, 6 };

	//Fog Of War Rendering Based
	if (current_tile.x >= 0 && current_tile.y >= 0)
	{
		if ((App->fowManager->GetFoWTileState({ this->current_tile })->tileFogBits != fow_ALL)||(App->render->debug))
		{
			//Character Render
			App->render->Blit(texture, render_position.x, render_position.y, &current_animation->GetCurrentFrame(last_dt));

			//Enemy Health Bar only if visible on fog of war and alive
			if (current_health > 0) {
				App->render->DrawQuad(background_health_bar, 75, 75, 75, 255);
				App->render->DrawQuad(foreground_health_bar, 0, 235, 0, 255);
				App->render->DrawQuad(frame_quad, 200, 200, 200, 185, false);		
			}	
		}

		if (App->render->debug) {
			//App->render->DrawQuad(next_tile_rect, 0, 255, 0, 255);
			//App->render->DrawQuad({ (int)position.x, (int)position.y, 2, 2 }, 255, 0, 0, 255);
		}
	}

	return true;
}

bool DynamicEntity::PathfindToPosition(iPoint destination) {

	bool ret = true;
	//UpdateTile();

	iPoint original_destination = destination;

	if (destination == current_tile)
		return ret;

	if (!App->pathfinding->IsWalkable(current_tile)) {
		destination = App->pathfinding->FindWalkableAdjacentTile(current_tile);
		if(!App->pathfinding->IsWalkable(destination))
			LOG("unwalkable origin");
		ret = false;
	}

	//if the tile is in the map but it's not walkable
	if (!App->pathfinding->IsWalkable(destination)) {
		destination = App->pathfinding->FindNearestWalkableTile(current_tile, destination);
		if (!App->pathfinding->IsWalkable(destination))
			LOG("unwalkable destination");
		ret = false;
	}

	if (App->entities->occupied_tiles[destination.x][destination.y]) {
		destination = App->entities->FindFreeAdjacentTile(current_tile, destination);
		if (App->entities->occupied_tiles[destination.x][destination.y])
			LOG("occupied destination");
	}

	if (node_path.size() == 0)
		target_tile = destination;
	else if (original_destination == node_path.back())
		node_path.back() = destination;

	if (current_tile.DistanceManhattan(destination) > DEFAULT_PATH_LENGTH) {
		node_path = App->pathfinding->CreateNodePath(current_tile, destination);

		if (!App->pathfinding->IsWalkable(node_path.back()))
			node_path.back() = App->pathfinding->FindNearestWalkableTile(current_tile, node_path.back());

		if (node_path.back() == current_tile)
			node_path.pop_back();

		App->pathfinding->CreatePath(current_tile, node_path.back());
		//target_tile = node_path.back();
	}
	else {
		if (App->pathfinding->CreatePath(current_tile, destination) == -2) {
			LOG("No");
			if (!App->pathfinding->IsWalkable(destination))
				LOG("Unwalkable destination");
			ret = false;
		}
	}

	path_to_target.clear();
	path_to_target = App->pathfinding->GetLastPath();

	state = WALK;

	if (path_to_target.size() > 0) {
		if (path_to_target.front() == current_tile)
			path_to_target.erase(path_to_target.begin());

		next_tile = path_to_target.front();
	}

	return ret;
}

void DynamicEntity::Move(float dt) {


	// -- Get next tile center
	next_tile_position = App->map->MapToWorld(next_tile.x, next_tile.y);
	next_tile_rect = { next_tile_position.x + HALF_TILE - 4, next_tile_position.y + HALF_TILE -2, 8, 8 };

	last_direction = direction;
	direction = GetDirectionToGo(next_tile_rect);

	fPoint auxPos = position; //We use that variable to optimize Fog Of War code 

	switch (direction)
	{
	case NO_DIRECTION:
		UpdateTile();

		//we are following a node path
		if (node_path.size() > 0) {
			//if we have reached the closest node
			if (current_tile == node_path.back()) {
				//forget it
				node_path.pop_back();
				//if we have more nodes to go to
				if (node_path.size() > 0) {
					//pathfind to the next one
					PathfindToPosition(node_path.back());
				}
				else {
					//if not pathfind to the destination
					PathfindToPosition(target_tile);
				}
			}
		}

		if (current_tile == target_tile){
			direction = last_direction;
			//target_tile = App->entities->FindFreeAdjacentTile(current_tile, target_tile);
			//state = IDLE;
		}
		else {
			if (path_to_target.size() > 0) {
				next_tile = path_to_target.front();

				if (path_to_target.size() > 0)
					path_to_target.erase(path_to_target.cbegin());

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

	//Update Fog Of War Position
	if (auxPos != position && visionEntity != NULL)
		visionEntity->SetNewPosition(App->map->MapToWorld(this->current_tile.x, this->current_tile.y));
}

Direction DynamicEntity::GetDirectionToGo(SDL_Rect next_tile_rect) const {

	if ((ceil(position.x) > next_tile_rect.x) && (floor(position.x) < next_tile_rect.x + next_tile_rect.w)
		&& (ceil(position.y) > next_tile_rect.y) && (floor(position.y) < next_tile_rect.y + next_tile_rect.h)) {
		return Direction::NO_DIRECTION;
	}
	if ((ceil(position.x) > floor(next_tile_rect.x + next_tile_rect.w * 0.5f)) && (ceil(position.y) > floor(next_tile_rect.y + next_tile_rect.h * 0.5f))) {
		return Direction::TOP_LEFT;
	}
	else if ((ceil(position.x) > floor(next_tile_rect.x + next_tile_rect.w * 0.5f)) && (floor(position.y) < ceil(next_tile_rect.y + next_tile_rect.h * 0.5f))) {
		return Direction::BOTTOM_LEFT;
	}
	else if ((floor(position.x < ceil(next_tile_rect.x + next_tile_rect.w * 0.5f))) && (ceil(position.y) > floor(next_tile_rect.y + next_tile_rect.h * 0.5f))) {
		return Direction::TOP_RIGHT;
	}
	else if ((floor(position.x) < ceil(next_tile_rect.x + next_tile_rect.w * 0.5f)) && (floor(position.y) < ceil(next_tile_rect.y + next_tile_rect.h * 0.5f))) {
		return Direction::BOTTOM_RIGHT;
	}

	/*	if ((ceil(position.x) > floor(next_tile_rect.x)) && (floor(position.x) < ceil(next_tile_rect.x + next_tile_rect.w))
		&& (ceil(position.y) > floor(next_tile_rect.y)) && (floor(position.y) < ceil(next_tile_rect.y + next_tile_rect.h))) {
		return Direction::NO_DIRECTION;
	}
	if ((position.x > next_tile_rect.x + next_tile_rect.w * 0.5f) && (position.y > next_tile_rect.y + next_tile_rect.h * 0.5f)) {
		return Direction::TOP_LEFT;
	}
	else if ((position.x > next_tile_rect.x + next_tile_rect.w * 0.5f) && (position.y < next_tile_rect.y + next_tile_rect.h * 0.5f)) {
		return Direction::BOTTOM_LEFT;
	}
	else if ((position.x < next_tile_rect.x + next_tile_rect.w * 0.5f) && (position.y > next_tile_rect.y + next_tile_rect.h * 0.5f)) {
		return Direction::TOP_RIGHT;
	}
	else if ((position.x < next_tile_rect.x + next_tile_rect.w * 0.5f) && (position.y < next_tile_rect.y + next_tile_rect.h * 0.5f)) {
		return Direction::BOTTOM_RIGHT;
	}*/
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

bool DynamicEntity::LoadAnimations(const char* folder, const char* file_name) {
	bool ret = true;
	float speed_reducer = 0.065f;
	
	std::string tmx = std::string(folder).append(file_name);

	pugi::xml_document animation_file;
	
	char* buffer;
	int bytesFile = App->assetManager->Load(tmx.c_str(), &buffer);
	pugi::xml_parse_result result = animation_file.load_buffer(buffer, bytesFile);
	RELEASE_ARRAY(buffer);

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

	for(int y = -detection_radius; y <= detection_radius; y++)
	{
		for(int x = -detection_radius; x <= detection_radius; x++)
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

void DynamicEntity::CheckDestination(iPoint destination) {
	j1Entity* target = App->entities->FindEntityByTile(destination);

	target_entity = target;

	if (target == nullptr) {
		if (type == GATHERER) {
			ResourceBuilding* resource_building = App->entities->FindResourceBuildingByTile(destination);

			//assign a resource building to the entity
			if ((resource_building != nullptr) && (resource_building->quantity > 0)) {
				((Gatherer*)this)->AssignResourceBuilding(resource_building);
			}
		}
		else if (is_agressive) {
			commanded = true;
		}
	}
}

// --- UnitInfo Constructors and Destructor ---

UnitInfo::UnitInfo() {}

UnitInfo::~UnitInfo() {}

UnitInfo::UnitInfo(const UnitInfo& info) : color(info.color) {}
