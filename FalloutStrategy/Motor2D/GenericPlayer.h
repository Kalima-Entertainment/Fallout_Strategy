#ifndef _GENERIC_PLAYER_H_
#define _GENERIC_PLAYER_H_
#include "j1Module.h"
#include <vector>

class j1Entity;
class DynamicEntity;
enum class Resource;
enum Faction;

class GenericPlayer : public j1Module
{
public:
	GenericPlayer();
	virtual ~GenericPlayer();

	virtual bool Start() { return true; };
	virtual bool Update(float dt) { return true; };

public:
	int caps;
	int water;
	int food;
	Faction faction;
	std::vector<j1Entity*> entities;
};

#endif // !_GENERIC_PLAYER_H_

