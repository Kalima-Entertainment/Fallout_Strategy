#ifndef _AI_PLAYER_H_
#define _AI_PLAYER_H_
#include "j1Module.h"

enum Faction;

class AI_Player : public j1Module 
{
public:
	AI_Player(Faction faction);
	~AI_Player();

private:
	Faction faction;
};


#endif // !_AI_PLAYER_H_

