#ifndef __j1PATHFINDING_H__
#define __j1PATHFINDING_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1Timer.h"
#include <list>

#define DEFAULT_PATH_LENGTH 50
#define INVALID_WALK_CODE 255
#define MAX_PATH_ITERATIONS 350

class j1PathFinding : public j1Module
{
public:

	j1PathFinding();

	// Destructor
	~j1PathFinding();

	// Called before quitting
	bool CleanUp();

	// Sets up the walkability map
	void SetMap();

	// Main function to request a path from A to B
	int CreatePath(const iPoint& origin, const iPoint& destination);

	// To request all tiles involved in the last generated path
	std::vector<iPoint> GetLastPath() const;

	// Utility: return true if pos is inside the map boundaries
	bool CheckBoundaries(const iPoint& pos) const;

	// Utility: returns true is the tile is walkable
	bool IsWalkable(const iPoint& pos) const;

	void SetTileAsUnwalkable(int tile_x, int tile_y);

	iPoint FindNearestWalkableTile(iPoint current_tile, iPoint target_tile) const;
	iPoint ExpandTile(iPoint target_tile) const;
	iPoint FindWalkableAdjacentTile(iPoint point) const; 

	std::vector<iPoint> CreateNodeMap();
	std::vector<iPoint> GetNodeMap() const;
	int GetDistanceBetweenNodes() const { return 150 / node_map_divisions; }
	std::vector<iPoint> CreateNodePath(iPoint origin, iPoint destination);

	float GetLastPathRequestTime() const;

public:
	bool show_nodes;

private:
	// size of the map
	uint width = 0u;
	uint height = 0u;
	bool map[150][150];
	std::vector<iPoint> last_path;
	j1Timer path_timer;
	//node map
	std::vector<iPoint> node_map;
	int node_map_divisions;
};

// forward declaration
struct PathList;

// ---------------------------------------------------------------------
// Pathnode: Helper struct to represent a node in the path creation
// ---------------------------------------------------------------------
struct PathNode
{
	// Convenient constructors
	PathNode();
	PathNode(int g, int h, const iPoint& pos, const PathNode* parent);
	PathNode(const PathNode& node);

	// Fills a list (PathList) of all valid adjacent pathnodes
	uint FindWalkableAdjacents(PathList& list_to_fill) const;
	// Calculates this tile score
	int Score() const;
	// Calculate the F for a specific destination tile
	int CalculateF(const iPoint& destination);

	// -----------
	int g = 0;
	int h = 0;
	iPoint pos = iPoint({ 0, 0 });
	const PathNode* parent = nullptr; // needed to reconstruct the path in the end
};

// ---------------------------------------------------------------------
// Helper struct to include a list of path nodes
// ---------------------------------------------------------------------
struct PathList
{
	// Looks for a node in this list and returns it's list node or NULL
	const PathNode* Find(const iPoint& point) const;

	// Returns the Pathnode with lowest score in this list or NULL if empty
	const PathNode* GetNodeLowestScore() const;

	// -----------
	// The list itself, note they are not pointers!
	std::list<PathNode> list;
};



#endif // __j1PATHFINDING_H__