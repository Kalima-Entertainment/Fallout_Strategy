#ifndef _STATIC_ENTITY_H
#define _STATIC_ENTITY_H
#include "j1Entity.h"
#include <vector>
#include <string>
#include <iostream>
#include <chrono>

enum StaticState {
	WAIT,
	WORK,
	EXPLODE,
	NO_STATE
};

struct Spawn_Stack {
	EntityType type;
	int spawn_seconds; //Seconds it requires to spawn
};

struct Upgrade_Stack {
	Faction faction;
	EntityType building;
	int upgrade_seconds;
};

class StaticEntity : public j1Entity

{
public:
	StaticEntity(Faction faction, EntityType type);
	~StaticEntity();
	bool Update(float dt);
	bool PostUpdate();

	bool LoadAnimations();
	bool LoadReferenceData();	

	void Upgrade(Upgrades_Data upgrades_data);
	void ExecuteUpgrade(Faction faction, Upgrades upgrade_name);
	void SpawnUnit(EntityType type, bool no_cost = false);
	void UpdateSpawnStack();
	void UpdateUpgradeStack();
	
public:
	std::vector<iPoint> tiles;
	StaticState state;
private:
	int gen_speed;
	Animation animations[3];
	iPoint render_texture_pos;

	Spawn_Stack spawn_stack[10];
	Upgrade_Stack upgrade_stack;

	j1Timer chrono_spawn;
	j1Timer chrono_upgrade;
	bool spawning;
	bool upgrading;
	bool want_to_upgrade;
	float time_left;
};

#endif // !_STATIC_ENTITY_H