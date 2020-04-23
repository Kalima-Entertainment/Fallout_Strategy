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
#include "j1Console.h"

j1Player::j1Player() : GenericPlayer() {
	selected_entity = last_selected_entity = nullptr;
	border_scroll = false;
	mouse_speed_multiplier = 1.5f;

	base = barrack[0] = barrack[1] = laboratory = nullptr;

	caps = 100;
	food = 100;
	water = 100;

	god_mode = false;

	//faction = VAULT;
	defeated = false;
}

j1Player::~j1Player() {
	selected_entity = nullptr;
	last_selected_entity = nullptr;
	troops.clear();
	gatherers_vector.clear();
	base = barrack[0] = barrack[1] = laboratory = nullptr;
}

bool j1Player::Start() {
	App->console->CreateCommand("caps+", "increase the amount of caps", this);
	App->console->CreateCommand("food+", "increase the amount of food", this);
	App->console->CreateCommand("water+", "increase the amount of water", this);
	App->console->CreateCommand("resources+", "increase all resources", this);
	App->console->CreateCommand("god_mode", "turn god mode on and off", this);
	App->console->CreateCommand("spawn_units", "spawn 1 gatherer, 1 melee and 1 ranged. Must have a building selected", this);	
	App->console->CreateCommand("spawn", "<spawn gatherer><spawn melee><spawn ranged><spawn army>. Spawn one unit or an army. Must have a building selected", this);
	return true;
}

bool j1Player::PreUpdate() {
	bool ret = true;

	//debug keys

	//enable/disable debug mode
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		App->render->debug = !App->render->debug;

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		god_mode = !god_mode;
		if (god_mode) LOG("God Mode: ON");
		else LOG("God Mode: OFF");
	}

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

	if (!App->isPaused)
	{
		//entity selection and interaction
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
			selected_entity = SelectEntity();

			if (App->entities->showing_building_menu && selected_entity == nullptr) {
				App->menu_manager->DestroyFaction(Menu::BUI_BASES, FACTION::ALL, BUILDING_TYPE::ALL);
				App->entities->showing_building_menu = false;
			}
		}

		if ((App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)&&(selected_entity != nullptr)) {
			if(selected_entity->is_dynamic)
				MoveEntity();
		}

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

	// --- We get the map coords of the mouse ---
	Map_mouseposition = App->map->WorldToMap((int)App->scene->mouse_pos.x, (int)App->scene->mouse_pos.y);


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

	return ret;
}

j1Entity* j1Player::SelectEntity() {
	int tx, ty;
	iPoint selected_spot;
	App->input->GetMousePosition(tx, ty);
	if (TouchingUI(tx, ty))
		return selected_entity;

	selected_spot = App->render->ScreenToWorld(tx, ty);
	selected_spot = App->map->WorldToMap(selected_spot.x, selected_spot.y);

	//check if there's an entity in the selected spot
	j1Entity* target = App->entities->FindEntityByTile(selected_spot);

	if (target != nullptr) {
		if ((god_mode) || (target->faction == faction)) {
			return target;
		}
	}

	if (selected_entity != nullptr) {
		last_selected_entity = selected_entity;
	}

	return nullptr;
}

void j1Player::MoveEntity(){
	int tx, ty;
	iPoint selected_spot;

	App->input->GetMousePosition(tx, ty);

	if (TouchingUI(tx,ty))
		return;

	selected_spot = App->render->ScreenToWorld(tx, ty);
	selected_spot = App->map->WorldToMap(selected_spot.x, selected_spot.y);

	j1Entity* target = App->entities->FindEntityByTile(selected_spot);

	//dynamic entities
	if (selected_entity->is_dynamic)
	{
		DynamicEntity* dynamic_entity;
		dynamic_entity = (DynamicEntity*)selected_entity;
		dynamic_entity->PathfindToPosition(selected_spot);
		dynamic_entity->state = WALK;

		if (target != nullptr) {
			dynamic_entity->target_entity = target;
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

void j1Player::UpdateResourceData(Resource resource_type, int quantity) {
	//update resoucres
	if (resource_type == Resource::CAPS)
		caps += quantity;
	else if (resource_type == Resource::WATER)
		water += quantity;
	else if (resource_type == Resource::FOOD)
		food += quantity;

	//update gui
	App->menu_manager->DestroyMenu(Menu::RESOURCES);
	App->menu_manager->CreateResources();
}

void j1Player::OnCommand(std::vector<std::string> command_parts) {
	std::string command_beginning = command_parts[0];

	//Increase all resources
	if (command_beginning == "resources+") {
		int resources_increase = std::stoi(command_parts[1].c_str());

		UpdateResourceData(Resource::CAPS, resources_increase);
		UpdateResourceData(Resource::FOOD, resources_increase);
		UpdateResourceData(Resource::WATER, resources_increase);
		LOG("All resources increased");
	}

	if (command_beginning == "caps+") {
		int caps_increase = std::stoi(command_parts[1].c_str());

		UpdateResourceData(Resource::CAPS, caps_increase);
		LOG("Caps increased");
	}

	if (command_beginning == "food+") {
		int food_increase = std::stoi(command_parts[1].c_str());

		UpdateResourceData(Resource::FOOD, food_increase);
		LOG("Food increased");
	}

	if (command_beginning == "water+") {
		int water_increase = std::stoi(command_parts[1].c_str());

		UpdateResourceData(Resource::WATER, water_increase);
		LOG("Water increased");
	}

	if (command_beginning == "god_mode") {
		if (command_parts[1] == "on") { 
			god_mode = true;
			LOG("God mode turned on");
		}
		if (command_parts[1] == "off") { 
			god_mode = false; 
			LOG("God mode turned off");
		}		
	}

	if (command_beginning == "spawn_units") {
		StaticEntity* static_entity;
		if (selected_entity == nullptr)
			static_entity = (StaticEntity*)last_selected_entity;
		else
			static_entity = (StaticEntity*)selected_entity;
		
		if (static_entity != nullptr) {
			App->entities->CreateEntity(static_entity->faction, GATHERER, static_entity->spawnPosition.x, static_entity->spawnPosition.y);
			App->entities->CreateEntity(static_entity->faction, MELEE, static_entity->spawnPosition.x, static_entity->spawnPosition.y);
			App->entities->CreateEntity(static_entity->faction, RANGED, static_entity->spawnPosition.x, static_entity->spawnPosition.y);

			LOG("1 unit from each type spawned successfully");
		}		
	}

	if (command_beginning == "spawn") {

		StaticEntity* static_entity;
		if (selected_entity == nullptr)
			static_entity = (StaticEntity*)last_selected_entity;
		else
			static_entity = (StaticEntity*)selected_entity;

		if (static_entity != nullptr)
		{
			if (command_parts[1] == "gatherer")
				App->entities->CreateEntity(static_entity->faction, GATHERER, static_entity->spawnPosition.x, static_entity->spawnPosition.y);
			if (command_parts[1] == "melee")
				App->entities->CreateEntity(static_entity->faction, MELEE, static_entity->spawnPosition.x, static_entity->spawnPosition.y);
			if (command_parts[1] == "ranged")
				App->entities->CreateEntity(static_entity->faction, RANGED, static_entity->spawnPosition.x, static_entity->spawnPosition.y);
			if (command_parts[1] == "army")
				for (int i = 0; i < 10; i++) {
					App->entities->CreateEntity(static_entity->faction, MELEE, static_entity->spawnPosition.x, static_entity->spawnPosition.y);
					App->entities->CreateEntity(static_entity->faction, RANGED, static_entity->spawnPosition.x, static_entity->spawnPosition.y);
				}

			LOG("Unit spawned");
		}
		else
			LOG("You must select a building while executing this command");
	}
}

bool j1Player::TouchingUI(int x, int y) {
	bool ret = false;
	if (y > App->minimap->position.y - 8) { ret = true; }
	return ret;
}
