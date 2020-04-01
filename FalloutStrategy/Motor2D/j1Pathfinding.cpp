#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1PathFinding.h"


j1PathFinding::j1PathFinding() : j1Module(), map(NULL),width(0), height(0)
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
	map = new uchar[width*height];
	memcpy(map, data, width*height);
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
	if(CheckBoundaries(pos))
		return map[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}

// To request all tiles involved in the last generated path
void j1PathFinding::GetLastPath(std::vector<iPoint>& vector_to_fill) const
{
	vector_to_fill.clear();
	for (int i = 0; i < last_path.size(); i++)
	{
		vector_to_fill.push_back(last_path[i]);
	}
}

void j1PathFinding::CreatePath(const iPoint& origin, const iPoint& destination)
{
	//BROFILER_CATEGORY("CreatePath", Profiler::Color::Azure)

	if ((!IsWalkable(origin)) || (!IsWalkable(destination)))
	{
		return;
	}
	last_path.clear();

	PathVector open, close;
	PathNode node(0, origin.DistanceNoSqrt(destination), origin, nullptr);

	node.pos = origin;
	open.vector.push_back(node);

	while ((open.vector.size() > 0) && (close.vector.size() < MAX_PATH_ITERATIONS)) {
		PathNode* item;

		item = &open.vector[open.GetNodeLowestScore()];
		close.vector.push_back(*item);
		open.vector.erase(open.vector.begin() + open.GetNodeLowestScore());

		if (item->pos != destination)
		{
			PathVector adjacentSquares;
			close.vector.back().FindWalkableAdjacents(adjacentSquares);

			for (int i = 0; i < adjacentSquares.vector.size(); i++)
			{
				if (close.Find(adjacentSquares.vector[i].pos) != -1)
				{
					continue;
				}
				else if (open.Find(adjacentSquares.vector[i].pos) != -1)
				{
					PathNode probable_path = open.vector[open.Find(adjacentSquares.vector[i].pos)];
					adjacentSquares.vector[i].CalculateF(destination);
					if (probable_path.g > adjacentSquares.vector[i].g)
						probable_path.parent = adjacentSquares.vector[i].parent;
				}
				else
				{
					adjacentSquares.vector[i].CalculateF(destination);
					open.vector.push_back(adjacentSquares.vector[i]);
				}
			}
			adjacentSquares.vector.clear();
		}
		else
		{
			PathNode item = close.vector.back();

			for (item; item.parent != nullptr; item = close.vector[close.Find(item.parent->pos)])
			{
				last_path.push_back(item.pos);
				if (item.parent == nullptr) {
					last_path.push_back(close.vector.front().pos);
				}
			}

			std::reverse(last_path.begin(), last_path.end());
			return;
		}
	}
}


#pragma region PathVector

// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
// ---------------------------------------------------------------------------------
int PathVector::Find(const iPoint& point) const
{
	if (vector.size() > 0)
	{
		PathNode item = vector.at(0);
		for (int i = 0; i < vector.size(); i++)
		{
			if (vector[i].pos == point) {
				return i;
			}
		}
	}
	return -1;
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
int PathVector::GetNodeLowestScore() const
{
	int ret = -1;
	int min = 65535;

	for (int i = 0; i < vector.size(); i++)
	{
		if (vector[i].Score() < min) {
			min = vector[i].Score();
			ret = i;
		}
	}

	return ret;
}
#pragma endregion

#pragma region PathNode

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
// Fills a vector (PathVector) of all valid adjacent pathnodes
// ----------------------------------------------------------------------------------
uint PathNode::FindWalkableAdjacents(PathVector& vector_to_fill) const
{
	iPoint cell;
	uint before = vector_to_fill.vector.size();

	// north
	cell.create(pos.x, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		vector_to_fill.vector.push_back(PathNode(-1, -1, cell, this));

	// south
	cell.create(pos.x, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		vector_to_fill.vector.push_back(PathNode(-1, -1, cell, this));

	// east
	cell.create(pos.x + 1, pos.y);
	if (App->pathfinding->IsWalkable(cell))
		vector_to_fill.vector.push_back(PathNode(-1, -1, cell, this));

	// west
	cell.create(pos.x - 1, pos.y);
	if (App->pathfinding->IsWalkable(cell))
		vector_to_fill.vector.push_back(PathNode(-1, -1, cell, this));

	return vector_to_fill.vector.size();
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
	h = pos.DistanceManhattan(destination);

	return g + h;
}
#pragma endregion
// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------

