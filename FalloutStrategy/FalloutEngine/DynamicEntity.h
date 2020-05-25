#ifndef _DYNAMIC_ENTITY_H
#define _DYNAMIC_ENTITY_H
#include "j1Entity.h"
#include "FoWEntity.h"

struct ResourceBuilding;
enum class Resource;
class j1Timer;

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
	DynamicEntity();
	DynamicEntity(Faction faction, EntityType type, iPoint current_tile, GenericPlayer* owner = nullptr);
	~DynamicEntity();

	//Core
	//bool Update(float dt);
	bool PostUpdate();

	void Move(float dt);
	void Flee();

	j1Entity* DetectEntitiesInRange();
	void PathfindToPosition(iPoint target);
	Direction GetDirectionToGo(SDL_Rect next_tile_rect) const;
	void UpdateTile();

	bool LoadAnimations(const char* folder, const char* file_name);

protected:
	iPoint target_tile;
	iPoint next_tile;
	iPoint next_tile_position;
	SDL_Rect next_tile_rect;
	int detection_radius;
	fPoint speed;

	j1Timer detection_timer;

public:

	Animation animations[NO_STATE][NO_DIRECTION];
	Direction direction;
	Direction last_direction;

	bool is_agressive;
	bool commanded;

	UnitInfo unitinfo;
	std::vector<iPoint> node_path;
	std::vector<j1Entity*> entities_in_range;
	std::vector<iPoint> path_to_target;
};

#endif // !_DYNAMIC_ENTITY_H