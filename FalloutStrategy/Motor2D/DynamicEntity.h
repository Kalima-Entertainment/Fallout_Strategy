#ifndef _DYNAMIC_ENTITY_H
#define _DYNAMIC_ENTITY_H
#include "j1Entity.h"
#include "j1Timer.h"

struct ResourceBuilding;
enum class Resource;

class DynamicEntity : public j1Entity
{
public:
	DynamicEntity(Faction faction, EntityType type);
	~DynamicEntity();
	bool Update(float dt);
	bool PostUpdate();

	bool LoadAnimations(const char* animation_file);
	bool LoadReferenceData();
	void PathfindToPosition(iPoint target);
	void Move();
	void Attack();
	void Gather();

public:
	int mov_speed;
	int attack_speed;
	iPoint current_speed;
	iPoint next_tile;

	int range;
	int resource_collected;
	j1Entity* target_entity;
	ResourceBuilding* resource_building;
	Resource resource_type;
	j1Timer attack_timer;
	float attack_time;

	Collider* attack_collider;

	iPoint next_tile_center;
	SDL_Rect next_tile_center_rect;


};



#endif // !_DYNAMIC_ENTITY_H

