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
#include <vector>

AI_Player::AI_Player(Faction g_faction) : GenericPlayer() {
	faction = g_faction;
	base = barrack[0] = barrack[1] = laboratory = nullptr;
	caps = 500;
	water = 500;
	food = 500;
	melee_minimum = 2;
	ranged_minimum = 1;
	is_attacking = false;
	defeated = false;
}

AI_Player::~AI_Player() {
	target_player = nullptr;
	troops.clear();
	gatherers_vector.clear();
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
	float mr_proportion = melees / rangeds;

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

	//if the ai_player is ready choose a player to attack
	if ((rangeds > ranged_minimum) && (melees >= melee_minimum) && (target_player == nullptr)) { 
		ChooseRandomPlayerEnemy();
		is_attacking = true;
	}

	// -------------------------------------------------------------

	// Fight -------------------------------------------------------

	//Assign all attacking units an entity to attack
	if (is_attacking) {
		/*
	
		*/
		/*
		if (target_player->entities.size() == 4) {
			if (target_player->GetTroopsAmount() == 0) {
				target_player->defeated = true;
				is_attacking = false;
				target_player = nullptr;
			}
		}
		*/
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
	iPoint enemy_base_position = App->entities->ClosestTile(origin, target_player->base->tiles);
	CreateNodePath(origin, enemy_base_position, path_to_enemy_base);
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

std::vector<iPoint> AI_Player::CreateNodePath(iPoint origin, iPoint destination, std::vector<iPoint> &node_path) {
	std::vector<iPoint> path;
	iPoint current_node;
	iPoint destination_node;
	std::vector<iPoint> node_map = App->ai_manager->node_map;
	//App->ai_manager->GetNodeMap(node_map);
	current_node = node_map.back();

	for (int i = 0; i < node_map.size(); i++)
	{
		if (node_map[i].DistanceTo(origin) < current_node.DistanceTo(origin))
			current_node = node_map[i];
	}

	for (int i = 0; i < node_map.size(); i++)
	{
		if (node_map[i].DistanceTo(destination) < destination_node.DistanceTo(destination))
			destination_node = node_map[i];
	}

	path.push_back(current_node);

	while (current_node != destination_node)
	{
		iPoint possible_node;
		iPoint best_node;
		//find neighbour nodes
		for (int y = -25; y <= 25; y +=25)
		{
			for (int x = -25; x <= 25; x += 25)
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
		path.push_back(best_node);
	}

	for (int i = 0; i < path.size(); i++)
	{
		node_path.push_back(path[i]);
	}

	return path;
}

void AI_Player::CreateAttackingGroup() {
	/*
	for (int i = 0; i < troops.size(); i++)
	{
		if (troops[i]->info.current_group == nullptr)
		{
			group.addUnit(troops[i]);
			troops[i]->info.current_group = &group;
		}
	}
	App->Mmanager->AddGroup(&group);
	*/
}