#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Audio.h"
#include "p2List.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer\include\SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

j1Audio::j1Audio() : j1Module()
{
	music = NULL;
	name.create("audio");
}

// Destructor
j1Audio::~j1Audio()
{}

// Called before render is available
bool j1Audio::Awake(pugi::xml_node& config)
{
	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	// load support for the JPG and PNG image formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	//Initialize SDL_mixer
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}

	explosion = LoadFx("audio/fx/Others/Weapons/bigexplode1.wav");

	return ret;
}

// Called before quitting
bool j1Audio::CleanUp()
{
	if(!active)
		return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if(music != NULL)
	{
		Mix_FreeMusic(music);
	}

	p2List_item<Mix_Chunk*>* item;
	for(item = fx.start; item != NULL; item = item->next)
		Mix_FreeChunk(item->data);

	fx.clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

// Play a music file
bool j1Audio::PlayMusic(const char* path, float fade_time)
{
	bool ret = true;

	if(!active)
		return false;

	if(music != NULL)
	{
		if(fade_time > 0.0f)
		{
			Mix_FadeOutMusic(int(fade_time * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS(path);

	if(music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if(fade_time > 0.0f)
		{
			if(Mix_FadeInMusic(music, -1, (int) (fade_time * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if(Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}

	LOG("Successfully playing %s", path);
	return ret;
}

// Load WAV
unsigned int j1Audio::LoadFx(const char* path)
{
	unsigned int ret = 0;

	if(!active)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV(path);

	if(chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.add(chunk);
		ret = fx.count();
	}

	return ret;
}

// Play WAV
bool j1Audio::PlayFx(int channel, unsigned int id, int repeat)
{
	bool ret = false;
	id += 0;

	if(!active)
		return false;

	if(id > 0 && id <= fx.count())
	{
		Mix_PlayChannel(channel, fx[id - 1], repeat);
	}

	return ret;
}


void j1Audio::Change_Volume_Music(float value)
{
	value = value / 100;
	Music_Volume = value;

	if (Music_Volume < 0.0f) {

		Music_Volume = 0;

	}

	if (Music_Volume > 1.0f) {

		Music_Volume = 1;

	}

	float total_volume = 128 * Music_Volume;
	Mix_VolumeMusic(total_volume);

	//LOG("%f", total_volume);

}
void j1Audio::Change_Volume_FX(float value)
{
	value = value / 100;
	fx_volume = value;

	if (fx_volume < 0) {

		fx_volume = 0;

	}

	if (fx_volume > 1) {

		fx_volume = 1;

	}
	float fx_volume_total = 128 * fx_volume;
	Mix_Volume(-1, fx_volume_total);
}

float j1Audio::Get_Music_Volume() {
	return music_volume;
}

float j1Audio::Get_FX_Volume() {
	return fx_volume;
}
