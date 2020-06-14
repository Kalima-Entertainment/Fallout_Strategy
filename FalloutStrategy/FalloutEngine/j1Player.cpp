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
#include "UI_Button.h"
#include "j1Gui.h"
#include "j1Minimap.h"
#include "j1Image.h"
#include "MenuManager.h"
#include "j1Scene.h"
#include "j1Console.h"
#include "Gatherer.h"
#include "FoWManager.h"

j1Player::j1Player() : GenericPlayer() {

	name.assign("Player");

	selected_entity = nullptr;
	last_selected_entity = nullptr;
	selected_group = nullptr;
	border_scroll = false;
	mouse_speed_multiplier = 1.5f;
	resource_fow_added = false;

	god_mode = false;

	qcaps = false;
	qwater = false;
	qfood = false;

	reward = 0;

	is_ai = false;

	Map_mouseposition = {1,1};
}

j1Player::~j1Player() {
	selected_entity = nullptr;
	last_selected_entity = nullptr;
	selected_group = nullptr;

	for(size_t t = 0; t < troops.size(); t++) { troops[t] = nullptr; }
	troops.clear();

	for(size_t g = 0; g < gatherers_vector.size(); g++) { gatherers_vector[g] = nullptr; }
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
	defeated = false;

	return true;
}

bool j1Player::PreUpdate() {
	bool ret = true;

	//debug keys -------------------------------------------------------

	//enable/disable debug mode
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		App->render->debug = !App->render->debug;

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		god_mode = !god_mode;
		if (god_mode) LOG("God Mode: ON");
		else LOG("God Mode: OFF");
	}

	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) {
		App->entities->CreateEntity(faction, MR_HANDY, 1, 1, this);
	}

	if (base != nullptr && resource_fow_added == false) {
		App->fowManager->AddFowToResourceBuildings(base->current_tile);
		resource_fow_added = true;
	}

	if (!App->isPaused)
	{
		//keyboard keys ------------------------------------------------------
		//block border scroll
		if (App->input->GetKey(SDL_SCANCODE_Y) == KEY_DOWN)
			border_scroll = !border_scroll;

		//center camera
		if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
		{
			if (selected_entity != nullptr)
			{
				App->render->camera.x = (int)(-selected_entity->position.x + (App->render->camera.w * 0.5f));
				App->render->camera.y = (int)(-selected_entity->position.y + (App->render->camera.h * 0.5f));
			}
			else
			{
				App->render->camera.x = 0;
				App->render->camera.y = 0;
			}
		}

		//use radar
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
			App->minimap->EnableRadar();

		//mouse --------------------------------------------------------------------------
		App->input->GetMousePosition(mouse_position.x, mouse_position.y);
		//entity selection and interaction
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
			selected_entity = SelectEntity();

			if ((selected_group != nullptr)) 
			{
				if (!TouchingUI(mouse_position.x, mouse_position.y)) {
					selected_group->DeselectGroup();
					selected_group = nullptr;
				}
			}

			if (App->entities->showing_building_menu) {
				if (selected_entity == nullptr) {
					App->menu_manager->DestroyFaction(Menu::BUI_BASES, App->menu_manager->current_building_faction, App->menu_manager->current_building_type);
					App->entities->showing_building_menu = false;
				}
				else
				{
					if ((App->entities->showing_building_menu) && (last_selected_entity != selected_entity)) {
					App->menu_manager->DestroyFaction(Menu::BUI_BASES, App->menu_manager->current_building_faction, App->menu_manager->current_building_type);
					App->menu_manager->CreateMenuFaction(Menu::BUI_BASES, selected_entity->faction, selected_entity->type);
					}
				}
				last_selected_entity = selected_entity;
			}
		}


		//move camera
		if (App->input->GetMouseButtonDown(SDL_BUTTON_MIDDLE) == KEY_REPEAT) {
			int x, y;
			App->input->GetMouseMotion(x, y);
			App->render->camera.x += (int)(x * mouse_speed_multiplier);
			App->render->camera.y += (int)(y * mouse_speed_multiplier);
			// CAMERA LIMITS X
			if (App->render->camera.x >= 5070)
			{
				App->render->camera.x = 5070;
			}
			if (App->render->camera.x <= -3695)
			{
				App->render->camera.x = -3695;
			}
			// CAMERA LIMITS Y
			if (App->render->camera.y >= 314)
			{
				App->render->camera.y = 314;
			}
			if (App->render->camera.y <= -4452)
			{
				App->render->camera.y = -4452;
			}
		}

		//move camera through minimap

		if ((App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) || (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) && (App->minimap->IsVisible()))
		{
			SDL_Rect minimap = { App->minimap->position.x, App->minimap->position.y, App->minimap->width, App->minimap->height };

			if ((mouse_position.x > minimap.x) && (mouse_position.x < minimap.x + minimap.w) && (mouse_position.y > minimap.y) && (mouse_position.y < minimap.y + minimap.h))
			{
				iPoint minimap_mouse_position;
				minimap_mouse_position = App->minimap->ScreenToMinimapToWorld(mouse_position.x, mouse_position.y);
				App->render->camera.x = (int)(-(minimap_mouse_position.x - App->render->camera.w * 0.5f));
				App->render->camera.y = (int)(-(minimap_mouse_position.y - App->render->camera.h * 0.5f));
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
		if (x < 40) App->render->camera.x += (int)(floor(600.0f * dt));
		if (x > width - 40) App->render->camera.x -= (int)(floor(600.0f * dt));
		if (y < 40) App->render->camera.y += (int)(floor(600.0f * dt));
		if (y > height - 40) App->render->camera.y -= (int)(floor(600.0f * dt));
	}

	// --- We get the map coords of the mouse ---
	Map_mouseposition = App->map->WorldToMap((int)App->scene->mouse_pos.x, (int)App->scene->mouse_pos.y);

	if (qcaps == false) {

		if (caps >= 1000) {

			if (App->gui->open == false) {
				App->menu_manager->quest[8] = (j1Image*)App->gui->CreateImage(-274, 200, Image, { 3061, 619, 30, 27 }, NULL, this);
				reward++;
				if (reward == 1) App->menu_manager->quest[3] = (j1Image*)App->gui->CreateImage(-257, 261, Image, { 3155, 809, 60, 17 }, NULL, this);
				if (reward == 2) App->menu_manager->quest[4] = (j1Image*)App->gui->CreateImage(-193, 261, Image, { 3219, 809, 63, 17 }, NULL, this);
				if (reward == 3) {
					App->menu_manager->quest[5] = (j1Image*)App->gui->CreateImage(-126, 261, Image, { 3286, 809, 51, 17 }, NULL, this);
					App->menu_manager->quest[9] = (j1Image*)App->gui->CreateImage(480, 180, Image, { 1226, 559, 282, 262 }, NULL, this);
					App->menu_manager->quest[10] = (UI_Button*)App->gui->CreateButton(500, 450, continue_button, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,244,64 }, NULL, this);
					App->menu_manager->quest[11] = (UI_Label*)App->gui->CreateLabel(550, 470, Label, "CONTINUE", NULL, this);
					App->entities->CreateEntity(App->player->faction, MR_HANDY, 75, 75, App->player);
				}
			}
			else if (App->gui->open == true) {
				App->menu_manager->quest[8] = (j1Image*)App->gui->CreateImage(33, 200, Image, { 3061, 619, 30, 27 }, NULL, this);
				reward++;
				if (reward == 1) App->menu_manager->quest[3] = (j1Image*)App->gui->CreateImage(50, 261, Image, { 3155, 809, 60, 17 }, NULL, this);
				if (reward == 2) App->menu_manager->quest[4] = (j1Image*)App->gui->CreateImage(114, 261, Image, { 3219, 809, 63, 17 }, NULL, this);
				if (reward == 3) {
					App->menu_manager->quest[5] = (j1Image*)App->gui->CreateImage(181, 261, Image, { 3286, 809, 51, 17 }, NULL, this);
					App->menu_manager->quest[9] = (j1Image*)App->gui->CreateImage(480, 180, Image, { 1226, 559, 282, 262 }, NULL, this);
					App->menu_manager->quest[10] = (UI_Button*)App->gui->CreateButton(500, 450, continue_button, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,244,64 }, NULL, this);
					App->menu_manager->quest[11] = (UI_Label*)App->gui->CreateLabel(550, 470, Label, "CONTINUE", NULL, this);
					App->entities->CreateEntity(App->player->faction, MR_HANDY, 75, 75, App->player);
				}
			}

			qcaps = true;
		}
	}

	if (qwater == false) {

		if (water >= 1000) {

			if (App->gui->open == false) {
				App->menu_manager->quest[6] = (j1Image*)App->gui->CreateImage(-274, 120, Image, { 3061, 619, 30, 27 }, NULL, this);
				reward++;
				if (reward == 1) App->menu_manager->quest[3] = (j1Image*)App->gui->CreateImage(-257, 261, Image, { 3155, 809, 60, 17 }, NULL, this);
				if (reward == 2) App->menu_manager->quest[4] = (j1Image*)App->gui->CreateImage(-193, 261, Image, { 3219, 809, 63, 17 }, NULL, this);
				if (reward == 3) {
					App->menu_manager->quest[5] = (j1Image*)App->gui->CreateImage(-126, 261, Image, { 3286, 809, 51, 17 }, NULL, this);
					App->menu_manager->quest[9] = (j1Image*)App->gui->CreateImage(480, 180, Image, { 1226, 559, 282, 262 }, NULL, this);
					App->menu_manager->quest[10] = (UI_Button*)App->gui->CreateButton(500, 450, continue_button, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,244,64 }, NULL, this);
					App->menu_manager->quest[11] = (UI_Label*)App->gui->CreateLabel(550, 470, Label, "CONTINUE", NULL, this);
					App->entities->CreateEntity(App->player->faction, MR_HANDY, 75, 75, App->player);
				}
			}
			else if (App->gui->open == true) {
				App->menu_manager->quest[6] = (j1Image*)App->gui->CreateImage(33, 120, Image, { 3061, 619, 30, 27 }, NULL, this);
				reward++;
				if (reward == 1) App->menu_manager->quest[3] = (j1Image*)App->gui->CreateImage(50, 261, Image, { 3155, 809, 60, 17 }, NULL, this);
				if (reward == 2) App->menu_manager->quest[4] = (j1Image*)App->gui->CreateImage(114, 261, Image, { 3219, 809, 63, 17 }, NULL, this);
				if (reward == 3) {
					App->menu_manager->quest[5] = (j1Image*)App->gui->CreateImage(181, 261, Image, { 3286, 809, 51, 17 }, NULL, this);
					App->menu_manager->quest[9] = (j1Image*)App->gui->CreateImage(480, 180, Image, { 1226, 559, 282, 262 }, NULL, this);
					App->menu_manager->quest[10] = (UI_Button*)App->gui->CreateButton(500, 450, continue_button, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,244,64 }, NULL, this);
					App->menu_manager->quest[11] = (UI_Label*)App->gui->CreateLabel(550, 470, Label, "CONTINUE", NULL, this);
					App->entities->CreateEntity(App->player->faction, MR_HANDY, 75, 75, App->player);
				}
			}

			qwater = true;
		}
	}

	if (qfood == false) {

		if (food >= 400) {

			if (App->gui->open == false) {
				App->menu_manager->quest[7] = (j1Image*)App->gui->CreateImage(-274, 160, Image, { 3061, 619, 30, 27 }, NULL, this);
				reward++;
				if (reward == 1) App->menu_manager->quest[3] = (j1Image*)App->gui->CreateImage(-257, 261, Image, { 3155, 809, 60, 17 }, NULL, this);
				if (reward == 2) App->menu_manager->quest[4] = (j1Image*)App->gui->CreateImage(-193, 261, Image, { 3219, 809, 63, 17 }, NULL, this);
				if (reward == 3) {
					App->menu_manager->quest[5] = dynamic_cast<j1Image*>(App->gui->CreateImage(-126, 261, Image, { 3286, 809, 51, 17 }, NULL, this));
					App->menu_manager->quest[9] = dynamic_cast<j1Image*>(App->gui->CreateImage(480, 180, Image, { 1226, 559, 282, 262 }, NULL, this));
					App->menu_manager->quest[10] = dynamic_cast<UI_Button*>(App->gui->CreateButton(500, 450, continue_button, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,244,64 }, NULL, this));
					App->menu_manager->quest[11] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(550, 470, Label, "CONTINUE", NULL, this));
					App->entities->CreateEntity(App->player->faction, MR_HANDY, 75, 75, App->player);
				}
			}
			else if (App->gui->open == true) {
				App->menu_manager->quest[7] = (j1Image*)App->gui->CreateImage(33, 160, Image, { 3061, 619, 30, 27 }, NULL, this);
				reward++;
				if (reward == 1) App->menu_manager->quest[3] = (j1Image*)App->gui->CreateImage(50, 261, Image, { 3155, 809, 60, 17 }, NULL, this);
				if (reward == 2) App->menu_manager->quest[4] = (j1Image*)App->gui->CreateImage(114, 261, Image, { 3219, 809, 63, 17 }, NULL, this);
				if (reward == 3) {
					App->menu_manager->quest[5] = (j1Image*)App->gui->CreateImage(181, 261, Image, { 3286, 809, 51, 17 }, NULL, this);
					App->menu_manager->quest[9] = (j1Image*)App->gui->CreateImage(480, 180, Image, { 1226, 559, 282, 262 }, NULL, this);
					App->menu_manager->quest[10] = (UI_Button*)App->gui->CreateButton(500, 450, continue_button, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,244,64 }, NULL, this);
					App->menu_manager->quest[11] = (UI_Label*)App->gui->CreateLabel(550, 470, Label, "CONTINUE", NULL, this);
					App->entities->CreateEntity(App->player->faction, MR_HANDY, 75, 75, App->player);
				}
			}

			qfood = true;
		}
	}

	iPoint selected_spot;
	App->input->GetMousePosition(selected_spot.x, selected_spot.y);

	if ((App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN) && ((selected_entity != nullptr)||(selected_group != nullptr))) {

		App->scene->blit_destination = true;
		App->scene->debug_destiny = selected_spot;

		if ((selected_entity != nullptr)&&(selected_entity->is_dynamic))
			MoveEntity(dynamic_cast<DynamicEntity*>(selected_entity));

		if (selected_group != nullptr)
			MoveGroup();
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN) {
		if (TouchingUI(selected_spot.x, selected_spot.y)) {
			if ((selected_spot.x > App->minimap->position.x) && (selected_spot.x < App->minimap->position.x + App->minimap->width)
				&& (selected_spot.y > App->minimap->position.y) && (selected_spot.y < App->minimap->position.y + App->minimap->height)) {

				selected_spot = App->minimap->ScreenToMinimapToWorld(selected_spot.x, selected_spot.y);
				selected_spot = App->map->WorldToMap(selected_spot.x, selected_spot.y);
				Map_mouseposition = selected_spot;
			}
		}
	}

	//Move map
	if ((App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)&&(App->render->camera.y < App->render->camera.h * 0.25f))
		App->render->camera.y += (int)(floor(200.0f * dt));

	if ((App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)&&(App->render->camera.y > -MAP_LENGTH * HALF_TILE + App->render->camera.h * 0.75f))
		App->render->camera.y -= (int)(floor(200.0f * dt));

	if ((App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) && (App->render->camera.x < MAP_LENGTH * HALF_TILE + App->render->camera.w * 0.25f))
		App->render->camera.x += (int)(floor(200.0f * dt));

	if ((App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) && (App->render->camera.x > -MAP_LENGTH* HALF_TILE + App->render->camera.w * 0.75f))
		App->render->camera.x -= (int)(floor(200.0f * dt));

	return ret;
}

bool j1Player::CleanUp() {
	bool ret = true;
	selected_entity = nullptr;
	last_selected_entity = nullptr;
	selected_group = nullptr;

	for (size_t t = 0; t < troops.size(); t++) { troops[t] = nullptr; }
	troops.clear();

	for (size_t g = 0; g < gatherers_vector.size(); g++) { gatherers_vector[g] = nullptr; }
	gatherers_vector.clear();

	base = barrack[0] = barrack[1] = laboratory = nullptr;
	return ret;
}

j1Entity* j1Player::SelectEntity() {
	iPoint selected_spot;
	if (TouchingUI(mouse_position.x, mouse_position.y))
		return selected_entity;

	selected_spot = App->render->ScreenToWorld(mouse_position.x, mouse_position.y);
	selected_spot = App->map->WorldToMap(selected_spot.x, selected_spot.y);

	//check if there's an entity in the selected spot
	j1Entity* target = App->entities->FindEntityByTile(selected_spot);

	if (target != nullptr) {
		if (((god_mode) || (target->faction == faction)) && (target->state >= IDLE) && (target->state < NO_STATE)) {
			if (target->info.current_group != nullptr) {
				target->ClearUnitInfo();
			}
			return target;
		}
	}

	return nullptr;
}

void j1Player::MoveEntity(DynamicEntity* entity){
	iPoint selected_spot;

	if (TouchingUI(mouse_position.x, mouse_position.y)) {
		if ((mouse_position.x > App->minimap->position.x) && (mouse_position.x < App->minimap->position.x + App->minimap->width)
			&& (mouse_position.y > App->minimap->position.y) && (mouse_position.y < App->minimap->position.y + App->minimap->height)) {
			selected_spot = App->minimap->ScreenToMinimapToWorld(mouse_position.x, mouse_position.y);
			selected_spot = App->map->WorldToMap(selected_spot.x, selected_spot.y);
		}
		else {
			return;
		}
	}
	else {
		selected_spot = App->render->ScreenToWorld(mouse_position.x, mouse_position.y);
		selected_spot = App->map->WorldToMap(selected_spot.x, selected_spot.y);
	}

	//dynamic entities
	if ((entity->state != DIE)&&((entity->state >= IDLE)&&(entity->state < NO_STATE)))
	{
		if (entity->info.current_group != nullptr)
			entity->info.current_group = nullptr;

		entity->node_path.clear();
		entity->CheckDestination(selected_spot);
		entity->PathfindToPosition(selected_spot);
		entity->commanded = true;
	}
}

void j1Player::MoveGroup() {
	for (int i = 0; i < selected_group->GetSize(); i++)
	{
		MoveEntity(selected_group->Units[i]);
		//selected_group->Units[i]->state = WALK;
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
	App->menu_manager->CreateMenu(Menu::RESOURCES);
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
			static_entity = static_cast<StaticEntity*>(last_selected_entity);
		else
			static_entity = static_cast<StaticEntity*>(selected_entity);

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
			static_entity = dynamic_cast<StaticEntity*>(last_selected_entity);
		else
			static_entity = dynamic_cast<StaticEntity*>(selected_entity);

		if (static_entity != nullptr)
		{
			if (command_parts[1] == "gatherer")
				App->entities->CreateEntity(static_entity->faction, GATHERER, static_entity->spawnPosition.x, static_entity->spawnPosition.y);
			if (command_parts[1] == "melee")
				App->entities->CreateEntity(static_entity->faction, MELEE, static_entity->spawnPosition.x, static_entity->spawnPosition.y);
			if (command_parts[1] == "ranged")
				App->entities->CreateEntity(static_entity->faction, RANGED, static_entity->spawnPosition.x, static_entity->spawnPosition.y);
			if (command_parts[1] == "army")
				for(int i = 0; i < 10; i++) {
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
	return y > App->minimap->position.y - 8;
}

// Load Game State
bool j1Player::Load(pugi::xml_node& data)
{
	caps = data.child("resource").attribute("caps").as_int();
	food = data.child("resource").attribute("food").as_int();
	water= data.child("resource").attribute("water").as_int();
	App->menu_manager->DestroyMenu(Menu::RESOURCES);
	App->menu_manager->CreateMenu(Menu::RESOURCES);
	return true;
}

// Save Game State
bool j1Player::Save(pugi::xml_node& data) const
{
	pugi::xml_node res = data.append_child("resource");

	res.append_attribute("caps") = caps;
	res.append_attribute("food") = food;
	res.append_attribute("water") = water;

	return true;
}
