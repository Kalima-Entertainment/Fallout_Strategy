#ifndef __j1AUDIO_H__
#define __j1AUDIO_H__

#include "j1Module.h"
#include <vector>

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;
typedef unsigned int uint;

class j1Audio : public j1Module
{
public:

	j1Audio();

	// Destructor
	virtual ~j1Audio();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(int channel, unsigned int fx, int repeat = 0);

	void Change_Volume_Music(float value);
	void Change_Volume_FX(float value);

	float Music_Volume = 0;
	float Get_Music_Volume();
	float Get_FX_Volume();

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	int Animal_walk;
	int Animal_hit;
	int Animal_die;

	int Deathclaw_walk;
	int Deathclaw_hit;
	int Deathclaw_attack;
	int Deathclaw_die;

	int Mr_Handy_walk;
	int Mr_Handy_hit;
	int Mr_Handy_attack;
	int Mr_Handy_die;

	int Brotherhood_walk;
	int Brotherhood_die;
	int Brotherhood_hit;
	int Brotherhood_attack;

	int Mutant_die;
	int Mutant_hit;
	int Mutant_attack;

	int Vault_die;
	int Vault_hit;
	int Vault_attack;

	int Ghoul_die;
	int Ghoul_hit;
	int Ghoul_attack;

	int pistol;
	int pistol2;
	int minigun;
	int explode;
	int factory;

	int intro_fx;
	int F_press;
	int Press_F_to_start;

	int loading;
	int win;
	int lose;
	int you_win;
	int you_lose;

	int click_fx;
	int hover_fx;
	int back_fx;
	int volume_fx;
	int members_fx;
	int character_fx;

	float				music_volume;
	float				fx_volume;

	_Mix_Music* music = NULL;
	std::vector<Mix_Chunk*> fx;

private:

	



};

#endif // __j1AUDIO_H__
