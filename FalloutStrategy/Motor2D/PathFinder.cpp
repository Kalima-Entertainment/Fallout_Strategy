#include "PathFinder.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Pathfinding.h"
#include <algorithm>

PathFinder::PathFinder() : last_path(DEFAULT_PATH_LENGTH), initSuccessful(false), pathCompleted(false),max_iterations(50),available(true)
{
	LOG("PathFinder created");
}

PathFinder::~PathFinder()
{
}

void PathFinder::PreparePath(const iPoint& origin, const iPoint& destination)
{
	// Add the origin tile to open
	if (open.GetNodeLowestScore() == -1)
		open.vector.push_back(PathNode(0, origin.DistanceTo(destination), origin, nullptr));

	uint iterations = 0;

	this->origin = origin;
	this->destination = destination;
	initSuccessful = true;
	available = false;
	
}

bool PathFinder::IteratePath()
{
	bool ret = true;
	PathNode* node = new PathNode(open.vector[open.GetNodeLowestScore()]);
	close.vector.push_back(*node);
	open.vector.erase(open.vector.cbegin() + open.Find(node->pos));
	

	if (node->pos == destination) {
		const PathNode* iterator = node;

		last_path.clear();
		// Backtrack to create the final path
		for (iterator; iterator->pos != origin; iterator = iterator->parent)
		{
			last_path.push_back(iterator->pos);
		}

		last_path.push_back(origin);

		// Use the Pathnode::parent and Flip() the path when you are done
		std::reverse(last_path.begin(), last_path.end());
		pathCompleted = true;
		initSuccessful = false;
		available = true;
		open.vector.clear();
		close.vector.clear();
		RELEASE(node);
		return false;
	}

	PathVector adjacentNodes;
	uint numNodes = node->FindWalkableAdjacents(adjacentNodes);


	for (uint i = 0; i < numNodes; i++)
	{
		// ignore nodes in the closed list
		if (close.Find(adjacentNodes.vector[i].pos) == -1) {
			// If it is NOT found, calculate its F and add it to the open list
			if (open.Find(adjacentNodes.vector[i].pos) == -1) {
				adjacentNodes.vector[i].CalculateF(destination);
				open.vector.push_back(adjacentNodes.vector[i]);
			}
			// If it is already in the open list, check if it is a better path (compare G)
			else {
				//open[open.Find(adjacentNodes.vector[i].pos)]
				if (adjacentNodes.vector[i].g < open.vector[open.Find(adjacentNodes.vector[i].pos)].g) {
					// If it is a better path, Update the parent
					adjacentNodes.vector[i].CalculateF(destination);
					open.vector.erase(open.vector.begin() + i);
					open.vector.push_back(adjacentNodes.vector[i]);
				}
			}
		}
	}
	return ret;
}


// To request all tiles involved in the last generated path
const std::vector<iPoint> PathFinder::GetLastPath(std::vector<iPoint> vector_to_fill) const
{
	vector_to_fill.clear();

	for (int i = 0; i < last_path.size(); i++)
	{
		vector_to_fill.push_back(last_path[i]);
	}

	return last_path;
}

bool PathFinder::Update()
{	
	bool ret = true;
	for (int i = 0; i < max_iterations && ret; i++)
	{
		 ret = IteratePath();
	}

	return ret;
}


#pragma region PathList



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

	/*
	if (vector.size() != 0) {
		for (std::vector<PathNode>::const_iterator* item = &vector.end(); item != &vector.begin(); item--) {
			if (item->_Ptr->Score() < min)
			{
				min = item->_Ptr->Score();
				ret = item;
			}
		}
	}
	*/

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
// Fills a list (PathList) of all valid adjacent pathnodes
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

