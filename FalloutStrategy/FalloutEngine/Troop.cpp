#include "Troop.h"
#include "j1Player.h"
#include "j1Map.h"
#include "StaticEntity.h"
#include "j1Render.h"
#include "FoWManager.h"
#include "SDL_mixer/include/SDL_mixer.h"

Troop::Troop(EntityType g_type, Faction g_faction, iPoint g_current_tile, GenericPlayer* g_owner) : DynamicEntity() {
	type = g_type;
	faction = g_faction;
	current_tile = g_current_tile;
	owner = g_owner;
	is_agressive = true;
	attack_timer.Start();
	attack_time = 3;
	detection_radius = 4;

	position = App->map->fMapToWorld(current_tile.x, current_tile.y);
	position.x += HALF_TILE;
	position.y += HALF_TILE;

	target_building = nullptr;

	switch (type)
	{
	case MELEE:
		range = 1;
		break;
	case RANGED:
		range = 4;
		break;
	case MR_HANDY:
		range = 4;
		break;
	default:
		break;
	}

	//if (App->render->fog_of_war) {
		if (this->faction == App->player->faction) {
			//Player
			visionEntity = App->fowManager->CreateFoWEntity({ this->current_tile.x, this->current_tile.y }, true);
			visionEntity->SetNewVisionRadius(5);
		}
		else {
			//Enemy
			visionEntity = App->fowManager->CreateFoWEntity({ this->current_tile.x, this->current_tile.y }, false);
		}
	//}
}

Troop::~Troop() {
	target_building = nullptr;
}

bool Troop::Update(float dt) {
	bool ret = true;
	j1Entity* enemy_in_range = nullptr;
	current_animation = &animations[state][direction];

	Mix_AllocateChannels(35);

	switch (state)
	{
    case IDLE:
		enemy_in_range = DetectEntitiesInRange();

		if ((enemy_in_range) && (enemy_in_range != target_entity)) {
			target_entity = enemy_in_range;
			PathfindToPosition(enemy_in_range->current_tile);
		}
		else if(target_building)
		{
			target_entity = target_building;
			PathfindToPosition(target_building->current_tile);
		}
        break;

    case WALK:
		Move(dt);

		if (target_entity) {
			if (current_tile.DistanceManhattan(target_entity->current_tile) <= range) {
				UpdateTile();
				path_to_target.clear();
				if (target_entity->faction != faction) {
					state = ATTACK;
					//Attack();
					//attack_timer.Start();
				}
				else {
					state = IDLE;
				}
			}
		}
		else if(!commanded){
			enemy_in_range = DetectEntitiesInRange();

			if ((enemy_in_range) && (enemy_in_range != target_entity)) {
				target_entity = enemy_in_range;
				PathfindToPosition(enemy_in_range->current_tile);
			}
			else if ((target_building) && (target_entity != target_building))
			{
				target_entity = target_building;
				PathfindToPosition(App->entities->ClosestTile(current_tile, target_building->tiles));
			}
		}

		SpatialAudio(position.x, position.y, faction, state, type);
        break;

    case ATTACK:
		if (attack_timer.ReadSec() > attack_time) {
			current_animation->Reset();
			if ((current_tile.DistanceNoSqrt(target_entity->current_tile) > range)&&(target_entity->is_dynamic)) {
				PathfindToPosition(target_entity->current_tile);
			}
			else
			{
				UpdateTile();
				path_to_target.clear();
				Attack();
				animations[ATTACK][direction].Reset();
			}
		}
		SpatialAudio(position.x, position.y, faction, state, type);

        break;

    case HIT:
		if (current_animation->Finished()) {
			current_animation->Reset();
			if (attacking_entity != nullptr) {
				state = ATTACK;
				target_entity = attacking_entity;
			}
			else
				state = IDLE;
		}

		SpatialAudio(position.x, position.y, faction, state, type);
        break;

    case DIE:
		direction = TOP_LEFT;
		if (!delete_timer.Started()) {
			delete_timer.Start();
			direction = TOP_LEFT;
			if (attacking_entity) {
				attacking_entity->target_entity = nullptr;
				attacking_entity->state = IDLE;
			}
		}
		visionEntity->SetNewPosition(App->map->MapToWorld(-10, -10));

		if (delete_timer.ReadSec() > 4) {
			to_delete = true;
			App->entities->occupied_tiles[current_tile.x][current_tile.y] = false;
		}
		SpatialAudio(position.x, position.y, faction, state, type);
        break;

    default:
        break;
	}

	last_dt = dt;

	return ret;
}

void Troop::Attack() {

	attack_timer.Start();

	//damage unit if god_mode isn't activated
	if ((target_entity->faction == App->player->faction) && (App->player->god_mode))
		return;

	target_entity->current_health -= damage;

	if (target_entity->current_health <= 0) {
		target_entity->state = DIE;
		path_to_target.clear();
		state = IDLE;

		if (attacking_entity == target_entity)
			attacking_entity = nullptr;

		target_entity = nullptr;
	}
	else if (target_entity->is_dynamic) {
		DynamicEntity* dynamic_target = (DynamicEntity*)target_entity;

		target_entity->attacking_entity = this;
		dynamic_target->state = HIT;

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
	}
}

bool Troop::LoadDataFromReference() {
	bool ret = true;
	Troop* reference_troop = (Troop*)reference_entity;

	//load animations
	for (int i = 0; i < NO_STATE; i++)
	{
		for (int j = 0; j < NO_DIRECTION; j++)
		{
			animations[i][j] = reference_troop->animations[i][j];
		}
	}

	//load property data
	current_health = max_health = reference_entity->max_health;
	damage = reference_troop->damage;
	speed = reference_troop->speed;
	sprite_size = reference_entity->sprite_size;
	texture = reference_entity->texture;

	return ret;
}

bool Troop::LoadReferenceData(pugi::xml_node& node) {
	bool ret = true;
	
	max_health = node.attribute("health").as_int();
	damage = node.attribute("damage").as_int();
	speed.x = node.attribute("speed").as_int();
	speed.y = speed.x * 0.5f;

	return ret;
}