#ifndef _ANIMAL_H_
#define _ANIMAL_H_
#include "DynamicEntity.h"

class Animal : public DynamicEntity {
public:
	Animal(EntityType g_type, iPoint g_current_tile);
	~Animal();

	bool Update(float dt);

public:

private:

};

#endif // !_ANIMAL_H_

