#ifndef _DYNAMIC_ENTITY_H
#define _DYNAMIC_ENTITY_H
#include "j1Entity.h"
#include "j1Timer.h"

struct ResourceBuilding;
enum class Resource;

enum Direction {
	TOP_LEFT,
	TOP_RIGHT,
	RIGHT,
	BOTTOM_RIGHT,
	BOTTOM_LEFT,
	LEFT,
	NO_DIRECTION
};

enum DynamicState {
	IDLE,
	WALK,
	ATTACK,
	GATHER,
	HIT,
	DIE,
	MAX_ANIMATIONS
};

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
	float action_time;
	iPoint current_speed;
	iPoint next_tile;

	Animation animations[MAX_ANIMATIONS][7];
	Direction direction;
	DynamicState state;
	int range;
	int resource_collected;
	DynamicEntity* target_entity;
	ResourceBuilding* resource_building;
	Resource resource_type;
	j1Timer timer;

	iPoint next_tile_position;
	SDL_Rect next_tile_rect_center;


};



#endif // !_DYNAMIC_ENTITY_H

