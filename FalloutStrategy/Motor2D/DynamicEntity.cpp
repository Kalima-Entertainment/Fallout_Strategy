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
}

DynamicEntity::~DynamicEntity() {}

bool DynamicEntity::Update(float dt) {
	return true;
}

bool DynamicEntity::LoadReferenceData() {
	bool ret = true;
	/*
	int size = reference_entity->animations.size();
	//animations
	for (int i = 0; i < size; i++)
	{
		animations.push_back(reference_entity->animations.at(i));
	}
	idle[0] = *animations.at(0);
	*/
	for (int i = 0; i < MAX_ANIMATIONS; i++)
	{
		for (int j = 0; j <= 6; j++)
		{
			animations[i][j] = reference_entity->animations[i][j];
		}
	}

	current_animation = &animations[IDLE][TOP_LEFT];

	return ret;
}

