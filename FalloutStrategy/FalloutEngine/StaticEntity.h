#ifndef _STATIC_ENTITY_H
#define _STATIC_ENTITY_H
#include "j1Entity.h"
#include <vector>
#include <string>
#include <iostream>
#include <chrono>

struct Spawn_Stack {
	EntityType type;
	int spawn_seconds; //Seconds it requires to spawn
};

struct Upgrade_Stack {
	Faction faction;
	EntityType building;
	int upgrade_seconds;
};

struct UpgradeSprite {
	iPoint position;
	SDL_Rect rect;
};

class StaticEntity : public j1Entity

{
public:
	StaticEntity(Faction faction, EntityType type, iPoint current_tile, GenericPlayer* owner = nullptr);
	~StaticEntity();	
	bool Update(float dt);
	bool PostUpdate();

	bool LoadAnimations(const char* folder, const char* file_name);
	bool LoadReferenceData(pugi::xml_node& node);
	bool LoadDataFromReference();

	void Upgrade(Upgrades_Data upgrades_data);
	void ExecuteUpgrade(Faction faction, Upgrades upgrade_name);
	void SpawnUnit(EntityType type, bool no_cost = false);
	void UpdateSpawnStack();

	void SpawnChrono();
	void UpgradeChrono();

	void DebugSpawnsUpgrades();
	void CalculateRenderAndSpawnPositions();

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
	
public:
	std::vector<iPoint> tiles;
	float time_left;
	Spawn_Stack spawn_stack[10];
	bool spawning;
	int level;
	int GetUnitsInStack(EntityType type);

private:
	int gen_speed;
	Animation animations[2];

	int max_capacity;

	Upgrade_Stack upgrade_stack;

	j1Timer chrono_spawn;
	j1Timer chrono_upgrade;
	
	bool upgrading;
	bool want_to_upgrade;
	
	UpgradeSprite upgrade_sprite[4];
};

#endif // !_STATIC_ENTITY_H