#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1PathFinding.h"
#include "j1Entity.h"
#include "StaticEntity.h"
#include "brofiler/Brofiler/Brofiler.h"

j1PathFinding::j1PathFinding() : j1Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH), width(0), height(0)
{
	name = ("pathfinding");
	path_timer.Start();
}

// Destructor
j1PathFinding::~j1PathFinding()
{
	RELEASE_ARRAY(map);
}

// Called before quitting
bool j1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	last_path.clear();
	RELEASE_ARRAY(map);
	return true;
}

// Sets up the walkability map
void j1PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width * height];
	memcpy(map, data, width * height);
}

// Utility: return true if pos is inside the map boundaries
bool j1PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
		pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool j1PathFinding::IsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}

// Utility: return the walkability value of a tile
uchar j1PathFinding::GetTileAt(const iPoint& pos) const
{
	if (CheckBoundaries(pos))
		return map[(pos.y * width) + pos.x];

	return INVALID_WALK_CODE;
}

iPoint j1PathFinding::FindWalkableAdjacentTile(iPoint point) const {
	iPoint tile;
	
	//north
	tile = { point.x, point.y - 1 };
	if (App->pathfinding->IsWalkable(tile))
			return tile;

	// south
	tile = { point.x, point.y + 1 };
	if (App->pathfinding->IsWalkable(tile))
		return tile;

	// east
	tile = { point.x + 1, point.y };
	if (App->pathfinding->IsWalkable(tile))
		return tile;

	// west
	tile = { point.x - 1, point.y };
	if (App->pathfinding->IsWalkable(tile))
		return tile;

	return { -1,-1 };
}

iPoint j1PathFinding::FindNearestWalkableTile(iPoint origin, iPoint destination) const
{
	//if there is a resource building 
	ResourceBuilding* reference_resource_building = App->entities->FindResourceBuildingByTile(destination);
	if (reference_resource_building != nullptr)
	{
		destination = App->entities->ClosestTile(origin, reference_resource_building->tiles);
		destination = FindWalkableAdjacentTile(destination);
		if (!IsWalkable(destination))
			LOG("Resource building");
	}
	else
	{
		j1Entity* entity;
		entity = App->entities->FindEntityByTile(destination);

		if ((entity != nullptr) && (!entity->is_dynamic))
		{
			StaticEntity* reference_static_entity = (StaticEntity*)App->entities->FindEntityByTile(destination);
			{
				destination = App->entities->ClosestTile(origin, reference_static_entity->tiles);
				destination = App->pathfinding->FindWalkableAdjacentTile(destination);
			}
		}
		else
		{
			destination = App->pathfinding->ExpandTile(destination);
		}
	}

	if (!IsWalkable(destination))
		LOG("Unwalkable");

	return destination;
}

std::vector<iPoint> j1PathFinding::GetLastPath() const
{
	std::vector<iPoint> vector;
	for (int i = 0; i < last_path.size(); i++)
	{
		vector.push_back(last_path[i]);
	}
	return vector;
}

float j1PathFinding::GetLastPathRequestTime() const {
	return path_timer.Read();
}

// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
// ---------------------------------------------------------------------------------
const PathNode* PathList::Find(const iPoint& point) const
{
	std::list<PathNode>::const_iterator item = list.begin();
	while (item != list.end())
	{
		if ((*item).pos == point)
			return &(*item);
		++item;
	}
	return NULL;
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
const PathNode* PathList::GetNodeLowestScore() const
{
	const PathNode* ret = NULL;
	int min = 65535;

	std::list<PathNode>::const_reverse_iterator item = list.rbegin();
	while (item != list.rend())
	{
		if ((*item).Score() < min)
		{
			min = (*item).Score();
			ret = &(*item);
		}
		++item;
	}
	return ret;
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

// PathNode -------------------------------------------------------------------------
// Fills a list (PathList) of all valid adjacent pathnodes
// ----------------------------------------------------------------------------------
uint PathNode::FindWalkableAdjacents(PathList& list_to_fill) const
{
	iPoint cell;
	uint before = list_to_fill.list.size();

	// north
	cell.create(pos.x, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	// south
	cell.create(pos.x, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	// east
	cell.create(pos.x + 1, pos.y);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	// west
	cell.create(pos.x - 1, pos.y);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	return list_to_fill.list.size();
}

// PathNode -------------------------------------------------------------------------
// Calculates this tile score
// ----------------------------------------------------------------------------------
int PathNode::Score() const
{
	return g + h;
}

// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
// ----------------------------------------------------------------------------------
int PathNode::CalculateF(const iPoint& destination)
{
	g = parent->g + 1;
	h = pos.DistanceTo(destination);

	return g + h;
}

// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------
int j1PathFinding::CreatePath(const iPoint& origin, const iPoint& destination)
{
	BROFILER_CATEGORY("CreatePath", Profiler::Color::Azure)

	last_path.clear();

	if ((!IsWalkable(origin)) || (!IsWalkable(destination))) 
		return -1;

	PathList open;
	PathList close;

	PathNode node(0, origin.DistanceNoSqrt(destination), origin, nullptr);

	open.list.push_back(node);

	while ((open.list.size() != 0)&&(close.list.size() < MAX_PATH_ITERATIONS))
	{
		const PathNode* current_node = open.GetNodeLowestScore();
		close.list.push_back(*current_node);

		if (current_node->pos == destination)
		{
			const PathNode* path_item = nullptr;

			for (path_item = &(*current_node); path_item->pos != origin; path_item = path_item->parent) {
				last_path.push_back(path_item->pos);
			}

			last_path.push_back(path_item->pos);
			std::reverse(last_path.begin(), last_path.end());
			
			path_timer.Start();
			return last_path.size();
		}

		std::list<PathNode>::iterator it = open.list.begin();
		while (it != open.list.end()) {

			if (&(*it) == &(*current_node))
				break;
			++it;
		}
		open.list.erase(it);

		PathList childs;
		close.list.back().FindWalkableAdjacents(childs);
			
		std::list<PathNode>::iterator child_node = childs.list.begin();
		while (child_node != childs.list.end())
		{
			if (!close.Find((*child_node).pos) != NULL)	
			{
				(*child_node).CalculateF(destination); 
				if (open.Find((*child_node).pos) != NULL) 
				{
					if ((*open.Find((*child_node).pos)).g > (*child_node).g)
					{
						PathNode old_node = *open.Find((*child_node).pos);
						old_node.parent = (*child_node).parent;
					}
				}
				else open.list.push_back(*child_node);
			}
			++child_node;
		}
	}

	path_timer.Start();

	return -1;
}

iPoint j1PathFinding::ExpandTile(iPoint target_tile) const {

	iPoint pivot = target_tile;
	int max = 1;
	
	while (!IsWalkable(pivot) && (max < 50))
	{
		for (int y = -max; y < max; y++)
		{
			for (int x = -max; x < max; x++)
			{
				pivot.x = target_tile.x + x;
				pivot.y = target_tile.y + y;

				if (IsWalkable(pivot)) 
					return pivot;
			}
		}
		max++;
	}
	return pivot;
}