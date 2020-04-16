#include "AI_Player.h"
#include "j1Entity.h"
#include "DynamicEntity.h"
#include "StaticEntity.h"
#include "AI_Manager.h"
#include "j1EntityManager.h"
#include "j1Player.h"

AI_Player::AI_Player(Faction g_faction) : GenericPlayer() {
	faction = g_faction;
	base = barrack[0] = barrack[1] = laboratory = nullptr;
	caps = 100;
	water = 100;
	food = 100;
	melee_minimum = 6;
	ranged_minimum = 3;
	is_attacking = false;
	defeated = false;
}

AI_Player::~AI_Player() {
	
}

bool AI_Player::Update(float dt) {
	bool ret = true;

	// Gather -----------------------------------------------------

	for (int i = 0; i < gatherers.size(); i++)
	{
		//authomatic gathering
		if (gatherers[i]->resource_building == nullptr) {
			gatherers[i]->resource_building = App->entities->GetClosestResourceBuilding(gatherers[i]->current_tile);
			//if there is at least a resource building left, go there
			if (gatherers[i]->resource_building != nullptr) {
				gatherers[i]->PathfindToPosition(App->entities->ClosestTile(gatherers[i]->current_tile, gatherers[i]->resource_building->tiles));
				gatherers[i]->state = WALK;
			}
			//if there are no resource buildings left
			else
			{
				gatherers[i]->state = IDLE;
			}
		}
	}

	// ------------------------------------------------------------

	//Spawn Units -------------------------------------------------

	//melee-ranged proportion
	float mr_proportion = melees.size() / rangeds.size();

	//spawn melee
	if ((water > App->entities->unit_data[faction][MELEE].cost_water)&&(caps > App->entities->unit_data[faction][MELEE].cost_meat) && (mr_proportion < 2)) {
		barrack[0]->SpawnUnit(MELEE);
		water -= App->entities->unit_data[faction][MELEE].cost_water;
		food -= App->entities->unit_data[faction][MELEE].cost_meat;
	}

	//spawn ranged
	if ((water > App->entities->unit_data[faction][RANGED].cost_water) && (caps > App->entities->unit_data[faction][RANGED].cost_meat)) {
		barrack[1]->SpawnUnit(RANGED);
		water -= App->entities->unit_data[faction][RANGED].cost_water;
		food -= App->entities->unit_data[faction][RANGED].cost_meat;
	}

	//if the ai_player is ready choose a player to attack
	if ((rangeds.size() > ranged_minimum) && (melees.size() > melee_minimum) && (target_player == nullptr)) { 
		ChooseRandomPlayerEnemy();
		is_attacking = true;
	}

	// -------------------------------------------------------------

	// Fight -------------------------------------------------------

	//Assign all attacking units an entity to attack
	if (is_attacking) {
		for (int i = 0; i < entities.size(); i++)
		{
			if ((entities[i]->is_dynamic) && (entities[i]->type != GATHERER) && (entities[i]->target_entity == nullptr)) {
				DynamicEntity* target_entity = GetClosestDynamicEntity();
				DynamicEntity* troop = nullptr;

				entities[i]->target_entity = target_entity;
				troop = (DynamicEntity*)entities[i];
				troop->PathfindToPosition(target_entity->current_tile);
				troop->state = WALK;
			}
		}	
		if (target_player->entities.size() == 4) {
			if (target_player->GetTroopsAmount() == 0) {
				target_player->defeated = true;
				is_attacking = false;
				target_player = nullptr;
			}
		}
	}

	// -------------------------------------------------------------

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
}

DynamicEntity* AI_Player::GetClosestDynamicEntity() {
	DynamicEntity* target_entity = nullptr;
	int distance = 1000000;

	for (int i = 0; i < target_player->entities.size(); i++)
	{
		if ((target_player->entities[i]->current_tile.DistanceManhattan(base->current_tile) < distance)&&(target_player->entities[i]->is_dynamic)) {
			target_entity = (DynamicEntity*)target_player->entities[i];
			distance = target_player->entities[i]->current_tile.DistanceManhattan(base->current_tile);
		}
	}
	return target_entity;
}
