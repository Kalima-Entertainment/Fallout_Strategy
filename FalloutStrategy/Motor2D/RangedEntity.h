#ifndef _RANGED_ENTITY_H
#define _RANGED_ENTITY_H

#include "DynamicEntity.h"

class RangedEntity : public DynamicEntity {

public:

	RangedEntity(Faction type);
	~RangedEntity();
	bool Update(float dt);

private:
	int scope;
	int ammo;
};

#endif // !_RANGED_ENTITY_H