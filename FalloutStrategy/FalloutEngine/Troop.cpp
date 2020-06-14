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

	//First Row
	anim.PushBack(SDL_Rect{   0, 0 , 64 , 64 }, 10);
	anim.PushBack(SDL_Rect{ 64, 0 , 64 , 64 }, 10);
	anim.PushBack(SDL_Rect{ 128, 0 , 64 , 64 }, 10);
	anim.PushBack(SDL_Rect{ 192, 0 , 64 , 64 }, 10);

	//Second Row
	anim.PushBack(SDL_Rect{ 0, 64,  64 , 64 }, 10);
	anim.PushBack(SDL_Rect{ 64,  64, 64 , 64 }, 10);
	anim.PushBack(SDL_Rect{ 128, 64, 64 , 64 }, 10);
	anim.PushBack(SDL_Rect{ 192, 64, 64 , 64 }, 10);

	//Third Row
	anim.PushBack(SDL_Rect{ 0,  128,64 , 64 }, 10);
	anim.PushBack(SDL_Rect{ 64, 128 ,64 , 64 }, 10);
	anim.PushBack(SDL_Rect{ 128, 128, 64 , 64 }, 10);
	anim.PushBack(SDL_Rect{ 192, 128, 64 , 64 }, 10);
	
	anim.Reset();

	Emiter Blood(position.x, position.y, 0.0f, 0.0f, 0, 0, 0, 0, 0, 0, 0 , 0, 1, 2.5f, nullptr, App->entities->blood, anim, true);
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
	DynamicEntity* enemy_in_range = nullptr;
	current_animation = &animations[state][direction];

	if ((dynamic_target != nullptr) && (!dynamic_target->isValidTarget())) {
		state = IDLE;
		dynamic_target = nullptr;
	}

	if ((target_building != nullptr) && ((!target_building->isValidTarget()))) {
		state = IDLE;
		target_building = nullptr;
	}

	switch (state)
	{
    case IDLE:

		//if is not commanded and there is no building to attack
		if ((!commanded)&&(target_building == nullptr)) {
			dynamic_target = DetectEntitiesInRange();
			if(dynamic_target != nullptr)
				PathfindToPosition(dynamic_target->current_tile);
		}
		else {
			if (target_building != nullptr) {
				//if the building is being destroyed
				if (target_building->state == DIE) {
					//find another one
					target_building = RequestTargetBuilding(target_building->faction);

					//if there are no buildings left idle or attack surrounding entities in next frame
					if (target_building == nullptr)
						break;
				}
				else {
					if (current_tile.DistanceManhattan(App->entities->ClosestTile(current_tile, target_building->tiles)) > range) {
						PathfindToPosition(App->entities->ClosestTile(current_tile, target_building->tiles));
					}
					else if (target_building->faction != faction){
						state = ATTACK;
					}
				}
			}
			else if (dynamic_target != nullptr) {
				if (dynamic_target->state == DIE) 
					dynamic_target = nullptr;
				else 
					PathfindToPosition(dynamic_target->current_tile);
			}
		}
        break;

    case WALK:
		Move(dt);

		//if we have a target building to attack
		if (target_building != nullptr) {
			//if the building is about to be destroyed find another one
			if (target_building->state == DIE) {
				target_building = RequestTargetBuilding(target_building->faction);

				if (target_building != nullptr){
					if (type == MELEE) {
						if (App->entities->IsTileOccupied(target_tile)) {
							iPoint free_surrounding_tile = App->entities->FindClosestFreeTileFromVector(current_tile, target_building->surrounding_tiles);
							if (free_surrounding_tile != iPoint(-1, -1)) {
								PathfindToPosition(free_surrounding_tile);
							}
							else {
								state = IDLE;
								target_building = nullptr;
								commanded = false;
								LOG("building surrounded");
							}
						}
					}
					else {
						PathfindToPosition(App->entities->ClosestTile(current_tile, target_building->tiles));
					}
				}
				else
					commanded = false;
			}
			//if the building is still standing still and close enough attack it
			else if (current_tile.DistanceManhattan(App->entities->ClosestTile(current_tile, target_building->tiles)) <= range) {
				UpdateTile();
				path_to_target.clear();
				if (target_building->faction != faction) {
					direction = GetBuildingDirection(target_building->tiles);
					state = ATTACK;
				}
				else {
					state = IDLE;
					commanded = false;
				}
			}
			//if we are too far away to attack go where the building is
			else {
				if (type == MELEE) {
					if (App->entities->IsTileOccupied(target_tile)) {
						iPoint free_surrounding_tile = App->entities->FindClosestFreeTileFromVector(current_tile, target_building->surrounding_tiles);
						if (free_surrounding_tile != iPoint(-1, -1)) {
							PathfindToPosition(free_surrounding_tile);
						}
						else {
							state = IDLE;
							target_building = nullptr;
							commanded = false;
							//LOG("building surrounded");
						}
					}
				}
				else if (App->entities->IsTileOccupied(target_tile)) {
					PathfindToPosition(App->entities->ClosestTile(current_tile, target_building->tiles));
				}
			}
		}
		else if (dynamic_target != nullptr) {
			//if the entitiy is in range
			if (dynamic_target->state != DIE) {
				if (current_tile.DistanceManhattan(dynamic_target->current_tile) <= range) {
					UpdateTile();
					path_to_target.clear();
					if (dynamic_target->faction != faction) {
						direction = GetDirectionToGo({static_cast<int>(dynamic_target->position.x), static_cast<int>(dynamic_target->position.y), 2, 2});
						state = ATTACK;
					}
					else {
						state = IDLE;
						commanded = false;
					}
				}
				//if the entity has changed its position
				else if (App->entities->IsTileOccupied(target_tile)) {
					//check that the entity is not surrounded
					if (type == MELEE) {
						iPoint free_adjacent_tile = App->entities->FindFreeAdjacentTile(current_tile, dynamic_target->current_tile);
						if (free_adjacent_tile == iPoint(-1, -1)) {
							state = IDLE;
							dynamic_target = nullptr;
							commanded = false;
						}
						else {
							PathfindToPosition(free_adjacent_tile);
						}
					}
					else {
						PathfindToPosition(dynamic_target->current_tile);
					}
				}
			}
			else {
				dynamic_target = nullptr;
			}
		}
		//atack closest enemies in range
		else if(!commanded){
			enemy_in_range = DetectEntitiesInRange();

			if (enemy_in_range != nullptr) {
				dynamic_target = enemy_in_range;
				PathfindToPosition(enemy_in_range->current_tile);
			}
			else if (target_building) {
				PathfindToPosition(App->entities->ClosestTile(current_tile, target_building->tiles));
			}
			else {
				state = IDLE;
				UpdateTile();
				commanded = false;
			}
		}
		else if (current_tile == target_tile){
			state = IDLE;
			UpdateTile();
			//path_to_target.clear();
			commanded = false;
		}

		SpatialAudio(static_cast<int>(position.x), static_cast<int>(position.y), faction, state, type);
        break;

    case ATTACK:
		if ((target_building != nullptr) && (target_building->state == DIE)) {
			target_building = RequestTargetBuilding(target_building->faction);

			if(target_building)
				PathfindToPosition(App->entities->ClosestTile(current_tile, target_building->tiles));
		}

		if ((dynamic_target != nullptr) && (dynamic_target->state == DIE)) {
			dynamic_target = DetectEntitiesInRange();
			if (dynamic_target != nullptr) {
				if (current_tile.DistanceManhattan(dynamic_target->current_tile) > range)
					PathfindToPosition(dynamic_target->current_tile);
			}
		}

		if (attack_timer.ReadSec() > attack_time) {
			if (target_building) {
				iPoint closest_tile = App->entities->ClosestTile(current_tile, target_building->tiles);
				if (current_tile.DistanceManhattan(closest_tile) <= range) {
					UpdateTile();
					path_to_target.clear();
					Attack();
				}
				else {
					PathfindToPosition(closest_tile);
				}
			}
			else if (dynamic_target != nullptr) {
				if (current_tile.DistanceManhattan(dynamic_target->current_tile) > range) {
					PathfindToPosition(dynamic_target->current_tile);
				}
				else {
					UpdateTile();
					path_to_target.clear();
					Attack();
				}
			}
			else {
				state = IDLE;
				commanded = false;
			}
		}

		SpatialAudio(static_cast<int>(position.x), static_cast<int>(position.y), faction, state, type);

        break;

    case HIT:
		if (current_animation->Finished()) {
			current_animation->Reset();
			if (attacking_entity != nullptr) {
				state = ATTACK;
				dynamic_target = attacking_entity;
			}
			else
				state = IDLE;
		}

		SpatialAudio(static_cast<int>(position.x), static_cast<int>(position.y), faction, state, type);
        break;

    case DIE:
		direction = TOP_LEFT;
		if (!delete_timer.Started()) {
			delete_timer.Start();
			direction = TOP_LEFT;

			if ((attacking_entity)&&(attacking_entity->dynamic_target == this)) {
				attacking_entity->dynamic_target = nullptr;
				attacking_entity->state = IDLE;
			}

			//Mr Handy explode
			if (type == MR_HANDY) {
				DetectEntitiesInRange();
				for(int i = 0; i < entities_in_range.size(); i++)
				{
					if (entities_in_range[i]->faction != faction)
						entities_in_range[i]->current_health -= 2 * damage;
				}
			}
		}
		visionEntity = nullptr;

		if (delete_timer.ReadSec() > 4) {
			to_delete = true;
			App->entities->occupied_tiles[current_tile.x][current_tile.y] = false;
		}

		SpatialAudio(static_cast<int>(position.x), static_cast<int>(position.y), faction, state, type);
        break;

    default:
        break;
	}

	// -- If there are any particle then move and blits when current state equals hit
	if (DynaParticle != nullptr) {
		if (state == HIT)
			DynaParticle->Activate();
		else
			DynaParticle->Desactivate();
	}	

	if (DynaParticle->IsActive()) {
		DynaParticle->Move(static_cast<int>(position.x) - 38, static_cast<int>(position.y) - 55);
		DynaParticle->Update(dt);
	}

	/*
	if (this->info.current_group != nullptr)
	{
		if (info.current_group->IsGroupLead(this)) {
			if (this->faction == App->player->faction)
				info.current_group->CheckForMovementRequest(App->player->Map_mouseposition, dt);
		}
	}
	*/
	last_dt = dt;

	return ret;
}

void Troop::Attack() {

	attack_timer.Start();

	//avoid damagin player's units if in god mode
	if (App->player->god_mode) {
		if ((dynamic_target != nullptr) && (dynamic_target->faction == App->player->faction))
			return;
		if ((target_building != nullptr) && (target_building->faction == App->player->faction))
			return;
	}

	if (dynamic_target != nullptr) {

		dynamic_target->current_health -= damage;
		dynamic_target->state = HIT;
		dynamic_target->attacking_entity = this;

		if (dynamic_target->state == ATTACK)
			dynamic_target->current_animation->Reset();

		if ((current_tile.x > dynamic_target->current_tile.x) && (current_tile.y == dynamic_target->current_tile.y)) {
			direction = TOP_LEFT;
			dynamic_target->direction = BOTTOM_RIGHT;
		}
		else if ((current_tile.x == dynamic_target->current_tile.x) && (current_tile.y > dynamic_target->current_tile.y)) {
			direction = TOP_RIGHT;
			dynamic_target->direction = BOTTOM_LEFT;
		}
		else if ((current_tile.x == dynamic_target->current_tile.x) && (current_tile.y < dynamic_target->current_tile.y)) {
			direction = BOTTOM_LEFT;
			dynamic_target->direction = TOP_RIGHT;
		}
		else if ((current_tile.x < dynamic_target->current_tile.x) && (current_tile.y == dynamic_target->current_tile.y)) {
			direction = BOTTOM_RIGHT;
			dynamic_target->direction = TOP_LEFT;
		}

		//kill entity
		if (dynamic_target->current_health <= 0) {
			dynamic_target->state = DIE;
			dynamic_target->direction = TOP_LEFT;
			path_to_target.clear();
			state = IDLE;
		}
	}
	else if(target_building != nullptr){
		target_building->current_health -= damage;
		target_building->GetHit();
		direction = GetBuildingDirection(target_building->tiles);

		//destroy building
		if (target_building->current_health <= 0) {
			target_building->state = DIE;
			path_to_target.clear();
			state = IDLE;
		}
	}
}

bool Troop::LoadDataFromReference() {
	bool ret = true;
	Troop* reference_troop = dynamic_cast<Troop*>(reference_entity);

	//load animations
	for(int i = 0; i < NO_STATE; i++)
	{
		for(int j = 0; j < NO_DIRECTION; j++)
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
	speed.x = node.attribute("speed").as_float();
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
