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
	for (size_t i = 0; i < Groups.size(); i++)
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
	else {
		delete group;
		group = nullptr;
		return group;
	}

}

void j1MovementManager::Move(j1Group* group, iPoint goal_path, float dt)
{
	std::vector <DynamicEntity*>::const_iterator unit = group->Units.cbegin();

	// -- We will just check units inside group
	while (unit != group->Units.cend())
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
		}
		unit++;
	}
}
