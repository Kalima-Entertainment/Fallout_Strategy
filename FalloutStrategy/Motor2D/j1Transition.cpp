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
#include "AI_Manager.h"
#include "j1Gui.h"
#include "j1Minimap.h"
#include "j1Player.h"
#include "j1Audio.h"
#include "j1MovementManager.h"

#include "SDL_mixer\include\SDL_mixer.h"

j1Transition::j1Transition() : j1Module()
{
	name = ("transition");
}

j1Transition::~j1Transition()
{
}

bool j1Transition::LoadAnimations() {
	bool ret = true;
	pugi::xml_document animation_file;
	pugi::xml_parse_result result = animation_file.load_file("Assets/gui/loading.tmx");

	std::string image = std::string(animation_file.child("tileset").child("image").attribute("source").as_string());

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
		int id, tile_id;
		float speed;

		pugi::xml_node animation = tileset.child("tile");
		pugi::xml_node frame = animation.child("animation").child("frame");

		SDL_Rect rect;
		rect.w = tile_width;
		rect.h = tile_height;

		id = animation.attribute("id").as_int();
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
			tile_id = frame.attribute("tileid").as_int();
			speed = frame.attribute("duration").as_int() * speed_reducer;
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
	transition = false;
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
		App->player->Enable();
		App->ai_manager->Enable();
		App->scene->Enable();
		//App->minimap->Enable();
		App->isPaused = false;
	}
	else if ((fadetimer.Read() > 2500)&&(!App->gui->ingame)) {
		Mix_ResumeMusic();
		Mix_HaltChannel(1);
		App->ai_manager->Disable();
		App->player->Disable();
		App->entities->Disable();
		App->scene->Disable();
		App->map->Disable();
		App->minimap->Disable();
		App->Mmanager->Disable();
		transition = false;
		App->isPaused = false;
		App->scene->win = false;
		App->scene->lose = false;
	}
}

void j1Transition::StartTimer() {fadetimer.Start();}
