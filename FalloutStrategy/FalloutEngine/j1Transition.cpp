#include <math.h>
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Transition.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Textures.h"
#include "j1EntityManager.h"
#include "FoWManager.h"
#include "AI_Manager.h"
#include "j1Gui.h"
#include "j1Minimap.h"
#include "j1Player.h"
#include "j1Audio.h"
#include "j1MovementManager.h"
#include "j1Hud.h"
#include "DialogManager.h"
#include "MenuManager.h"
#include "MainMenu.h"
#include "j1Video.h"
#include "AssetsManager.h"
#include "SDL_mixer\include\SDL_mixer.h"

j1Transition::j1Transition() : j1Module()
{
	name = ("transition");

	speed_reducer = 0;
	gif_tex = nullptr;
	logo_tex = nullptr;
	background = nullptr;
	lastdt = 0.01f;
	transition = false;
	freeTransitionTex = false;
}

j1Transition::~j1Transition() {
}

bool j1Transition::LoadAnimations() {
	bool ret = true;

	char* buffer;
	pugi::xml_document animation_file;

	int bytesFile = App->assetManager->Load("Assets/gui/loading.tmx", &buffer);
	pugi::xml_parse_result result = animation_file.load_buffer(buffer, bytesFile);
	RELEASE_ARRAY(buffer);

	logo_tex = App->tex->Load("Assets/gui/logo_spritesheet.png");
	gif_tex = App->tex->Load("Assets/gui/gifwheel.png");
	background = App->tex->Load("Assets/gui/background.png");
	if (result == NULL)
	{
		LOG("Could not load animation tmx file %s. pugi error: %s", "Assets/gui/LogoIntro.tmx", result.description());
		ret = false;
	}

	pugi::xml_node tileset = animation_file.child("map").child("tileset");

	while (tileset != nullptr)
	{
		int tile_width = tileset.attribute("tilewidth").as_int();
		int tile_height = tileset.attribute("tileheight").as_int();
		int columns = tileset.attribute("columns").as_int();
		int firstgid = tileset.attribute("firstgid").as_int();

		pugi::xml_node animation = tileset.child("tile");
		pugi::xml_node frame = animation.child("animation").child("frame");

		SDL_Rect rect;
		rect.w = tile_width;
		rect.h = tile_height;

		loader = nullptr;

		if (tileset.attribute("firstgid").as_int() == 1)
		{
			loader = &animationGif;
		}
		if (tileset.attribute("firstgid").as_int() == 65)
		{
			loader = &animationLogo;
		}

		while (frame != nullptr) {
			int tile_id = frame.attribute("tileid").as_int();
			float speed = frame.attribute("duration").as_int() * speed_reducer;
			rect.x = rect.w * ((tile_id) % columns);
			rect.y = rect.h * ((tile_id) / columns);
			loader->PushBack(rect, speed);
			frame = frame.next_sibling();
		}
		loader->loop = true;

		animation = animation.next_sibling();
		frame = animation.child("animation").child("frame");

		tileset = tileset.next_sibling("tileset");
	}
	return ret;
}

bool j1Transition::Start()
{
	speed_reducer = 0.01f;

	if (loader == nullptr)
	{
		LoadAnimations();
	}
	
	return true;
}

bool j1Transition::CleanUp()
{
	App->tex->UnLoad(logo_tex);
	logo_tex = nullptr;
	
	App->tex->UnLoad(gif_tex);
	gif_tex = nullptr;
	
	App->tex->UnLoad(background);
	background = nullptr;
	
	loader = nullptr;
	return true;
}

bool j1Transition::Update(float dt)
{
	lastdt = dt;

	return true;
}
bool j1Transition::PostUpdate()
{
	if (transition)
	{
		Transition();
	}
	return true;
}


void j1Transition::Transition()
{
	Mix_PauseMusic();
	if (Mix_Playing(1) == 0) {
		App->audio->PlayFx(1, App->audio->loading, 0);

	}
	App->render->Blit(background, 0, 0, 0, 1.0F, 0);
	App->render->Blit(gif_tex, 536, 191, &animationGif.GetCurrentFrame(lastdt), 1.0F, 0);
	App->render->Blit(logo_tex, 470, 400, &animationLogo.GetCurrentFrame(lastdt), 1.0F, 0);


	if ((!App->entities->loading_reference_entities) && (App->gui->ingame == true) && (fadetimer.Read() > 2500))
	{
		Mix_HaltChannel(1);
		transition = false;
		App->gui->active;
		App->Mmanager->Enable();
		App->scene->Enable();
		App->main_menu->Disable();
		App->hud->Enable();
		App->video->Disable();
		if(App->gui->load==false){
			App->dialog_manager->Enable();
			App->menu_manager->CreateMenu(Menu::DIALOG); 
			App->isPaused = true;
		}				
		App->gui->load = false;
		freeTransitionTex = true;
		App->player->resource_fow_added = false;
	}
	else if ((fadetimer.Read() > 2500)&&(!App->gui->ingame)) {
		Mix_PauseMusic();
		App->audio->PlayMusic("Assets/audio/music/FalloutStrategyMainTheme.ogg", 0.0F);
		Mix_HaltChannel(1);
		App->ai_manager->Disable();
		App->player->Disable();
		App->entities->Disable();
		App->fowManager->Disable();
		App->scene->Disable();
		App->map->Disable();
		App->minimap->Disable();
		App->Mmanager->Disable();
		transition = false;
		App->gui->ingame = false;
		App->menu_manager->CreateMenu(Menu::MAIN_MENU);
		App->isPaused = false;
		App->scene->win = false;
		App->scene->lose = false;
		App->gui->open = true;
		App->main_menu->Enable();
		if (App->entities->showing_building_menu == true) {
			App->menu_manager->DestroyFaction(Menu::BUI_BASES, App->menu_manager->current_building_faction, App->menu_manager->current_building_type);
			App->entities->showing_building_menu = false;
			App->player->selected_entity = nullptr;
		}
	}

	if(logo_tex != nullptr)
	{
		if (freeTransitionTex)
		{
			CleanUp();
		}
	}
	if(logo_tex == nullptr && !freeTransitionTex)
	{
		Start();
	}
	

	
}

void j1Transition::StartTimer() {fadetimer.Start();}
