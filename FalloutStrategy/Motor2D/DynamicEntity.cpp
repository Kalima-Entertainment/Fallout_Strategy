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
		if ((target_tile.x >= 0)&&(target_tile.y >= 0))
		{

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

<<<<<<< Updated upstream
=======
bool DynamicEntity::PostUpdate() {
	current_animation = &animations[state][direction];

	if (path_to_target != NULL)
	{
		for (uint i = 0; i < path_to_target->Count(); ++i)
		{
			iPoint pos = App->map->MapToWorld(path_to_target->At(i)->x, path_to_target->At(i)->y);
			App->render->Blit(App->scene->debug_tex, pos.x, pos.y);
		}
	}

	App->render->Blit(reference_entity->texture, position.x - 32, position.y - 96, &current_animation->GetCurrentFrame());
	//App->render->DrawQuad({(int)(position.x - 2), (int)(position.y - 2) , 4 , 4 }, 255, 0, 0, 255);
	return true;
}

>>>>>>> Stashed changes
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

