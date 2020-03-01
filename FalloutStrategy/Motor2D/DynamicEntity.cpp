#include "DynamicEntity.h"

DynamicEntity::DynamicEntity(Faction g_faction, Troop g_type) {

	switch (g_type)
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

	type = g_type;
	faction = g_faction;
	current_animation = &idle;
}

DynamicEntity::~DynamicEntity() {}

bool DynamicEntity::Update(float dt) {
	return true;
}

bool DynamicEntity::LoadReferenceData() {
	bool ret = true;
	int size = reference_entity->animations.size();

	for (int i = 0; i < size; i++)
	{
		animations.push_back(reference_entity->animations.at(i));
	}

	return ret;
}

