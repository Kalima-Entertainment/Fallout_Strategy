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
#include "Deathclaw.h"
#include "j1Video.h"

j1Scene::j1Scene() : j1Module()
{
	name = ("scene");

	create = false;
	load_game = false;
	create_tutorial = false;
	menu_state = StatesMenu::NONE;
	mouse_pos = { 0,0 };
	rectangle_origin = { 0,0 };
	rectangle_width = 0;
	rectangle_height = 0;
	win = false;
	lose = false;
	deathclaw1 = deathclaw2 = deathclaw3 = deathclaw4 = false;
	Deathclaws[0] = Deathclaws[1] = Deathclaws[2] = Deathclaws[3] = nullptr;
	players[0] = players[1] = players[2] = players[3] = nullptr;
	SongPlaying = 0;
	beaten_enemies = 0;

	destination_texture = nullptr;
	destination.PushBack(SDL_Rect{ 0, 0 , 64, 32 }, 12.0f);
	destination.PushBack(SDL_Rect{ 64, 0 , 64, 32 }, 12.0f);
	destination.PushBack(SDL_Rect{ 0, 32 , 64, 32 }, 12.0f);
	destination.PushBack(SDL_Rect{ 64, 32 , 64, 32 }, 12.0f);
	destination.PushBack(SDL_Rect{ 0, 32 , 64, 32 }, 12.0f);
	destination.PushBack(SDL_Rect{ 64, 0 , 64, 32 }, 12.0f);
	destination.loop = false;
	destination.Reset();
	blit_destination = false;
	attack_destination = false;
}

// Destructor
j1Scene::~j1Scene()
{
	App->tex->UnLoad(destination_texture);
	destination_texture = nullptr;
}

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
	App->player->caps = 500;
	App->player->food = 250;
	App->player->water = 500;

	srand(time(NULL));
	menu_state = StatesMenu::NONE;
	beaten_enemies = 0;

	if (App->render->fog_of_war) {
		App->fowManager->ResetFoWEntities();
		App->fowManager->ResetFoWMap();
		App->fowManager->Enable();
	}		

	deathclaw1 = deathclaw2 = deathclaw3 = deathclaw4 = false;

	destination_texture = App->tex->Load("Assets/textures/player/destination_debug.png");

	App->console->CreateCommand("win", "Automatically win the game", this);
	App->console->CreateCommand("lose", "Automatically lose the game", this);

	App->player->Enable();
	App->ai_manager->Enable();

	//random map ----------------------------

	App->pathfinding->SetMap();

	for(int i = 0; i < 4; i++)
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

	if (App->map->Load(modules) == true) {
		App->map->CreateWalkabilityMap();
	}

	// -- Creates FoW in current Map
	if (App->render->fog_of_war)App->fowManager->CreateFoWMap(App->map->data.width, App->map->data.height);

	App->minimap->Enable();
	
	/*
	//top_left
	App->entities->CreateEntity(VAULT, MELEE, 20, 20, App->player);
	App->entities->CreateEntity(VAULT, MELEE, 21, 20, App->player);
	App->entities->CreateEntity(VAULT, RANGED, 20, 21, App->player);
	App->entities->CreateEntity(VAULT, RANGED, 21, 21, App->player);
	//top_right
	App->entities->CreateEntity(VAULT, MELEE, 130, 20, App->player);
	App->entities->CreateEntity(VAULT, MELEE, 131, 20, App->player);
	App->entities->CreateEntity(VAULT, RANGED, 130, 21, App->player);
	App->entities->CreateEntity(VAULT, RANGED, 131, 21, App->player);
	//bottom_left
	App->entities->CreateEntity(VAULT, MELEE, 20, 130, App->player);
	App->entities->CreateEntity(VAULT, MELEE, 21, 130, App->player);
	App->entities->CreateEntity(VAULT, RANGED, 20, 131, App->player);
	App->entities->CreateEntity(VAULT, RANGED, 21, 131, App->player);
	//bottom_right
	App->entities->CreateEntity(VAULT, MELEE, 130,110, App->player);
	App->entities->CreateEntity(VAULT, MELEE, 131, 110, App->player);
	App->entities->CreateEntity(VAULT, RANGED, 130, 111, App->player);
	App->entities->CreateEntity(VAULT, RANGED, 131, 111, App->player);
	*/

	//Set camera to player's base position
	App->render->camera.x -= (int)(App->player->base->position.x - App->render->camera.w * 0.5f);
	App->render->camera.y -= (int)(App->player->base->position.y - App->render->camera.h * 0.5f);

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

	// -- Blit cursor destination
	if(blit_destination)App->render->Blit(destination_texture, debug_destiny.x, debug_destiny.y, &destination.GetCurrentFrame(dt), 1.0f, 1.0f, true, Color_Code::GREEN);
	if (attack_destination)App->render->Blit(destination_texture, debug_destiny.x, debug_destiny.y, &destination.GetCurrentFrame(dt), 1.0f, 1.0f, true, Color_Code::RED);

	
	// If finished restart animation and set again blit to false, just to be rendered when mouse right click with any entitie able to move.
	if (destination.Finished()) { 
		destination.Reset();
		blit_destination = false; 
		attack_destination = false;
	}


	if ((App->hud->minutes == 4) && (deathclaw1 == false))
	{
		if (players[0]->base != nullptr && deathclaw1 == false)
		{
			Deathclaws[0] = dynamic_cast<Deathclaw*>(App->entities->CreateEntity(NO_FACTION, DEATHCLAW, 75, 75));
			iPoint pos = App->entities->ClosestTile(Deathclaws[0]->current_tile, players[0]->base->tiles);
			Deathclaws[0]->PathfindToPosition(pos);
			Deathclaws[0]->target_building = players[0]->base;
			deathclaw1 = true;
		}
		if (players[1]->base != nullptr && deathclaw2 == false)
		{
			Deathclaws[1] = dynamic_cast<Deathclaw*>(App->entities->CreateEntity(NO_FACTION, DEATHCLAW, 76, 75));
			iPoint pos = App->entities->ClosestTile(Deathclaws[1]->current_tile, players[1]->base->tiles);
			Deathclaws[1]->PathfindToPosition(pos);
			Deathclaws[1]->target_building = players[1]->base;
			deathclaw2  = true;
		}
		if (players[2]->base != nullptr && deathclaw3 == false)
		{
			Deathclaws[2] = dynamic_cast<Deathclaw*>(App->entities->CreateEntity(NO_FACTION, DEATHCLAW, 75, 76));
			iPoint pos = App->entities->ClosestTile(Deathclaws[2]->current_tile, players[2]->base->tiles);
			Deathclaws[2]->PathfindToPosition(pos);
			Deathclaws[2]->target_building = players[2]->base;
			deathclaw3 = true;
		}
		if (players[3]->base != nullptr && deathclaw4 == false)
		{
			Deathclaws[3] = dynamic_cast<Deathclaw*>(App->entities->CreateEntity(NO_FACTION, DEATHCLAW, 76, 76));
			iPoint pos = App->entities->ClosestTile(Deathclaws[3]->current_tile, players[3]->base->tiles);
			Deathclaws[3]->PathfindToPosition(pos);
			Deathclaws[3]->target_building = players[3]->base;
			deathclaw4 = true;
		}
	}

	// Gui ---
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {

		if (create == false) {
			App->menu_manager->CreateMenu(Menu::PAUSE_MENU);
			App->isPaused = true;
			App->hud->activateTimer = false;
			create = true;
			if (Mix_PlayingMusic() == 1) {
				Mix_ResumeMusic();
			}
			App->audio->PlayMusic("Assets/audio/music/Fallout4TitleScreenwithThemeMusic.ogg", 0.0F);
		}
		else if (create == true && App->menu_manager->current_menu != Menu::PAUSE_SETTINGS) {

			App->menu_manager->DestroyMenu(Menu::PAUSE_MENU);
			App->isPaused = false;
			create = false;
			Mix_PauseMusic();
			App->hud->activateTimer = true;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN){
		App->SaveGame("save_file.xml");
	}

	else if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN || load_game == true) {
		App->LoadGame("save_file.xml");
		load_game = false;
	}

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	//Used to select units and groups
	if (!App->isPaused) {
		RectangleSelection();
	}

	if(App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
	{
		win = true;
		closeGame();
	}
	else if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		lose = true;
		closeGame();
	}
	//LOSE BY TIME
	if (App->hud->minutes == 0 && App->hud->timer == 1)
	{
		lose = true;
		closeGame();
	}
	return true;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	App->tex->UnLoad(destination_texture);
	destination_texture = nullptr;
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

	int mouse_x;
	int mouse_y;

	App->input->GetMousePosition(mouse_x, mouse_y);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		rectangle_origin = mouse_pos;
	else if (std::abs(rectangle_width) >= RECT_MIN_AREA && std::abs(rectangle_height) >= RECT_MIN_AREA && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && !App->player->TouchingUI(mouse_x, mouse_y)) {

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
	else if (((App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) && (App->player->TouchingUI(mouse_x, mouse_y))) || (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)) {
		std::vector<DynamicEntity*> selected_entities;
		for(size_t i = 0; i < App->entities->entities.size(); i++)
		{
			if (App->entities->entities[i]->info.IsSelected && App->entities->entities[i]->is_dynamic) {
				selected_entities.push_back(dynamic_cast<DynamicEntity*>(App->entities->entities[i]));
			}
		}
		if(selected_entities.size() > 0)
			App->player->selected_group = App->Mmanager->CreateGroup(selected_entities);
	}
}

void j1Scene::CheckWinner() {
	for(int i = 0; i < 4; i++)
	{
		if (!players[i]->defeated) {
			if ((players[i]->base == nullptr) && (players[i]->laboratory == nullptr) && (players[i]->barrack[0] == nullptr) && (players[i]->barrack[1] == nullptr)) {
				players[i]->defeated = true;
				if (players[i]->faction == App->player->faction) {
					//LOSE
					lose = true;
					closeGame();
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
			else if ((players[App->player->faction]->base == nullptr) && (Deathclaws[App->player->faction] != nullptr)) {
				//LOSE
				lose = true;
				closeGame();
			}
		}
	}

	//WIN
	if (((beaten_enemies == 3) && (Deathclaws[App->player->faction] == nullptr)) || ((Deathclaws[App->player->faction] != nullptr) && (Deathclaws [App->player->faction]->state == DIE) && (App->player->base != nullptr))) {
		LOG("You won!");
		win = true;
		closeGame();
	}
}

void j1Scene::OnCommand(std::vector<std::string> command_parts) {
	std::string command_beginning = command_parts[0];

	//Instantly win the game
	if (command_beginning == "win") {
		for(int i = 0; i < 4; i++)
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
		App->menu_manager->DestroyMenu(Menu::GUI);
		App->hud->Disable();
		App->logo_scene->playsound = true;
		App->gui->ingame = false;
		App->isPaused = true;
		App->logo_scene->Loop = true;
		win = true;
		deathclaw1 = false;
		deathclaw2 = false;
		deathclaw3 = false;
		deathclaw4 = false;
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
		App->menu_manager->DestroyMenu(Menu::GUI);
		App->hud->Disable();
		App->logo_scene->playsound = true;
		App->gui->ingame = false;
		App->isPaused = true;
		App->logo_scene->Loop = true;
	}
}


// Load Game State
bool j1Scene::Load(pugi::xml_node& data)
{
	App->entities->DestroyAllEntitiesNow();
	App->ai_manager->CleanUp();
	App->map->CleanUp();
	App->minimap->CleanUp();

	App->ai_manager->CleanUp();
	App->ai_manager->Start();
	App->player->CleanUp();

	pugi::xml_node iterator = data.first_child();
	int i = 0;

	while(iterator){
		modules[i] = iterator.attribute("map").as_string();
		LOG("%s", modules[i]);
		iterator = iterator.next_sibling();

		App->entities->randomFaction[i] = iterator.attribute("value").as_int();
		LOG("%i", App->entities->randomFaction[i]);
		iterator = iterator.next_sibling();
		i++;
	}

	App->pathfinding->SetMap();

	if (App->map->Load(modules) == true)
	{
		App->map->CreateWalkabilityMap();
	}

	App->minimap->Start();
	App->minimap->Show();
	

	return true;
}

// Save Game State
bool j1Scene::Save(pugi::xml_node& data) const
{
	for(int i=0; i<4;i++){

		pugi::xml_node module = data.append_child("modules");
		module.append_attribute("map") = modules[i].c_str();
		
		pugi::xml_node randomfaction = data.append_child("randomfaction");
		randomfaction.append_attribute("value") = App->entities->randomFaction[i];

	}
	return true;
}

void j1Scene::closeGame()
{
	App->menu_manager->DestroyMenu(Menu::PAUSE_MENU);
	App->menu_manager->DestroyMenu(Menu::RESOURCES);
	App->menu_manager->DestroyMenu(Menu::TUTORIAL);
	App->menu_manager->DestroyMenu(Menu::RADIO);
	App->menu_manager->DestroyMenu(Menu::GUI);
	App->menu_manager->DestroyMenu(Menu::QUEST);	
	App->gui->ingame = false;
	App->isPaused = true;
	App->logo_scene->Loop = true;
	App->logo_scene->playsound = true;
	App->hud->Disable();
	App->video->Enable();
	App->transition->freeTransitionTex = false;	
}