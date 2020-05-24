#ifndef _GATHERER_H_
#define _GATHERER_H_
#include "DynamicEntity.h"
#include "j1Entity.h"

class Gatherer : public DynamicEntity
{
public:
	Gatherer(Faction g_faction, iPoint g_current_tile, GenericPlayer* g_owner);
	~Gatherer();

	bool Update(float dt);
	//bool PostUpdate();
	void Gather();
	void StoreGatheredResources();

	bool LoadDataFromReference();
	bool LoadReferenceData(pugi::xml_node& node);

	void AssignResourceBuilding(ResourceBuilding* resource_building);
	int GetResourceCollected() { return resource_collected; }
	ResourceBuilding* GetResourceBuilding() { return resource_building; }

private:
	ResourceBuilding* resource_building;
	Resource resource_type;
	int resource_capacity;
	int resource_collected;
	j1Timer gathering_timer;
	int gather_time;
	StaticEntity* base;

	ParticleSystem* particle = nullptr;
};


#endif // !_GATHERER_H_

