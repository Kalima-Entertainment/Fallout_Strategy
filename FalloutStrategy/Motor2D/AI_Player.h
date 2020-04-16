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

	GenericPlayer* target_player;

	std::vector<DynamicEntity*> gatherers;
	std::vector<DynamicEntity*> rangeds;
	std::vector<DynamicEntity*> melees;

	bool is_attacking;

private:
	void ChooseRandomPlayerEnemy();
	DynamicEntity* GetClosestDynamicEntity();
	int ranged_minimum;
	int melee_minimum;
};


#endif // !_AI_PLAYER_H_

