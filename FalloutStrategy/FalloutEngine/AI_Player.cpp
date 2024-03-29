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
#include "j1Audio.h"

#include "j1Hud.h"

#include <vector>
#include <math.h>

AI_Player::AI_Player(Faction g_faction) : GenericPlayer(), is_attacking(false), last_barrack_to_spawn(0), gatherers_commanded(false) {
	
	name.assign("AI");

	faction = g_faction;
	caps = App->ai_manager->GetAI_PlayerInfo(faction).initial_caps;
	water = App->ai_manager->GetAI_PlayerInfo(faction).initial_water;
	food = App->ai_manager->GetAI_PlayerInfo(faction).initial_food;
	melee_minimum = App->ai_manager->GetAI_PlayerInfo(faction).minimum_melees;
	ranged_minimum = App->ai_manager->GetAI_PlayerInfo(faction).minimum_rangeds;
	wave_time = App->ai_manager->GetAI_PlayerInfo(faction).wave_time;

	target_building_position = { -1, -1 };
	wave_timer.Start();
	is_ai = true;
	is_attacking = false;
	group = nullptr;
}

AI_Player::~AI_Player() 
{
	for(size_t t = 0; t < troops.size(); t++) { troops[t] = nullptr;}
	troops.clear();

	for(size_t g = 0; g < gatherers_vector.size(); g++) { gatherers_vector[g] = nullptr; }
	gatherers_vector.clear();

	target_player = nullptr;
	target_building = nullptr;
	base = barrack[0] = barrack[1] = laboratory = nullptr;
}

bool AI_Player::Update(float dt) {
	bool ret = true;

	// Gather -----------------------------------------------------

	if (!gatherers_commanded) {
		for(size_t i = 0; i < gatherers_vector.size(); i++)
		{
			//authomatic gathering
			if (gatherers_vector[i]->GetResourceBuilding() == nullptr) {
				gatherers_vector[i]->AssignResourceBuilding(App->entities->GetClosestResourceBuilding(gatherers_vector[i]->current_tile, gatherers_vector[i]->resource_type));
				//if there is at least a resource building left, go there
				if (gatherers_vector[i]->GetResourceBuilding() != nullptr) {
					gatherers_vector[i]->PathfindToPosition(App->entities->ClosestTile(gatherers_vector[i]->current_tile, gatherers_vector[i]->GetResourceBuilding()->tiles));
					gatherers_vector[i]->gathering = true;
				}
				//if there are no resource buildings left
				else {
					gatherers_vector[i]->state = State::IDLE;
				}
			}
		}
		gatherers_commanded = true;
	}

	//Spawn Units -------------------------------------------------

	//melee-ranged proportion
	float mr_proportion = 0;
	if (rangeds > 0)
		mr_proportion = melees / rangeds;

	//spawn melee
	if ((barrack[0] != nullptr) &&(water >= App->entities->unit_data[faction][MELEE].cost_water)&&(food >= App->entities->unit_data[faction][MELEE].cost_meat) && (last_barrack_to_spawn == 1) && (mr_proportion < 2)) {
		barrack[0]->SpawnUnit(MELEE);
		water -= App->entities->unit_data[faction][MELEE].cost_water;
		food -= App->entities->unit_data[faction][MELEE].cost_meat;
		last_barrack_to_spawn = 0;
	}

	//spawn ranged
	if ((barrack[1] != nullptr) && (water >= App->entities->unit_data[faction][RANGED].cost_water)&&(food >= App->entities->unit_data[faction][RANGED].cost_meat) && (last_barrack_to_spawn == 0)) {
		barrack[1]->SpawnUnit(RANGED);
		water -= App->entities->unit_data[faction][RANGED].cost_water;
		food -= App->entities->unit_data[faction][RANGED].cost_meat;
		last_barrack_to_spawn = 1;
	}

	//Choose enemy player -----------------------------------------

	//if the ai_player is ready choose a player to attack
	if ((wave_timer.ReadSec() >= wave_time) && (!is_attacking)) {
		if ((rangeds >= ranged_minimum) && (melees >= melee_minimum)) {
			if (target_player == nullptr) {
				ChooseRandomPlayerEnemy();
			}
			is_attacking = true;
			wave_timer.Start();

			std::string faction_name;
			if (faction == VAULT)
				faction_name = "vault";
			else if (faction == BROTHERHOOD)
				faction_name = "brotherhood";
			else if (faction == MUTANT)
				faction_name = "mutant";
			else if (faction == GHOUL)
				faction_name = "ghoul";

			LOG("%s attacking %i:%i", faction_name.c_str(), App->hud->minutes, App->hud->timer);
		}
	}

	// -------------------------------------------------------------

	// Fight -------------------------------------------------------

	//Assign all attacking units an entity to attack
	
	if (is_attacking) 
	{
		//if the target building is destroyed forget about it
		if ((target_building != nullptr) && (!target_building->isValidTarget() || target_building->state == DIE))
			target_building = nullptr;

		//if there is no target building find one
		if ((target_building == nullptr)||(!target_building->isValidTarget())) {
			target_building = ChooseTargetBuilding();
			//if the enemy player has no buildings left choose another player
			if ((target_building == nullptr) || (!target_building->isValidTarget())) {
				ChooseRandomPlayerEnemy();
				target_building = ChooseTargetBuilding();
			}
		}
		//else {
		for (size_t i = 0; i < troops.size(); i++)
		{
			if ((target_building != nullptr) && (target_building->isValidTarget())) {
				troops[i]->target_building = target_building;
				troops[i]->PathfindToPosition(App->entities->ClosestTile(troops[i]->current_tile, target_building->surrounding_tiles));
			}
		}
		//}

		is_attacking = false;
	}

	return ret;
}

void AI_Player::ChooseRandomPlayerEnemy() {
	srand(time(NULL));
	int enemy_faction = NO_FACTION;

	do
	{
		enemy_faction = rand() % 4;
		target_player = App->ai_manager->ai_player[enemy_faction];
	} while (enemy_faction == faction);

	if (target_player == nullptr)
		target_player = dynamic_cast<GenericPlayer*>(App->player);
}

DynamicEntity* AI_Player::GetClosestDynamicEntity() {
	DynamicEntity* target_entity = nullptr;
	int distance = 1000000;

	for(size_t i = 0; i < target_player->troops.size(); i++)
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
	if ((target_player->barrack[0] != nullptr) && (target_player->barrack[0]->isValidTarget()))
		target_building = target_player->barrack[0];
	else if ((target_player->laboratory != nullptr) && (target_player->laboratory->isValidTarget()))
		target_building = target_player->laboratory;
	else if ((target_player->barrack[1] != nullptr) && (target_player->barrack[1]->isValidTarget()))
		target_building = target_player->barrack[1];
	else if ((target_player->base != nullptr) && (target_player->base->isValidTarget()))
		target_building = target_player->base;
	else
		return nullptr;

	return target_building;
}

void AI_Player::GatherFood(ResourceBuilding* resource_spot) {

	for(int i = 0; i < gatherers; i++)
	{
		if ((gatherers_vector[i]->GetResourceBuilding() == nullptr)||(gatherers_vector[i]->GetResourceCollected() == 0)) {
			gatherers_vector[i]->AssignResourceBuilding(resource_spot);
			gatherers_vector[i]->PathfindToPosition(resource_spot->tiles.front());
			return;
		}
	}
	App->audio->die_sound = false;
}

