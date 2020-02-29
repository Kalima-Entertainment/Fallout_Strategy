#ifndef _MELEE_ENTITY_H
#define _MELEE_ENTITY_H

#include "DynamicEntity.h"

class MeleeEntity : public DynamicEntity {

public:

	MeleeEntity(Faction type);
	~MeleeEntity();
	bool Update(float dt);

private:
	int scope;
	int ammo;
};

#endif // !_MELEE_ENTITY_H
