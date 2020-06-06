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

j1MovementManager::j1MovementManager() {
}

j1MovementManager::~j1MovementManager() {

}

bool j1MovementManager::Update(float dt)
{
	return true;
}

bool j1MovementManager::CleanUp()
{
	for (int i = 0; i < Groups.size(); i++)
	{
		Groups[i]->ClearGroup();
		delete Groups[i];
		Groups[i] = nullptr;
	}

	Groups.clear();

	return true;
}

void j1MovementManager::SelectEntities_inRect(SDL_Rect SRect)
{
	//This method needs to check only dynamic entities adapt
	std::vector<j1Entity*>::const_iterator entity = App->entities->entities.cbegin();

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

j1Group* j1MovementManager::CreateGroup(std::vector<DynamicEntity*> entities_vector)
{
	//LOG("Group Creation Called");
	bool Validgroup = false;

	j1Group* group = new j1Group;

	std::vector<DynamicEntity*>::const_iterator entity = entities_vector.begin();

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
				(*entity)->info.current_group = nullptr;
		}

		// --- Add the entity to the new group, update its current group pointer ---
		group->addUnit(*entity);
		(*entity)->info.current_group = group;
		entity++;
	}

	// --- Finally, If the group is Valid add it to our Groups list, else delete it ---
	if (Validgroup) {
		Groups.push_back(group);
		//LOG("Group Created");
		return group;
	}
	else delete group;

}

void j1MovementManager::Move(j1Group* group, iPoint goal_path, float dt)
{
	std::vector <DynamicEntity*>::const_iterator unit = group->Units.cbegin();

	iPoint Map_Entityposition;
	fPoint distanceToNextTile;
	fPoint next_tile_world;
	float DirectDistance;
	fPoint to_fPoint;

	while (unit != group->Units.cend())
	{
		(*unit)->UpdateTile();

		// --- We Get the map coords of the Entity ---
		Map_Entityposition.x = (*unit)->position.x;
		Map_Entityposition.y = (*unit)->position.y;
		Map_Entityposition = App->map->WorldToMap(Map_Entityposition.x, Map_Entityposition.y);

		if ((App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN) && (*unit)->info.IsSelected) 
			(*unit)->info.UnitMovementState = MovementState::MovementState_NoState;

		switch ((*unit)->info.UnitMovementState)
		{

		case MovementState::MovementState_NoState:

			// --- On call to Move, Units will request a path to the destination ---

			if ((App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN) && ((*unit)->info.IsSelected))
			{
				(*unit)->commanded = true;
				if (group->IsGroupLead((*unit)) == false)
				{
					// --- If any other unit of the group has the same goal, change the goal tile ---
					group->SetUnitGoalTile((*unit));
					App->entities->occupied_tiles[(*unit)->info.goal_tile.x][(*unit)->info.goal_tile.y];
				}
				else
				{
					// --- Clear previous path request occupied goal tiles ---
					group->ClearOccupiedlist();
					(*unit)->info.goal_tile = goal_path;
					group->Occupied_tiles.push_back(&(*unit)->info.goal_tile);
					//LOG("CREATE PATH RETURNS: %i", App->pathfinding->CreatePath(Map_Entityposition, (*unit)->info.goal_tile));
					//(*unit)->owner->goal_tile_set = true;
				}

				(*unit)->CheckDestination(goal_path);

				/*
				if (App->pathfinding->CreatePath(Map_Entityposition, (*unit)->info.goal_tile) != -1)
				{
					(*unit)->info.Current_path = App->pathfinding->GetLastPath();
					(*unit)->info.Current_path.erase((*unit)->info.Current_path.begin());
					(*unit)->info.Current_path.erase((*unit)->info.Current_path.begin());

					(*unit)->info.UnitMovementState = MovementState::MovementState_NextStep;
				}
				else */
				if ((*unit)->PathfindToPosition(goal_path)){
					(*unit)->info.Current_path = (*unit)->path_to_target;
					(*unit)->info.UnitMovementState = MovementState::MovementState_NextStep;
				}
				else {
					stop_iteration = true;
				}
			}

			break;

		case MovementState::MovementState_Wait:

			// --- Whenever the unit faces an obstacle of any type during a scheduled path, overcome it ---
			(*unit)->state = IDLE;
			(*unit)->commanded = false;
			break;

		case MovementState::MovementState_FollowPath:

			//dynamic_entity->state = WALK;
			(*unit)->state = WALK;
			// --- If a path is created, the unit will start following it ---

			next_tile_world = App->map->fMapToWorld((*unit)->info.next_tile.x, (*unit)->info.next_tile.y);
			next_tile_world.x += 32.0f;
			next_tile_world.y += 32.0f;

			//distanceToNextTile = { (float)(next_tile_world.x + 32.0f - (*unit)->position.x),(float)(next_tile_world.y + 32.0f - (*unit)->position.y)};
			distanceToNextTile = { next_tile_world.x - (*unit)->position.x, next_tile_world.y - (*unit)->position.y };

			// --- We compute the module of our vector ---
			DirectDistance = sqrtf(pow(distanceToNextTile.x, 2.0f) + pow(distanceToNextTile.y, 2.0f));

			// --- We want a unitary vector to update the unit's direction/position ---
			if (DirectDistance > 0.0f)
			{
				distanceToNextTile.x /= DirectDistance;
				distanceToNextTile.y /= DirectDistance;
			}

			// --- Now we Apply the unit's Speed and the dt to the unitary vector  ---
			distanceToNextTile.x *= (*unit)->speed.x * dt;
			distanceToNextTile.y *= (*unit)->speed.y * dt;

			if ((distanceToNextTile.x >= 0) && (distanceToNextTile.y < 0)) { (*unit)->direction = TOP_RIGHT; }
			else if ((distanceToNextTile.x > 0) && (distanceToNextTile.y >= 0)) { (*unit)->direction = BOTTOM_RIGHT; }
			else if ((distanceToNextTile.x <= 0) && (distanceToNextTile.y > 0)) { (*unit)->direction = BOTTOM_LEFT; }
			else if ((distanceToNextTile.x < 0) && (distanceToNextTile.y <= 0)) { (*unit)->direction = TOP_LEFT; }

			(*unit)->current_tile = App->map->WorldToMap((*unit)->position.x, (*unit)->position.y);

			// --- We convert an iPoint to fPoint for comparing purposes ---

			if ((*unit)->position.DistanceTo(next_tile_world) < 3)
			{
				(*unit)->position.x = next_tile_world.x;
				(*unit)->position.y = next_tile_world.y;
				(*unit)->info.UnitMovementState = MovementState::MovementState_NextStep;
			}

			else
			{
				//LOG("Current distance %.2f", (*unit)->position.DistanceTo(next_tile_world));
				(*unit)->position.x += distanceToNextTile.x;
				(*unit)->position.y += distanceToNextTile.y;
			}


			break;

		case MovementState::MovementState_NextStep:

			// --- If a path is being followed, the unit will get the next tile in the path ---

			if (App->entities->IsTileOccupied((*unit)->target_tile))
				(*unit)->PathfindToPosition((*unit)->target_tile);

			//if we haven't reached our destination
			if ((*unit)->path_to_target.size() > 0)
			{
				(*unit)->info.next_tile = (*unit)->path_to_target.front();
				(*unit)->path_to_target.erase((*unit)->path_to_target.begin());
				(*unit)->info.UnitMovementState = MovementState::MovementState_FollowPath;
			}
			else
			{
				//we reach the following node
				if (((*unit)->node_path.size() > 0)&&((*unit)->current_tile == (*unit)->node_path.back())) {
					(*unit)->node_path.pop_back();
					//if there are more nodes to follow get the next one
					if((*unit)->node_path.size() > 0){
						(*unit)->info.goal_tile = (*unit)->node_path.back();
					}
					//if not pathfind to target tile
					else {
						(*unit)->info.goal_tile = (*unit)->target_tile;
					}

					if((*unit)->PathfindToPosition((*unit)->info.goal_tile))
					{
						(*unit)->info.next_tile = (*unit)->path_to_target.front();
						(*unit)->path_to_target.erase((*unit)->path_to_target.begin());
						(*unit)->info.UnitMovementState = MovementState::MovementState_FollowPath;
					}
				}
				else {
					(*unit)->info.UnitMovementState = MovementState::MovementState_DestinationReached;
				}
			}

			break;

		case MovementState::MovementState_DestinationReached:

			// --- The unit reaches the end of the path, thus stopping and returning to NoState ---
			(*unit)->info.UnitMovementState = MovementState::MovementState_NoState;
			(*unit)->UpdateTile();
			(*unit)->state = IDLE;
			(*unit)->commanded = false;
			break;
		}


		if (stop_iteration)
		{
			stop_iteration = false;
			break;
		}
		unit++;
	}
}