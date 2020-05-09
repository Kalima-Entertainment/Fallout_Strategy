#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "LogoScene.h"
#include "j1Audio.h"
#include "j1Video.h"
#include "SDL_mixer/include/SDL_mixer.h"
#include "MenuManager.h"

LogoScene::LogoScene() : j1Module()
{
	name = ("logo_scene");
}

// Destructor
LogoScene::~LogoScene() {
}

bool LogoScene::LoadAnimations()
{
	bool ret = true;
	pugi::xml_document animation_file;
	pugi::xml_parse_result result = animation_file.load_file("Assets/gui/textures/LogoIntro.tmx");

	std::string image = std::string(animation_file.child("tileset").child("image").attribute("source").as_string());

	logo_tex = App->tex->Load("Assets/gui/textures/LogoFallout.png");
	start_game_tex = App->tex->Load("Assets/gui/textures/StartGameTex.png");

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

bool LogoScene::Start()
{
	App->audio->PlayFx(1, App->audio->intro_fx, 0);
	my_video = App->video->Load("Assets/video/intro.ogv", App->render->renderer);
	
	start_game_rect = { 0, 0,561,30 };
	LoadAnimations();
	Loop = true;
	Timer.Start();

	return true;
}

bool LogoScene::CleanUp() {

	bool ret = true;
	ret = App->tex->UnLoad(logo_tex);
	logo_tex = nullptr;
	ret = App->tex->UnLoad(start_game_tex);
	start_game_tex = nullptr;
	loader = nullptr;

	return ret;
}

bool LogoScene::Update(float dt) {
	
	last_dt = dt;

	if(App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		App->audio->PlayFx(2, App->audio->F_press, 0);
		App->audio->PlayMusic("Assets/audio/music/FalloutStrategyMainTheme.ogg", 4.0F);
		App->video->DestroyVideo(my_video);
		my_video = 0;
		Loop = false;
	}

	return true;
}

bool LogoScene::PostUpdate()
{
	
	if (my_video != 0)
	{
		video_texture = App->video->UpdateVideo(my_video);

		App->render->Blit(video_texture, 0, 0);

		if (App->video->IsPlaying(my_video) == 0)
		{
			App->video->DestroyVideo(my_video);
			my_video = 0;
		}
	}
	

	if(my_video == 0 && Loop)
	{
		my_video = App->video->Load("Assets/video/intro.ogv", App->render->renderer);

		
	}

	if (!Loop && App->menu_manager->current_menu == Menu::MAIN_MENU)
	{
		
		App->render->Blit(logo_tex, 70, -130, &loader->GetCurrentFrame(last_dt), 1.0f, 0.0f);

	}

	return true;
}


