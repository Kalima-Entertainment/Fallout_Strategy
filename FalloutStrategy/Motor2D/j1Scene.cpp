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
	
	//random map ----------------------------

	std::string modules[4];

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
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	App->audio->PlayMusic("audio/music/FalloutStrategyMainTheme.ogg", 4.0F);

	App->entities->CreateEntity(VAULT, RANGED, 20, 20);

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
		}

		else if (create == false) {

			App->menu_manager->CreatePauseMenu();
			App->isPaused = true;
			create = true;
		}

	}

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);

	/*
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count(),
					map_coordinates.x, map_coordinates.y);

	//App->win->SetTitle(title.GetString());
	*/
	/*
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		Mix_HaltChannel(-1);
		Mix_SetPosition(1, 270, 1);
		App->audio->PlayFx(1, App->audio->explosion, 0);
	}
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		Mix_HaltChannel(-1);
		Mix_SetPosition(2, 270, 200);
		App->audio->PlayFx(2, App->audio->explosion, 0);
	}
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		Mix_HaltChannel(-1);
		Mix_SetPosition(3, 90, 1);
		App->audio->PlayFx(3, App->audio->explosion, 0);
	}
	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
	{
		Mix_HaltChannel(-1);
		Mix_SetPosition(4, 90, 200);
		App->audio->PlayFx(4, App->audio->explosion, 0);
	}
	*/


	//Used to select units and groups
	if (!App->player->TouchingUI(x,y)) {
		RectangleSelection();
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;


	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

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

	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		App->Mmanager->CreateGroup();
}
