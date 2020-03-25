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
#include "MenuManager.h"
#include "Player.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
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
	menu_state = StatesMenu::NONE;

	DynamicEntity* test_melee, *test_enemy;
  
	if(App->map->Load("iso_walk.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if(App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	debug_tex = App->tex->Load("maps/path2.png");
	test_melee = (DynamicEntity*)App->entities->CreateEntity(VAULT, MELEE, 14, 4);
	test_enemy = (DynamicEntity*)App->entities->CreateEntity(MUTANT, RANGED, 14, 2);
	App->entities->CreateEntity(VAULT, GATHERER, 18, 6);

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
	// Gui ---
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		
		if (create == true) {
			
			App->menu_manager->DestroyPauseMenu();
			create = false;
			
		}
		
		else if (create == false) {
			
			App->menu_manager->CreatePauseMenu();
			create = true;

		}
	
	}

	// -------
	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += floor(200.0f * dt);

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= floor(200.0f * dt);

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += floor(200.0f * dt);

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= floor(200.0f * dt);

	App->map->Draw();

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count(),
					map_coordinates.x, map_coordinates.y);

	//App->win->SetTitle(title.GetString());

	// Debug pathfinding ------------------------------
	//int x, y;

	if (App->player->selected_entity != nullptr)
	{
		App->input->GetMousePosition(x, y);
		iPoint p = App->render->ScreenToWorld(x, y);
		p = App->map->WorldToMap(p.x, p.y);
		p = App->map->MapToWorld(p.x, p.y);

		App->render->Blit(debug_tex, p.x, p.y);
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