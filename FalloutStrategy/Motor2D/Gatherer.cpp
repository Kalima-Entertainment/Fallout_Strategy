#include "Gatherer.h"
#include "j1Entity.h"
#include "j1Render.h"
#include "StaticEntity.h"
#include "GenericPlayer.h"
#include "j1Player.h"

Gatherer::Gatherer(Faction g_faction, iPoint g_current_tile, GenericPlayer* g_owner) : DynamicEntity(), resource_collected(0) {
	type = GATHERER;
}

Gatherer::~Gatherer() {
	target_entity = nullptr;
	attacking_entity = nullptr;
	resource_building = nullptr;
}

bool Gatherer::Update(float dt) {
	bool ret = true;

	switch (state)
	{
	case IDLE:
		break;
	case WALK:
		Move(dt);
		break;
	case GATHER:
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

bool Gatherer::PostUpdate() {
	bool ret = true;

	current_animation = &animations[state][direction];
	App->render->Blit(texture, render_position.x, render_position.y, &current_animation->GetCurrentFrame(last_dt));

	return ret;
}

void Gatherer::Gather() {
	uint resource = resource_building->quantity - (resource_capacity - resource_collected);

	resource_building->quantity -= resource;
	resource_collected += resource;
	resource_type = resource_building->resource_type;

	if (owner->base != nullptr) {
		PathfindToPosition(App->entities->ClosestTile(current_tile, owner->base->tiles));
		target_entity = owner->base;
	}
}

void Gatherer::StoreGatheredResources() {
	owner->base += resource_collected;

	if (owner == App->player) {
		App->player->UpdateResourceData(resource_type, resource_collected);
	}
	else {
		//update owner resources
		if (resource_type == Resource::CAPS) owner->caps += resource_collected;
		else if (resource_type == Resource::WATER) owner->water += resource_collected;
		else if (resource_type == Resource::FOOD) owner->food += resource_collected;
	}

	resource_collected = 0;
	target_entity = nullptr;
}