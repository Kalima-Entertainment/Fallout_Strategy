#include "DynamicEntity.h"
#include "j1Map.h"
#include "j1App.h"
#include "j1Scene.h"
#include "j1Render.h"

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
		Move();
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

void DynamicEntity::Move() {
	if (path_to_target != NULL)
	{
		if (path_to_target->Count() != 0)
		{
			next_tile = *path_to_target->At(0);
			if ((current_tile.x > next_tile.x) && (current_tile.y == next_tile.y))
			{
				direction = TOP_LEFT;
				position.x -= speed.x;
				position.y -= speed.y;
			}
			else if ((current_tile.x == next_tile.x) && (current_tile.y > next_tile.y)) 
			{
				direction = TOP_RIGHT;
				position.x += speed.x;
				position.y -= speed.y;
			}
			else if ((current_tile.x == next_tile.x) && (current_tile.y < next_tile.y)) 
			{
				direction = BOTTOM_LEFT;
				position.x -= speed.x;
				position.y += speed.y;
			}
			else if ((current_tile.x < next_tile.x) && (current_tile.y == next_tile.y)) 
			{
				direction = BOTTOM_RIGHT;
				position.x += speed.x;
				position.y += speed.y;
			}
			else if (current_tile.x == (target_tile.x - 1) && (current_tile.y == (target_tile.y + 1)))
			{
				direction = RIGHT;
				position.x += speed.x;
			}
			else if ((current_tile.x == (target_tile.x -1))&&(current_tile.y == (target_tile.y + 1)))
			{
				direction = LEFT;
				position.x += speed.x;
			}
		}
		else
		{
			state = IDLE;
			target_tile = current_tile;
		}
	}
}