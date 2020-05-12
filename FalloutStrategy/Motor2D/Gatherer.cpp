#include "Gatherer.h"
#include "j1Entity.h"
#include "j1Render.h"
#include "StaticEntity.h"
#include "GenericPlayer.h"
#include "j1Player.h"
#include "j1Map.h"
#include "j1Render.h"
#include "FoWManager.h"

Gatherer::Gatherer(Faction g_faction, iPoint g_current_tile, GenericPlayer* g_owner) : DynamicEntity(), resource_collected(0) {
	type = GATHERER;
	faction = g_faction;
	current_tile = g_current_tile;
	owner = g_owner;
	is_dynamic = true;
	is_agressive = false;
	position = App->map->fMapToWorld(current_tile.x, current_tile.y);
	position.x += HALF_TILE;
	position.y += HALF_TILE;
	gather_time = 2;
	
	if (owner)
		base = owner->base;

	//Fog Of War
	if (App->render->fog_of_war) {
		if (this->faction == App->player->faction) {
			//Player
			visionEntity = App->fowManager->CreateFoWEntity({ this->current_tile.x, this->current_tile.y }, true);
			visionEntity->SetNewVisionRadius(5);
		}
		else {
			//Enemy
			visionEntity = App->fowManager->CreateFoWEntity({ this->current_tile.x, this->current_tile.y }, false);
		}
		visionEntity->SetNewPosition(App->map->MapToWorld(this->current_tile.x, this->current_tile.y));
	}
}

Gatherer::~Gatherer() {
	target_entity = nullptr;
	attacking_entity = nullptr;
	resource_building = nullptr;
	reference_entity = nullptr;
	owner = nullptr;
	current_animation = nullptr;
	texture = nullptr;
	node_path.clear();
	path_to_target.clear();
	entities_in_range.clear();
	base = nullptr;
}

bool Gatherer::Update(float dt) {
	bool ret = true;

	switch (state)
	{
	case IDLE:
		break;
	case WALK:
		Move(dt);
		if (App->render->fog_of_war)
			visionEntity->SetNewPosition(App->map->MapToWorld(this->current_tile.x, this->current_tile.y));

		if (next_tile == target_tile) {
			//gather
			if (((resource_building != nullptr) && (resource_collected < storage_capacity))
				|| ((resource_collected > 0) && (target_entity != nullptr))) {
				state = GATHER;
				gathering_timer.Start();
			}
		}

		break;
	case GATHER:
		if (gathering_timer.ReadSec() > gather_time) {
			//collect resources
			if ((resource_building)&&(resource_collected < storage_capacity)) {
				Gather();
				state = WALK;
			}
			else {
				StoreGatheredResources();

				//go back to resource building to get more resources
				if ((resource_building)&&(resource_building->quantity > 0)) {
					PathfindToPosition(App->entities->ClosestTile(current_tile, resource_building->tiles));
					state = WALK;
				}
				//find another building
				else
				{
					resource_building = App->entities->GetClosestResourceBuilding(current_tile);
					//if there is at least a resource building left, go there
					if (resource_building != nullptr) {
						PathfindToPosition(App->entities->ClosestTile(current_tile, resource_building->tiles));
						state = WALK;
					}
					//if there are no resource buildings left
					else {
						state = IDLE;
					}
				}
			}
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

	last_dt = dt;

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

	if (base != nullptr) {
		PathfindToPosition(App->entities->ClosestTile(current_tile, base->tiles));
		target_entity = base;
	}

	if (resource_building->quantity <= 0)
		resource_building = nullptr;
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
	storage_capacity = reference_gatherer->storage_capacity;
	texture = reference_entity->texture;

	return ret;
}

bool Gatherer::LoadReferenceData(pugi::xml_node& node) {
	bool ret = true;

	max_health = node.attribute("health").as_int();
	resource_capacity = node.attribute("damage").as_int();
	speed.x = node.attribute("speed").as_int();
	speed.y = speed.x * 0.5f;
	storage_capacity = node.attribute("storage_capacity").as_int();

	return ret;
}