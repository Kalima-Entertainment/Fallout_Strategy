#ifndef _GENERIC_PLAYER_H_
#define _GENERIC_PLAYER_H_
#include "j1Module.h"
#include <vector>

class j1Entity;
class DynamicEntity;
class StaticEntity;
enum class Resource;
enum Faction;

class GenericPlayer : public j1Module
{
public:
	GenericPlayer();
	virtual ~GenericPlayer();

	virtual bool Start() { return true; };
	virtual bool Update(float dt) { return true; };

	//virtual int GetMelees() const;
	//virtual int GetRangeds() const;
	int GetTroopsAmount() const;
	bool DeleteEntity(j1Entity* entity);

public:
	int caps;
	int water;
	int food;

	Faction faction;
	bool defeated;

	StaticEntity* base;
	StaticEntity* barrack[2];
	StaticEntity* laboratory;
	int melees;
	int rangeds;
	int gatherers;
	std::vector<DynamicEntity*> troops;
	std::vector<DynamicEntity*> gatherers_vector;

	bool goal_tile_set;
	GenericPlayer* target_player;
	StaticEntity* target_building;
};

#endif // !_GENERIC_PLAYER_H_

