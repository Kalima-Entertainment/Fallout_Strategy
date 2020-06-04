#include "Deathclaw.h"
#include "j1Player.h"
#include "j1Map.h"
#include "StaticEntity.h"
#include "j1Player.h"
#include "j1Scene.h"
#include "SDL_mixer/include/SDL_mixer.h"

#include "Emiter.h"
#include "ParticleSystem.h"

Deathclaw::Deathclaw(iPoint g_current_tile) : DynamicEntity() {
	current_tile = g_current_tile;
	is_dynamic = true;
	is_agressive = true;
	position = App->map->floatMapToWorld(current_tile.x, current_tile.y);
	position.x += HALF_TILE;
	position.y += HALF_TILE;
	attack_time = 3;
	range = 1;
	attack_timer.Start();
	target_building = nullptr;
	target_entity = nullptr;
	DynaParticle = nullptr;
	type = DEATHCLAW;

	DynaParticle = App->entities->CreateParticle(position);
	Animation anim;
	anim.PushBack(SDL_Rect{ 0, 0 , 5, 5 }, 1);
	anim.Reset();
	Emiter Blood(position.x, position.y - 20, 0.2f, 0.2f, 5, 5, 0, 0, 0, 0, 2.0f, 2, 20, 0.4f, nullptr, App->entities->blood, anim, true);
	DynaParticle->PushEmiter(Blood);
	DynaParticle->Desactivate();
}

Deathclaw::~Deathclaw() {
	target_entity = nullptr;
	DynaParticle = nullptr;
	target_building = nullptr;
	//App->entities->ReleaseParticle(DynaParticle);
}

bool Deathclaw::Update(float dt) {
	bool ret = true;

	current_animation = &animations[state][direction];

	switch (state)
	{
    case IDLE:
		if (target_building) {
			PathfindToPosition(App->entities->ClosestTile(current_tile, target_building->tiles));
		}
		break;
    case WALK:
		Move(dt);

		if ((node_path.size() == 0) && (current_tile.DistanceManhattan(target_tile) <= range)) {
			//path_to_target.clear();
			state = ATTACK;
		}

		SpatialAudio(position.x, position.y, faction, state, type);

        break;
    case ATTACK:
		if (attack_timer.ReadSec() > attack_time) {
			current_animation->Reset();
			UpdateTile();
			Attack();
		}
		SpatialAudio(position.x, position.y, faction, state, type);
        break;
    case HIT:
		current_animation = &animations[HIT][direction];
		if (current_animation->Finished()) {
			current_animation->Reset();
			//PathfindToPosition(App->entities->ClosestTile(current_tile, target_building->tiles));
			state = WALK;
		}

		SpatialAudio(position.x, position.y, faction, state, type);
        break;
    case DIE:
		direction = TOP_LEFT;
		if (!delete_timer.Started()) {
			delete_timer.Start();
			direction = TOP_LEFT;
			if (attacking_entity) {
				attacking_entity->target_entity = nullptr;
				attacking_entity->state = IDLE;
			}
		}

		if (delete_timer.ReadSec() > 4) {
			to_delete = true;
			App->entities->occupied_tiles[current_tile.x][current_tile.y] = false;
		}
		SpatialAudio(position.x, position.y, faction, state, type);

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
		DynaParticle->Move(position.x, position.y);
		DynaParticle->Update(dt);
	}

	last_dt = dt;

	return ret;
}

void Deathclaw::Attack() {

	attack_timer.Start();

	if (target_building == nullptr) {
		state = DIE;
		return;
	}

	//damage unit if god_mode isn't activated
	if ((target_building->faction == App->player->faction) && (App->player->god_mode))
		return;

	target_building->current_health -= damage;

	if (target_building->current_health <= 0) {
		if (target_building->faction == App->player->faction) {
			state = IDLE;
		}
		else {
			path_to_target.clear();
			delete_timer.Start();
			state = DIE;
		}

		target_building->attacking_entity = this;
		target_building->state = DIE;
		target_building = nullptr;
	}
}

bool Deathclaw::LoadDataFromReference() {
	bool ret = true;
	Deathclaw* reference_deathclaw = (Deathclaw*)reference_entity;

	//load animations
	for(int i = 0; i < NO_STATE; i++)
	{
		for(int j = 0; j < NO_DIRECTION; j++)
		{
			animations[i][j] = reference_deathclaw->animations[i][j];
		}
	}

	//load property data
	current_health = max_health = reference_entity->max_health;
	damage = reference_deathclaw->damage;
	speed = reference_deathclaw->speed;
	sprite_size = reference_entity->sprite_size;
	texture = reference_entity->texture;

	return ret;
}

bool Deathclaw::LoadReferenceData(pugi::xml_node& node) {
	bool ret = true;

	max_health = node.attribute("health").as_float();
	damage = node.attribute("damage").as_int();
	speed.x = node.attribute("speed").as_int();
	speed.y = speed.x * 0.5f;

	return ret;
}