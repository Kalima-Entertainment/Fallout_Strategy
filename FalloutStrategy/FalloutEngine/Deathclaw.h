#ifndef _DEATHCLAW_H_
#define _DEATHCLAW_H_
#include "DynamicEntity.h"
#include "j1Entity.h"

class StaticEntity;

class Deathclaw : public DynamicEntity {
public:
	Deathclaw(iPoint current_tile);
	~Deathclaw();

	bool Update(float dt);
	void Attack();

	bool LoadDataFromReference();
	bool LoadReferenceData(pugi::xml_node& node);

public:
	StaticEntity* target_building;
private:
	int attack_time;
	j1Timer attack_timer;
	int range;
};

#endif // !_DEATHCLAW_H_

