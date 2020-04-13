#ifndef _PLAYER_H
#define _PLAYER_H
#include "j1Module.h"

class j1Entity;
class DynamicEntity;
class UI_Label;

class Player : public j1Module
{
public:
	Player();
	~Player();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);

	void OnCommand(std::vector<std::string> command_parts);

	j1Entity* SelectEntity();
	void MoveEntity();
	void UpdateResourceData(Resource resource_type, int quantity);

public:
	bool border_scroll;

	j1Entity* selected_entity;
	bool god_mode;

private:
	float mouse_speed_multiplier;
	bool border_scroll;
};


#endif // !_PLAYER_H
