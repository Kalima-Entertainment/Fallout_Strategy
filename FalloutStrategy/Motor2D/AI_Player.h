#ifndef _AI_PLAYER_H_
#define _AI_PLAYER_H_
#include "GenericPlayer.h"
#include <vector>

class j1Entity;
enum Faction;

class AI_Player : public GenericPlayer
{
public:
	AI_Player(Faction faction);
	~AI_Player();

	bool Update(float dt);

private:
};


#endif // !_AI_PLAYER_H_

