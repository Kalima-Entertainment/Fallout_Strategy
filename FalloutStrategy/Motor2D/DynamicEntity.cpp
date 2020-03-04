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
		if (target_tile != current_tile)
		{
			//state = WALK;
		}
		break;
	case WALK:
		PathfindToPosition(target_tile);
		CheckAnimation();
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

	return ret;
}

void DynamicEntity::CheckAnimation() {
	if ((current_tile.x >= target_tile.x)&&(current_tile.y >= target_tile.y))
	{
		//current_animation = &animations[state][TOP_LEFT];
		direction = TOP_LEFT;
	}
	else if ((current_tile.x <= target_tile.x) && (current_tile.y >= target_tile.y)) {
		//current_animation = &animations[state][TOP_RIGHT];
		direction = TOP_RIGHT;
	}
	else if ((current_tile.x >= target_tile.x) && (current_tile.y <= target_tile.y)){
		//current_animation = &animations[state][BOTTOM_LEFT];
		direction = BOTTOM_LEFT;
	}
	else if ((current_tile.x <= target_tile.x) && (current_tile.y <= target_tile.y)) {
		//current_animation = &animations[state][BOTTOM_RIGHT];
		direction = BOTTOM_RIGHT;
	}
}