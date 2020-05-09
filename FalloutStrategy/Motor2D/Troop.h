#ifndef _TROOP_H_
#define _TROOP_H_
#include "DynamicEntity.h"
#include "j1Timer.h"

class Troop : public DynamicEntity {
public:
	Troop(EntityType type, Faction faction, iPoint position, GenericPlayer* owner = nullptr);
	~Troop();

	bool Update(float dt);
	void Attack();

private:
	int damage;
	int range;
	int attack_time;
	j1Timer attack_timer;
};

#endif // !_TROOP_H_

