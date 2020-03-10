#include "StaticEntity.h"

StaticEntity::StaticEntity(Faction faction, EntityType type) {
	switch (type)
	{
	case BASE:
		break;
	case LABORATORY:
		break;
	case BARRACK:
		break;
	default:
		break;
	}
}

StaticEntity::~StaticEntity() {}