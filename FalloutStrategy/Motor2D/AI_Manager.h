#ifndef _AI_MANAGER_H_
#define _AI_MANAGER_H_
#include "j1Module.h"
#include "p2Point.h"
#include <vector>

class AI_Player;
class AI_Manager : public j1Module
{
public:
	AI_Manager();
	~AI_Manager();

	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	std::vector<iPoint> CreateNodeMap();
	std::vector<iPoint> GetNodeMap();
	int GetDistanceBetweenNodes() const { return 150 / node_map_divisions; }

public:
	AI_Player* ai_player[4];
	std::vector<iPoint> node_map;

public:
	bool players_created;
	int beaten_ai_players;
	int node_map_divisions;
};

#endif // !_AI_MANAGER_H_

