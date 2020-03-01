#include "StaticEntity.h"

StaticEntity::StaticEntity(BuildingType type) {
	switch (type)
	{
	case BuildingType::BASE:
		break;
	case BuildingType::WAREHOUSE:
		break;
	case BuildingType::BARRACK:
		break;
	default:
		break;
	}
}

StaticEntity::~StaticEntity() {}