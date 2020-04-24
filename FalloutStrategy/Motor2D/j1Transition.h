#ifndef __j1TRANSITION_H__
#define __j1TRANSITION_H__

#include "PugiXml/src/pugixml.hpp"
#include "j1Module.h"
#include "Animation.h"
#include "p2List.h"
#include "p2Point.h"
#include "j1Timer.h"

struct SDL_Texture;

class j1Transition : public j1Module
{
public:
	j1Transition();
	bool j1Transition::LoadAnimations();
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	~j1Transition();
	bool CleanUp();
	void Transition();
	void StartTimer();

private:
	float speed_reducer;
	SDL_Texture* gif_tex;
	SDL_Texture* logo_tex;
	SDL_Texture* background;
	Animation	animationLogo;
	Animation	animationGif;
	Animation* loader;
	float lastdt;
public:
	bool transition;
	j1Timer fadetimer;

};



#endif