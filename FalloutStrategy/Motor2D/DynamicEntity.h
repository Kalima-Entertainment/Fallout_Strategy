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

struct UnitInfo {

	UnitInfo();
	~UnitInfo();
	UnitInfo(const UnitInfo& info);

	SDL_Color color = { 255,255,255,255 };
};

class DynamicEntity : public j1Entity
{
public:
	DynamicEntity(Faction faction, EntityType type);
	~DynamicEntity();

	//Core
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate();

	bool LoadAnimations();
	//bool LoadFx();
	bool LoadReferenceData();
	void PathfindToPosition(iPoint target);
	void Move(float dt);
	void Attack();
	void Gather();
	void DrawQuad();

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
	DynamicEntity*	target_entity;
	StaticEntity*	target_building;
	ResourceBuilding* resource_building;
	Resource resource_type;
	j1Timer timer;

	iPoint next_tile_position;
	SDL_Rect next_tile_rect_center;

	uint fx[6];

	UnitInfo unitinfo;	
};



#endif // !_DYNAMIC_ENTITY_H
