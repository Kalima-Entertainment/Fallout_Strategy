#include "j1Entity.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1EntityManager.h"
#include "j1Audio.h"
#include "j1Map.h"
#include "p2Log.h"
#include "FoWManager.h"
#include "j1Player.h"
#include "j1Scene.h"
#include "SDL_mixer/include/SDL_mixer.h"

j1Entity::j1Entity() {
	spawnPosition = {NULL,NULL};

	max_health = current_health = 0;

	texture = nullptr;
	current_animation = nullptr;
	reference_entity = nullptr;
	attacking_entity = nullptr;
	owner = nullptr;

	state = NO_STATE;
	faction = NO_FACTION;
	type = NO_TYPE;

	is_dynamic = false;
	to_delete = false;
	particles_created = false;
	playing_fx = false;

	volume = 0;
	channel = 0;
	fx = 0;

	info = { false, nullptr, MovementState::MovementState_NoState,{0,0} };

	background_health_bar = { 0,0,0,0 };
	foreground_health_bar = { 0,0,0,0 };
	frame_quad = { 0,0,0,0 };

	last_dt = 0.01;
	sprite_size = -1;

	Mix_AllocateChannels(32);
}

j1Entity::~j1Entity() {
	reference_entity = nullptr;
	target_entity = nullptr;
	attacking_entity = nullptr;
	owner = nullptr;
	current_animation = nullptr;
	texture = nullptr;
}

// to be updated
iPoint j1Entity::MapPosition() {
	iPoint spot = App->render->ScreenToWorld(position.x, position.y);
	spot = App->map->WorldToMap(spot.x, spot.y);
	return spot;
}

void j1Entity::ClearUnitInfo() {
	info.IsSelected = false;
	info.current_group = nullptr;
	info.UnitMovementState = MovementState::MovementState_NoState;
	info.goal_tile = { 0,0 };
}

int j1Entity::GetPositionScore() const {
	return current_tile.x + current_tile.y;
}

void j1Entity::SpatialAudio(int positionx, int positiony, Faction faction, State state, EntityType type) {

	switch (state)
	{
	case WALK:
		if (faction == VAULT) { channel = 17; }
		else if (faction == BROTHERHOOD) { channel = 18; }
		else if (faction == MUTANT) { channel = 2; }
		else if (faction == GHOUL) { channel = 19; }
		fx = App->audio->Brotherhood_walk;

		if (type == MR_HANDY) {
			channel = 24;
			fx = App->audio->Mr_Handy_walk;
		}

		if (type == DEATHCLAW) {
			channel = 28;
			fx = App->audio->Deathclaw_walk;
		}

		if (type == BIGHORNER || type == BRAHAM) {
			channel = 21;
			fx = App->audio->Animal_walk;
		}
		break;
	case ATTACK:
		if (faction == VAULT) {
			if (type == RANGED) {
				channel = 16;
				fx = App->audio->pistol;
			}
			else if (type != RANGED) {
				channel = 4;
				fx = App->audio->Vault_attack;
			}
		}
		else if (faction == BROTHERHOOD) {
			if (type == RANGED) {
				channel = 15;
				fx = App->audio->minigun;
			}
			else if (type != RANGED) {
				channel = 5;
				fx = App->audio->Brotherhood_attack;
			}
		}
		else if (faction == MUTANT) {
			if (type == RANGED) {
				channel = 15;
				fx = App->audio->minigun;
			}
			else if (type != RANGED) {
				channel = 3;
				fx = App->audio->Mutant_attack;
			}
		}
		else if (faction == GHOUL) {
			if (type == RANGED) {
				channel = 20;
				fx = App->audio->pistol2;
			}
			else if (type != RANGED) {
				channel = 6;
				fx = App->audio->Ghoul_attack;
			}
		}
		if (type == MR_HANDY) {
			channel = 25;
			fx = App->audio->Mr_Handy_attack;
		}
		if (type == DEATHCLAW) {
			channel = 29;
			fx = App->audio->Deathclaw_attack;
		}

		break;
	case HIT:
		if (faction == MUTANT) {
			channel = 7;
			fx = App->audio->Mutant_hit;
		}
		else if (faction == VAULT) {
			channel = 8;
			fx = App->audio->Vault_hit;
		}
		else if (faction == BROTHERHOOD) {
			channel = 9;
			fx = App->audio->Brotherhood_hit;
		}
		else if (faction == GHOUL) {
			channel = 10;
			fx = App->audio->Ghoul_hit;
		}

		if (type == MR_HANDY) {
			channel = 26;
			fx = App->audio->Mr_Handy_hit;
		}

		if (type == DEATHCLAW) {
			channel = 30;
			fx = App->audio->Deathclaw_hit;
		}

		if (type == BIGHORNER || type == BRAHAM) {
			channel = 22;
			fx = App->audio->Animal_hit;
		}
		break;
	case DIE:
		if (faction == MUTANT) {
			channel = 11;
			fx = App->audio->Mutant_die;
		}
		else if (faction == VAULT) {
			channel = 12;
			fx = App->audio->Vault_die;
		}
		else if (faction == BROTHERHOOD) {
			channel = 13;
			fx = App->audio->Brotherhood_die;
		}
		else if (faction == GHOUL) {
			channel = 14;
			fx = App->audio->Ghoul_die;
		}

		if (type == MR_HANDY) {
			channel = 27;
			fx = App->audio->Mr_Handy_die;
		}

		if (type == DEATHCLAW) {
			channel = 31;
			fx = App->audio->Deathclaw_die;
		}

		if (type == BIGHORNER || type == BRAHAM) {
			channel = 23;
			fx = App->audio->Animal_die;
		}
		break;
	case NO_STATE:
		break;
	default:
		break;
	}

	if (Mix_Playing(channel) == 0) {
		Mix_HaltChannel(channel);

		iPoint distance = { positionx - (-App->render->camera.x + App->render->camera.w / 2), positiony - (-App->render->camera.y + App->render->camera.h / 2) };

		int distance_normalized = (distance.x * distance.x + distance.y * distance.y);
		distance_normalized = distance_normalized / 500;
		volume = (distance_normalized * 255) / App->render->camera.w;

		if (volume < 0) { volume = 0; }
		if (volume > 255) { volume = 255; }

		Mix_SetPosition(channel, 0, volume);
		App->audio->PlayFx(channel, fx, 0);
	}
}
