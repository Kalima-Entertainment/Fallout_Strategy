#include "Animal.h"
#include "GenericPlayer.h"
#include "AI_Player.h"
#include "j1Map.h"

Animal::Animal(EntityType g_type, iPoint g_current_tile) : DynamicEntity() {
	type = g_type;
	current_tile = g_current_tile;
	is_dynamic = true;
	is_agressive = false;
	position = App->map->fMapToWorld(current_tile.x, current_tile.y);
	position.x += HALF_TILE;
	position.y += HALF_TILE;
	pasturing_time = 20;
	faction = NO_FACTION;
}

Animal::~Animal() {
	target_entity = nullptr;
	attacking_entity = nullptr;
	resource_spot = nullptr;
	reference_entity = nullptr;
	owner = nullptr;
	current_animation = nullptr;
	texture = nullptr;
	node_path.clear();
	path_to_target.clear();
	entities_in_range.clear();
}

bool Animal::Update(float dt) {
	bool ret = true;
	current_animation = &animations[state][direction];

	switch (state)
	{
    case IDLE:
		if (timer.ReadSec() > pasturing_time) {
			Flee();
			timer.Start();
		}
        break;
    case WALK:
        Move(dt);

        break;
    case HIT:
		if (current_animation->Finished()) {
			current_animation->Reset();
			if (attacking_entity != nullptr) 
				Flee();
			else
				state = IDLE;
		}
        break;

    case DIE:
        direction = TOP_LEFT;

		if (!delete_timer.Started()) {
			delete_timer.Start();
			direction = TOP_LEFT;

			resource_spot = App->entities->CreateResourceSpot(current_tile.x, current_tile.y, Resource::FOOD, food_quantity);
			App->entities->occupied_tiles[current_tile.x][current_tile.y] = false;
			current_tile = { -1,-1 };
			next_tile = { -1,-1 };
			if (attacking_entity->owner->is_ai) {
				((AI_Player*)attacking_entity->owner)->GatherFood(resource_spot);
				attacking_entity = nullptr;
			}
		}
        break;

    default:
        break;
	}

	last_dt = dt;

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
	food_quantity = reference_animal->food_quantity;
	speed = reference_animal->speed;
	sprite_size = reference_entity->sprite_size;
	texture = reference_entity->texture;

	return ret;
}

bool Animal::LoadReferenceData(pugi::xml_node& node) {
	bool ret = true;

	max_health = node.attribute("health").as_int();
	food_quantity = node.attribute("resource_quantity").as_int();
	speed.x = node.attribute("speed").as_int();
	speed.y = speed.x * 0.5f;

	return ret;
}