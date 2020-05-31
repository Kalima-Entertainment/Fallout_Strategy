#include "Troop.h"
#include "j1Player.h"
#include "j1Map.h"
#include "StaticEntity.h"
#include "j1Render.h"
#include "FoWManager.h"
#include "j1Scene.h"
#include "SDL_mixer/include/SDL_mixer.h"

#include "Emiter.h"
#include "ParticleSystem.h"

Troop::Troop(EntityType g_type, Faction g_faction, iPoint g_current_tile, GenericPlayer* g_owner) : DynamicEntity() {
	type = g_type;
	faction = g_faction;
	current_tile = g_current_tile;
	owner = g_owner;
	is_agressive = true;
	commanded = false;
	attack_timer.Start();

	attack_time = 3;
	detection_radius = 4;

	position = App->map->floatMapToWorld(current_tile.x, current_tile.y);
	position.x += HALF_TILE;
	position.y += HALF_TILE;

	attacking_entity = nullptr;
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

	if (this->faction == App->player->faction) {
		//Player
		visionEntity = App->fowManager->CreateFoWEntity({ this->current_tile.x, this->current_tile.y }, true);
		visionEntity->SetNewVisionRadius(5);
	}
	else {
		//Enemy
		visionEntity = App->fowManager->CreateFoWEntity({ this->current_tile.x, this->current_tile.y }, false);
	}
	
	DynaParticle = App->entities->CreateParticle(position);
	Animation anim;
	anim.PushBack(SDL_Rect{ 0, 0 , 5, 5 }, 1);
	anim.Reset();
	Emiter Blood(position.x, position.y - 20, 0.2f, 0.2f, 5, 5, 0, 0, 0, 0, 2.0f, 2, 20, 0.4f, nullptr, App->entities->blood, anim, true);
	DynaParticle->PushEmiter(Blood);
	DynaParticle->Desactivate();
}

Troop::~Troop() {
	target_building = nullptr;
	visionEntity = nullptr;
	DynaParticle = nullptr;
}

bool Troop::Update(float dt) {

	bool ret = true;
	j1Entity* enemy_in_range = nullptr;
	current_animation = &animations[state][direction];

	if ((target_entity)&&(!target_entity->is_dynamic)&&(target_entity->state != DIE)) {
		target_building = (StaticEntity*)target_entity;
	}
	
	switch (state)
	{
    case IDLE:
		enemy_in_range = DetectEntitiesInRange();

		if ((enemy_in_range) && (enemy_in_range != target_entity)) {
			target_entity = enemy_in_range;
			PathfindToPosition(enemy_in_range->current_tile);
		}
		else {
			if (target_building) {
				if ((target_building->to_delete)||(target_building->state == DIE)) {
					target_building = RequestTargetBuilding(target_building->faction);
					target_entity = target_building;

					if (target_building == nullptr)
						break;
				}
				else if(current_tile.DistanceManhattan(App->entities->ClosestTile(current_tile, target_building->tiles)) > range) {
					PathfindToPosition(App->entities->ClosestTile(current_tile, target_building->tiles));
				}
			}
		}
        break;

    case WALK:
		Move(dt);

		if (target_entity) {
			if (target_entity->is_dynamic) {
				if (current_tile.DistanceManhattan(target_entity->current_tile) <= range) {
					UpdateTile();
					path_to_target.clear();
					if (target_entity->faction != faction) {
						state = ATTACK;
					}
					else {
						state = IDLE;
						commanded = false;
					}
				}
			}
			else if (target_building == target_entity){
				if (target_building->state == DIE) {
					target_building = RequestTargetBuilding(target_building->faction);
					target_entity = target_building;

					if(target_building)
						PathfindToPosition(App->entities->ClosestTile(current_tile, target_building->tiles));
				}
				else if (current_tile.DistanceManhattan(App->entities->ClosestTile(current_tile, target_building->tiles)) <= range) {
					UpdateTile();
					path_to_target.clear();
					if (target_entity->faction != faction) {
						state = ATTACK;
					}
					else {
						state = IDLE;
						commanded = false;
					}
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
		if ((target_entity) && (target_entity->state == DIE)) {
			if (target_entity == target_building) {
				target_entity = target_building = RequestTargetBuilding(target_building->faction);
				if(target_building)
					PathfindToPosition(App->entities->ClosestTile(current_tile, target_building->tiles));
			}
			else {
				target_entity = nullptr;
				state = IDLE;
			}
			break;
		}

		if (attack_timer.ReadSec() > attack_time) {
			if (target_entity) {
				if ((current_tile.DistanceNoSqrt(target_entity->current_tile) > range) && (target_entity->is_dynamic)) {
				PathfindToPosition(target_entity->current_tile);
				}
				else {
					UpdateTile();
					path_to_target.clear();
					Attack();
				}
			}
			else {
				state = IDLE;
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

			if ((attacking_entity)&&(attacking_entity->target_entity == this)) {
				attacking_entity->target_entity = nullptr;
				attacking_entity->state = IDLE;
			}

			if (type == MR_HANDY) {
				DetectEntitiesInRange();
				for(size_t i = 0; i < entities_in_range.size(); i++)
				{
					if (entities_in_range[i]->faction != faction)
						entities_in_range[i]->current_health -= 2 * damage;
				}
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

	// -- If there are any particle then move and blits when current state equals hit
	if (DynaParticle != nullptr) {
		if (state == HIT) DynaParticle->Activate();
		else DynaParticle->Desactivate();
	}

	if (DynaParticle->IsActive()) {
		DynaParticle->Move(position.x, position.y);
		DynaParticle->Update(dt);
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
		target_building = nullptr;
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
	for(size_t i = 0; i < NO_STATE; i++)
	{
		for(size_t j = 0; j < NO_DIRECTION; j++)
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
	
	max_health = node.attribute("health").as_float();
	damage = node.attribute("damage").as_int();
	speed.x = node.attribute("speed").as_int();
	speed.y = speed.x * 0.5f;

	return ret;
}

StaticEntity* Troop::RequestTargetBuilding(Faction faction) {
	if ((App->scene->players[faction]->laboratory != nullptr)&&(App->scene->players[faction]->laboratory->state != DIE))
		return App->scene->players[faction]->laboratory;

	else if ((App->scene->players[faction]->barrack[0] != nullptr) && (App->scene->players[faction]->barrack[0]->state != DIE))
		return App->scene->players[faction]->barrack[0];

	else if ((App->scene->players[faction]->barrack[1] != nullptr) && (App->scene->players[faction]->barrack[1]->state != DIE))
		return App->scene->players[faction]->barrack[1];

	else if ((App->scene->players[faction]->base != nullptr) && (App->scene->players[faction]->base->state != DIE))
		return App->scene->players[faction]->base;

	else return nullptr;
}