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
	void addUnit(DynamicEntity* unit_toadd);
	void removeUnit(DynamicEntity* unit_toremove);
	void AddTiletoOccupied(iPoint to_add);

	// --- Clear Stuff ---
	void ClearGroup();
	void ClearOccupiedlist();
	void DeselectGroup();

	// --- Getters ---
	int GetSize();

	// --- Check Movement Request ---
	void CheckForMovementRequest(iPoint destiny ,float dt);
	
	// --- Enquiries ---
	bool IsGroupLead(DynamicEntity* entity);
	void SetUnitGoalTile(DynamicEntity* entity);
	bool FindFreeAdjacents(iPoint* base_tile);
	bool IsTileFree(iPoint* adjacent);

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

public:
	std::vector<DynamicEntity*> Units;

private:
	iPoint last_goal;

	std::list <iPoint*> Occupied_tiles;
};

struct Group_Unit
{
	bool IsSelected = false;

	// --- Group Movement stuff ---
	j1Group* current_group = nullptr;
	MovementState UnitMovementState = MovementState::MovementState_NoState;

	// --- Path to follow ---
	iPoint goal_tile = { 0,0 };
};


#endif //__j1Group_H__