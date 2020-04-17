#ifndef _AI_PLAYER_H_
#define _AI_PLAYER_H_
#include "GenericPlayer.h"
#include <vector>

class j1Entity;
class DynamicEntity;
enum Faction;

class AI_Player : public GenericPlayer
{
public:
	AI_Player(Faction faction);
	~AI_Player();

	bool Update(float dt);
	void RecountEntities();

	GenericPlayer* target_player;

	bool is_attacking;

private:
	void ChooseRandomPlayerEnemy();
	DynamicEntity* GetClosestDynamicEntity();
	int ranged_minimum;
	int melee_minimum;
	std::vector<DynamicEntity*> gatherers_vector;
};

#endif // !_AI_PLAYER_H_

