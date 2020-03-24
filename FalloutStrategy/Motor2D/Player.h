#ifndef _PLAYER_H
#define _PLAYER_H
#include "j1Module.h"

class j1Entity;
class DynamicEntity;

class Player : public j1Module
{
public:
	Player();
	~Player();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);

public:
	bool border_scroll;

	j1Entity* selected_entity;
	int caps;
	int water;
	int food;
private:
	float mouse_speed_multiplier;
};


#endif // !_PLAYER_H

