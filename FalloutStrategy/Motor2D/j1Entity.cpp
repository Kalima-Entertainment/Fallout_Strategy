#include "j1Entity.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1EntityManager.h"
#include "j1Audio.h"
#include "j1Map.h"
#include "p2Log.h"
#include "j1Collision.h"

#include "j1Scene.h"
#include "SDL_mixer/include/SDL_mixer.h"

j1Entity::j1Entity() {

	position = { 0, 0 };
	current_tile = { 0, 0 };
	target_tile = { 0, 0 };

	spawnPosition = {NULL,NULL};

	max_health = current_health = 100;

	current_animation = nullptr;
	faction = VAULT;

	texture = nullptr;

	is_dynamic = false;
	to_delete = false;
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


void j1Entity::SpatialAudio(int fx, int channel, int positionx, int positiony) {

	Mix_Playing(channel);
	Mix_HaltChannel(channel);

	iPoint distance = { positionx - (-App->render->camera.x + App->render->camera.w / 2), positiony - (-App->render->camera.y + App->render->camera.h / 2 )};

	int distance_normalized = (distance.x * distance.x + distance.y * distance.y);
	distance_normalized = distance_normalized / 500;
	volume = (distance_normalized * 255) / App->render->camera.w;

	if (volume < 0) { volume = 0; }
	if (volume > 255) { volume = 255; }

	Mix_SetPosition(channel, 0, volume);
	App->audio->PlayFx(channel, fx, 0);

}
