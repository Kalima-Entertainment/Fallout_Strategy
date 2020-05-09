#include "Animal.h"

Animal::Animal(EntityType type, iPoint current_tile) : DynamicEntity() {

}

Animal::~Animal() {

}

bool Animal::Update(float dt) {
	bool ret = true;

	switch (state)
	{
    case IDLE:
        break;
    case WALK:
        Move(dt);
        break;
    case HIT:
        break;
    case DIE:
        direction = TOP_LEFT;
        break;
    default:
        break;
	}

	return ret;
}