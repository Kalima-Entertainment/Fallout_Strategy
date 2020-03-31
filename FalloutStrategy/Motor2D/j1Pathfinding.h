#ifndef __j1PATHFINDING_H__
#define __j1PATHFINDING_H__

#include "j1Module.h"
#include "p2Point.h"

#include "j1Timer.h"
#include "PathFinder.h"

#include <vector>

#define DEFAULT_PATH_LENGTH 50
#define INVALID_WALK_CODE 255


class j1PathFinding : public j1Module
{
public:

	j1PathFinding();

	// Destructor
	~j1PathFinding();

	// Called before quitting
	bool CleanUp();

	// Sets up the walkability map
	void SetMap(uint width, uint height, uchar* data);

	// Utility: return true if pos is inside the map boundaries
	bool CheckBoundaries(const iPoint& pos) const;

	// Utility: returns true is the tile is walkable
	bool IsWalkable(const iPoint& pos) const;

	// Utility: return the walkability value of a tile
	uchar GetTileAt(const iPoint& pos) const;

	void RequestPath(const iPoint& origin, const iPoint& destination);

	bool Start() override;

	bool Update(float dt) override;

	j1Timer timer;

	std::vector<PathFinder*> pathfinderList;

private:
	uint width;
	uint height;
	uchar* map;

	bool requestPath;
};


#endif // __j1PATHFINDING_H__