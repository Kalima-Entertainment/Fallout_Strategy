#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "LogoScene.h"
#include "j1Audio.h"




LogoScene::LogoScene() : j1Module()
{
	
	name = ("logo_scene");
	
}

// Destructor
LogoScene::~LogoScene()
{}

bool LogoScene::LoadAnimations() 
{
	bool ret = true;
	pugi::xml_document animation_file;
	pugi::xml_parse_result result = animation_file.load_file("gui/textures/LogoIntro.tmx");

	std::string image = std::string(animation_file.child("tileset").child("image").attribute("source").as_string());
	
	logo_tex = App->tex->Load("gui/textures/LogoFallout.png");
	start_game_tex = App->tex->Load("gui/textures/StartGameTex.png");

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

	loader = &animationLogo;

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

// Called before the first frame
bool LogoScene::Start()
{
	start_game_rect = { 0, 0,561,30 };
	LoadAnimations();
	renderLogo = true;
	Timer.Start();

	return true;
}

// Called each loop iteration
bool LogoScene::Update(float dt)
{
	
	last_dt = dt;

	if(App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		renderLogo = false;
	}


	return true;
}


bool LogoScene::PostUpdate()
{
	if(renderLogo)
	{
		SDL_Rect background = { 0, 0, 1280, 720 };
		App->render->DrawQuad(background, 0, 0, 0, 255);
		App->render->Blit(logo_tex, 400, 50, &loader->GetCurrentFrame(last_dt));
		
		if(drawable)
		{
			App->render->Blit(start_game_tex, 325, 510, &start_game_rect);
		}
		
		if (Timer.Read() > 1000 && Timer.Read() < 2000)
		{
			drawable = false;

		}
		if(Timer.Read() > 2000)
		{
			drawable = true;
			Timer.Start();
			
		}
	}
	
	

	return true;
}

// Called before quitting
bool LogoScene::CleanUp()
{
	SDL_DestroyTexture(logo_tex);
	SDL_DestroyTexture(start_game_tex);

	return true;
}
