#ifndef _TROOP_H_
#define _TROOP_H_
#include "DynamicEntity.h"
#include "j1Entity.h"
#include "j1Timer.h"

class StaticEntity;

class Troop : public DynamicEntity {
public:
	Troop(EntityType type, Faction faction, iPoint current_tile, GenericPlayer* owner = nullptr);
	~Troop();

	bool Update(float dt);
	void Attack();

	bool LoadDataFromReference();
	bool LoadReferenceData(pugi::xml_node& node);

private:
	int damage;
	int range;
	int attack_time;
	j1Timer attack_timer;
	StaticEntity* target_building;
};

#endif // !_TROOP_H_

