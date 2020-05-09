#ifndef _ANIMAL_H_
#define _ANIMAL_H_
#include "DynamicEntity.h"

class Animal : public DynamicEntity {
public:
	Animal(EntityType g_type, iPoint g_current_tile);
	~Animal();

	bool Update(float dt);

	bool LoadDataFromReference();
	bool LoadReferenceData(pugi::xml_node& node);

public:

private:
	int resource_quantity;
};

#endif // !_ANIMAL_H_

