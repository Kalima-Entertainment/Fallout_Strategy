#include "j1Entity.h"
#include "j1Group.h"
#include "j1MovementManager.h"
#include "j1App.h"
#include "j1EntityManager.h"
#include "p2Log.h"
#include "j1Pathfinding.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Player.h"
#include "DynamicEntity.h"
#include "StaticEntity.h"


j1MovementManager::j1MovementManager()
{

	name.assign("MovementManager");


}

j1MovementManager::~j1MovementManager()
{

}

bool j1MovementManager::Update(float dt)
{
	return true;
}

bool j1MovementManager::CleanUp()
{
	std::list <j1Group*>::const_iterator group = Groups.begin();

	while (group != Groups.end())
	{
		(*group)->ClearGroup();
		delete* group;
		group++;
	}

	Groups.clear();

	return true;
}

void j1MovementManager::SelectEntities_inRect(SDL_Rect SRect)
{
	//This method needs to check only dynamic entities adapt
	std::vector<j1Entity*>::iterator entity = App->entities->entities.begin();

	SDL_Rect entityrect = { 0,0,0,0 };

	while (entity != App->entities->entities.end())
	{
		//If entity list find some dynamic entity and its inside the Selection Rect we make this dynamic entity info isSelected True
		if ((*entity)->is_dynamic) {
			// Checks if Dynamic entitie selected owns our selected faction
			if (((*entity)->faction == App->player->faction)||(App->player->god_mode)) {
				entityrect = { (int)(*entity)->position.x, (int)(*entity)->position.y, 5 , 5 };

				//Comparing if intersection between Selection Rect and Entity Rect
				if (SDL_HasIntersection(&entityrect, &SRect))
					(*entity)->info.IsSelected = true;
				else
					(*entity)->info.IsSelected = false;
			}
		}
		entity++;
	}
}

void j1MovementManager::CreateGroup(std::vector<DynamicEntity*> entities_vector)
{
	//LOG("Group Creation Called");
	bool Validgroup = false;

	j1Group* group = new j1Group;

	std::vector<DynamicEntity*>::iterator entity = entities_vector.begin();

	while (entity != entities_vector.end())
	{
		// --- Check if just Dynamic entities are enabled to be in group
		// --- If we find an entity then the group is valid and can be created ---
		Validgroup = true;

		// --- Remove the entity from a previous group if any is found ---
		if ((*entity)->info.current_group != nullptr)
		{
			(*entity)->info.current_group->removeUnit(*entity);

			if ((*entity)->info.current_group->GetSize() == 0)
				Groups.remove((*entity)->info.current_group);
		}

		// --- Add the entity to the new group, update its current group pointer ---
		group->addUnit(*entity);
		(*entity)->info.current_group = group;
		entity++;
	}

	// --- Finally, If the group is Valid add it to our Groups list, else delete it ---
	if (Validgroup) Groups.push_back(group);
	else delete group;
}

void j1MovementManager::Move(j1Group* group, iPoint goal_path, float dt)
{
	/*
	// -- Getting group information
	std::list <j1Entity*>::const_iterator unit = group->Units.begin();

	for (unit = group->Units.begin();  unit != group->Units.end(); unit++)
	{
		// -- Cast to Dynamic to work properly
		DynamicEntity* entity = (DynamicEntity*)(*unit);

		if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN && entity->info.IsSelected) {
			// -- Calculate new path to reach goal
			entity->PathfindToPosition(goal_path);

			if (group->IsGroupLead(entity) == false) {
				group->SetUnitGoalTile(entity);
			}
			else
			{
				// --- Clear previous path request occupied goal tiles ---
				group->ClearOccupiedlist();
				(*unit)->info.goal_tile = goal_path;
				group->Occupied_tiles.push_back(&(*unit)->info.goal_tile);
			}

			if (entity->path_to_target.size() > 0) {

				// -- Get next tile center
				entity->next_tile_position = App->map->MapToWorld(entity->next_tile.x, entity->next_tile.y);
				entity->next_tile_rect = { entity->next_tile_position.x + HALF_TILE - 5, entity->next_tile_position.y + HALF_TILE - 3, 10, 10 };

				entity->last_direction = entity->direction;
				entity->direction = entity->GetDirectionToGo(entity->next_tile_rect);

				switch (entity->direction)
				{
				case NO_DIRECTION:
					if (entity->next_tile != entity->target_tile)
					{
						//current_tile = path_to_target.front();
						if (entity->path_to_target.size() > 1)
						{
							entity->next_tile = entity->path_to_target[1];

							if (App->entities->occupied_tiles[entity->next_tile.x][entity->next_tile.y]) {
								entity->PathfindToPosition(entity->target_tile);
								return;
							}

						}
						entity->path_to_target.erase(entity->path_to_target.begin());
					}
					else if (entity->node_path.size() == 0)
					{
						entity->path_to_target.clear();
						entity->state = IDLE;
					}

					entity->direction = entity->last_direction;
					entity->UpdateTile();
					break;

				case TOP_LEFT:
					entity->position.x -= entity->speed.x * dt;
					entity->position.y -= entity->speed.y * dt;
					break;
				case TOP_RIGHT:
					entity->position.x += entity->speed.x * dt;
					entity->position.y -= entity->speed.y * dt;
					break;
				case BOTTOM_LEFT:
					entity->position.x -= entity->speed.x * dt;
					entity->position.y += entity->speed.y * dt;
					break;
				case BOTTOM_RIGHT:
					entity->position.x += entity->speed.x * dt;
					entity->position.y += entity->speed.y * dt;
					break;
				default:
					break;
				}
			}
			else
			{
				entity->UpdateTile();
				entity->direction = entity->last_direction;
				entity->state = IDLE;
			}

		}

	}
	*/
}

// Load Game State
bool j1MovementManager::Load(pugi::xml_node& data)
{
	//camera.x = data.child("camera").attribute("x").as_int();
	//camera.y = data.child("camera").attribute("y").as_int();

	return true;
}

// Save Game State
bool j1MovementManager::Save(pugi::xml_node& data) const
{
	//pugi::xml_node cam = data.append_child("camera");

	//cam.append_attribute("x") = camera.x;
	//cam.append_attribute("y") = camera.y;

	return true;
}
