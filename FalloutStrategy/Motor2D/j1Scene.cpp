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
#include "Player.h"
#include "SDL_mixer/include/SDL_mixer.h"
#include <stdlib.h>
#include <time.h> 
#include <string>

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

	DynamicEntity* vault[3], * brotherhood[3], * ghoul[3], *mutant[3];
	DynamicEntity* test_melee, *test_enemy, *test_ranged, *test_gatherer;
	StaticEntity* ghoul_base, *ghoul_barrack, *ghoul_laboratory;
	StaticEntity* vault_base, *vault_barrack, *vault_laboratory;
	StaticEntity* mutant_base, *mutant_barrack, *mutant_laboratory;
	StaticEntity* brotherhood_base, *brotherhood_barrack, *brotherhood_laboratory;

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

	if (App->map->Load("grassland_low_left.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	vault[0] = (DynamicEntity*)App->entities->CreateEntity(VAULT, MELEE, 14, 6);
	vault[1] = (DynamicEntity*)App->entities->CreateEntity(VAULT, RANGED, 15, 6);
	vault[2] = (DynamicEntity*)App->entities->CreateEntity(VAULT, GATHERER, 16, 6);

	ghoul[0] = (DynamicEntity*)App->entities->CreateEntity(GHOUL, MELEE, 14, 3);
	ghoul[0]->direction = BOTTOM_LEFT;
	ghoul[1] = (DynamicEntity*)App->entities->CreateEntity(GHOUL, RANGED, 15, 3);
	ghoul[1]->direction = BOTTOM_LEFT;
	ghoul[2] = (DynamicEntity*)App->entities->CreateEntity(GHOUL, GATHERER, 16, 3);
	ghoul[2]->direction = BOTTOM_LEFT;

	mutant[0] = (DynamicEntity*)App->entities->CreateEntity(MUTANT, MELEE, 18, 3);
	mutant[0]->direction = BOTTOM_LEFT;
	mutant[1] = (DynamicEntity*)App->entities->CreateEntity(MUTANT, RANGED, 19, 3);
	mutant[1]->direction = BOTTOM_LEFT;
	mutant[2] = (DynamicEntity*)App->entities->CreateEntity(MUTANT, GATHERER, 20, 3);
	mutant[2]->direction = BOTTOM_LEFT;

	brotherhood[0] = (DynamicEntity*)App->entities->CreateEntity(BROTHERHOOD, MELEE, 18, 6);
	brotherhood[1] = (DynamicEntity*)App->entities->CreateEntity(BROTHERHOOD, RANGED, 19, 6);
	brotherhood[2] = (DynamicEntity*)App->entities->CreateEntity(BROTHERHOOD, GATHERER, 20, 6);

	//ghoul_base = (StaticEntity*)App->entities->CreateBuilding(GHOUL, BASE, {0,0}, {3,3});
	//ghoul_barrack = (StaticEntity*)App->entities->CreateBuilding(GHOUL, BARRACK, { 64,64 }, { 3,3 });
	//ghoul_laboratory = (StaticEntity*)App->entities->CreateBuilding(GHOUL, LABORATORY, { 6,6 }, { 3,3 });

	//vault_base = (StaticEntity*)App->entities->CreateBuilding(VAULT, BASE, { 0,0 }, { 3,3 });
	//vault_barrack = (StaticEntity*)App->entities->CreateBuilding(VAULT, BARRACK, { 3,3 }, { 3,3 });
	//vault_laboratory = (StaticEntity*)App->entities->CreateBuilding(VAULT, LABORATORY, { 6,6 }, { 3,3 });

	//mutant_base = (StaticEntity*)App->entities->CreateBuilding(MUTANT, BASE, { 0,0 }, { 3,3 });
	//mutant_barrack = (StaticEntity*)App->entities->CreateBuilding(MUTANT, BARRACK, { 3,3 }, { 3,3 });
	//mutant_laboratory = (StaticEntity*)App->entities->CreateBuilding(MUTANT, LABORATORY, { 6,6 }, { 3,3 });

	//brotherhood_base = (StaticEntity*)App->entities->CreateBuilding(BROTHERHOOD, BASE, { 0,0 }, { 3,3 });
	//brotherhood_barrack = (StaticEntity*)App->entities->CreateBuilding(BROTHERHOOD, BARRACK, { 3,3 }, { 3,3 });
	//brotherhood_laboratory = (StaticEntity*)App->entities->CreateBuilding(BROTHERHOOD, LABORATORY, { 6,6 }, { 3,3 });

	//App->audio->PlayMusic("audio/music/elevator_music.ogg", 4.0F);
	App->audio->PlayMusic("audio/music/FalloutStrategyMainTheme.ogg", 4.0F);

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	// debug pathfing ------------------
	static iPoint origin;
	static bool origin_selected = false;

	/*
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	if(App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if(origin_selected == true)
		{
			App->pathfinding->CreatePath(origin, p);
			origin_selected = false;
		}
		else
		{
			origin = p;
			origin_selected = true;
		}
	}
	*/

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
			create = false;

		}

		else if (create == false) {

			
			App->menu_manager->CreatePauseMenu();
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
	// Debug pathfinding ------------------------------
	//int x, y;

	if (App->player->selected_entity != nullptr)
	{
		App->input->GetMousePosition(x, y);
		iPoint p = App->render->ScreenToWorld(x, y);
		p = App->map->WorldToMap(p.x, p.y);
		p = App->map->MapToWorld(p.x, p.y);
		SDL_Rect debug_rect = { 128,0,64,64 };
		App->render->Blit(App->render->debug_tex, p.x, p.y, &debug_rect);
	}

	/*
	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

	for(uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		App->render->Blit(debug_tex, pos.x, pos.y);
	}

	//Creates temporal x and y, that will be stored when we make left click with mouse
	int tx, ty;
	iPoint selected_spot;
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
		App->input->GetMousePosition(tx, ty);
		selected_spot = App->render->ScreenToWorld(tx, ty);
		selected_spot = App->map->WorldToMap(selected_spot.x, selected_spot.y);
		LOG("Actual Map Position is X: %i and Y: %i", selected_spot.x, selected_spot.y);

		//Now with this info stored we must loop ONCE all entity list to know if any entity its located in that spot comparing with selected_spot updated in map.
		for (int i = 0; i < App->entities->entities.size(); i++)
		{
			if(App->entities->entities[i]->MapPosition() == selected_spot) LOG("COINCIDENCE IN MAP");
		}

	}
	*/

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

	/*
	Mix_HaltChannel(-1);
	int distance = (App->render->camera.x * App->render->camera.x + App->render->camera.y * App->render->camera.y); // cause remember, inverse square law
	distance = distance / 500; //to scale a bit
	int volume = (distance * 255) / App->render->camera.w;
	if (volume < 0) { volume = 0; } if (volume > 255) { volume = 255; }

	float angle = 90;
	if (App->render->camera.y == 0) {
		angle = atan(-App->render->camera.x);
	}
	else {
		angle = atan((-App->render->camera.x) / (App->render->camera.y));
	}
	angle = angle * 57 + 360; //conversion from rad to degree +270. We add +90 extra cause sdl has 0 as its front for some fkn reason.

	Mix_SetPosition(5, angle, volume);
	App->audio->PlayFx(5, App->audio->explosion, 0);
	*/

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	/*if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;*/

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