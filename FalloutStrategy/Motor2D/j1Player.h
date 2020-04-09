#ifndef _PLAYER_H
#define _PLAYER_H
#include "j1Module.h"

class j1Entity;
class DynamicEntity;
enum class Resource;

class j1Player : public j1Module
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
	int caps;
	int water;
	int food;
	bool factions[4];

private:
	float mouse_speed_multiplier;
	bool border_scroll;
	bool god_mode;
};


#endif // !_PLAYER_H

