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
	bool PostUpdate(float dt);
	~j1Transition();
	bool CleanUp();
	void Transition();
private:
	float deltatime;
	SDL_Texture* gif;
	SDL_Texture* logo;
	Animation	animationLogo;
	Animation	animationGif;
	Animation* loader;
	float lastdt;

public:
	bool transition;
	j1Timer fadetimer;

};



#endif