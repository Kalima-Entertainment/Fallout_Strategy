#ifndef _DYNAMIC_ENTITY_H
#define _DYNAMIC_ENTITY_H
#include "j1Entity.h"

enum class Troop {
	NONE,
	GATHERER,
	MELEE,
	RANGED
};

class DynamicEntity : public j1Entity
{
public:
	DynamicEntity(Faction faction, Troop type);
	~DynamicEntity();
	bool Update(float dt);
	bool LoadReferenceData();

public:
	State current_state;
	int mov_speed;
	int attack_speed;
	int damage;
	Troop type;
};



#endif // !_DYNAMIC_ENTITY_H

