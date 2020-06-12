#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1PathFinding.h"
#include "j1Entity.h"
#include "StaticEntity.h"
#include "brofiler/Brofiler/Brofiler.h"

j1PathFinding::j1PathFinding() : j1Module(), last_path(DEFAULT_PATH_LENGTH), width(150), height(150)
{
	name = ("pathfinding");
	node_map_divisions = 10;
	node_map = CreateNodeMap();
	path_timer.Start();
}

// Destructor
j1PathFinding::~j1PathFinding()
{
	node_map.clear();
}

// Called before quitting
bool j1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	last_path.clear();
	return true;
}

// Sets up the walkability map
void j1PathFinding::SetMap()
{
	for(int y = 0; y < 150; y++)
	{
		for(int x = 0; x < 150; x++)
		{
			map[x][y] = true;
		}
	}
}

// Utility: return true if pos is inside the map boundaries
bool j1PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x < (int)width &&
		pos.y >= 0 && pos.y < (int)height);
}

// Utility: returns true is the tile is walkable
bool j1PathFinding::IsWalkable(const iPoint& pos) const
{
	return map[pos.x][pos.y] && CheckBoundaries(pos);
}

void j1PathFinding::SetTileAsUnwalkable(int tile_x, int tile_y) {
	map[tile_x][tile_y] = false;
}

iPoint j1PathFinding::FindWalkableAdjacentTile(iPoint point) const {
	iPoint tile;

	//north
	tile = { point.x, point.y - 1 };
	if (App->pathfinding->IsWalkable(tile) && !App->entities->IsTileOccupied(tile))
			return tile;

	// west
	tile = { point.x - 1, point.y };
	if (App->pathfinding->IsWalkable(tile) && !App->entities->IsTileOccupied(tile))
		return tile;

	// east
	tile = { point.x + 1, point.y };
	if (App->pathfinding->IsWalkable(tile) && !App->entities->IsTileOccupied(tile))
		return tile;

	// south
	tile = { point.x, point.y + 1 };
	if (App->pathfinding->IsWalkable(tile) && !App->entities->IsTileOccupied(tile))
		return tile;

	//north-west
	tile = { point.x - 1, point.y - 1 };
	if (App->pathfinding->IsWalkable(tile) && !App->entities->IsTileOccupied(tile))
		return tile;

	//north-east
	tile = { point.x + 1, point.y - 1 };
	if (App->pathfinding->IsWalkable(tile) && !App->entities->IsTileOccupied(tile))
		return tile;

	// south-west
	tile = { point.x - 1, point.y + 1 };
	if (App->pathfinding->IsWalkable(tile) && !App->entities->IsTileOccupied(tile))
		return tile;

	// south-east
	tile = { point.x + 1, point.y + 1 };
	if (App->pathfinding->IsWalkable(tile) && !App->entities->IsTileOccupied(tile))
		return tile;

	return { -1,-1 };
}

iPoint j1PathFinding::FindNearestWalkableTile(iPoint origin, iPoint destination) const
{
	if (!IsWalkable(origin))
		origin = FindWalkableAdjacentTile(origin);

	if (!IsWalkable(destination)) {
		ResourceBuilding* reference_resource_building = App->entities->FindResourceBuildingByTile(destination);
		if (reference_resource_building != nullptr)
		{
			destination = App->entities->ClosestTile(origin, reference_resource_building->tiles);
			destination = FindWalkableAdjacentTile(destination);

			if (!IsWalkable(destination)) {
				destination = App->pathfinding->ExpandTile(destination);
			}
		}
		else
		{
			j1Entity* entity;
			entity = App->entities->FindEntityByTile(destination);

			if ((entity != nullptr) && (!entity->is_dynamic))
			{
				StaticEntity* reference_static_entity = dynamic_cast<StaticEntity*>(entity);
				{
					destination = App->entities->ClosestTile(origin, reference_static_entity->tiles);
					destination = App->pathfinding->FindWalkableAdjacentTile(destination);
					if (!IsWalkable(destination))
						LOG("Unwalkable building tile");
				}
			}
			else
			{
				destination = App->pathfinding->ExpandTile(destination);
				if (!IsWalkable(destination))
					LOG("Unwalkable tile 2");
			}
		}
	}

	if (!IsWalkable(destination))
		LOG("Unwalkable destination 3");

	return destination;
}

std::vector<iPoint> j1PathFinding::GetLastPath() const
{
	std::vector<iPoint> vector;
	for(int i = 0; i < last_path.size(); i++)
	{
		vector.push_back(last_path[i]);
	}
	return vector;
}

float j1PathFinding::GetLastPathRequestTime() const {
	return path_timer.Read();
}

//Node map ------------------------------------------------------------------------

std::vector<iPoint> j1PathFinding::CreateNodeMap() {
	std::vector<iPoint> map;
	int distance = 150 / node_map_divisions;

	for(int y = distance; y < 150; y += distance)
	{
		for(int x = distance; x < 150; x += distance)
		{
			map.push_back(iPoint(x, y));
		}
	}
	return map;
}

std::vector<iPoint>j1PathFinding::GetNodeMap() const { return node_map; }

std::vector<iPoint> j1PathFinding::CreateNodePath(iPoint origin, iPoint destination) {
	BROFILER_CATEGORY("CreateNodePath", Profiler::Color::Azure)
	std::vector<iPoint> path;
	int node_distance = GetDistanceBetweenNodes();
	srand(time(NULL));
	//int x_delay = (rand() % 3) - 1;
	//int y_delay = (rand() % 3) - 1;

	iPoint origin_node = node_map[0];
	iPoint destination_node = node_map[0];

	//closest origin node
	for(int i = 0; i < node_map.size(); i++)
	{
		if (node_map[i].DistanceTo(origin) < origin_node.DistanceTo(origin))
			origin_node = node_map[i];
	}

	//closest destination node
	for(int i = 0; i < node_map.size(); i++)
	{
		if (node_map[i].DistanceTo(destination) < destination_node.DistanceTo(destination))
			destination_node = node_map[i];
	}

	iPoint current_node = origin_node;
	path.push_back(current_node);

	//iterate nodes to create the path
	while (current_node != destination_node)
	{
		iPoint possible_node = {0,0};
		iPoint best_node = current_node;
		//find neighbour nodes
		for(int y = -node_distance; y <= node_distance; y += node_distance)
		{
			for(int x = -node_distance; x <= node_distance; x += node_distance)
			{
				possible_node.x = current_node.x + x;
				possible_node.y = current_node.y + y;

				if (possible_node.DistanceTo(destination_node) < current_node.DistanceTo(destination_node)) {
					if (possible_node.DistanceTo(destination_node) < best_node.DistanceTo(destination_node))
						best_node = possible_node;
				}
			}
		}
		current_node = best_node;
		path.push_back(best_node);
	}

	/*
	for (size_t i = 0; i < path.size(); i++)
	{
		path[i].x += x_delay;
		path[i].y += y_delay;
	}
	*/
	//flip final path
	std::reverse(path.begin(), path.end());

	return path;
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
	if (App->pathfinding->IsWalkable(cell) && !App->entities->occupied_tiles[cell.x][cell.y])
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	// south
	cell.create(pos.x, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell) && !App->entities->occupied_tiles[cell.x][cell.y])
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	// east
	cell.create(pos.x + 1, pos.y);
	if (App->pathfinding->IsWalkable(cell) && !App->entities->occupied_tiles[cell.x][cell.y])
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	// west
	cell.create(pos.x - 1, pos.y);
	if (App->pathfinding->IsWalkable(cell) && !App->entities->occupied_tiles[cell.x][cell.y])
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

	if (!IsWalkable(origin)) {
		LOG("Unwalkable origin");
		return -2;
	}

	if ((!IsWalkable(destination))) {
		LOG("Unwalkable destination");
		return -2;
	}


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

	LOG("Impossible path");
	path_timer.Start();

	return -1;
}

iPoint j1PathFinding::ExpandTile(iPoint target_tile) const {

	iPoint pivot = target_tile;
	int max = 1;

	while (!IsWalkable(pivot) && (max < 50))
	{
		for(int y = -max; y <= max; y++)
		{
			for(int x = -max; x <= max; x++)
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

int j1PathFinding::GetDistanceBetweenNodes() const {
	return 150 / node_map_divisions;
}