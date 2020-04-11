#include "j1Player.h"
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
#include "j1Scene.h"

j1Player::j1Player() : GenericPlayer() {
	selected_entity = nullptr;
	border_scroll = false;
	mouse_speed_multiplier = 1.5f;

	caps = 0;
	food = 0;
	water = 0;

	god_mode = false;

	faction = VAULT;
}

j1Player::~j1Player() {}

bool j1Player::Start() {
	return true;
}

bool j1Player::PreUpdate() {
	bool ret = true;

	//debug keys

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		LOG("Water %i Caps: %i Food: %i", water, caps, food);

	//enable/disable debug mode
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		App->render->debug = !App->render->debug;

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		god_mode = !god_mode;

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


	//Deselect entity
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN) {
		selected_entity = nullptr;
		//Remove HUD data from the UI
		App->menu_manager->DestroyFaction(Menu::BUI_BASES, FACTION::ALL, BUILDING_TYPE::ALL);
		App->entities->count = 0;
	}

	if (!App->isPaused)
	{
		//entity selection and interaction
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) InteractWithEntity();
		

		//move camera
		if (App->input->GetMouseButtonDown(SDL_BUTTON_MIDDLE) == KEY_REPEAT) {
			int x, y;
			App->input->GetMouseMotion(x, y);
			App->render->camera.x += x * mouse_speed_multiplier;
			App->render->camera.y += y * mouse_speed_multiplier;
		}

		//move camera through minimap
		int mouse_x, mouse_y;
		if ((App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) || (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT))
		{
			App->input->GetMousePosition(mouse_x, mouse_y);
			SDL_Rect minimap = { App->minimap->position.x, App->minimap->position.y, App->minimap->width, App->minimap->height };

			if ((mouse_x > minimap.x) && (mouse_x < minimap.x + minimap.w) && (mouse_y > minimap.y) && (mouse_y < minimap.y + minimap.h))
			{
				iPoint minimap_mouse_position;
				minimap_mouse_position = App->minimap->ScreenToMinimapToWorld(mouse_x, mouse_y);
				App->render->camera.x = -(minimap_mouse_position.x - App->render->camera.w * 0.5f);
				App->render->camera.y = -(minimap_mouse_position.y - App->render->camera.h * 0.5f);
			}
		}
	}

	return ret;
}

bool j1Player::Update(float dt) {
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

	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");
	*/

	//Move map

	if ((App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)&&(App->render->camera.y < App->render->camera.h * 0.25f))
		App->render->camera.y += floor(200.0f * dt);

	if ((App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)&&(App->render->camera.y > -MAP_LENGTH * HALF_TILE + App->render->camera.h * 0.75f))
		App->render->camera.y -= floor(200.0f * dt);

	if ((App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) && (App->render->camera.x < MAP_LENGTH * HALF_TILE + App->render->camera.w * 0.25f))
		App->render->camera.x += floor(200.0f * dt);

	if ((App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) && (App->render->camera.x > -MAP_LENGTH* HALF_TILE + App->render->camera.w * 0.75f))
		App->render->camera.x -= floor(200.0f * dt);

	if (App->input->GetKey(SDL_SCANCODE_H) == KEY_REPEAT)
		caps += 100;

	return ret;
}

void j1Player::InteractWithEntity() {
	int tx, ty;
	iPoint selected_spot;

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
				if ((resource_building != nullptr)&&(resource_building->quantity > 0))
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

void j1Player::UpdateResourceData(Resource resource_type, int quantity) {
	//update resoucres
	if (resource_type == Resource::CAPS)
		caps += quantity;
	else if (resource_type == Resource::WATER)
		water += quantity;
	else if (resource_type == Resource::FOOD)
		food += quantity;	

	//update gui
	App->gui->DeleteArrayElements(App->menu_manager->gui_ingame, 4);
	App->menu_manager->CreateGUI();
}