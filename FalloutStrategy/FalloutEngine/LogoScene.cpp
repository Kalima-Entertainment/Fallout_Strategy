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
#include "j1Scene.h"
#include "SDL_mixer/include/SDL_mixer.h"
#include "MenuManager.h"
#include "j1Transition.h"
#include "j1Player.h"
#include "j1Entity.h"

LogoScene::LogoScene() : j1Module()
{
	name = ("logo_scene");
	video_texture = nullptr;
	win_tex = nullptr;
	lose_tex = nullptr;
	Loop = true;
	playsound = false;
	drawable = true;
	sound_end = true;
	quit = false;
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
	// Win and lose video
	win_video = App->video->Load("Assets/video/win.ogv", App->render->renderer);
	lose_video = App->video->Load("Assets/video/lose.ogv", App->render->renderer);

	start_game_rect = { 0, 0,561,30 };
	LoadAnimations();
	quit = false;
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
	win_tex = nullptr;
	lose_tex = nullptr;
	return ret;
}

bool LogoScene::Update(float dt) {
	
	last_dt = dt;

	if(App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN && App->scene->win == false && App->scene->lose == false && quit == false)
	{
		App->audio->PlayMusic("Assets/audio/music/FalloutStrategyMainTheme.ogg", 0.0F);
		App->audio->PlayFx(2, App->audio->F_press, 0);
		App->video->DestroyVideo(my_video);
		my_video = 0;
		Loop = false;
		quit = true;
	}
	else if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN && App->scene->win == true)
	{
		Loop = false;
		App->audio->PlayFx(2, App->audio->F_press, 0);
		win_video = 0;
		App->menu_manager->DestroyMenu(Menu::RESOURCES);
		App->menu_manager->DestroyMenu(Menu::QUEST);
		if ((App->player->selected_entity) && (!App->player->selected_entity->is_dynamic)) {
			App->menu_manager->DestroyFaction(Menu::BUI_BASES, App->player->selected_entity->faction, App->player->selected_entity->type);
		}
		App->menu_manager->CreateMenu(Menu::MAIN_MENU);
		App->audio->PlayFx(1, App->audio->back_fx, 0);
		App->gui->ingame = false;
		App->transition->StartTimer();
		App->transition->transition = true;
		App->transition->fadetimer.Start();
		Mix_PauseMusic();
	}
	else if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN && App->scene->lose == true)
	{
		Loop = false;
		App->audio->PlayFx(2, App->audio->F_press, 0);
		lose_video = 0;
		App->menu_manager->DestroyMenu(Menu::RESOURCES);
		App->menu_manager->DestroyMenu(Menu::QUEST);
		if ((App->player->selected_entity) && (!App->player->selected_entity->is_dynamic)) {
			App->menu_manager->DestroyFaction(Menu::BUI_BASES, App->player->selected_entity->faction, App->player->selected_entity->type);
		}
		App->menu_manager->CreateMenu(Menu::MAIN_MENU);
		App->audio->PlayFx(1, App->audio->back_fx, 0);
		App->gui->ingame = false;
		App->transition->StartTimer();
		App->transition->transition = true;
		App->transition->fadetimer.Start();
	}
	return true;
}

bool LogoScene::PostUpdate()
{
	if (App->scene->win == false && App->scene->lose == false)
	{
		if (my_video != 0)
		{
			video_texture = App->video->UpdateVideo(my_video);

			App->render->Blit(video_texture, 0, 0, 0, 1.0f, 0);

			if (App->video->IsPlaying(my_video) == 0)
			{
				App->video->DestroyVideo(my_video);
				my_video = 0;
			}
		}


		if (my_video == 0 && Loop)
		{
			my_video = App->video->Load("Assets/video/intro.ogv", App->render->renderer);


		}

		if (!Loop && App->menu_manager->current_menu == Menu::MAIN_MENU)
		{

			App->render->Blit(logo_tex, 70, -130, &loader->GetCurrentFrame(last_dt), 1.0f, 0.0f);

		}

	}
	
	if (App->scene->win == true && App->scene->lose == false)
	{
		if (playsound == true)
		{
			if (Mix_Playing(2) == 0)
				App->audio->PlayFx(2, App->audio->win, 0);
			if (Mix_Playing(3) == 0)
				App->audio->PlayFx(3, App->audio->you_win, 0);
			Mix_PauseMusic();
			App->audio->PlayMusic("Assets/audio/music/winmusic.ogg", 0.0F);
			playsound = false;
		
		}
		
		if (win_video != 0)
		{
			win_tex = App->video->UpdateVideo(win_video);

			App->render->Blit(win_tex, 0, 0,0,1.0F,0);

			if (App->video->IsPlaying(win_video) == 0)
			{
				App->video->DestroyVideo(win_video);
				win_video = 0;
			}
		}
		if (win_video == 0 && Loop)
		{
			win_video = App->video->Load("Assets/video/win.ogv", App->render->renderer);
		}

	}

	if (App->scene->lose == true && App->scene->win == false)
	{
		if (playsound == true)
		{
			if (Mix_Playing(4) == 0)
				App->audio->PlayFx(4, App->audio->lose, 0);
			if (Mix_Playing(5) == 0)
				App->audio->PlayFx(5, App->audio->you_lose, 0);
			Mix_PauseMusic();
			App->audio->PlayMusic("Assets/audio/music/losemusic.ogg", 0.0F);
			playsound = false;
		}
		

		if (lose_video != 0)
		{
			lose_tex = App->video->UpdateVideo(lose_video);

			App->render->Blit(lose_tex, 0, 0, 0, 1.0F, 0);

			if (App->video->IsPlaying(lose_video) == 0)
			{
				App->video->DestroyVideo(lose_video);
				lose_video = 0;
			}
		}
		if (lose_video == 0 && Loop)
		{
			lose_video = App->video->Load("Assets/video/lose.ogv", App->render->renderer);
		}

	}

	return true;
}


