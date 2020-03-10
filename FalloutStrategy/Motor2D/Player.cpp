#include "Player.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1EntityManager.h"
#include "j1Map.h"
#include "j1Entity.h"
#include "DynamicEntity.h"
#include "p2Log.h"

Player::Player() : j1Module() {
	selected_entity = nullptr;
	border_scroll = false;
}

Player::~Player() {}

bool Player::Start() {
	return true;
}

bool Player::PreUpdate() {
	bool ret = true;
	int tx, ty;
	iPoint selected_spot;

	//debug keys
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		App->render->debug = !App->render->debug;
	
	if (App->input->GetKey(SDL_SCANCODE_Y) == KEY_DOWN)
		border_scroll = !border_scroll;

	//Movement
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
		App->input->GetMousePosition(tx, ty);

		selected_spot = App->render->ScreenToWorld(tx, ty);
		selected_spot = App->map->WorldToMap(selected_spot.x, selected_spot.y);

		LOG("Actual Map Position is X: %i and Y: %i", selected_spot.x, selected_spot.y);
		if ((selected_entity != nullptr)&&(selected_spot != selected_entity->current_tile))
		{
			//choose a tile for the entity to go to
			selected_entity->target_tile = selected_spot;
			selected_entity->PathfindToPosition(selected_spot);
			
			//check if there is an entity there
			j1Entity* target = nullptr;
			target = App->entities->FindEntityByTile(selected_spot);

			//if there is an enemy in that tile save it
			if ( target != nullptr)
			{
				selected_entity->target_entity = target;
			}
			else
			{
				selected_entity->state = WALK;
			}

		}

		for (int i = REFERENCE_ENTITIES; i < App->entities->entities.size(); i++)
		{
			if (App->entities->entities[i]->current_tile == selected_spot) {
				LOG("COINCIDENCE IN MAP");
				selected_entity = (DynamicEntity*)App->entities->entities[i];
				break;
			}
		}
	}

	//deselect entity
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN) {
		selected_entity = nullptr;
	}

	return ret;
}

bool Player::Update(float dt) {
	bool ret = true;

	//Margin camera movement
	int x, y;
	App->input->GetMousePosition(x, y);
	uint width, height;
	App->win->GetWindowSize(width, height);

	if (border_scroll)
	{

		if (x < 40) App->render->camera.x += floor(600.0f * dt);
		if (x > width - 40) App->render->camera.x -= floor(600.0f * dt);
		if (y < 40) App->render->camera.y += floor(600.0f * dt);
		if (y > height - 40) App->render->camera.y -= floor(600.0f * dt);
	}

	//Zoom in, zoom out
	uint zoom;
	App->input->GetMouseWheel(zoom);
	if (zoom != 0)App->win->SetScale(zoom);	//Check this condition
	//LOG("WHEEL VALUE %i", zoom);
	
	return ret;
}

bool Player::PostUpdate() {
	bool ret = true;
	return ret;
}