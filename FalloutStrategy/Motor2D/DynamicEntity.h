#ifndef _DYNAMIC_ENTITY_H
#define _DYNAMIC_ENTITY_H
#include "j1Entity.h"
#include "j1Timer.h"

class DynamicEntity : public j1Entity
{
public:
	DynamicEntity(Faction faction, EntityType type);
	~DynamicEntity();
	bool Update(float dt);
	bool PostUpdate();
	bool LoadReferenceData();
	void PathfindToPosition(iPoint target);
	void Move();
	void Attack();

public:
	int mov_speed;
	int attack_speed;
	iPoint current_speed;
	fPoint speed;
	iPoint next_tile;

	int damage;
	j1Entity* target_entity;
	j1Timer attack_timer;
	float attack_time;

	Collider* attack_collider;

	iPoint next_tile_center;
	SDL_Rect next_tile_center_rect;


};



#endif // !_DYNAMIC_ENTITY_H

