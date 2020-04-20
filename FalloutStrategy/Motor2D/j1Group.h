#ifndef __j1Group_H__
#define __j1Group_H__

#include <list>
#include "p2Point.h"

class j1Entity;
enum class MOV_STATE;

class j1Group
{
public:
	friend class j1MovementManager;

	j1Group();
	~j1Group();

	// --- Add/Remove ---
	void addUnit(j1Entity* unit_toadd);
	void removeUnit(j1Entity* unit_toremove);
	void AddTiletoOccupied(iPoint to_add);

	// --- Clear Stuff ---
	void ClearGroup();
	void ClearOccupiedlist();

	// --- Getters ---
	int GetSize();

	// --- Check Movement Request ---
	void CheckForMovementRequest(iPoint destiny ,float dt);
	
	// --- Enquiries ---
	bool IsGroupLead(j1Entity* entity);
	void SetUnitGoalTile(j1Entity* entity);
	bool FindFreeAdjacents(iPoint* base_tile);
	bool IsTileFree(iPoint* adjacent);

public:
	std::list <j1Entity*> Units;
private:
	iPoint last_goal = { 0,0 };

	std::list <iPoint*> Occupied_tiles;
};

struct Group_Unit
{
	bool IsSelected = false;

	// --- Path to follow ---
	std::vector <iPoint> Current_path;
	iPoint next_tile = { 0,0 };
	iPoint goal_tile = { 0,0 };

	// --- Group Movement stuff ---
	j1Group* current_group = nullptr;
	MovementState UnitMovementState = MovementState::MovementState_NoState;
};


#endif //__j1Group_H__