#ifndef _PLAYER_H
#define _PLAYER_H
#include "j1Module.h"

class j1Entity;

class Player : public j1Module
{
public:
	Player();
	~Player();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

private:
	j1Entity* selected_entity;
};


#endif // !_PLAYER_H

