#include "AI_Player.h"
#include "j1Entity.h"
#include "DynamicEntity.h"
#include "StaticEntity.h"
#include "AI_Manager.h"

AI_Player::AI_Player(Faction g_faction) : GenericPlayer() {
	faction = g_faction;
	base = barrack[0] = barrack[1] = laboratory = nullptr;
	caps = 100;
	water = 100;
	food = 100;
}

AI_Player::~AI_Player() {
	
}

bool AI_Player::Update(float dt) {
	bool ret = true;

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

	//if the ai_player is ready choose a player to attack
	if ((rangeds.size() > 3) && (melees.size() > 6) && (target_player == nullptr)) { 
		ChooseRandomPlayerEnemy();
	}

	return ret;
}

void AI_Player::ChooseRandomPlayerEnemy() {
	srand(time(NULL));
	int enemy_faction = rand() % 4;
	do
	{
		target_player = App->ai_manager->ai_player[enemy_faction];
	} while (enemy_faction == faction);

	if (target_player == nullptr)
		target_player = (GenericPlayer*)App->player;
}