#include "Troop.h"
#include "j1Player.h"

Troop::Troop(EntityType type, Faction faction, iPoint position, GenericPlayer* owner) : DynamicEntity() {

}

Troop::~Troop() {

}

bool Troop::Update(float dt) {
	bool ret = true;

	switch (state)
	{
    case IDLE:
        break;
    case WALK:
		Move(dt);
        break;
    case ATTACK:
		if (attack_timer.Read() > attack_time) {
			Attack();
		}
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

void Troop::Attack() {

	action_timer.Start();

	//damage unit if god_mode isn't activated
	if ((target_entity->faction == App->player->faction) && (App->player->god_mode))
		return;

	target_entity->current_health -= damage;

	if (target_entity->current_health <= 0) {
		target_entity->attacking_entity = this;
		target_entity->state = DIE;
		path_to_target.clear();
		state = IDLE;

		if (attacking_entity == target_entity)
			attacking_entity = nullptr;

		target_entity = nullptr;
	}

	//Change animation directions to fit
	else if (target_entity->is_dynamic) {
		DynamicEntity* dynamic_target = (DynamicEntity*)target_entity;

		target_entity->attacking_entity = this;
		dynamic_target->state = HIT;

		if ((current_tile.x > target_entity->current_tile.x) && (current_tile.y == target_entity->current_tile.y)) {
			direction = TOP_LEFT;
			dynamic_target->direction = BOTTOM_RIGHT;
		}
		else if ((current_tile.x == target_entity->current_tile.x) && (current_tile.y > target_entity->current_tile.y)) {
			direction = TOP_RIGHT;
			dynamic_target->direction = BOTTOM_LEFT;
		}
		else if ((current_tile.x == target_entity->current_tile.x) && (current_tile.y < target_entity->current_tile.y)) {
			direction = BOTTOM_LEFT;
			dynamic_target->direction = TOP_RIGHT;
		}
		else if ((current_tile.x < target_entity->current_tile.x) && (current_tile.y == target_entity->current_tile.y)) {
			direction = BOTTOM_RIGHT;
			dynamic_target->direction = TOP_LEFT;
		}
	}
}

bool Troop::LoadDataFromReference() {
	bool ret = true;
	Troop* reference_troop = (Troop*)reference_entity;

	//load animations
	for (int i = 0; i < NO_STATE; i++)
	{
		for (int j = 0; j < NO_DIRECTION; j++)
		{
			animations[i][j] = reference_troop->animations[i][j];
		}
	}

	//load property data
	current_health = max_health = reference_entity->max_health;
	damage = reference_troop->damage;
	speed = reference_troop->speed;
	sprite_size = reference_entity->sprite_size;

	return ret;
}

bool Troop::LoadReferenceData(pugi::xml_node& node) {
	bool ret = true;
	
	max_health = node.attribute("health").as_int();
	damage = node.attribute("damage").as_int();
	speed.x = node.attribute("speed").as_int();
	speed.y = speed.x * 0.5f;

	return ret;
}