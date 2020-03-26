#include "j1Entity.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1EntityManager.h"
#include "j1Map.h"
#include "p2Log.h"
#include "j1Collision.h"

#include "j1Scene.h"

j1Entity::j1Entity() {

	position = { 0, 0 };
	current_tile = { 0, 0 };
	target_tile = { 0, 0 };
	path_to_target = nullptr;

	spawnPosition = {NULL,NULL};

	max_health = current_health = 100;

	collider = nullptr;
	last_collider = nullptr;

	current_animation = nullptr;
	faction = VAULT;

	texture = nullptr;

	is_dynamic = false;
	to_destroy = false;
	particles_created = false;
	playing_fx = false;
}

j1Entity::~j1Entity() {}

// to be updated


iPoint j1Entity::MapPosition() {
	iPoint spot = App->render->ScreenToWorld(position.x, position.y);
	spot = App->map->WorldToMap(spot.x, spot.y);
	return spot;
}

int j1Entity::GetPositionScore() const {
	return current_tile.x + current_tile.y;
}
