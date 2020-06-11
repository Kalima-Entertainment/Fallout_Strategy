#include "Animal.h"
#include "GenericPlayer.h"
#include "AI_Player.h"
#include "j1Map.h"
#include "j1Audio.h"
#include "SDL_mixer/include/SDL_mixer.h"

#include "ParticleSystem.h"
#include "Emiter.h"

Animal::Animal(EntityType g_type, iPoint g_current_tile) : DynamicEntity() {
	type = g_type;
	current_tile = g_current_tile;
	is_agressive = false;
	pasturing_time = 20;
	faction = NO_FACTION;
	food_quantity = 0;
	resource_spot = nullptr;

	position = App->map->floatMapToWorld(current_tile.x, current_tile.y);
	position.x += HALF_TILE;
	position.y += HALF_TILE;

	DynaParticle = App->entities->CreateParticle(position);
	Animation anim;
	anim.PushBack(SDL_Rect{ 0, 0 , 5, 5 }, 1);
	anim.Reset();
	Emiter Blood(position.x, position.y - 20, 0.2f, 0.2f, 5, 5, 0, 0, 0, 0, 2.0f, 2, 20, 0.4f, nullptr, App->entities->blood, anim, true);
	DynaParticle->PushEmiter(Blood);
	DynaParticle->Desactivate();
}

Animal::~Animal() {
	resource_spot = nullptr;
	DynaParticle = nullptr;
	//App->entities->ReleaseParticle(DynaParticle);
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

		if (current_tile == target_tile)
			state = IDLE;

		SpatialAudio(static_cast<int>(position.x), static_cast<int>(position.y), faction, state, type);

        break;
    case HIT:
		if (current_animation->Finished()) {
			current_animation->Reset();
			if (attacking_entity != nullptr) 
				Flee();
			else
				state = IDLE;
		}
		SpatialAudio(static_cast<int>(position.x), static_cast<int>(position.y), faction, state, type);
        break;

    case DIE:
		if (!delete_timer.Started()) {
			delete_timer.Start();
			direction = TOP_LEFT;

			resource_spot = App->entities->CreateResourceSpot(current_tile.x, current_tile.y, Resource::FOOD, food_quantity);
			App->entities->occupied_tiles[current_tile.x][current_tile.y] = false;
			path_to_target.clear();
			current_tile = { 149, 149 };
			next_tile = { -1,-1 };

			if ((attacking_entity != nullptr)&&(attacking_entity->owner->is_ai)) {
				((AI_Player*)attacking_entity->owner)->GatherFood(resource_spot);
				attacking_entity = nullptr;
			}
		}

		if ((resource_spot->quantity <= 0) && (delete_timer.ReadSec() > 4)) {
			App->entities->DestroyResourceSpot(resource_spot);
			resource_spot = nullptr;
			to_delete = true;
		}
		if (App->audio->die_sound == false) {
			SpatialAudio(static_cast<int>(position.x), static_cast<int>(position.y), faction, state, type);
			App->audio->die_sound = true;
		}
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
		DynaParticle->Move(static_cast<int>(position.x), static_cast<int>(position.y));
		DynaParticle->Update(dt);
	}

	last_dt = dt;

	return ret;
}

bool Animal::LoadDataFromReference() {
	bool ret = true;
	Animal* reference_animal = dynamic_cast<Animal*>(reference_entity);

	//load animations
	for(int i = 0; i < NO_STATE; i++)
	{
		for(int j = 0; j < NO_DIRECTION; j++)
		{
			animations[i][j] = reference_animal->animations[i][j];
		}
	}

	//load property data
	current_health = max_health = reference_entity->max_health;
	food_quantity = reference_animal->food_quantity;
	speed = reference_animal->speed;
	sprite_size = reference_entity->sprite_size;
	texture = reference_animal->texture;

	return ret;
}

bool Animal::LoadReferenceData(pugi::xml_node& node) {
	bool ret = true;

	max_health = node.attribute("health").as_float();
	food_quantity = node.attribute("resource_quantity").as_int();
	speed.x = node.attribute("speed").as_float();
	speed.y = speed.x * 0.5f;

	return ret;
}