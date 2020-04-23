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
#include <vector>
#include <math.h>

AI_Player::AI_Player(Faction g_faction) : GenericPlayer() {
	faction = g_faction;
	caps = 500;
	water = 500;
	food = 500;
	melee_minimum = 1;
	ranged_minimum = 1;
	is_attacking = false;
	defeated = false;
	goal_tile_set = false;
	target_player = nullptr;
	target_building = nullptr;
	base = barrack[0] = barrack[1] = laboratory = nullptr;
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


	// ------------------------------------------------------------

	//Spawn Units -------------------------------------------------

	//melee-ranged proportion
	
	float mr_proportion = 0;

	if (rangeds > 0)
		mr_proportion = melees / rangeds;


	//spawn melee
	if ((water > App->entities->unit_data[faction][MELEE].cost_water)&&(caps > App->entities->unit_data[faction][MELEE].cost_meat) && (mr_proportion < 2) && (barrack[0] != nullptr)) {
		barrack[0]->SpawnUnit(MELEE);
		water -= App->entities->unit_data[faction][MELEE].cost_water;
		food -= App->entities->unit_data[faction][MELEE].cost_meat;
	}

	//spawn ranged
	if ((water > App->entities->unit_data[faction][RANGED].cost_water) && (caps > App->entities->unit_data[faction][RANGED].cost_meat)&&(barrack[1] != nullptr)) {
		barrack[1]->SpawnUnit(RANGED);
		water -= App->entities->unit_data[faction][RANGED].cost_water;
		food -= App->entities->unit_data[faction][RANGED].cost_meat;
	}

	//Choose enemy player -----------------------------------------

	//if the ai_player is ready choose a player to attack
	if ((rangeds >= ranged_minimum) && (melees >= melee_minimum) && (target_player == nullptr)) { 
		ChooseRandomPlayerEnemy();
		is_attacking = true;
	}

	// -------------------------------------------------------------

	// Fight -------------------------------------------------------

	//Assign all attacking units an entity to attack
	if (is_attacking) 
	{
		if ((target_building!=nullptr) && (target_building->state == EXPLODE))
			target_building = nullptr;

		if (target_building == nullptr) {
			target_building = ChooseTargetBuilding();
			if (target_building == nullptr) {
				ChooseRandomPlayerEnemy();
				target_building = ChooseTargetBuilding();
			}
			if(target_building != nullptr)
				target_building_position = target_building->current_tile;
		}

		for (int i = 0; i < troops.size(); i++)
		{
			if (target_building != nullptr)
			{
				if ((troops[i]->target_entity == nullptr)||(troops[i]->target_entity != target_building))
				{
						troops[i]->target_entity = target_building;
				}
			}
			else { troops[i]->target_entity = nullptr; }

			if (troops[i]->path_to_target.size() == 0) 
			{
				if (troops[i]->node_path.size() == 0)
				{
					if (troops[i]->current_tile.DistanceManhattan(target_building_position) > 40)
						troops[i]->node_path = CreateNodePath(troops[i]->current_tile, target_building_position);
					else
						troops[i]->PathfindToPosition(target_building_position);
				}
			}
		}
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

std::vector<iPoint> AI_Player::CreateNodePath(iPoint origin, iPoint destination) {
	BROFILER_CATEGORY("CreateNodePath", Profiler::Color::Azure)
	std::vector<iPoint> path;
	iPoint current_node;
	iPoint origin_node;
	iPoint destination_node;
	std::vector<iPoint> node_map = App->ai_manager->node_map;
	int node_distance = App->ai_manager->GetDistanceBetweenNodes();

	origin_node = node_map[0];
	destination_node = node_map[0];

	//closest origin node
	for (int i = 0; i < node_map.size(); i++)
	{
		if (node_map[i].DistanceTo(origin) < origin_node.DistanceTo(origin))
			origin_node = node_map[i];
	}
	
	//closest destination node
	for (int i = 0; i < node_map.size(); i++)
	{
		if (node_map[i].DistanceTo(destination) < destination_node.DistanceTo(destination))
			destination_node = node_map[i];
	}

	current_node = origin_node;
	path.push_back(current_node);

	//iterate nodes to create the path
	while (current_node != destination_node)
	{
		iPoint possible_node;
		iPoint best_node;
		//find neighbour nodes
		for (int y = -node_distance; y <= node_distance; y += node_distance)
		{
			for (int x = -node_distance; x <= node_distance; x += node_distance)
			{
				possible_node.x = current_node.x + x;
				possible_node.y = current_node.y + y;

				if (possible_node.DistanceTo(destination_node) < current_node.DistanceTo(destination_node)) {
					if (possible_node.DistanceTo(destination_node) < best_node.DistanceTo(destination_node))
						best_node = possible_node;
				}
			}
		}
		current_node = best_node;

		//if (best_node.DistanceManhattan(destination) > 20)
			path.push_back(best_node);
		//else
			//break;
	}

	//flip final path 
	std::reverse(path.begin(), path.end());

	return path;
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