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

	std::vector<iPoint> CreateNodePath(iPoint origin, iPoint destination);
	void CreateAttackingGroup();

public:
	GenericPlayer* target_player;
	bool is_attacking;

private:
	void ChooseRandomPlayerEnemy();
	DynamicEntity* GetClosestDynamicEntity();
	std::vector<iPoint> node_path;
	int ranged_minimum;
	int melee_minimum;
	j1Group group;
};

#endif // !_AI_PLAYER_H_

