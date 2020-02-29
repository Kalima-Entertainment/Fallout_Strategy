#ifndef _GATHERER_ENTITY_H
#define _GATHERER_ENTITY_H

#include "DynamicEntity.h"

class GathererEntity : public DynamicEntity {

public:
	GathererEntity(Faction type);
	~GathererEntity();
	bool Update(float dt);

private:
	Resource capacity;
};

#endif // !_GATHERER_ENTITY_H