#ifndef _GATHERER_H_
#define _GATHERER_H_
#include "DynamicEntity.h"

class Gatherer : public DynamicEntity
{
public:
	Gatherer(Faction g_faction, iPoint g_current_tile, GenericPlayer* g_owner);
	~Gatherer();

	bool Update(float dt);
	bool PostUpdate();
	void Gather();
	void StoreGatheredResources();

private:
	ResourceBuilding* resource_building;
	Resource resource_type;
	int resource_capacity;
	int resource_collected;
	int gather_time;
};


#endif // !_GATHERER_H_

