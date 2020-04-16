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

public:
	int caps;
	int water;
	int food;

	Faction faction;
	bool defeated;

	std::vector<j1Entity*> entities;
	StaticEntity* base;
	StaticEntity* barrack[2];
	StaticEntity* laboratory;
};

#endif // !_GENERIC_PLAYER_H_

