#ifndef _ANIMAL_H_
#define _ANIMAL_H_
#include "DynamicEntity.h"
#include "j1Entity.h"

class Animal : public DynamicEntity {
public:
	Animal(EntityType g_type, iPoint g_current_tile);
	~Animal();

	bool Update(float dt);

	bool LoadDataFromReference();
	bool LoadReferenceData(pugi::xml_node& node);

public:

private:
	int food_quantity;
	ResourceBuilding* resource_spot;
	j1Timer timer;
	int pasturing_time;
};

#endif // !_ANIMAL_H_

