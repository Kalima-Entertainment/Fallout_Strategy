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

struct Upgrades_Data{
	std::string name;
	int upgrade_num;
	int first_price;
	int price_increment;
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

	Upgrades_Data base_resource_limit;
	Upgrades_Data gatherer_resource_limit;
	Upgrades_Data units_damage;
	Upgrades_Data units_speed;
	Upgrades_Data units_health;
	Upgrades_Data units_creation_time;
};

#endif // !_STATIC_ENTITY_H