#ifndef _DYNAMIC_ENTITY_H
#define _DYNAMIC_ENTITY_H
#include "j1Entity.h"

enum class Troop {
	GATHERER,
	MELEE,
	RANGE
};

class DynamicEntity : public j1Entity
{
public:
	DynamicEntity(Troop type);
	~DynamicEntity();
	bool Update(float dt);
	State current_state;
private:
	int mov_speed;
	int attack_speed;
	int damage;
};

#endif // !_DYNAMIC_ENTITY_H

