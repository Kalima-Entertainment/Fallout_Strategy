#include "AI_Player.h"
#include "j1Entity.h"
#include "DynamicEntity.h"
#include "StaticEntity.h"
#include "AI_Manager.h"
#include "j1EntityManager.h"
#include "j1Player.h"
#include "p2Point.h"
#include "j1Group.h"
#include "j1MovementManager.h"
#include "brofiler/Brofiler/Brofiler.h"
#include "j1Pathfinding.h"
#include "Troop.h"
#include "Gatherer.h"
#include <vector>
#include <math.h>

AI_Player::AI_Player(Faction g_faction) : GenericPlayer(), is_attacking(false), last_barrack_to_spawn(1), gatherers_commanded(false) {
	faction = g_faction;
	caps = App->ai_manager->GetAI_PlayerInfo(faction).initial_caps;
	water = App->ai_manager->GetAI_PlayerInfo(faction).initial_water;
	food = App->ai_manager->GetAI_PlayerInfo(faction).initial_food;
	melee_minimum = App->ai_manager->GetAI_PlayerInfo(faction).minimum_melees;
	ranged_minimum = App->ai_manager->GetAI_PlayerInfo(faction).minimum_rangeds;
	wave_time = App->ai_manager->GetAI_PlayerInfo(faction).wave_time;
	defeated = false;
	goal_tile_set = false;
	target_player = nullptr;
	target_building = nullptr;
	target_building_position = { -1, -1 };
	base = barrack[0] = barrack[1] = laboratory = nullptr;
	wave_timer.Start();
	is_ai = true;
}

AI_Player::~AI_Player() 
{
	for (int t = 0; t < troops.size(); t++) { troops[t] = nullptr;}
	troops.clear();

	for (int g = 0; g < gatherers_vector.size(); g++) { gatherers_vector[g] = nullptr; }
	gatherers_vector.clear();

	target_player = nullptr;
	target_building = nullptr;
	base = barrack[0] = barrack[1] = laboratory = nullptr;
}

bool AI_Player::Update(float dt) {
	bool ret = true;

	// Gather -----------------------------------------------------
	if (!gatherers_commanded) {
		for (int i = 0; i < gatherers_vector.size(); i++)
		{
			//authomatic gathering
			if (gatherers_vector[i]->resource_building == nullptr) {
				gatherers_vector[i]->resource_building = App->entities->GetClosestResourceBuilding(gatherers_vector[i]->current_tile);
				//if there is at least a resource building left, go there
				if (gatherers_vector[i]->resource_building != nullptr) {
					gatherers_vector[i]->PathfindToPosition(App->entities->ClosestTile(gatherers_vector[i]->current_tile, gatherers_vector[i]->resource_building->tiles));
					gatherers_vector[i]->state = WALK;
				}
				//if there are no resource buildings left
				else
				{
					gatherers_vector[i]->state = IDLE;
				}
			}
		}
		gatherers_commanded = true;
	}

	// ------------------------------------------------------------

	//Spawn Units -------------------------------------------------

	//melee-ranged proportion

	float mr_proportion = 0;
	if (rangeds > 0)
		mr_proportion = melees / rangeds;

	//spawn melee
	if ((barrack[0] != nullptr) &&(water > App->entities->unit_data[faction][MELEE].cost_water)&&(caps > App->entities->unit_data[faction][MELEE].cost_meat) && (last_barrack_to_spawn == 1) && (mr_proportion < 2)) {
		barrack[0]->SpawnUnit(MELEE);
		water -= App->entities->unit_data[faction][MELEE].cost_water;
		food -= App->entities->unit_data[faction][MELEE].cost_meat;
		last_barrack_to_spawn = 0;
	}

	//spawn ranged
	if ((barrack[0] != nullptr) && (water > App->entities->unit_data[faction][RANGED].cost_water) && (caps > App->entities->unit_data[faction][RANGED].cost_meat)&&(barrack[1] != nullptr) && (last_barrack_to_spawn == 0)) {
		barrack[1]->SpawnUnit(RANGED);
		water -= App->entities->unit_data[faction][RANGED].cost_water;
		food -= App->entities->unit_data[faction][RANGED].cost_meat;
		last_barrack_to_spawn = 1;
	}

	//Choose enemy player -----------------------------------------

	//if the ai_player is ready choose a player to attack

	if (wave_timer.ReadSec() > wave_time) {
		if ((rangeds >= ranged_minimum) && (melees >= melee_minimum) && (target_player == nullptr)) {
			ChooseRandomPlayerEnemy();
			is_attacking = true;
			wave_timer.Start();
			LOG("attacking");
		}
	}

	// -------------------------------------------------------------

	// Fight -------------------------------------------------------

	//Assign all attacking units an entity to attack
	
	if (is_attacking) 
	{
		//if the target building is destroyed forget about it
		if ((target_building!=nullptr) && (target_building->state == DIE))
			target_building = nullptr;

		//if there is no target building find one
		if (target_building == nullptr) {
			target_building = ChooseTargetBuilding();
			//if the enemy player has no buildings left choose another player
			if (target_building == nullptr) {
				ChooseRandomPlayerEnemy();
				target_building = ChooseTargetBuilding();
			}
			//and find its position
			if(target_building != nullptr)
				target_building_position = target_building->current_tile;
		}

		for (int i = 0; i < troops.size(); i++)
		{
			if (target_building != nullptr)
			{
				//if the troop has no target assign one
				if ((troops[i]->target_entity == nullptr)||(troops[i]->target_entity != target_building))
				{
						troops[i]->target_entity = target_building;
						//and make it go there
						troops[i]->PathfindToPosition(target_building_position);
				}
			}
			//forget about the target if it is null because it means it has been destroyed 
			//else { troops[i]->target_entity = nullptr; }

			//if the unit has no path 
			/*
			if (troops[i]->path_to_target.size() == 0) 
			{
				//and has no node path
				if (troops[i]->node_path.size() == 0)
				{
					//check if it is too far to get to  position
					//if it is create a node path for it
					if (troops[i]->current_tile.DistanceManhattan(target_building_position) > 40)
						troops[i]->node_path = App->pathfinding->CreateNodePath(troops[i]->current_tile, target_building_position);
					//if it is close enough pathfind to the target
					else
						troops[i]->PathfindToPosition(target_building_position);
				}
			}
			*/
		}

		is_attacking = false;
	}

	return ret;
}

void AI_Player::ChooseRandomPlayerEnemy() {
	srand(time(NULL));
	int enemy_faction;

	do
	{
		enemy_faction = rand() % 4;
		target_player = App->ai_manager->ai_player[enemy_faction];
	} while (enemy_faction == faction);

	if (target_player == nullptr)
		target_player = (GenericPlayer*)App->player;

	iPoint origin = { (int)troops[0]->current_tile.x, (int)troops[0]->current_tile.y };
	if (target_player->base != nullptr) {
		iPoint enemy_base_position = App->entities->ClosestTile(origin, target_player->base->tiles);
	}

	//CreateNodePath(troops[0]->current_tile, enemy_base_position, path_to_enemy_base);
	//App->Mmanager->CreateGroup(troops);
}

DynamicEntity* AI_Player::GetClosestDynamicEntity() {
	DynamicEntity* target_entity = nullptr;
	int distance = 1000000;

	for (int i = 0; i < target_player->troops.size(); i++)
	{
		if ((target_player->troops[i]->current_tile.DistanceManhattan(base->current_tile) < distance)&&(target_player->troops[i])) {
			target_entity = target_player->troops[i];
			distance = target_player->troops[i]->current_tile.DistanceManhattan(base->current_tile);
		}
	}
	return target_entity;
}

StaticEntity* AI_Player::ChooseTargetBuilding() {

	//choose a building to attack in preference order

	if (target_player->barrack[0] != nullptr)
		target_building = target_player->barrack[0];
	else if (target_player->laboratory != nullptr)
		target_building = target_player->laboratory;
	else if (target_player->barrack[1] != nullptr)
		target_building = target_player->barrack[1];
	else if (target_player->base != nullptr)
		target_building = target_player->base;

	return target_building;
}

void AI_Player::GatherFood(ResourceBuilding* resource_spot) {

	for (int i = 0; i < gatherers; i++)
	{
		if ((gatherers_vector[i]->resource_building == nullptr)||(gatherers_vector[i]->resource_collected == 0)) {
			gatherers_vector[i]->resource_building = resource_spot;
			gatherers_vector[i]->PathfindToPosition(resource_spot->tiles.front());
			return;
		}
	}
}