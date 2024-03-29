#ifndef __LOGOSCENE__
#define __LOGOSCENE__

#include "j1Module.h"
#include "j1Timer.h"
#include "Animation.h"
#include "j1App.h"

struct SDL_Texture;

class LogoScene : public j1Module
{
public:

	LogoScene();

	// Destructor
	virtual ~LogoScene();

	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	
	SDL_Texture* video_texture;
	SDL_Texture* win_tex;
	SDL_Texture* lose_tex;
	int my_video;
	int win_video;
	int lose_video;
	bool Loop;
	bool playsound;

private:
	
	float last_dt;
	j1Timer Timer;
	bool drawable;
	bool sound_end;
	bool quit;

};

#endif // __j1LOGOSCENE_H__
