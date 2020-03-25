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
	bool LoadAnimations();
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
	j1Timer timer;
	float action_time;

	iPoint next_tile_position;
	SDL_Rect next_tile_rect_center;


};



#endif // !_DYNAMIC_ENTITY_H

