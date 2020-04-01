#ifndef __PATHFINDER_H__
#define __PATHFINDER_H__
#include "p2Point.h"
#include <vector>

#pragma region Structs

struct PathVector;

struct PathNode
{
	// Convenient constructors
	PathNode();
	PathNode(int g, int h, const iPoint& pos, const PathNode* parent);
	PathNode(const PathNode& node);

	// Fills a list (PathList) of all valid adjacent pathnodes
	uint FindWalkableAdjacents(PathVector& list_to_fill) const;
	// Calculates this tile score
	int Score() const;
	// Calculate the F for a specific destination tile
	int CalculateF(const iPoint& destination);

	// -----------
	int g;
	int h;
	iPoint pos;
	const PathNode* parent; // needed to reconstruct the path in the end
};

// ---------------------------------------------------------------------
// Helper struct to include a list of path nodes
// ---------------------------------------------------------------------
struct PathVector
{
	// -----------
	// The vector itself, note they are not pointers!
	std::vector<PathNode> vector;

	// Looks for a node in this list and returns it's list node or NULL
	int Find(const iPoint& point) const;

	// Returns the Pathnode with lowest score in this list or NULL if empty
	int GetNodeLowestScore() const;

};
#pragma endregion

class PathFinder
{
public:
	PathFinder();
	~PathFinder();

	// Main function to request a path from A to B
	void PreparePath(const iPoint& origin, const iPoint& destination);
	bool IteratePath();

	// To request all tiles involved in the last generated path
	void GetLastPath(std::vector<iPoint>& vector) const;
	
	std::vector<iPoint> last_path;

	bool Update();

	bool initSuccessful;
	bool pathCompleted;
	bool available;

private:

	PathVector open;
	PathVector close;

	iPoint origin;
	iPoint destination;

	int max_iterations;

	// we store the created path here
};

#endif // !__PATHFINDER_H__

