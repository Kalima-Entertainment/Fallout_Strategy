#include "Gatherer.h"
#include "j1Entity.h"
#include "j1Render.h"
#include "StaticEntity.h"
#include "GenericPlayer.h"
#include "j1Player.h"

Gatherer::Gatherer(Faction g_faction, iPoint g_current_tile, GenericPlayer* g_owner) : DynamicEntity(), resource_collected(0) {
	type = GATHERER;
	faction = g_faction;
	current_tile = g_current_tile;
	owner = g_owner;
	is_dynamic = true;
	is_agressive = false;
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

/*
bool Gatherer::PostUpdate() {
	bool ret = true;

	current_animation = &animations[state][direction];
	App->render->Blit(texture, render_position.x, render_position.y, &current_animation->GetCurrentFrame(last_dt));

	return ret;
}
*/

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

void Gatherer::AssignResourceBuilding(ResourceBuilding* g_resource_building) {
	resource_building = g_resource_building;
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

bool Gatherer::LoadDataFromReference() {
	bool ret = true;
	Gatherer* reference_gatherer = (Gatherer*)reference_entity;

	//load animations
	for (int i = 0; i < NO_STATE; i++)
	{
		for (int j = 0; j < NO_DIRECTION; j++)
		{
			animations[i][j] = reference_gatherer->animations[i][j];
		}
	}

	//load property data
	current_health = max_health = reference_entity->max_health;
	resource_capacity = reference_gatherer->resource_capacity;
	speed = reference_gatherer->speed;
	sprite_size = reference_entity->sprite_size;
	texture = reference_entity->texture;

	return ret;
}

bool Gatherer::LoadReferenceData(pugi::xml_node& node) {
	bool ret = true;

	max_health = node.attribute("health").as_int();
	resource_capacity = node.attribute("damage").as_int();
	speed.x = node.attribute("speed").as_int();
	speed.y = speed.x * 0.5f;

	return ret;
}