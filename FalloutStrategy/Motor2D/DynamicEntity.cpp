#include "DynamicEntity.h"

DynamicEntity::DynamicEntity(Troop type) {
	switch (type)
	{
	case Troop::GATHERER:
		break;
	case Troop::MELEE:
		break;
	case Troop::RANGED:
		break;
	default:
		break;
	}
	current_animation = &idle;
}

DynamicEntity::~DynamicEntity() {}

bool DynamicEntity::Update(float dt) {
	return true;
}