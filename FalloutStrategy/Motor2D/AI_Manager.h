#ifndef _AI_MANAGER_H_
#define _AI_MANAGER_H_
#include "j1Module.h"
#include <vector>

class AI_Player;

class AI_Manager : public j1Module
{
public:
	AI_Manager();
	~AI_Manager();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

public:
	AI_Player* ai_player[4];
private:
	bool players_created;
	int beaten_ai_players;
};

#endif // !_AI_MANAGER_H_

