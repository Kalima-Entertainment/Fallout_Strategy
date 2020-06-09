#include "j1Entity.h"
#include "j1Group.h"
#include "j1MovementManager.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Pathfinding.h"
#include "DynamicEntity.h"

j1Group::j1Group() {
	last_goal.x = 0;
	last_goal.y = 0;
}

j1Group::~j1Group()
{
}

void j1Group::addUnit(DynamicEntity* unit_toadd)
{
	Units.push_back(unit_toadd);
}

void j1Group::removeUnit(DynamicEntity* unit_toremove)
{
	for (int i = 0; i < Units.size(); i++)
	{
		if (unit_toremove == Units[i])
			Units.erase(Units.cbegin() + i);
	}
}

void j1Group::AddTiletoOccupied(iPoint to_add)
{
	iPoint* new_tile = new iPoint;
	*new_tile = to_add;

	Occupied_tiles.push_back(new_tile);
}

void j1Group::ClearGroup()
{
	for (int i = 0; i < Units.size(); i++)
	{
		// -- If we have any unit info inside the vector we ensure that will be cleaned.
		if (Units[i] != nullptr) {
			Units[i]->info.IsSelected = false;
			Units[i]->info.current_group = nullptr;
		}
	}
	Units.clear();
}

void j1Group::ClearOccupiedlist()
{
	Occupied_tiles.clear();
}

void j1Group::DeselectGroup() {
	for (int i = 0; i < Units.size(); i++)
	{
		Units[i]->info.IsSelected = false;
	}
}

int j1Group::GetSize()
{
	return Units.size();
}

void j1Group::CheckForMovementRequest(iPoint destiny, float dt)
{
	App->Mmanager->Move(this, destiny, dt);
}

bool j1Group::IsGroupLead(DynamicEntity* entity)
{
	return (*Units.begin()) == entity;
}

void j1Group::SetUnitGoalTile(DynamicEntity* entity)
{
	std::vector<DynamicEntity*>::const_iterator it = Units.cbegin();
	bool Goal_found = false;

	while (it != Units.cend())
	{
		if (*it == entity)
		{
			++it;
			continue;
		}

		Goal_found = FindFreeAdjacents(&(*it)->info.goal_tile);

		if (Goal_found)
		{
			entity->info.goal_tile = last_goal;
			entity->target_tile = last_goal;
			break;
		}

		++it;
	}
}

bool j1Group::FindFreeAdjacents(iPoint* base_tile)
{
	bool ret = false;

	// --- Check if any adjacent tile to the base_tile is free ---

	iPoint cell;

	// north
	cell.create(base_tile->x, base_tile->y + 1);
	if (App->pathfinding->IsWalkable(cell) && IsTileFree(&cell))
	{
		ret = true;
		last_goal = cell;
		return ret;
	}

	// north east
	cell.create(base_tile->x + 1, base_tile->y + 1);
	if (App->pathfinding->IsWalkable(cell) && IsTileFree(&cell))
	{
		ret = true;
		last_goal = cell;
		return ret;
	}

	// north west
	cell.create(base_tile->x - 1, base_tile->y + 1);
	if (App->pathfinding->IsWalkable(cell) && IsTileFree(&cell))
	{
		ret = true;
		last_goal = cell;
		return ret;
	}

	// south
	cell.create(base_tile->x, base_tile->y - 1);
	if (App->pathfinding->IsWalkable(cell) && IsTileFree(&cell))
	{
		ret = true;
		last_goal = cell;
		return ret;
	}

	// south east
	cell.create(base_tile->x + 1, base_tile->y - 1);
	if (App->pathfinding->IsWalkable(cell) && IsTileFree(&cell))
	{
		ret = true;
		last_goal = cell;
		return ret;
	}

	// south west
	cell.create(base_tile->x - 1, base_tile->y - 1);
	if (App->pathfinding->IsWalkable(cell) && IsTileFree(&cell))
	{
		ret = true;
		last_goal = cell;
		return ret;
	}

	// east
	cell.create(base_tile->x + 1, base_tile->y);
	if (App->pathfinding->IsWalkable(cell) && IsTileFree(&cell))
	{
		ret = true;
		last_goal = cell;
		return ret;
	}

	// west
	cell.create(base_tile->x - 1, base_tile->y);
	if (App->pathfinding->IsWalkable(cell) && IsTileFree(&cell))
	{
		ret = true;
		last_goal = cell;
		return ret;
	}


	return ret;
}

bool j1Group::IsTileFree(iPoint* adjacent)
{
	std::list <iPoint*>::const_iterator it = Occupied_tiles.begin();

	while (it != Occupied_tiles.end())
	{
		if (adjacent->x == (*it)->x && adjacent->y == (*it)->y)
		{
			return false;
		}

		++it;
	}

	AddTiletoOccupied(*adjacent);

	return true;
}
