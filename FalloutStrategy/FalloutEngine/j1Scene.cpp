#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1Gui.h"
#include "j1Scene.h"
#include "j1EntityManager.h"
#include "j1Entity.h"
#include "DynamicEntity.h"
#include "StaticEntity.h"
#include "MenuManager.h"
#include "UI_Label.h"
#include "j1Player.h"
#include "SDL_mixer/include/SDL_mixer.h"
#include <stdlib.h>
#include <time.h>
#include <string>
#include "j1MovementManager.h"
#include "GenericPlayer.h"
#include "AI_Manager.h"
#include "j1Minimap.h"
#include "AI_Player.h"
#include "j1Transition.h"
#include "LogoScene.h"
#include "j1Console.h"
#include "FoWManager.h"
#include "j1Hud.h"

j1Scene::j1Scene() : j1Module()
{
	name = ("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	srand(time(NULL));
	menu_state = StatesMenu::NONE;
	beaten_enemies = 0;

	if (App->render->fog_of_war)App->fowManager->Enable();


	App->console->CreateCommand("win", "Automatically win the game", this);
	App->console->CreateCommand("lose", "Automatically lose the game", this);

	//random map ----------------------------

	App->pathfinding->SetMap();

	for (int i = 0; i < 4; i++)
	{
		int type = rand() % 4;
		if (type == 0)
			modules[i] = ("grassland");
		else if (type == 1)
			modules[i] = ("wasteland");
		else if (type == 2)
			modules[i] = ("desert");
		else if (type == 3)
			modules[i] = ("oldstone");
	}

	modules[0] += ("_up_left.tmx");
	modules[1] += ("_up_right.tmx");
	modules[2] += ("_low_left.tmx");
	modules[3] += ("_low_right.tmx");

	// --------------------------------------

	if (App->map->Load(modules) == true)
	{
		App->map->CreateWalkabilityMap();
	}

	// -- Creates FoW in current Map
	if (App->render->fog_of_war)App->fowManager->CreateFoWMap(App->map->data.width, App->map->data.height);
		
	App->minimap->Enable();

	//top_left
	App->entities->CreateEntity(VAULT, MELEE, 20, 20, App->player);
	//App->entities->CreateEntity(ANIMALS, BIGHRONER, 15, 20, App->player);
	//App->entities->CreateEntity(ANIMALS, BRAHAM, 22, 20, App->player);
	//App->entities->CreateEntity(ANIMALS, DEATHCLAW, 28, 20, App->player);

	//top_right
	App->entities->CreateEntity(VAULT, MELEE, 130, 20, App->player);
	//bottom_left
	App->entities->CreateEntity(VAULT, MELEE, 20, 130, App->player);
	//bottom_right
	App->entities->CreateEntity(VAULT, MELEE, 130,110, App->player);

	//Set camera to player's base position
	iPoint camera_pos = App->map->MapToWorld(App->player->base->current_tile.x - 25, App->player->base->current_tile.y);
	App->render->camera.x = -camera_pos.x;
	App->render->camera.y = -camera_pos.y;

	//App->render->camera.x = 2000;
	//App->render->camera.y = 0;

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	//GetMouse Position to create rectangles
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	mouse_pos = App->render->ScreenToWorld(x, y);

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	App->map->Draw();

	// Gui ---
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {

		if (create == true) {

			App->menu_manager->DestroyMenu(Menu::PAUSE_MENU);
			App->isPaused = false;
			create = false;
			Mix_PauseMusic();
			App->hud->activateTimer = true;
		}

		else if (create == false) {

			App->menu_manager->CreateMenu(Menu::PAUSE_MENU);
			App->isPaused = true;
			App->hud->activateTimer = false;
			create = true;
			if (Mix_PlayingMusic() == 1) {
				Mix_ResumeMusic();
			}
			App->audio->PlayMusic("Assets/audio/music/Fallout4TitleScreenwithThemeMusic.ogg", 0.0F);
		}

	}

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN){
		App->SaveGame("save_file.xml");
	}
	else if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
		App->LoadGame("save_file.xml");
	}

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);

	//Used to select units and groups
	if (!App->player->TouchingUI(x, y)) {
		RectangleSelection();
	}

	/*
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count(),
					map_coordinates.x, map_coordinates.y);

	//App->win->SetTitle(title.GetString());
	*/

	if(App->input->GetKey(SDL_SCANCODE_KP_1) == KEY_DOWN)
	{
		
		win = true;
		App->menu_manager->DestroyMenu(App->menu_manager->current_menu);
		App->menu_manager->DestroyMenu(Menu::RESOURCES);
		App->gui->ingame = false;
		App->isPaused = true;
		App->logo_scene->Loop = true;

	}
	else if (App->input->GetKey(SDL_SCANCODE_KP_2) == KEY_DOWN)
	{

		lose = true;
		App->menu_manager->DestroyMenu(App->menu_manager->current_menu);
		App->menu_manager->DestroyMenu(Menu::RESOURCES);
		App->gui->ingame = false;
		App->isPaused = true;
		App->logo_scene->Loop = true;
	}

	return true;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	players[0] = players[1] = players[2] = players[3] = nullptr;
	return true;
}

StatesMenu j1Scene::GetMenuState()
{
	return menu_state;
}

void j1Scene::SetMenuState(const StatesMenu& menu)
{
	menu_state = menu;
}


void j1Scene::RectangleSelection()
{
	rectangle_width = mouse_pos.x - rectangle_origin.x;
	rectangle_height = mouse_pos.y - rectangle_origin.y;

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) rectangle_origin = mouse_pos;
	else if (std::abs(rectangle_width) >= RECT_MIN_AREA && std::abs(rectangle_height) >= RECT_MIN_AREA && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {

		// --- Rectangle size ---
		int width = mouse_pos.x - rectangle_origin.x;
		int height = mouse_pos.y - rectangle_origin.y;

		// --- Draw Rectangle ---
		SDL_Rect SRect = { rectangle_origin.x, rectangle_origin.y, width, height };
		App->render->DrawQuad(SRect, 255, 255, 255, 255, false);

		// --- Once we get to the negative side of SRect numbers must be adjusted ---
		if (width < 0) {
			SRect.x = mouse_pos.x;
			SRect.w *= -1;
		}
		if (height < 0) {
			SRect.y = mouse_pos.y;
			SRect.h *= -1;
		}

		// --- Check for Units in the rectangle, select them ---
		App->Mmanager->SelectEntities_inRect(SRect);
	}

	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
		std::vector<DynamicEntity*> selected_entities;
		for (int i = 0; i < App->entities->entities.size(); i++)
		{
			if (App->entities->entities[i]->info.IsSelected && App->entities->entities[i]->is_dynamic) {
				selected_entities.push_back((DynamicEntity*)App->entities->entities[i]);
			}
		}
		App->Mmanager->CreateGroup(selected_entities);
	}
}

void j1Scene::CheckWinner() {
	for (int i = 0; i < 4; i++)
	{
		if (!players[i]->defeated) {
			if ((players[i]->base == nullptr) && (players[i]->laboratory == nullptr) && (players[i]->barrack[0] == nullptr) && (players[i]->barrack[1] == nullptr)) {
				players[i]->defeated = true;
				if (players[i]->faction == App->player->faction) {
					//LOSE
					lose = true;
					App->logo_scene->Loop = true;
					App->menu_manager->DestroyMenu(App->menu_manager->current_menu);
					App->gui->ingame = false;
					App->isPaused = true;
					App->logo_scene->playsound = true;
				}
				else { 
					beaten_enemies++;
					if (players[i]->faction == VAULT)
						LOG("Vault Dwellers faction defeated!");
					else if (players[i]->faction == BROTHERHOOD)
						LOG("Brotherhood of Steel defeated!");
					else if (players[i]->faction == MUTANT)
						LOG("Mutants defeated!");
					else if (players[i]->faction == GHOUL)
						LOG("Ghouls defeated!");
				}
			}
		}
	}

	//WIN
	if (beaten_enemies == 3) {
		LOG("You won!");
		win = true;
		App->menu_manager->DestroyMenu(App->menu_manager->current_menu);
		App->gui->ingame = false;
		App->isPaused = true;
		App->logo_scene->Loop = true;
		App->logo_scene->playsound = true;
	}
}

void j1Scene::OnCommand(std::vector<std::string> command_parts) {
	std::string command_beginning = command_parts[0];

	//Instantly win the game
	if (command_beginning == "win") {
		for (int i = 0; i < 4; i++)
		{
			if (players[i]->faction != App->player->faction) {
				if (players[i]->base != nullptr)
					players[i]->base->state = DIE;
				if (players[i]->laboratory != nullptr)
					players[i]->laboratory->state = DIE;
				if (players[i]->barrack[0] != nullptr)
					players[i]->barrack[0]->state = DIE;
				if (players[i]->barrack[1] != nullptr)
					players[i]->barrack[1]->state = DIE;
			}
		}
		App->menu_manager->DestroyMenu(App->menu_manager->current_menu);
		App->menu_manager->DestroyMenu(Menu::RESOURCES);
		App->logo_scene->playsound = true;
		App->gui->ingame = false;
		App->isPaused = true;
		App->logo_scene->Loop = true;
		win = true;
	}
	//Instantly lose the game
	else if (command_beginning == "lose") {
		if (App->player->base != nullptr)
			App->player->base->state = DIE;
		if (App->player->laboratory != nullptr)
			App->player->laboratory->state = DIE;
		if (App->player->barrack[0] != nullptr)
			App->player->barrack[0]->state = DIE;
		if (App->player->barrack[1] != nullptr)
			App->player->barrack[1]->state = DIE;
		lose = true;
		App->menu_manager->DestroyMenu(App->menu_manager->current_menu);
		App->menu_manager->DestroyMenu(Menu::RESOURCES);
		App->logo_scene->playsound = true;
		App->gui->ingame = false;
		App->isPaused = true;
		App->logo_scene->Loop = true;
	}
}


// Load Game State
bool j1Scene::Load(pugi::xml_node& data)
{
	
	App->map->CleanUp();
	App->minimap->CleanUp();
	pugi::xml_node iterator;
	int i = 0;

	while(iterator){
		modules[i] = iterator.attribute("map").as_string();
		LOG("%s", modules[i]);
		iterator = iterator.next_sibling();
		i++;
	}

	App->pathfinding->SetMap();

	if (App->map->Load(modules) == true)
	{
		App->map->CreateWalkabilityMap();
		App->minimap->Start();
	}


	return true;
}

// Save Game State
bool j1Scene::Save(pugi::xml_node& data) const
{

	for(int i=0; i<4;i++){

		pugi::xml_node module = data.append_child("modules");
		module.append_attribute("map") = modules[i].c_str();

	}

	return true;

}