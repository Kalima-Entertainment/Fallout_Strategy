#ifndef _AI_PLAYER_H_
#define _AI_PLAYER_H_
#include "GenericPlayer.h"
#include <vector>
#include "p2Point.h"

class j1Entity;
class DynamicEntity;
enum Faction;
class j1Group;

class AI_Player : public GenericPlayer
{
public:
	AI_Player(Faction faction);
	~AI_Player();

	bool Update(float dt);

private:
	void ChooseRandomPlayerEnemy();
	DynamicEntity* GetClosestDynamicEntity();
	StaticEntity* ChooseTargetBuilding();

public:
	bool is_attacking;
	std::vector<iPoint> path_to_enemy_base;

private:
	int ranged_minimum;
	int melee_minimum;
	j1Group* group;
	iPoint target_building_position;
	int last_barrack_to_spawn;
};

#endif // !_AI_PLAYER_H_

