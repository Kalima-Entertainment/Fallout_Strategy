#ifndef _AI_MANAGER_H_
#define _AI_MANAGER_H_
#include "j1Module.h"
#include "p2Point.h"
#include <vector>

class AI_Player;
enum Faction;

struct AI_Info {
	int initial_caps;
	int initial_water;
	int initial_food;
	int minimum_melees;
	int minimum_rangeds;
};

class AI_Manager : public j1Module

{
public:
	AI_Manager();
	~AI_Manager();

	bool Start();
	bool Awake(pugi::xml_node& config);
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	std::vector<iPoint> CreateNodeMap();
	std::vector<iPoint> GetNodeMap();
	int GetDistanceBetweenNodes() const { return 150 / node_map_divisions; }
	AI_Info GetAI_PlayerInfo(Faction faction);

public:
	AI_Player* ai_player[4];
	std::vector<iPoint> node_map;
	bool players_created;
	int beaten_ai_players;
	int node_map_divisions;
	bool show_nodes;
private:
	AI_Info ai_info[4];
};

#endif // !_AI_MANAGER_H_

