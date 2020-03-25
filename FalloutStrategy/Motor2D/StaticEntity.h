#ifndef _STATIC_ENTITY_H
#define _STATIC_ENTITY_H
#include "j1Entity.h"
#include <vector>

class StaticEntity : public j1Entity

{
public:
	StaticEntity(Faction faction, EntityType type);
	~StaticEntity();
	bool Update(float dt);
	bool PostUpdate();

	bool LoadAnimations();
	bool LoadReferenceData();

public:
	std::vector<iPoint> tiles;
private:
	int gen_speed;
};

#endif // !_STATIC_ENTITY_H