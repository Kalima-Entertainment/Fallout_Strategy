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

enum Upgrades {
	RESOURCES_LIMIT,
	GATHERER_CAPACITY,
	UNITS_DAMAGE,
	UNITS_SPEED,
	UNITS_HEALTH,
	CREATION_TIME,
	NO_UPGRADE
};

struct Upgrades_Data{
	Faction faction;
	Upgrades upgrade;
	int upgrade_num;
	int first_price;
	int price_increment;
	int seconds;
};

struct Spawn_Stack {
	EntityType type;
	int spawn_seconds; //Seconds it requires to spawn
};

struct Upgrade_Stack {
	Faction faction;
	Upgrades upgrade;
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

	Upgrades_Data base_resource_limit[4];
	Upgrades_Data gatherer_resource_limit[4];
	Upgrades_Data units_damage[4];
	Upgrades_Data units_speed[4];
	Upgrades_Data units_health[4];
	Upgrades_Data units_creation_time[4];

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