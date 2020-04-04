#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1PathFinding.h"

j1PathFinding::j1PathFinding() : j1Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH), width(0), height(0)
{
	name.assign("pathfinding");
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

// To request all tiles involved in the last generated path
const std::vector<iPoint>* j1PathFinding::GetLastPath() const
{
	return &last_path;
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
		item++;
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

	std::list <PathNode>::const_reverse_iterator item = list.rbegin();

	while (item != list.rend())
	{
		if ((*item).Score() < min)
		{
			min = (*item).Score();
			ret = &(*item);
		}
		item++;
	}
	return ret;
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(float g, float h, const iPoint& pos, const PathNode* parent, const bool diagonal = false) : g(g), h(h), pos(pos), parent(parent), diagonal(diagonal)
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

	// north east
	cell.create(pos.x + 1, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this, true));

	// north west
	cell.create(pos.x - 1, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this, true));

	// south
	cell.create(pos.x, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));


	// south east
	cell.create(pos.x + 1, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this, true));

	// south west
	cell.create(pos.x - 1, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this, true));

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
	if (this->diagonal)
		g = parent->g + 1.7f;
	h = pos.DistanceNoSqrt(destination);

	return g + h;
}

// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------
int j1PathFinding::CreatePath(const iPoint& origin, const iPoint& destination)
{
	//if origin or destination are not walkable, return -1

	int ret = 1;

	// --- We ensure that origin nor destination are not walkable tiles and that they are not the same ---
	if (/*IsWalkable(origin) == false
		||*/ IsWalkable(destination) == false
		|| origin == destination)
	{
		ret = -1;
		return ret;
	}

	// --- We declare the lists needed to create the path ---
	PathList open;
	PathList closed;
	PathList adjacent;

	// --- We add the origin to the open/frontier list ---
	open.list.push_back(PathNode(0, 0, origin, NULL));


	// --- We iterate until no element is left in frontier/open list ---
	while (open.list.size() > 0)
	{
		// --- We delete the lowest score tile from open and move it to closed, frontier-->visited ---
		PathNode* lowest = (PathNode*)open.GetNodeLowestScore();
		closed.list.push_back(*lowest);

		std::list <PathNode>::iterator it = open.list.begin();

		while (it != open.list.end())
		{
			if (&(*it) == &(*lowest))
			{
				open.list.erase(it);
				break;
			}
			it++;
		}

		//// --- If we find the destination we do backtracking and create the path, breaking the loop ---
		if (closed.list.back().pos == destination)
		{
			last_path.clear();
			// Backtrack to create the final path

			for (PathNode it = closed.list.back(); it.parent != NULL; it = *closed.Find(it.parent->pos))
			{
				last_path.push_back(it.pos);
			}

			last_path.push_back(closed.list.front().pos);

			std::reverse(last_path.begin(), last_path.end());
			break;
		}

		// --- If we have not found the destination we proceed to find the current tile's neighbors ---

		// Fill a list with all adjacent nodes
		adjacent.list.clear();
		closed.list.back().FindWalkableAdjacents(adjacent);

		/*p2List_item<PathNode>* item = adjacent.list.start;*/
		std::list<PathNode>::iterator item = adjacent.list.begin();

		// --- While we have neighbors left to check ... iterate! ---
		for (; item != adjacent.list.end(); item++)
		{
			//If we find it in the closed list we do not want to continue
			if (closed.Find((*item).pos) == NULL)
			{
				PathNode adjacent_in_open;

				// --- If we do not find the neighbor in the open/frontier list we calculate its cost (F) and add it to the list ---
				if (open.Find((*item).pos) == NULL)
				{
					(*item).CalculateF(destination);
					open.list.push_back(*item);
				}

				// --- If we have found it in open/frontier list ... ---
				else
				{
					adjacent_in_open = *open.Find((*item).pos);
					// --- We compare walkable cost and if the neighbor's is bigger than current + offset we know this is a better path and update neighbor's parent and F ---

					if (adjacent_in_open.diagonal == false)
					{
						if (adjacent_in_open.g > (*item).g + 1)
						{
							adjacent_in_open.parent = (*item).parent;
							adjacent_in_open.CalculateF(destination);
						}
					}
				}
			}
		}

	}

	return ret;
}