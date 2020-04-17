#ifndef _PLAYER_H
#define _PLAYER_H
#include "GenericPlayer.h"

class j1Player : public GenericPlayer
{
public:
	j1Player();
	~j1Player();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	void RecountEntities();

	void OnCommand(std::vector<std::string> command_parts);

	j1Entity* SelectEntity();
	void MoveEntity();
	void UpdateResourceData(Resource resource_type, int quantity);
	bool TouchingUI(int x, int y);
public:
	j1Entity* selected_entity;
	j1Entity* last_selected_entity;
	bool god_mode;

private:
	float mouse_speed_multiplier;
	bool border_scroll;
};


#endif // !_PLAYER_H

