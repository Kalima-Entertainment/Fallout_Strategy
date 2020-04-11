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

	void InteractWithEntity();
	void UpdateResourceData(Resource resource_type, int quantity);

public:
	j1Entity* selected_entity;

private:
	float mouse_speed_multiplier;
	bool border_scroll;
	bool god_mode;
};


#endif // !_PLAYER_H

