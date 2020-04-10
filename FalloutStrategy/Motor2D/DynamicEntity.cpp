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
#include "StaticEntity.h"
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
	direction = TOP_RIGHT;

	current_speed = { 0, 0 };
	target_entity = nullptr;
	resource_building = nullptr;
	action_time = 3.0f;
	resource_collected = 0;
	sprite_size = 128;
}

DynamicEntity::~DynamicEntity() {}

bool DynamicEntity::PreUpdate(float dt) {
	return true;
}

bool DynamicEntity::Update(float dt) {

	Mix_AllocateChannels(20);


	switch (state)
	{
	case IDLE:
		break;
	case WALK:
		Move(dt);
		if (Mix_Playing(2) == 0) { SpatialAudio(App->audio->Brotherhood_walk, 2, position.x, position.y); }
		break;
	case ATTACK:
		if (timer.ReadSec() > action_time)
		{
			if (current_tile.DistanceManhattan(target_entity->current_tile) <= range)
			{
				Attack();
			}
		}

		if (reference_entity->faction == MUTANT || reference_entity->faction == BROTHERHOOD && reference_entity->type == RANGED)
			if (Mix_Playing(15) == 0) { SpatialAudio(App->audio->minigun, 15, position.x, position.y); }
		if (reference_entity->faction == VAULT || reference_entity->faction == GHOUL && reference_entity->type == RANGED)
			if (Mix_Playing(16) == 0) { SpatialAudio(App->audio->pistol, 16, position.x, position.y); }

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
		LOG("This unit belongs to any group");
		if (info.current_group->IsGroupLead(this))
			info.current_group->CheckForMovementRequest(dt);
	}

	last_dt = dt;

	return true;
}

bool DynamicEntity::PostUpdate() {

	current_animation = &animations[state][direction];

	//Render path
	if (App->render->debug)
	{
		if (path_to_target .size() > 0)
		{
			for (uint i = 0; i < path_to_target.size(); ++i)
			{
				iPoint pos = App->map->MapToWorld(path_to_target[i].x, path_to_target[i].y);
				SDL_Rect debug_rect = { 192, 0, 64,64 };
				App->render->Blit(App->render->debug_tex, pos.x, pos.y, &debug_rect);
			}
		}
	}

	//Render character
	render_position = { (int)(position.x - sprite_size * 0.5f), (int)(position.y - 1.82f * TILE_SIZE)};
	App->render->Blit(reference_entity->texture,render_position.x, render_position.y, &current_animation->GetCurrentFrame(last_dt));

	//Rendering Selected Units Quad
	if (this->info.IsSelected) DrawQuad();

	//Health Bar
	SDL_Rect background_bar = { position.x - HALF_TILE * 0.75f, position.y - TILE_SIZE * 1.5f, 50, 4 };
	SDL_Rect foreground_bar = { position.x - HALF_TILE * 0.75f, position.y - TILE_SIZE * 1.5f, (float)current_health/max_health * 50, 4 };
	if (foreground_bar.w < 0) foreground_bar.w = 0;

	//Life Bar Render
	App->render->DrawQuad(background_bar, 255, 255, 255, 255);
	App->render->DrawQuad(foreground_bar, 0, 255, 0, 255);


	return true;
}

void DynamicEntity::Move(float dt) {
	if (path_to_target.size() > 0) {
		//get next tile center
		next_tile_position = App->map->MapToWorld(next_tile.x, next_tile.y);
		next_tile_rect_center = { next_tile_position.x + HALF_TILE - 2, next_tile_position.y + HALF_TILE,4,4 };

			//if the entitiy is about to reach it's target tile
			if (current_tile.LinealDistance(target_tile) <= range) {
			//we reach the destination and there is an entity in it
				//ranged and melee
				if (type != GATHERER){
					if (target_entity != nullptr)
					{
						//enemy target
						if (faction != target_entity->faction) {
							state = ATTACK;
							Attack();
							target_entity->attacking_entity = this;
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
							return;
						}

						//give gathered resources
						else if ((resource_collected > 0) && (target_building != nullptr) && (target_building->volume < target_building->storage_capacity)) {
							target_building->volume += resource_collected;
							App->player->UpdateResourceData(resource_type, resource_collected);
							resource_collected = 0;
							target_building = nullptr;

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
								else { state = IDLE; }
							}
						}
						if (target_entity != nullptr) {
							state = IDLE;
						}
					}
				}
			}

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
	else
	{
		state = IDLE;
	}
}

void DynamicEntity::Attack() {

	timer.Start();
	target_entity->current_health -= damage;
	target_entity->state = HIT;


	if ((current_tile.x > target_entity->current_tile.x) && (current_tile.y == target_entity->current_tile.y)) {
		direction = TOP_LEFT;
		target_entity->direction = BOTTOM_RIGHT;
	}
	else if ((current_tile.x == target_entity->current_tile.x) && (current_tile.y > target_entity->current_tile.y)) {
		direction = TOP_RIGHT;
		target_entity->direction = BOTTOM_LEFT;
	}
	else if ((current_tile.x == target_entity->current_tile.x) && (current_tile.y < target_entity->current_tile.y)) {
		direction = BOTTOM_LEFT;
		target_entity->direction = TOP_RIGHT;
	}
	else if ((current_tile.x < target_entity->current_tile.x) && (current_tile.y == target_entity->current_tile.y)) {
		direction = BOTTOM_RIGHT;
		target_entity->direction = TOP_LEFT;
	}

	if (target_entity->current_health <= 0) {
		target_entity->state = DIE;
		target_entity->direction = TOP_LEFT;
		target_entity = nullptr;
		state = IDLE;
	}
}

void DynamicEntity::Gather() {
	uint resource = resource_building->quantity - (resource_building->quantity - damage);
	resource_building->quantity -= resource;
	resource_collected += resource;
	resource_type = resource_building->resource_type;
	StaticEntity* base = (StaticEntity*)App->entities->FindEntityByType(faction, BASE);
	PathfindToPosition(App->entities->ClosestTile(current_tile, base->tiles));
	target_building = base;
	//resource_building = nullptr;
}

void DynamicEntity::PathfindToPosition(iPoint destination) {
	if ((!App->pathfinding->IsWalkable(destination)) && (App->pathfinding->CheckBoundaries(destination))) {
		destination = App->pathfinding->FindWalkableAdjacentTile(destination);
	}
	current_tile = App->map->WorldToMap(position.x, position.y);
	target_tile = destination;
	App->pathfinding->CreatePath(current_tile, destination);

	//pathfinding debug
	int x, y;
	SDL_Rect Debug_rect = { 0,0,32,32 };

	path_to_target.clear();
	path_to_target = App->pathfinding->GetLastPath();


	if (path_to_target.size() > 0)
		next_tile = path_to_target.front();

	for (uint i = 0; i < path_to_target.size(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path_to_target[i].x, path_to_target[i].y);
		//LOG("CURRENT PATH IS: x: %i || y: %i ", path_to_target[i].x, path_to_target[i].y);

		Debug_rect.x = pos.x;
		Debug_rect.y = pos.y;
		if (App->render->debug)App->render->DrawQuad(Debug_rect, 90, 850, 230, 40);
	}
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
		else if (animation_direction == "left")
			direction = LEFT;
		else if (animation_direction == "right")
			direction = RIGHT;
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
		for (int j = 0; j <= 6; j++)
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
	const SDL_Rect entityrect = { position.x - sprite_size * 0.5f ,  position.y - 1.82f * TILE_SIZE,  128,  128 };
	App->render->DrawQuad(entityrect, unitinfo.color.r, unitinfo.color.g, unitinfo.color.b, unitinfo.color.a, false);
}

// --- UnitInfo Constructors and Destructor ---
UnitInfo::UnitInfo() {}

UnitInfo::~UnitInfo() {}

UnitInfo::UnitInfo(const UnitInfo& info) : color(info.color) {}
