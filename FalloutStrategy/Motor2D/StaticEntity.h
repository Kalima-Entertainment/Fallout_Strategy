#ifndef _STATIC_ENTITY_H
#define _STATIC_ENTITY_H
#include "j1Entity.h"

enum class BuildingType {
	BASE,
	WAREHOUSE,
	BARRACK
};

class StaticEntity : public j1Entity

{
public:
	StaticEntity(BuildingType type);
	~StaticEntity();

public:
	BuildingType type;
private:
	int gen_speed;
};

#endif // !_STATIC_ENTITY_H