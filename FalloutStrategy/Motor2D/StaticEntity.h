#ifndef _STATIC_ENTITY_H
#define _STATIC_ENTITY_H
#include "j1Entity.h"

class StaticEntity : public j1Entity

{
public:
	StaticEntity(Faction faction, EntityType type);
	~StaticEntity();
	bool Update(float dt);
	bool PostUpdate();

	bool LoadAnimations(const char* animation_file);
	bool LoadReferenceData();
public:
	j1Entity* target_entity;
private:
	int gen_speed;
};

#endif // !_STATIC_ENTITY_H