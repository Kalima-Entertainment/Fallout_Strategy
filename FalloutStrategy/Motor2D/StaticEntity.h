#ifndef _STATIC_ENTITY_H
#define _STATIC_ENTITY_H
#include "j1Entity.h"
#include <vector>
#include <string>

enum StaticState {
	WAIT,
	WORK,
	EXPLODE,
	NO_STATE
};

class StaticEntity : public j1Entity

{
public:
	StaticEntity(Faction faction, EntityType type);
	~StaticEntity();
	bool Update(float dt);
	bool PostUpdate();

	bool LoadAnimations();
	bool LoadReferenceData();	

	void Upgrade(Faction faction, std::string upgrade_name);
	
public:
	std::vector<iPoint> tiles;
private:
	int gen_speed;
	Animation animations[3];
	StaticState state;
	iPoint render_texture_pos;
};

#endif // !_STATIC_ENTITY_H