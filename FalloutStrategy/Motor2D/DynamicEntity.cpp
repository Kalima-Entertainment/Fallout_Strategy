#include "DynamicEntity.h"

DynamicEntity::DynamicEntity(Troop type) {
	switch (type)
	{
	case Troop::GATHERER:
		break;
	case Troop::MELEE:
		break;
	case Troop::RANGE:
		break;
	default:
		break;
	}
}

DynamicEntity::~DynamicEntity() {}