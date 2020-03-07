#include "Player.h"
#include "j1App.h"
#include "j1EntityManager.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Entity.h"

Player::Player() : j1Module() {}

Player::~Player() {}

bool Player::Start() {
	return true;
}

bool Player::PreUpdate() {
	bool ret = true;
	int tx, ty;
	iPoint selected_spot;
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
		App->input->GetMousePosition(tx, ty);

		//TODO: ScreenToMap
		selected_spot = App->render->ScreenToWorld(tx, ty);
		selected_spot = App->map->WorldToMap(selected_spot.x, selected_spot.y);

		LOG("Actual Map Position is X: %i and Y: %i", selected_spot.x, selected_spot.y);
		if ((selected_entity != nullptr)&&(selected_spot != selected_entity->current_tile))
		{
			selected_entity->target_tile = selected_spot;
			selected_entity->state = WALK;
		}

		for (int i = REFERENCE_ENTITIES; i < App->entities->entities.size(); i++)
		{
			if (App->entities->entities[i]->current_tile == selected_spot) {
				LOG("COINCIDENCE IN MAP");
				selected_entity = App->entities->entities[i];
				break;
			}
		}
	}
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN) {
		selected_entity = nullptr;
	}
	return ret;
}

bool Player::Update(float dt) {
	bool ret = true;
	return ret;
}

bool Player::PostUpdate() {
	bool ret = true;
	return ret;
}