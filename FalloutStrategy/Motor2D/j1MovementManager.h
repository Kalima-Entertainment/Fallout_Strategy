#ifndef __J1MOVEMENTMANAGER_H__
#define __J1MOVEMENTMANAGER_H__


#include "SDL\include\SDL.h"
#include "j1Module.h"
#include <list>

enum class MovementState
{
	MovementState_NoState,
	MovementState_Wait,
	MovementState_FollowPath,
	MovementState_NextStep,
	MovementState_DestinationReached
};

class j1Group;

class j1MovementManager : public j1Module
{
public:

	j1MovementManager();

	virtual ~j1MovementManager();

	// --- Called each loop iteration ---
	bool Update(float dt);

	// --- Called before quitting ---
	bool CleanUp();

	// --- Group Tools ---
	void SelectEntities_inRect(SDL_Rect SRect);
	void CreateGroup(std::vector<DynamicEntity*> entities_vector);

	// --- Move Unit ---
	void Move(j1Group* unit, iPoint goal_path, float dt);

	void AddGroup(j1Group* group){ Groups.push_back(group);}

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

private:
	std::list <j1Group*> Groups;
	bool stop_iteration = false;
};

#endif // __J1MOVEMENTMANAGER_H__
