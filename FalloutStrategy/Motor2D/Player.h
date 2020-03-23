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
	bool PostUpdate();

public:
	j1Entity* selected_entity;
	bool border_scroll;
private:
	float mouse_speed_multiplier;
};


#endif // !_PLAYER_H

