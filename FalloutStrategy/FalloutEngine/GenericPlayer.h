#ifndef _GENERIC_PLAYER_H_
#define _GENERIC_PLAYER_H_
#include "j1Module.h"
#include <vector>

class j1Entity;
class DynamicEntity;
class StaticEntity;
enum class Resource;
enum Faction;
class Troop;
class Gatherer;

class GenericPlayer : public j1Module
{
public:
	GenericPlayer();
	virtual ~GenericPlayer();

	virtual bool Start() { return true; };
	virtual bool Update(float dt) { return true; };

	int GetTroopsAmount() const;
	bool DeleteEntity(j1Entity* entity);

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

public:
	int caps;
	int water;
	int food;

	Faction faction;
	bool defeated;
	bool is_ai;

	StaticEntity* base;
	StaticEntity* barrack[2];
	StaticEntity* laboratory;

	int melees;
	int rangeds;
	int gatherers;

	std::vector<Troop*> troops;
	std::vector<Gatherer*> gatherers_vector;

	bool goal_tile_set;
	GenericPlayer* target_player;
	StaticEntity* target_building;
};

#endif // !_GENERIC_PLAYER_H_

