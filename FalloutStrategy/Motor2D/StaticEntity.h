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

struct Upgrades_Data{
	Faction faction;
	std::string name;
	int upgrade_num;
	int first_price;
	int price_increment;
};

struct Spawn_Stack {
	EntityType type;
	int spawn_seconds; //Seconds it requires to spawn
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

	void Upgrade(Faction faction, std::string upgrade_name);
	void SpawnUnit(EntityType type);
	void UpdateSpawnStack();
	
public:
	std::vector<iPoint> tiles;
	StaticState state;
	Spawn_Stack spawn_stack[10];

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

	j1Timer chrono;
	bool spawning;
	float time_left;
};

#endif // !_STATIC_ENTITY_H