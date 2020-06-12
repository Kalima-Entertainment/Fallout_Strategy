#ifndef _PLAYER_H
#define _PLAYER_H
#include "GenericPlayer.h"
#include "p2Point.h"
#include "FoWEntity.h"

class j1Group;

class j1Player : public GenericPlayer
{
public:
	j1Player();
	~j1Player();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);

	void OnCommand(std::vector<std::string> command_parts);

	j1Entity* SelectEntity();
	void MoveEntity(DynamicEntity* entity);
	void MoveGroup();
	void UpdateResourceData(Resource resource_type, int quantity);
	bool TouchingUI(int x, int y);

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
public:
	j1Entity* selected_entity;
	j1Entity* last_selected_entity;
	j1Group* selected_group;
	bool god_mode;
	iPoint mouse_position;
	iPoint Map_mouseposition;
	bool qcaps;
	bool qfood;
	bool qwater;
	int reward ;

private:
	float mouse_speed_multiplier;
	bool border_scroll;
	bool resource_fow_added;
};


#endif // !_PLAYER_H

