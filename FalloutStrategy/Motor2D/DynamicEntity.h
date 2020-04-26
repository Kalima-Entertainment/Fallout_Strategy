#ifndef _DYNAMIC_ENTITY_H
#define _DYNAMIC_ENTITY_H
#include "j1Entity.h"
#include "j1Timer.h"

struct ResourceBuilding;
enum class Resource;

enum Direction {
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM_RIGHT,
	BOTTOM_LEFT,
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
	DynamicEntity(Faction faction, EntityType type, GenericPlayer* owner = nullptr);
	~DynamicEntity();

	//Core
	bool Update(float dt);
	bool PostUpdate();

	bool LoadAnimations();
	//bool LoadFx();
	bool LoadReferenceData();
	void PathfindToGather(iPoint target);
	void PathfindToPosition(iPoint target);
	void Move(float dt);
	void Attack();
	void Gather();
	void DrawQuad();
	bool TargetTileReached(iPoint target_tile);
	Direction GetDirectionToGo(SDL_Rect next_tile_rect) const;

public:
	float action_time;
	iPoint next_tile;
	iPoint next_tile_position;
	SDL_Rect next_tile_rect;

	Animation animations[MAX_ANIMATIONS][4];
	Direction direction;
	Direction last_direction;
	DynamicState state;
	int range;
	int resource_collected;
	ResourceBuilding* resource_building;
	Resource resource_type;
	j1Timer timer;
	bool agressive;

	uint fx[6];

	UnitInfo unitinfo;	
	std::vector<iPoint> node_path;
};



#endif // !_DYNAMIC_ENTITY_H
