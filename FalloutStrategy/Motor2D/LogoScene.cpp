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


LogoScene::LogoScene() : j1Module()
{
	name = ("logo_scene");
}

// Destructor
LogoScene::~LogoScene() {
}

bool LogoScene::Start()
{
	App->audio->PlayFx(1, App->audio->intro_fx, 0);
	my_video = App->video->Load("Assets/video/intro.ogv", App->render->renderer);
	

	return true;
}

bool LogoScene::CleanUp() {
	bool ret = true;
	
	return ret;
}

bool LogoScene::Update(float dt) {
	

	if(App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		App->audio->PlayFx(2, App->audio->F_press, 0);
		App->audio->PlayMusic("Assets/audio/music/FalloutStrategyMainTheme.ogg", 4.0F);
		App->logo_scene->Disable();
		App->video->DestroyVideo(my_video);
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

	return true;
}


