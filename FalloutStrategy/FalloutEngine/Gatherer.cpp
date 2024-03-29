#include "Gatherer.h"
#include "j1Entity.h"
#include "j1Render.h"
#include "StaticEntity.h"
#include "GenericPlayer.h"
#include "j1Player.h"
#include "j1Map.h"
#include "j1Render.h"
#include "FoWManager.h"
#include "SDL_mixer/include/SDL_mixer.h"

#include "Emiter.h"
#include "ParticleSystem.h"

Gatherer::Gatherer(Faction g_faction, iPoint g_current_tile, GenericPlayer* g_owner) : DynamicEntity(), resource_collected(0) {
	type = GATHERER;
	faction = g_faction;
	current_tile = g_current_tile;
	owner = g_owner;
	is_agressive = false;
	gather_time = 2;
	storage_capacity = 0;
	resource_type = Resource::NO_TYPE;
	gathering = false;

	position = App->map->floatMapToWorld(current_tile.x, current_tile.y);
	position.x += HALF_TILE;
	position.y += HALF_TILE;

	if (owner)
		base = owner->base;

	resource_building = nullptr;

	if (this->faction == App->player->faction) {
		//Player
		visionEntity = App->fowManager->CreateFoWEntity({ this->current_tile.x, this->current_tile.y }, true);
		visionEntity->SetNewVisionRadius(5);
	}
	else {
		//Enemy
		visionEntity = App->fowManager->CreateFoWEntity({ this->current_tile.x, this->current_tile.y }, false);
	}


	DynaParticle = App->entities->CreateParticle(position);
	Animation anim;

	//First Row
	anim.PushBack(SDL_Rect{ 0, 0 , 64 , 64 }, 10);
	anim.PushBack(SDL_Rect{ 64, 0 , 64 , 64 }, 10);
	anim.PushBack(SDL_Rect{ 128, 0 , 64 , 64 }, 10);
	anim.PushBack(SDL_Rect{ 192, 0 , 64 , 64 }, 10);

	//Second Row
	anim.PushBack(SDL_Rect{ 0, 64,  64 , 64 }, 10);
	anim.PushBack(SDL_Rect{ 64,  64, 64 , 64 }, 10);
	anim.PushBack(SDL_Rect{ 128, 64, 64 , 64 }, 10);
	anim.PushBack(SDL_Rect{ 192, 64, 64 , 64 }, 10);

	//Third Row
	anim.PushBack(SDL_Rect{ 0,  128,64 , 64 }, 10);
	anim.PushBack(SDL_Rect{ 64, 128 ,64 , 64 }, 10);
	anim.PushBack(SDL_Rect{ 128, 128, 64 , 64 }, 10);
	anim.PushBack(SDL_Rect{ 192, 128, 64 , 64 }, 10);

	anim.Reset();

	Emiter Blood(position.x, position.y, 0.0f, 0.0f, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2.5f, nullptr, App->entities->blood, anim, true);
	DynaParticle->PushEmiter(Blood);
	DynaParticle->Desactivate();
}

Gatherer::~Gatherer() {
	resource_building = nullptr;
	base = nullptr;
	visionEntity = nullptr;
	DynaParticle = nullptr;
}

bool Gatherer::Update(float dt) {
	bool ret = true;
	current_animation = &animations[state][direction];

	if ((base != nullptr) && ((base->state >= NO_STATE) || (base->state < IDLE))) {
		state = IDLE;
		base = nullptr;
	}

	switch (state)
	{
	case IDLE:
		if (!commanded) {
			if ((resource_collected > 0) && (base != nullptr) && (base->state != DIE)) {
				PathfindToPosition(App->entities->ClosestTile(current_tile, base->tiles));
			}
			else if ((resource_building != nullptr) && (resource_building->quantity > 0)) {
				PathfindToPosition(App->entities->ClosestTile(current_tile, resource_building->tiles));
			}
			else if (faction != App->player->faction){
				resource_building = App->entities->GetClosestResourceBuilding(current_tile, resource_type);

				if (resource_building != nullptr) 
					PathfindToPosition(App->entities->ClosestTile(current_tile, resource_building->tiles));
			}
		}
		break;
	case WALK:
		Move(dt);

		if ((current_tile == target_tile)&&(node_path.size() == 0)) {
			//gather
			if ((((resource_building != nullptr) && (resource_collected < storage_capacity)) || ((resource_collected > 0) && (base != nullptr))) && gathering) {
				state = GATHER;
				gathering_timer.Start();
			}
			else {
				state = IDLE;
			}
		}
		SpatialAudio(static_cast<int>(position.x), static_cast<int>(position.y), faction, state, type);

		break;
	case GATHER:
		if (gathering_timer.ReadSec() > gather_time) {
			//collect resources
			if ((resource_building)&&(resource_collected < storage_capacity)) {
				Gather();
				state = WALK;
			}
			else if ((base != nullptr)&&(base->state != DIE)){

				StoreGatheredResources();

				//go back to resource building to get more resources
				if ((resource_building != nullptr)&&(resource_building->quantity > 0)) {
					PathfindToPosition(App->entities->ClosestTile(current_tile, resource_building->tiles));
				}
				//find another building
				else {
					resource_building = App->entities->GetClosestResourceBuilding(current_tile, resource_type);

					//if there is at least a resource building left, go there
					if (resource_building != nullptr) {
						PathfindToPosition(App->entities->ClosestTile(current_tile, resource_building->tiles));
						state = WALK;
					}
					//if there are no resource buildings left
					else {
						LOG("No more resource buildings");
						state = IDLE;
					}
				}
			}
			else {
				state = IDLE;
				path_to_target.clear();
				commanded = false;
			}
		}
		break;
	case HIT:
		if (current_animation->Finished()) {
			current_animation->Reset();
			//state = IDLE;
			Flee();
		}
		SpatialAudio(static_cast<int>(position.x), static_cast<int>(position.y), faction, state, type);
		break;
	case DIE:
		direction = TOP_LEFT;
		if (!delete_timer.Started()) {
			delete_timer.Start();
			direction = TOP_LEFT;
			if ((attacking_entity)&&(attacking_entity->dynamic_target == this)) {
				attacking_entity->dynamic_target = nullptr;
				attacking_entity->state = IDLE;
			}
		}
		visionEntity->deleteEntity = true;

		if (delete_timer.ReadSec() > 4) {
			to_delete = true;
			App->entities->occupied_tiles[current_tile.x][current_tile.y] = false;
		}
		SpatialAudio(static_cast<int>(position.x), static_cast<int>(position.y), faction, state, type);
		break;
	default:
		break;
	}

	// -- If there are any particle then move and blits when current state equals hit
	if (DynaParticle != nullptr) {
		if (state == HIT) DynaParticle->Activate();
		else DynaParticle->Desactivate();
	}

	if (DynaParticle->IsActive()) {
		DynaParticle->Move(static_cast<int>(position.x) - 38, static_cast<int>(position.y) - 55);
		DynaParticle->Update(dt);
	}

	last_dt = dt;

	return ret;
}

void Gatherer::Gather() {
	//go back to base if resource building type is differrent from resource collected
	if ((resource_collected > 0) && (resource_type != resource_building->resource_type)) {
		if ((base != nullptr) && (base->state != DIE)) {
			PathfindToPosition(App->entities->ClosestTile(current_tile, base->tiles));
			gathering = true;
			return;
		}
	}

	uint resource = storage_capacity - resource_collected;

	if ((resource_building->quantity -= resource) < 0)
		resource = resource_building->quantity;

	resource_building->quantity -= resource;
	resource_collected += resource;
	resource_type = resource_building->resource_type;

	if (base != nullptr) {
		if (base->state == DIE) {
			state = IDLE;
			gathering = false;
			return;
		}

		PathfindToPosition(App->entities->ClosestTile(current_tile, base->tiles));
		gathering = true;
	}

	if (resource_building->quantity <= 0)
		resource_building = nullptr;
}

void Gatherer::AssignResourceBuilding(ResourceBuilding* g_resource_building) {
	resource_building = g_resource_building;
	gathering = true;
}

void Gatherer::StoreGatheredResources() {

	if (owner == App->player) {
		App->player->UpdateResourceData(resource_type, resource_collected);
	}
	else {
		//update owner resources
		if (resource_type == Resource::CAPS)
			owner->caps += resource_collected;
		else if (resource_type == Resource::WATER)
			owner->water += resource_collected;
		else if (resource_type == Resource::FOOD)
			owner->food += resource_collected;
	}

	resource_type = Resource::NO_TYPE;
	resource_collected = 0;
}

bool Gatherer::LoadDataFromReference() {
	bool ret = true;
	Gatherer* reference_gatherer = dynamic_cast<Gatherer*>(reference_entity);

	//load animations
	for(int i = 0; i < NO_STATE; i++)
	{
		for(int j = 0; j < NO_DIRECTION; j++)
		{
			animations[i][j] = reference_gatherer->animations[i][j];
		}
	}

	//load property data
	texture = reference_entity->texture;
	current_health = max_health = reference_entity->max_health;
	sprite_size = reference_entity->sprite_size;
	speed = reference_gatherer->speed;
	storage_capacity = reference_gatherer->storage_capacity;

	return ret;
}

bool Gatherer::LoadReferenceData(pugi::xml_node& node) {
	bool ret = true;

	max_health = node.attribute("health").as_float();
	speed.x = node.attribute("speed").as_float();
	speed.y = speed.x * 0.5f;
	storage_capacity = node.attribute("storage_capacity").as_int();

	return ret;
}
