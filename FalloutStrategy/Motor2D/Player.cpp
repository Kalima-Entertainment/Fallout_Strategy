#include "Player.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1EntityManager.h"
#include "j1Map.h"
#include "j1Entity.h"
#include "DynamicEntity.h"
#include "StaticEntity.h"
#include "p2Log.h"
#include "UI_Label.h"
#include "j1Gui.h"
#include "j1Minimap.h"
#include "MenuManager.h"

Player::Player() : j1Module() {
	selected_entity = nullptr;
	border_scroll = false;
	mouse_speed_multiplier = 1.5f;

	caps = 0;
	food = 0;
	water = 0;
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

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		LOG("Water %i Caps: %i Food: %i", water, caps, food);

	//enable/disable debug mode
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		App->render->debug = !App->render->debug;
	
	//block border scroll
	if (App->input->GetKey(SDL_SCANCODE_Y) == KEY_DOWN)
		border_scroll = !border_scroll;

	//center camera
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		if (selected_entity != nullptr)
		{
			App->render->camera.x = -selected_entity->position.x + (App->render->camera.w * 0.5f);
			App->render->camera.y = -selected_entity->position.y + (App->render->camera.h * 0.5f);
		}
		else
		{
			App->render->camera.x = 0;
			App->render->camera.y = 0;
		}
	}


	

	//movement
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
		App->input->GetMousePosition(tx, ty);

		selected_spot = App->render->ScreenToWorld(tx, ty);
		selected_spot = App->map->WorldToMap(selected_spot.x, selected_spot.y);

		//check if there's an entity in the selected spot
		j1Entity* target;
		target = App->entities->FindEntityByTile(selected_spot);

		//if we hadn't any entity selected
		if (selected_entity == nullptr)
		{
			if (target != nullptr) {
				selected_entity = target;
			}
		}

		//if we had one
		else
		{
			//dynamic entities
			if (selected_entity->is_dynamic)
			{
				DynamicEntity* dynamic_entity;
				dynamic_entity = (DynamicEntity*)selected_entity;
				dynamic_entity->PathfindToPosition(selected_spot);
				dynamic_entity->target_tile = selected_spot;
				dynamic_entity->state = WALK;

				if (target != nullptr) {
					//assign a dynamic target to the entity
					if (target->is_dynamic)
						dynamic_entity->target_entity = (DynamicEntity*)target;
					//assign a static target to the entity
					else
						dynamic_entity->target_building = (StaticEntity*)target;
				}
				else {					
					dynamic_entity->target_entity = nullptr;
					ResourceBuilding* resource_building;
					resource_building = App->entities->FindResourceBuildingByTile(selected_spot);
					//assign a resource building to the entity
					if (resource_building != nullptr)
						dynamic_entity->resource_building = resource_building;
				}
			}
			//static entities
			else
			{
				StaticEntity* static_entity;
				static_entity = (StaticEntity*)selected_entity;				
			}
		}
	}

	//deselect entity
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN) {
		selected_entity = nullptr;

		//Remove HUD data from the UI
		App->menu_manager->DestroyAllBuildings();
		App->entities->count = 0;
	}

	//move camera
	int mouse_x, mouse_y;
	if ((App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) || (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT))
	{
		App->input->GetMousePosition(mouse_x, mouse_y);
		SDL_Rect minimap = { App->minimap->position.x, App->minimap->position.y, App->minimap->width, App->minimap->height };

		if ((mouse_x > minimap.x) && (mouse_x < minimap.x + minimap.w) && (mouse_y > minimap.y) && (mouse_y < minimap.y + minimap.h))
		{
			iPoint minimap_mouse_position;
			minimap_mouse_position = App->minimap->ScreenToMinimapToWorld(mouse_x, mouse_y);
			//LOG("Minimap position: x: %i y: %i", minimap_mouse_position.x, minimap_mouse_position.y);
			App->render->camera.x = -(minimap_mouse_position.x - App->render->camera.w * 0.5f);
			App->render->camera.y = -(minimap_mouse_position.y - App->render->camera.h * 0.5f);
		}
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_MIDDLE) == KEY_REPEAT) {
		int x, y;
		App->input->GetMouseMotion(x, y);
		App->render->camera.x += x * mouse_speed_multiplier;
		App->render->camera.y += y * mouse_speed_multiplier;
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

	/*
	//Zoom in, zoom out
	uint zoom;
	App->input->GetMouseWheel(zoom);
	if (zoom != 0)App->win->SetScale(zoom);	//Check this condition
	//LOG("WHEEL VALUE %i", zoom);
	*/

	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	//Move map

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += floor(200.0f * dt);

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= floor(200.0f * dt);

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += floor(200.0f * dt);

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= floor(200.0f * dt);

	return ret;
}
