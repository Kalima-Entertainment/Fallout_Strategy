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
#include "Troop.h"
#include "Deathclaw.h"

#include "Emiter.h"
#include "ParticleSystem.h"

DynamicEntity::DynamicEntity() : j1Entity() {
	dynamic_target = nullptr;
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

	state = NO_STATE;
	direction = last_direction = TOP_LEFT;

	dynamic_target = nullptr;
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
	SDL_Rect position_rect = {0,0,0,0};
	iPoint tile_tex_position = App->map->MapToWorld(current_tile.x, current_tile.y);

	//debug
	if (App->render->debug)
	{
		//pathfinding debug
		for (uint j = 0; j < path_to_target.size(); ++j) {
			iPoint pos = App->map->MapToWorld(path_to_target[j].x, path_to_target[j].y);
			position_rect = { 192, 0, 64,64 };
			App->render->Blit(App->entities->debug_tex, pos.x, pos.y, &position_rect);
		}

		//ally
		if (faction == App->player->faction) {
			if((App->player->selected_entity != this)&&(!info.IsSelected))
			position_rect = { 0,0,64,64 };
		}
		//enemy
		else 
			position_rect = { 64,0,64,64 };

		App->render->Blit(App->entities->debug_tex, tile_tex_position.x, tile_tex_position.y, &position_rect);
	}
	
	if ((App->player->selected_entity == this)||(info.IsSelected)) {
		position_rect = {394, 24, 42, 26};
		App->render->Blit(App->entities->debug_tex, static_cast<int>(position.x - 21), static_cast<int>(position.y - 13), &position_rect);
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
	App->entities->background_health_bar = { 0, 4, 50, 4 };
	App->entities->foreground_health_bar = { 0, 0,  static_cast<int>(current_health / max_health * 50), 4 };
	App->entities->gathering_health_bar = { 0, 8,  static_cast<int>(current_health / max_health * 50), 4 };

	if (App->entities->foreground_health_bar.w < 0 || App->entities->gathering_health_bar.w < 0) {
		App->entities->foreground_health_bar.w = 0;
		App->entities->gathering_health_bar.w = 0;
	}

	//App->entities->frame_quad = { (int)(position.x - HALF_TILE * 0.75f - 1), (int)(position.y - TILE_SIZE * 1.25f - 1), 52, 6 };

	//Fog Of War Rendering Based
	if (current_tile.x >= 0 && current_tile.y >= 0)
	{
		if ((App->fowManager->GetFoWTileState({ this->current_tile })->tileFogBits != fow_ALL)||(!App->render->fog_of_war))
		{
			//Character Render
			App->render->Blit(texture, render_position.x, render_position.y, &current_animation->GetCurrentFrame(last_dt));

			//Enemy Health Bar only if visible on fog of war and alive
			if (current_health > 0) {
				
				if (type == GATHERER) {

					if (dynamic_cast<Gatherer*>(this)->resource_collected > 0) {
						App->render->Blit(App->entities->life_bars, static_cast<int>(position.x - HALF_TILE * 0.75f), static_cast<int>(position.y - TILE_SIZE * 1.25f), &App->entities->background_health_bar);
						App->render->Blit(App->entities->life_bars, static_cast<int>(position.x - HALF_TILE * 0.75f), static_cast<int>(position.y - TILE_SIZE * 1.25f), &App->entities->gathering_health_bar);
					}
					else {
						App->render->Blit(App->entities->life_bars, static_cast<int>(position.x - HALF_TILE * 0.75f), static_cast<int>(position.y - TILE_SIZE * 1.25f), &App->entities->background_health_bar);
						App->render->Blit(App->entities->life_bars, static_cast<int>(position.x - HALF_TILE * 0.75f), static_cast<int>(position.y - TILE_SIZE * 1.25f), &App->entities->foreground_health_bar);
					}
				}
				else {

					App->render->Blit(App->entities->life_bars, static_cast<int>(position.x - HALF_TILE * 0.75f), static_cast<int>(position.y - TILE_SIZE * 1.25f), &App->entities->background_health_bar);
					App->render->Blit(App->entities->life_bars, static_cast<int>(position.x - HALF_TILE * 0.75f), static_cast<int>(position.y - TILE_SIZE * 1.25f), &App->entities->foreground_health_bar);

				}
				
				//App->render->DrawQuad(App->entities->background_health_bar, 75, 75, 75, 255, true, true);
				//App->render->DrawQuad(App->entities->foreground_health_bar, 0, 235, 0, 255, true, true);
				//App->render->DrawQuad(App->entities->frame_quad, 200, 200, 200, 185, false, true);
			
			}
		}

		if (App->render->debug) {
			App->render->DrawQuad(next_tile_rect, 0, 255, 0, 255, true, true);
			App->render->DrawQuad({ (int)position.x, (int)position.y, 2, 2 }, 255, 0, 0, 255, true, true);
		}
	}

	return true;
}

bool DynamicEntity::PathfindToPosition(iPoint destination) {

	bool ret = true;

	iPoint original_destination = destination;

	if (destination == current_tile)
		return ret;

	if (!App->pathfinding->IsWalkable(current_tile)) {
		next_tile = App->pathfinding->FindNearestWalkableTile(current_tile, destination);
		ret = App->pathfinding->IsWalkable(next_tile);
	}

	//if the tile is in the map but it's not walkable
	if (!App->pathfinding->IsWalkable(destination)) {
		destination = App->pathfinding->FindNearestWalkableTile(current_tile, destination);

		ret = App->pathfinding->IsWalkable(destination);
	}

	if (App->entities->occupied_tiles[destination.x][destination.y]) {
		destination = App->entities->FindClosestFreeTile(current_tile, destination);
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

		if (!App->pathfinding->IsWalkable(node_path.back()))
			node_path.back() = App->pathfinding->ExpandTile(node_path.back());

		if(node_path.size() > 0)
			App->pathfinding->CreatePath(current_tile, node_path.back());
	}
	else {
		if (App->pathfinding->CreatePath(current_tile, destination) == -1) {
			ret = App->pathfinding->IsWalkable(destination);
		}
	}

	path_to_target.clear();
	path_to_target = App->pathfinding->GetLastPath();

	if (path_to_target.size() > 0) {
		if (path_to_target.front() == current_tile)
			path_to_target.erase(path_to_target.begin());

		if (path_to_target.size() > 0)
			next_tile = path_to_target.front();
	}

	if (ret == true)
		state = WALK;

	return ret;
}

void DynamicEntity::Move(float dt) {
	fPoint auxPos = position; //We use that variable to optimize Fog Of War code

	//check if the tile that wants to be occupied is already occupied
	if (!App->entities->IsTileInPositionOccupied(position))
		UpdateTile();
	else {
		//iPoint tile = App->map->fWorldToMap(position.x, position.y);
		//if (App->entities->FindEntityByTile(tile, this) != nullptr) {
			if (node_path.size() > 0) {
				if ((current_tile.DistanceManhattan(node_path.back()) <= 4)
					||(current_tile.DistanceManhattan(node_path.back()) <= 8) && !App->pathfinding->IsWalkable(node_path.back()))
					node_path.pop_back();
				if (node_path.size() > 0)
					PathfindToPosition(node_path.back());
				else
					PathfindToPosition(target_tile);
			}
			else {
				PathfindToPosition(target_tile);
			}
		//}
	}

	//Follow node path -----------------------------------
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

	//Follow normal path ---------------------------------
	if (current_tile == target_tile) {
		direction = last_direction;
		path_to_target.clear();
		commanded = false;
	}
	else if (current_tile == next_tile){
		//if there is a new path erase the first element
		if (path_to_target.size() > 0)
			path_to_target.erase(path_to_target.cbegin());

		//if the entity has a path to follow
		if (path_to_target.size() > 0) {
			//next tile is the first tile in the list
			next_tile = path_to_target.front();

			//if next tile is occupied create the path again
			if (App->entities->IsTileOccupied(next_tile))
				PathfindToPosition(target_tile);
		}
		else {
			PathfindToPosition(target_tile);
		}
	}

	// -- Get next tile center
	next_tile_position = App->map->MapToWorld(next_tile.x, next_tile.y);
	next_tile_rect = { next_tile_position.x + HALF_TILE - 1, next_tile_position.y + HALF_TILE - 1, 1, 1 };

	//direction movement ---------------------------------
	last_direction = direction;
	direction = GetDirectionToGo(next_tile_rect);

	switch (direction) 
	{
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
	//---------------------------------------------------

	//Update Fog Of War Position
	if (auxPos != position && visionEntity != nullptr)
		visionEntity->SetNewPosition(App->map->MapToWorld(this->current_tile.x, this->current_tile.y));
}

Direction DynamicEntity::GetDirectionToGo(SDL_Rect next_tile_rect) const {

	if ((ceil(position.x) > floor(next_tile_rect.x)) && (floor(position.x) < ceil(next_tile_rect.x + next_tile_rect.w))
		&& (ceil(position.y) > floor(next_tile_rect.y)) && (floor(position.y) < ceil(next_tile_rect.y + next_tile_rect.h))) {
		return last_direction;
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
	else {
		return last_direction;
	}
}

Direction DynamicEntity::GetBuildingDirection(std::vector<iPoint> building_tiles) const {
	//we get the closest tile to the entity
	iPoint closest_tile = App->entities->ClosestTile(current_tile, building_tiles);

	//We get the center of the tile in world position and create it
	closest_tile = App->map->MapToWorld(closest_tile.x, closest_tile.y);
	SDL_Rect closest_tile_center = { closest_tile.x + 31, closest_tile.y + 31, 2, 2 };

	return GetDirectionToGo(closest_tile_center);
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
	current_tile = App->map->fWorldToMap(position.x, position.y);
	App->entities->occupied_tiles[current_tile.x][current_tile.y] = true;
}

DynamicEntity* DynamicEntity::DetectEntitiesInRange() {
	iPoint checked_tile = { -1,-1 };
	j1Entity* detected_entity = nullptr;
	DynamicEntity* closest_enemy = nullptr;

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
								closest_enemy = dynamic_cast<DynamicEntity*>(detected_entity);
							}
							else {
								if (detected_entity->current_tile.DistanceManhattan(current_tile) < closest_enemy->current_tile.DistanceManhattan(current_tile)) {
									closest_enemy = dynamic_cast<DynamicEntity*>(detected_entity);
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

	if (target == nullptr) {
		if (type == GATHERER) {
			ResourceBuilding* resource_building = App->entities->FindResourceBuildingByTile(destination);

			//assign a resource building to the entity
			if ((resource_building != nullptr) && (resource_building->quantity > 0)) {
				dynamic_cast<Gatherer*>(this)->AssignResourceBuilding(resource_building);
			}
			else {
				dynamic_cast<Gatherer*>(this)->gathering = false;
			}
		}
		else if (is_agressive) {
			if ((type == MELEE) || (type == RANGED) || (type == MR_HANDY)) {
				dynamic_cast<Troop*>(this)->target_building = nullptr;
				dynamic_cast<Troop*>(this)->dynamic_target = nullptr;
			}
			else if (type == DEATHCLAW) {
				dynamic_cast<Deathclaw*>(this)->target_building = dynamic_cast<StaticEntity*>(target);
			}
		}
	}
	else{
		if (target->is_dynamic) {
			dynamic_target = dynamic_cast<DynamicEntity*>(target);
			if (((type == MELEE) || (type == RANGED) || (type == MR_HANDY)) && (faction != target->faction)) {
				dynamic_cast<Troop*>(this)->target_building = nullptr;
			}
		}
		else {
			if (((type == MELEE) || (type == RANGED) || (type == MR_HANDY)) && (faction != target->faction)) {
				dynamic_cast<Troop*>(this)->target_building = dynamic_cast<StaticEntity*>(target);
				dynamic_cast<Troop*>(this)->dynamic_target = nullptr;
			}
			else if(type == DEATHCLAW)
				dynamic_cast<Deathclaw*>(this)->target_building = dynamic_cast<StaticEntity*>(target);
		}

		if (type == GATHERER) {
			dynamic_cast<Gatherer*>(this)->gathering = true;
			ResourceBuilding* resource_building = App->entities->FindResourceBuildingByTile(destination);

			//assign a resource building to the entity
			if ((resource_building != nullptr) && (resource_building->quantity > 0)) {
				dynamic_cast<Gatherer*>(this)->AssignResourceBuilding(resource_building);
			}
		}
	}
}

// --- UnitInfo Constructors and Destructor ---

UnitInfo::UnitInfo() {}

UnitInfo::~UnitInfo() {}

UnitInfo::UnitInfo(const UnitInfo& info) : color(info.color) {}
