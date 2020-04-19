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
#include "j1Gui.h"
#include "j1Minimap.h"
#include "j1Audio.h"

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
	pugi::xml_parse_result result = animation_file.load_file("gui/loading.tmx");

	std::string image = std::string(animation_file.child("tileset").child("image").attribute("source").as_string());

	logo_tex = App->tex->Load("gui/logo_spritesheet.png");
	gif_tex = App->tex->Load("gui/gifwheel.png");
	background = App->tex->Load("gui/background.png");
	if (result == NULL)
	{
		LOG("Could not load animation tmx file %s. pugi error: %s", "gui/LogoIntro.tmx", result.description());
		ret = false;
	}

	int tile_width = animation_file.child("map").child("tileset").attribute("tilewidth").as_int();
	int tile_height = animation_file.child("map").child("tileset").attribute("tileheight").as_int();
	int columns = animation_file.child("map").child("tileset").attribute("columns").as_int();
	int firstgid = animation_file.child("map").child("tileset").attribute("firstgid").as_int();
	int id, tile_id;
	float speed;

	pugi::xml_node animation = animation_file.child("map").child("tileset").child("tile");
	pugi::xml_node frame = animation.child("animation").child("frame");

	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;

	id = animation.attribute("id").as_int();
	loader = 0;
	for (int i = 0; i < 65; i++)
	{
		if (i == 0)
		{
			loader = &animationGif;
		}
		if (i == 1)
		{
			loader = &animationLogo;
		}
}

		while (frame != nullptr) {
			tile_id = frame.attribute("tileid").as_int();
			speed = frame.attribute("duration").as_int();
			rect.x = rect.w * ((tile_id) % columns);
			rect.y = rect.h * ((tile_id) / columns);
			loader->PushBack(rect, speed);
			frame = frame.next_sibling();
		}
		loader->loop = true;

	animation = animation.next_sibling();
	frame = animation.child("animation").child("frame");

	return ret;
}
bool j1Transition::CleanUp()
{
	App->tex->UnLoad(logo_tex);
	App->tex->UnLoad(gif_tex);
	return true;
}

bool j1Transition::Start()
{
	LoadAnimations();
	transition = false;
	return true;
}

bool j1Transition::Update(float dt)
{
	lastdt = dt*0.01;
	Transition();
	return true;
}
bool j1Transition::PostUpdate(float dt)
{
	deltatime = dt;


	return true;
}


void j1Transition::Transition()
{
	if (transition == true)
	{
		if (Mix_Playing(1) == 0) {
			App->audio->PlayFx(1, App->audio->loading, 0);
		}
		App->render->Blit(background, 0, 0);
		App->render->Blit(gif_tex, 536, 191, &loader->GetCurrentFrame(lastdt));
		App->render->Blit(logo_tex, 536, 515, &loader->GetCurrentFrame(lastdt));
		App->gui->active = false;
		App->minimap->active = false;
		App->entities->active = false;
	}
	
	if (fadetimer.Read() > 6000)
	{
		transition = false;
		App->gui->active = true;
		App->minimap->active = true;
		App->entities->active = true;
	}
}
