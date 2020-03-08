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
	DynamicEntity* selected_entity;
};


#endif // !_PLAYER_H

