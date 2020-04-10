#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1PathFinding.h"
//#include "brofiler/Brofiler/Brofiler.h"

j1PathFinding::j1PathFinding() : j1Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH), width(0), height(0)
{
	name = ("pathfinding");
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

// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
// ---------------------------------------------------------------------------------
p2List_item<PathNode>* PathList::Find(const iPoint& point) const
{
	p2List_item<PathNode>* item = list.start;
	while (item)
	{
		if (item->data.pos == point)
			return item;
		item = item->next;
	}
	return NULL;
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
p2List_item<PathNode>* PathList::GetNodeLowestScore() const
{
	p2List_item<PathNode>* ret = NULL;
	int min = 65535;

	p2List_item<PathNode>* item = list.end;
	while (item)
	{
		if (item->data.Score() < min)
		{
			min = item->data.Score();
			ret = item;
		}
		item = item->prev;
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
	uint before = list_to_fill.list.count();

	// north
	cell.create(pos.x, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// south
	cell.create(pos.x, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// east
	cell.create(pos.x + 1, pos.y);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// west
	cell.create(pos.x - 1, pos.y);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	return list_to_fill.list.count();
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
	//BROFILER_CATEGORY("CreatePath", Profiler::Color::Azure)

	if ((!CheckBoundaries(origin)) || (!CheckBoundaries(destination)))
	{
		return -1;
	}
	
	last_path.clear();

	PathList open, close;
	PathNode node(0, origin.DistanceNoSqrt(destination), origin, nullptr);

	node.pos = origin;
	open.list.add(node);

	while (open.list.count() > 0) {
		// && (close.list.count() < MAX_PATH_ITERATIONS)) 
		p2List_item<PathNode>* item;

		item = open.GetNodeLowestScore();
		close.list.add(item->data);
		open.list.del(item);

		if (item->data.pos != destination)
		{
			PathList adjacentSquares;
			close.list.end->data.FindWalkableAdjacents(adjacentSquares);

			for (p2List_item<PathNode>* node_item = adjacentSquares.list.start; node_item != NULL; node_item = node_item->next)
			{
				if (close.Find(node_item->data.pos))
				{
					continue;
				}
				else if (open.Find(node_item->data.pos))
				{
					PathNode probable_path = open.Find(node_item->data.pos)->data;
					node_item->data.CalculateF(destination);
					if (probable_path.g > node_item->data.g)
						probable_path.parent = node_item->data.parent;
				}
				else
				{
					node_item->data.CalculateF(destination);
					open.list.add(node_item->data);
				}
			}
			adjacentSquares.list.clear();
		}
		else
		{
			p2List_item<PathNode>* path_item = close.list.end;

			for (; path_item->data.parent != nullptr; path_item = close.Find(path_item->data.parent->pos))
			{

				last_path.push_back(path_item->data.pos);
				if (path_item->data.parent == nullptr) {
					last_path.push_back(close.list.start->data.pos);
				}
			}

			std::reverse(last_path.begin(), last_path.end());
			return last_path.size();
		}
	}
	return -1;
}