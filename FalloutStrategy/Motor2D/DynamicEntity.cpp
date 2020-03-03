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
	state = IDLE;
	direction = TOP_RIGHT;
}

DynamicEntity::~DynamicEntity() {}

bool DynamicEntity::Update(float dt) {
	switch (state)
	{
	case IDLE:
		position.x += 10;
		break;
	case WALK:
		PathfindToPosition(target_tile);
		if ((target_tile.x >= 0)&&(target_tile.y >= 0))
		{
			position.x++;
		}
		break;
	case ATTACK:
		break;
	case GATHER:
		break;
	case HIT:
		break;
	case DIE:
		break;
	default:
		break;
	}
	return true;
}

bool DynamicEntity::LoadReferenceData() {
	bool ret = true;

	for (int i = 0; i < MAX_ANIMATIONS; i++)
	{
		for (int j = 0; j <= 6; j++)
		{
			animations[i][j] = reference_entity->animations[i][j];
		}
	}

	current_animation = &animations[IDLE][TOP_RIGHT];

	return ret;
}

