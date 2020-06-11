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

	friend class j1MovementManager;
	friend class j1Group;

	DynamicEntity();
	~DynamicEntity();

	bool PostUpdate();

	void Move(float dt);
	void Flee();

	DynamicEntity* DetectEntitiesInRange();
	void CheckDestination(iPoint destination);

	bool PathfindToPosition(iPoint target);
	Direction GetDirectionToGo(SDL_Rect next_tile_rect) const;
	Direction GetBuildingDirection(std::vector<iPoint> building_tiles) const;
	void UpdateTile();

	bool LoadAnimations(const char* folder, const char* file_name);

protected:
	iPoint target_tile;
	iPoint next_tile;
	iPoint next_tile_position;
	SDL_Rect next_tile_rect;
	int detection_radius;

	j1Timer detection_timer;

public:

	Animation animations[NO_STATE][NO_DIRECTION];
	Direction direction;
	Direction last_direction;

	DynamicEntity* dynamic_target;

	bool is_agressive;
	bool commanded;

	UnitInfo unitinfo;
	std::vector<iPoint> node_path;
	std::vector<j1Entity*> entities_in_range;
	std::vector<iPoint> path_to_target;

	ParticleSystem* DynaParticle;
	FoWEntity* visionEntity;
};

#endif // !_DYNAMIC_ENTITY_H