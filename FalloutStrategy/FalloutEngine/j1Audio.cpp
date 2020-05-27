#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Audio.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer\include\SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

j1Audio::j1Audio() : j1Module()
{
	music = NULL;
	name = ("audio");

	die_sound = false;
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

	Animal_walk = LoadFx("Assets/audio/fx/CharactersSounds/Animals/Brahmin/Walk.wav");
	Animal_hit = LoadFx("Assets/audio/fx/CharactersSounds/Animals/Brahmin/Hit1.wav");
	Animal_die = LoadFx("Assets/audio/fx/CharactersSounds/Animals/Brahmin/Death1.wav");

	Deathclaw_walk = LoadFx("Assets/audio/fx/CharactersSounds/Deathclaw/babWalk.wav");
	Deathclaw_hit = LoadFx("Assets/audio/fx/CharactersSounds/Deathclaw/babHit2.wav");
	Deathclaw_attack = LoadFx("Assets/audio/fx/CharactersSounds/Deathclaw/Attack2.wav");
	Deathclaw_die = LoadFx("Assets/audio/fx/CharactersSounds/Deathclaw/babDeath1.wav");

	Mr_Handy_walk = LoadFx("Assets/audio/fx/CharactersSounds/Mr_Handy/Mr_Handy_walk.wav");
	Mr_Handy_hit = LoadFx("Assets/audio/fx/CharactersSounds/Mr_Handy/Mr_Handy_hit.wav");
	Mr_Handy_attack = LoadFx("Assets/audio/fx/CharactersSounds/Mr_Handy/Mr_Handy_attack.wav");
	Mr_Handy_die = LoadFx("Assets/audio/fx/CharactersSounds/Mr_Handy/Mr_Handy_die.wav");

	Brotherhood_walk = LoadFx("Assets/audio/fx/CharactersSounds/Brotherhood/Brotherhood_Walk.wav");
	Brotherhood_die = LoadFx("Assets/audio/fx/CharactersSounds/Brotherhood/Brotherhood_Die.wav");
	Brotherhood_hit = LoadFx("Assets/audio/fx/CharactersSounds/Brotherhood/Brotherhood_Hit.wav");
	Brotherhood_attack = LoadFx("Assets/audio/fx/CharactersSounds/Brotherhood/Brotherhood_Attack.wav");

	Mutant_die = LoadFx("Assets/audio/fx/CharactersSounds/Mutants/SuperMutant_Die.wav");
	Mutant_hit = LoadFx("Assets/audio/fx/CharactersSounds/Mutants/SuperMutant_Hit.wav");
	Mutant_attack = LoadFx("Assets/audio/fx/CharactersSounds/Mutants/SuperMutant_Attack.wav");

	Vault_die = LoadFx("Assets/audio/fx/CharactersSounds/VaultDwellers/VaultDwellers_Die.wav");
	Vault_hit = LoadFx("Assets/audio/fx/CharactersSounds/VaultDwellers/VaultDwellers_Hit.wav");
	Vault_attack = LoadFx("Assets/audio/fx/CharactersSounds/VaultDwellers/VaultDwellers_Attack.wav");

	Ghoul_die = LoadFx("Assets/audio/fx/CharactersSounds/Ghouls/Ghouls_Die.wav");
	Ghoul_hit = LoadFx("Assets/audio/fx/CharactersSounds/Ghouls/Ghouls_Hit.wav");
	Ghoul_attack = LoadFx("Assets/audio/fx/CharactersSounds/Ghouls/Ghouls_Attack.wav");

	pistol = LoadFx("Assets/audio/fx/Others/Weapons/pistolheavySingle1.wav");
	pistol2 = LoadFx("Assets/audio/fx/Others/Weapons/pistolrevolverSingle1.wav");
	minigun = LoadFx("Assets/audio/fx/Others/Weapons/minigunBurst1.wav");
	explode = LoadFx("Assets/audio/fx/Others/Weapons/bigexplode1.wav");
	factory = LoadFx("Assets/audio/fx/Others/Weapons/factory_working.wav");

	intro_fx = App->audio->LoadFx("Assets/audio/fx/Intro/Intro_fx.wav");
	F_press = App->audio->LoadFx("Assets/audio/fx/UISounds/Butn_CheckBox.wav");
	Press_F_to_start = App->audio->LoadFx("Assets/audio/fx/UISounds/Butn_Text.wav");
	loading = App->audio->LoadFx("Assets/audio/fx/Intro/loading.wav");
	win = App->audio->LoadFx("Assets/audio/fx/WinLose/WinSound.wav");
	lose = App->audio->LoadFx("Assets/audio/fx/WinLose/LoseSound.wav");
	you_win = App->audio->LoadFx("Assets/audio/fx/WinLose/you_win.wav");
	you_lose = App->audio->LoadFx("Assets/audio/fx/WinLose/you_lose.wav");
	upgrade_fx = App->audio->LoadFx("Assets/audio/fx/UISounds/Butn_Mission.wav");
	
	click_fx = App->audio->LoadFx("Assets/audio/fx/UISounds/Butn_Arrow.wav");
	hover_fx = App->audio->LoadFx("Assets/audio/fx/UISounds/Butn_Slider.wav");
	back_fx = App->audio->LoadFx("Assets/audio/fx/UISounds/Butn_ReadyOff.wav");
	volume_fx = App->audio->LoadFx("Assets/audio/fx/UISounds/Butn_Text.wav");
	members_fx = App->audio->LoadFx("Assets/audio/fx/UISounds/Butn_Skill.wav");
	character_fx = App->audio->LoadFx("Assets/audio/fx/UISounds/Butn_Character.wav");

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

	for (int i = 0; i < fx.size(); i++) {
		Mix_FreeChunk(fx[i]);
	}


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
		fx.push_back(chunk);
		ret = fx.size();
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

	if(id > 0 && id <= fx.size())
	{
		Mix_PlayChannel(channel, fx[id - 1], repeat);
	}

	return ret;
}

void j1Audio::Change_Volume_Music(float value)
{
	value = value / 50;
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
	value = value / 50;
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

// Load Game State
bool j1Audio::Load(pugi::xml_node& data)
{
	music_volume = data.child("volume").attribute("music").as_float();
	fx_volume = data.child("volume").attribute("fx").as_float();

	Change_Volume_FX(fx_volume);
	Change_Volume_Music(music_volume);

	return true;
}

// Save Game State
bool j1Audio::Save(pugi::xml_node& data) const
{
	pugi::xml_node volume = data.append_child("volume");

	volume.append_attribute("music") = music_volume;
	volume.append_attribute("fx") = fx_volume;

	return true;

}
