#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1PathFinding.h"
//#include "brofiler/Brofiler/Brofiler.h"

j1PathFinding::j1PathFinding() : j1Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH),width(0), height(0)
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
const std::vector<iPoint>* j1PathFinding::GetLastPath() const
{
	return &last_path;
}

// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
// ---------------------------------------------------------------------------------
std::vector<PathNode>::const_iterator PathVector::Find(const iPoint& point) const
{
	std::vector<PathNode>::const_iterator item = vector.begin();

	for (item; item != vector.end(); item++)
	{
		if (item._Ptr->pos == point)
			return item;
	}
	item._Ptr = NULL;
	return item;
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
std::vector<PathNode>::const_iterator PathVector::GetNodeLowestScore() const
{
	std::vector<PathNode>::const_iterator ret;
	ret._Ptr = NULL;
	int min = 65535;

	if (vector.size() == 1)
		return vector.begin();

	std::vector<PathNode>::const_iterator item = vector.begin();

	for (item; item != vector.end(); item++)
	{
		if(item._Ptr->Score() < min)
		{
			min = item._Ptr->Score();
			ret = item;
		}
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
uint PathNode::FindWalkableAdjacents(PathVector& vector_to_fill) const
{
	iPoint cell;
	uint before = vector_to_fill.vector.size();

	// north
	cell.create(pos.x, pos.y + 1);
	if(App->pathfinding->IsWalkable(cell))
		vector_to_fill.vector.push_back(PathNode(-1, -1, cell, this));

	// south
	cell.create(pos.x, pos.y - 1);
	if(App->pathfinding->IsWalkable(cell))
		vector_to_fill.vector.push_back(PathNode(-1, -1, cell, this));

	// east
	cell.create(pos.x + 1, pos.y);
	if(App->pathfinding->IsWalkable(cell))
		vector_to_fill.vector.push_back(PathNode(-1, -1, cell, this));

	// west
	cell.create(pos.x - 1, pos.y);
	if(App->pathfinding->IsWalkable(cell))
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
int PathNode::CalculateF(const iPoint& origin, const iPoint& destination)
{
	g = pos.DistanceTo(origin);
	h = pos.DistanceTo(destination);

	return g + h;
}

// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------
int j1PathFinding::CreatePath(const iPoint& origin, const iPoint& destination)
{
	//BROFILER_CATEGORY("CreatePath", Profiler::Color::Azure)

	if ((!IsWalkable(origin))||(!IsWalkable(destination)))
	{
		return -1;
	}
	last_path.clear();
	
	PathVector open, close;
	PathNode node(0,origin.DistanceNoSqrt(destination),origin, nullptr);

	node.pos = origin;
	open.vector.push_back(node);
	int iterations = 0;

	while ((open.vector.size() > 0) && (iterations < MAX_PATH_ITERATIONS)){
		
		std::vector<PathNode>::const_iterator item;

		item = open.GetNodeLowestScore();
		close.vector.push_back(*item._Ptr);
		open.vector.erase(item);

		if ((close.Find(destination)._Ptr) != NULL) {
			break;
		}
		else {
			PathVector adjacentSquares;
			close.vector.back().FindWalkableAdjacents(adjacentSquares);
			
			for (std::vector<PathNode>::iterator node_item = adjacentSquares.vector.begin(); node_item != adjacentSquares.vector.end(); ++node_item)
			{
				//if it's in the closed list
				if (close.Find(node_item->pos)._Ptr != NULL)
				{
					continue;
				}
				//if it's not in the open list
				if (open.Find(node_item->pos)._Ptr == NULL)
				{
					node_item._Ptr->CalculateF(origin, destination);
					open.vector.push_back(*node_item);
				}
				else
				{
					PathNode probable_path = *open.Find(node_item->pos);
					node_item._Ptr->CalculateF(origin, destination);
					if (probable_path.g > node_item._Ptr->g);
						probable_path.parent = node_item._Ptr->parent;
				}
			}
			adjacentSquares.vector.clear();
		}
		iterations++;
		LOG("Path iterations %i", iterations);
	}

	//create final path -------------------
	last_path.clear();
	PathNode* path_item = &close.vector.back();
 	for (path_item; path_item->parent != nullptr; path_item = (PathNode*)path_item->parent)
	{
		last_path.push_back(path_item->pos);
		if (path_item->parent == nullptr) {
			last_path.push_back(close.vector.begin()->pos);
			break;
		}
	}
	std::reverse(last_path.begin(), last_path.end());
	return last_path.size();
	LOG("Path completed");
	//-------------------------------------

	return -1;
}

