#include "DynamicEntity.h"
#include "j1Map.h"
#include "j1App.h"
#include "j1Scene.h"
#include "j1Render.h"
#include "j1Pathfinding.h"

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

	current_speed = { 0, 0 };
	speed = { 1, 0.5f };
	reference_entity = nullptr;
	attack_collider = nullptr;
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
		//PathfindToPosition(target_tile);
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

	if (App->render->debug)
	{
		if (path_to_target != NULL)
		{
			for (uint i = 0; i < path_to_target->Count(); ++i)
			{
				iPoint pos = App->map->MapToWorld(path_to_target->At(i)->x, path_to_target->At(i)->y);
				App->render->Blit(App->scene->debug_tex, pos.x, pos.y);
			}
		}
	}

	iPoint render_position;
	render_position = App->map->MapToWorld(current_tile.x, current_tile.y);

	App->render->Blit(reference_entity->texture, position.x - 64, position.y - 112, &current_animation->GetCurrentFrame());
	
	if (App->render->debug)
	{
		App->render->DrawQuad({ (int)position.x - 2, (int)position.y - 2, 4,4 }, 255, 0, 0, 255);
		App->render->DrawQuad(next_tile_center_rect, 0, 255, 0, 255);
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

void DynamicEntity::Move() {
	if (path_to_target != NULL)
	{
		if (path_to_target->Count() != 0)
		{
			//next_tile = *path_to_target->At(0);
			next_tile_center = App->map->MapToWorld(next_tile.x, next_tile.y);
			next_tile_center_rect = { next_tile_center.x + 30, next_tile_center.y + 30,4,4 };

			//Method 1
			/*
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
			*/

			//Method 2

			if ((position.x > next_tile_center_rect.x + next_tile_center_rect.w) && (position.x > next_tile_center_rect.x) && (position.y > next_tile_center_rect.y) && (position.y > next_tile_center_rect.y + next_tile_center_rect.h)) {
				direction = TOP_LEFT;
				position.x -= speed.x;
				position.y -= speed.y;
			}
			else if ((position.x < next_tile_center_rect.x) && (position.x < next_tile_center_rect.x + next_tile_center_rect.w) && (position.y > next_tile_center_rect.y) && (position.y > next_tile_center_rect.y + next_tile_center_rect.h)) {
				direction = TOP_RIGHT;
				position.x += speed.x;
				position.y -= speed.y;
			}
			else if ((position.x > next_tile_center_rect.x) && (position.x > next_tile_center_rect.x + next_tile_center_rect.w) && (position.y < next_tile_center_rect.y) && (position.y < next_tile_center_rect.y + next_tile_center_rect.h)) {
				direction = BOTTOM_LEFT;
				position.x -= speed.x;
				position.y += speed.y;
			}
			else if ((position.x < next_tile_center_rect.x) && (position.x < next_tile_center_rect.x + next_tile_center_rect.w) && (position.y < next_tile_center_rect.y) && (position.y < next_tile_center_rect.y + next_tile_center_rect.h)) {
				direction = BOTTOM_RIGHT;
				position.x += speed.x;
				position.y += speed.y;
			}
			else
			{
				if (*path_to_target->At(0) != target_tile)
				{
					current_tile = *path_to_target->At(0);
					next_tile = *path_to_target->At(1);
					path_to_target->Advance();
				}
				else
				{
					position.x = next_tile_center_rect.x +2;
					position.y = next_tile_center_rect.y +2;
					current_tile = next_tile;
					state = IDLE;
				}
			}

		}
		else
		{
			state = IDLE;
			target_tile = current_tile;
		}
	}
}

void DynamicEntity::PathfindToPosition(iPoint destination) {

	//iPoint origin = App->map->WorldToMap(position.x, position.y);
	current_tile = App->map->WorldToMap(position.x, position.y);
	App->pathfinding->CreatePath(current_tile, destination);

	//pathfinding debug
	int x, y;
	SDL_Rect Debug_rect = { 0,0,32,32 };

	path_to_target = (p2DynArray<iPoint>*)App->pathfinding->GetLastPath();
	next_tile = *path_to_target->At(0);

	for (uint i = 0; i < path_to_target->Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path_to_target->At(i)->x, path_to_target->At(i)->y);
		Debug_rect.x = pos.x;
		Debug_rect.y = pos.y;
		if (App->render->debug)App->render->DrawQuad(Debug_rect, 90, 850, 230, 40);
	}
}
