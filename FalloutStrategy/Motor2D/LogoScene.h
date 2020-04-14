#ifndef __LOGOSCENE__
#define __LOGOSCENE__

#include "j1Module.h"
#include "j1Timer.h"
#include "Animation.h"

struct SDL_Texture;

class LogoScene : public j1Module
{
public:

	LogoScene();

	// Destructor
	virtual ~LogoScene();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PostUpdate();

	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	
	bool LogoScene::LoadAnimations();


private:
	Animation	animationLogo;
	Animation	animationStartGameText;
	Animation* loader;
	SDL_Texture* logo_tex;
	SDL_Texture* start_game_tex;
	SDL_Rect start_game_rect;
	float last_dt;
	bool renderLogo = true;
	j1Timer Timer;
	bool drawable = true;
};

#endif // __j1LOGOSCENE_H__
