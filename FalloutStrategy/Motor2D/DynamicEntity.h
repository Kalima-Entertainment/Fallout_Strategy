#ifndef _DYNAMIC_ENTITY_H
#define _DYNAMIC_ENTITY_H
#include "j1Entity.h"

enum class Troop {
	NONE,
	GATHERER,
	MELEE,
	RANGED
};

class DynamicEntity : public j1Entity
{
public:
	DynamicEntity(Faction faction, Troop type);
	~DynamicEntity();
	bool Update(float dt);
	bool PostUpdate();
	bool LoadReferenceData();
	void CheckAnimation();
	void Move();

public:
	int mov_speed;
	int attack_speed;
	int damage;
	Troop type;
	iPoint next_tile;

	iPoint current_speed = { 0, 0 };
	fPoint speed = { 1, 0.5f };

	Collider* attack_collider = nullptr;
};



#endif // !_DYNAMIC_ENTITY_H

