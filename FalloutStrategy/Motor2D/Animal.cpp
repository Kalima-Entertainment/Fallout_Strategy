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

bool Animal::LoadDataFromReference() {
	bool ret = true;
	Animal* reference_animal = (Animal*)reference_entity;

	//load animations
	for (int i = 0; i < NO_STATE; i++)
	{
		for (int j = 0; j < NO_DIRECTION; j++)
		{
			animations[i][j] = reference_animal->animations[i][j];
		}
	}

	//load property data
	current_health = max_health = reference_entity->max_health;
	resource_quantity = reference_animal->resource_quantity;
	speed = reference_animal->speed;
	sprite_size = reference_entity->sprite_size;

	return ret;
}

bool Animal::LoadReferenceData(pugi::xml_node& node) {
	bool ret = true;

	max_health = node.attribute("health").as_int();
	resource_quantity = node.attribute("resource_quantity").as_int();
	speed.x = node.attribute("speed").as_int();
	speed.y = speed.x * 0.5f;

	return ret;
}