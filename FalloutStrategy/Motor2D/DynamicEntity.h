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


struct UnitInfo {

	UnitInfo();
	~UnitInfo();
	UnitInfo(const UnitInfo& info);

	SDL_Color color = { 255,255,255,255 };
};

class DynamicEntity : public j1Entity
{
public:
	DynamicEntity(Faction faction, EntityType type, iPoint current_tile, GenericPlayer* owner = nullptr);
	~DynamicEntity();

	//Core
	bool Update(float dt);
	bool PostUpdate();

	bool LoadAnimations();
	bool LoadReferenceData();

	void Move(float dt);
	void Attack();
	void Gather();
	void StoreGatheredResources();
	void Flee();

	j1Entity* DetectEntitiesInRange();
	void PathfindToPosition(iPoint target);
	Direction GetDirectionToGo(SDL_Rect next_tile_rect) const;
	void UpdateTile();

public:
	float action_time;
	iPoint next_tile;
	iPoint next_tile_position;
	SDL_Rect next_tile_rect;

	Animation animations[NO_STATE][NO_DIRECTION];
	Direction direction;
	Direction last_direction;

	int range;
	int detection_radius;
	int resource_collected;
	ResourceBuilding* resource_building;
	Resource resource_type;
	bool is_agressive;

	j1Timer action_timer;
	j1Timer detection_timer;

	UnitInfo unitinfo;	
	std::vector<iPoint> node_path;
	std::vector<j1Entity*> entities_in_range;
};



#endif // !_DYNAMIC_ENTITY_H
