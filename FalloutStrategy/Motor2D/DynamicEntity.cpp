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
#include "AI_Player.h"
#include "StaticEntity.h"
#include <iostream>
#include <string>
#include "SDL_mixer/include/SDL_mixer.h"

DynamicEntity::DynamicEntity(Faction g_faction, EntityType g_type) {

	switch (g_type)
	{
	case MELEE:
		range = 1;
		break;
	case RANGED:
		range = 3;
		break;
	case GATHERER:
		range = 1;
		break;
	default:
		break;
	}

	type = g_type;
	faction = g_faction;
	state = IDLE;
	direction = BOTTOM_RIGHT;

	speed = { 0, 0 };
	target_entity = nullptr;
	resource_building = nullptr;
	action_time = 3.0f;
	resource_collected = 0;
	sprite_size = 128;	
	target_tile = { -1,-1 };
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

	switch (state)
	{
	case IDLE:
		if (node_path.size() > 0) {
			target_tile = node_path.back();
			node_path.pop_back();
			PathfindToPosition(target_tile);
			state = WALK;
		}
		break;
	case WALK:
		//if the entitiy is about to reach it's target tile
		if (current_tile.LinealDistance(target_tile) <= range) {
			//we reach the destination and there is an entity in it
				//ranged and melee
			if (type != GATHERER) {
				if (target_entity != nullptr)
				{
					//enemy target
					if (faction != target_entity->faction) {
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
						//return;
					}

					//give gathered resources
					else if ((resource_collected > 0) && (target_entity != nullptr) && (target_entity->volume < target_entity->storage_capacity)) {
						target_entity->volume += resource_collected;

						if (resource_type == Resource::CAPS)
							owner->caps += resource_collected;
						else if (resource_type == Resource::WATER)
							owner->water += resource_collected;
						else if (resource_type == Resource::FOOD)
							owner->food += resource_collected;

						if (owner == App->player)
							App->player->UpdateResourceData(resource_type, resource_collected);

						resource_collected = 0;
						target_entity = nullptr;

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

		Move(dt);

		if (reference_entity->faction == MUTANT)
			if (Mix_Playing(2) == 0) { SpatialAudio(App->audio->Brotherhood_walk, 2, position.x, position.y); }
		if (reference_entity->faction == VAULT)
			if (Mix_Playing(17) == 0) { SpatialAudio(App->audio->Brotherhood_walk, 17, position.x, position.y); }
		if (reference_entity->faction == BROTHERHOOD)
			if (Mix_Playing(18) == 0) { SpatialAudio(App->audio->Brotherhood_walk, 18, position.x, position.y); }
		if (reference_entity->faction == GHOUL)
			if (Mix_Playing(19) == 0) { SpatialAudio(App->audio->Brotherhood_walk, 19, position.x, position.y); }

		if ((node_path.size() > 0)){// && (App->pathfinding->GetLastPathRequestTime() > 500)) {
			if (next_tile == target_tile) {
				node_path.pop_back();

				//if we have reached the final node pathfind to target building
				if (node_path.size() == 0) 
				{
					target_tile = App->entities->ClosestTile(current_tile, ((StaticEntity*)target_entity)->tiles);
				}
				//if not we keep following the path 
				else 
				{
					target_tile = node_path.back();
				}

				PathfindToPosition(target_tile);
			}
		}

		break;

	case ATTACK:

		if (timer.ReadSec() > action_time)
		{
			Attack();
		}

		if (reference_entity->faction == MUTANT && reference_entity->type == RANGED || reference_entity->faction == BROTHERHOOD && reference_entity->type == RANGED)
			if (Mix_Playing(15) == 0) { SpatialAudio(App->audio->minigun, 15, position.x, position.y); }
		if (reference_entity->faction == VAULT && reference_entity->type == RANGED)
			if (Mix_Playing(16) == 0) { SpatialAudio(App->audio->pistol, 16, position.x, position.y); }
		if (reference_entity->faction == GHOUL && reference_entity->type == RANGED)
			if (Mix_Playing(20) == 0) { SpatialAudio(App->audio->pistol2, 20, position.x, position.y); }

		if (reference_entity->faction == MUTANT && reference_entity->type != RANGED)
			if (Mix_Playing(3) == 0) { SpatialAudio(App->audio->Mutant_attack, 3, position.x, position.y); }
		if (reference_entity->faction == VAULT && reference_entity->type != RANGED)
			if (Mix_Playing(4) == 0) { SpatialAudio(App->audio->Vault_attack, 4, position.x, position.y); }
		if (reference_entity->faction == BROTHERHOOD && reference_entity->type != RANGED)
			if (Mix_Playing(5) == 0) { SpatialAudio(App->audio->Brotherhood_attack, 5, position.x, position.y); }
		if (reference_entity->faction == GHOUL && reference_entity->type != RANGED)
			if (Mix_Playing(6) == 0) { SpatialAudio(App->audio->Ghoul_attack, 6, position.x, position.y); }
	

		break;
	case GATHER:
		if (timer.ReadSec() > action_time)
		{
			Gather();
			state = WALK;
		}
		break;
	case HIT:
		if (current_animation->Finished())
		{
			state = IDLE;
			current_animation->Reset();
		}

		if (reference_entity->faction == MUTANT)
			if (Mix_Playing(7) == 0) { SpatialAudio(App->audio->Mutant_hit, 7, position.x, position.y); }
		if (reference_entity->faction == VAULT)
			if (Mix_Playing(8) == 0) { SpatialAudio(App->audio->Vault_hit, 8, position.x, position.y); }
		if (reference_entity->faction == BROTHERHOOD)
			if (Mix_Playing(9) == 0) { SpatialAudio(App->audio->Brotherhood_hit, 9, position.x, position.y); }
		if (reference_entity->faction == GHOUL)
			if (Mix_Playing(10) == 0) { SpatialAudio(App->audio->Ghoul_hit, 10, position.x, position.y); }
		
		break;
	case DIE:
		if (current_animation->Finished())
		{
			attacking_entity->target_entity = nullptr;
			to_destroy = true;
		}
		
		if (reference_entity->faction == MUTANT)
			if (Mix_Playing(11) == 0) { SpatialAudio(App->audio->Mutant_die, 11, position.x, position.y); }
		if (reference_entity->faction == VAULT)
			if (Mix_Playing(12) == 0) { SpatialAudio(App->audio->Vault_die, 12, position.x, position.y); }
		if (reference_entity->faction == BROTHERHOOD)
			if (Mix_Playing(13) == 0) { SpatialAudio(App->audio->Brotherhood_die, 13, position.x, position.y); }
		if (reference_entity->faction == GHOUL)
			if (Mix_Playing(14) == 0) { SpatialAudio(App->audio->Ghoul_die, 14, position.x, position.y); }
		
		
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
	iPoint tex_position = App->map->MapToWorld(current_tile.x, current_tile.y);

	//selected entity
	if (App->player->selected_entity == this)
	{
		tile_rect = { 128,0,64,64 };
		//blit tile
		App->render->Blit(App->render->debug_tex, tex_position.x, tex_position.y, &tile_rect);
	}
	//debug
	else if (App->render->debug) 
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
		App->render->Blit(App->render->debug_tex, tex_position.x, tex_position.y, &tile_rect);
	}

	current_animation = &animations[state][direction];

	//Render character
	render_position = { (int)(position.x - sprite_size * 0.5f), (int)(position.y - 1.82f * TILE_SIZE)};
	App->render->Blit(reference_entity->texture, render_position.x, render_position.y, &current_animation->GetCurrentFrame(last_dt));

	if (App->render->debug)
		App->render->DrawQuad({ (int)position.x -2, (int)position.y -2 , 4,4 }, 255, 0, 0, 255);

	//Rendering Selected Units Quad
	if (this->info.IsSelected) DrawQuad();

	//Health Bar
	SDL_Rect background_bar = { position.x - HALF_TILE * 0.75f, position.y - TILE_SIZE * 1.5f, 50, 4 };
	SDL_Rect foreground_bar = { position.x - HALF_TILE * 0.75f, position.y - TILE_SIZE * 1.5f, (float)current_health/max_health * 50, 4 };
	if (foreground_bar.w < 0) foreground_bar.w = 0;
	App->render->DrawQuad(background_bar, 255, 255, 255, 255);
	App->render->DrawQuad(foreground_bar, 0, 255, 0, 255);


	return true;
}

void DynamicEntity::Move(float dt) {
	if (path_to_target.size() > 0) {
		//get next tile center
		next_tile_position = App->map->MapToWorld(next_tile.x, next_tile.y);
		next_tile_rect_center = { next_tile_position.x + HALF_TILE - 2, next_tile_position.y + HALF_TILE, 4, 4 };

		//move to next tile
		if ((position.x > next_tile_rect_center.x + next_tile_rect_center.w) && (position.x > next_tile_rect_center.x) && (position.y > next_tile_rect_center.y) && (position.y > next_tile_rect_center.y + next_tile_rect_center.h)) {
			direction = TOP_LEFT;
			position.x -= speed.x * dt;
			position.y -= speed.y * dt;
		}
		else if ((position.x < next_tile_rect_center.x) && (position.x < next_tile_rect_center.x + next_tile_rect_center.w) && (position.y > next_tile_rect_center.y) && (position.y > next_tile_rect_center.y + next_tile_rect_center.h)) {
			direction = TOP_RIGHT;
			position.x += speed.x * dt;
			position.y -= speed.y * dt;
		}
		else if ((position.x > next_tile_rect_center.x) && (position.x > next_tile_rect_center.x + next_tile_rect_center.w) && (position.y < next_tile_rect_center.y) && (position.y < next_tile_rect_center.y + next_tile_rect_center.h)) {
			direction = BOTTOM_LEFT;
			position.x -= speed.x * dt;
			position.y += speed.y * dt;
		}
		else if ((position.x < next_tile_rect_center.x) && (position.x < next_tile_rect_center.x + next_tile_rect_center.w) && (position.y < next_tile_rect_center.y) && (position.y < next_tile_rect_center.y + next_tile_rect_center.h)) {
			direction = BOTTOM_RIGHT;
			position.x += speed.x * dt;
			position.y += speed.y * dt;
		}
		else
		{
			if (path_to_target.front() != target_tile)
			{
				current_tile = path_to_target.front();
				if (path_to_target.size() > 1)
				{
					next_tile = path_to_target[1];
				}
				path_to_target.erase(path_to_target.begin());
			}
			else
			{
				position.x = next_tile_rect_center.x + 2;
				position.y = next_tile_rect_center.y + 2;
				current_tile = target_tile;
				state = IDLE;
			}
		}
	}
}

void DynamicEntity::Attack() {

	timer.Start();

	//damage unit if god_mode isn't activated 
	if ((target_entity->faction != App->player->faction) || (!App->player->god_mode)) {
		target_entity->current_health -= damage;

		if (target_entity->is_dynamic) {
			DynamicEntity* dynamic_target = (DynamicEntity*)target_entity;
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
			dynamic_target->direction = TOP_LEFT;
			target_entity = nullptr;
			state = IDLE;
		}
	}
	else
	{
		//Destroy building
		StaticEntity* static_target = (StaticEntity*)target_entity;
		if (target_entity->current_health <= 0) {
			static_target->state = EXPLODE;
			target_entity = nullptr;
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
	//resource_building = nullptr;
}

void DynamicEntity::PathfindToPosition(iPoint destination) {

	current_tile = App->map->WorldToMap(position.x, position.y);

	//if the tile is in the map but it's not walkable
	if ((!App->pathfinding->IsWalkable(destination)) && (App->pathfinding->CheckBoundaries(destination))) 
	{
		destination = App->pathfinding->FindNearestWalkableTile(current_tile, destination);
	}

	target_tile = destination;

	if (App->pathfinding->CreatePath(current_tile, destination) == -1) {
		LOG("Invalid path");
		state = IDLE;
	}

	path_to_target.clear();
	path_to_target = App->pathfinding->GetLastPath();

	state = WALK;

	if (path_to_target.size() > 0)
		next_tile = path_to_target.front();
}

/*
bool DynamicEntity::LoadFx() {
	bool ret = true;
	char* faction_char = { "NoFaction" };
	char* state_char = { "NoState" };


	for (int faction = VAULT; faction < NO_FACTION; faction++)
	{
		//entity faction
		if (faction == VAULT)
			faction_char = "VaultDwellers";
		else if (faction == BROTHERHOOD)
			faction_char = "Brotherhood";
		else if (faction == MUTANT)
			faction_char = "SuperMutant";
		else if (faction == GHOUL)
			faction_char = "Ghouls";
	}

	for (int animation = IDLE; animation < MAX_ANIMATIONS; animation++)
	{
		//entity action
		if (animation == IDLE)
			state_char = "Idle";
		else if (animation == WALK)
			state_char = "Walk";
		else if (animation == ATTACK)
			state_char = "Attack";
		else if (animation == GATHER)
			state_char = "Gather";
		else if (animation == HIT)
			state_char = "Hit";
		else if (animation == DIE)
			state_char = "Die";

		std::string file = std::string("audio/fx/CharactersSounds/").append(faction_char).append("/").append(faction_char).append("_").append(state_char).append(".WAV");

		fx[animation] = App->audio->LoadFx(file.c_str());
	}

	return ret;
}
*/

bool DynamicEntity::LoadAnimations() {
	bool ret = true;
	char* faction_char = { "NoFaction" };
	char* type_char = { "NoType" };
	float speed_reducer = 0.065f;

	//entity faction
	if (faction == VAULT)
		faction_char = "VaultDwellers";
	else if (faction == BROTHERHOOD)
		faction_char = "Brotherhood";
	else if (faction == MUTANT)
		faction_char = "SuperMutant";
	else if (faction == GHOUL)
		faction_char = "Ghouls";

	//entity type
	if (type == MELEE)
		type_char = "Melee";
	else if (type == RANGED)
		type_char = "Ranged";
	if (type == GATHERER)
		type_char = "Gatherer";

	std::string file = std::string("textures/characters/").append(faction_char).append("/").append(faction_char).append("_").append(type_char);
	std::string animation_path = file;
	animation_path.append(".tmx");
	std::string texture_path = file;
	texture_path.append(".png");

	pugi::xml_document animation_file;
	pugi::xml_parse_result result = animation_file.load_file(animation_path.c_str());

	std::string image = std::string(animation_file.child("tileset").child("image").attribute("source").as_string());
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
		std::string animation_direction = std::string(animation.child("properties").child("property").attribute("value").as_string());
		std::string animation_name = std::string(animation.child("properties").child("property").attribute("name").as_string());
		int direction = TOP_RIGHT;
		DynamicState state = IDLE;
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
		if (animation_direction == "top_left")
			direction = TOP_LEFT;
		else if (animation_direction == "top_right")
			direction = TOP_RIGHT;
		else if (animation_direction == "bottom_left")
			direction = BOTTOM_LEFT;
		else if (animation_direction == "bottom_right")
			direction = BOTTOM_RIGHT;

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
	for (int i = 0; i < MAX_ANIMATIONS; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			animations[i][j] = dynamic_reference->animations[i][j];
		}
	}

	//load property data
	current_health = max_health = reference_entity->max_health;
	storage_capacity= damage = reference_entity->damage;
	speed = reference_entity->speed;

	return ret;
}

void DynamicEntity::DrawQuad()
{
	const SDL_Rect entityrect = { position.x - sprite_size * 0.5f  + 32,  position.y - 1.82f * TILE_SIZE + 32,  64,  96 };
	App->render->DrawQuad(entityrect, unitinfo.color.r, unitinfo.color.g, unitinfo.color.b, unitinfo.color.a, false);
}

bool DynamicEntity::TargetTileReached(iPoint target_tile) {
	bool ret = false;
	
	//north-west
	if ((current_tile.x - 1 == target_tile.x) && (current_tile.y - 1 == target_tile.y))
		return true;
	//north
	if ((current_tile.x == target_tile.x) && (current_tile.y - 1 == target_tile.y))
		return true;
	//north-east
	if ((current_tile.x + 1 == target_tile.x) && (current_tile.y - 1 == target_tile.y))
		return true;
	//east
	if ((current_tile.x + 1 == target_tile.x) && (current_tile.y == target_tile.y))
		return true;
	//south-east
	if ((current_tile.x + 1 == target_tile.x) && (current_tile.y + 1 == target_tile.y))
		return true;
	//south
	if ((current_tile.x  == target_tile.x) && (current_tile.y + 1 == target_tile.y))
		return true;
	//south-west
	if ((current_tile.x - 1 == target_tile.x) && (current_tile.y + 1 == target_tile.y))
		return true;
	//west
	if ((current_tile.x - 1 == target_tile.x) && (current_tile.y == target_tile.y))
		return true;

	return ret;
}

// --- UnitInfo Constructors and Destructor ---

UnitInfo::UnitInfo() {}

UnitInfo::~UnitInfo() {}

UnitInfo::UnitInfo(const UnitInfo& info) : color(info.color) {}
