#ifndef __j1HUD_H__
#define __j1HUD_H__

#include "j1Module.h"
#include "UI_element.h"
#include "Animation.h"


struct SDL_Texture;

class UI_Label;

class j1Hud : public j1Module
{
public:
	j1Hud();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;



public:

	
	j1Timer	timer_game;
	int lifesCounter = 3;
	bool activateTimer = false;
	bool extraLife = false;
	bool drawLife = false;
	uint timer = 0;
	uint minutes = 0;

	char coins_text[10];
	char score_text[10];
	char timer_text[10];
	char minutes_text[10];
	

	

	
};



#endif // !__Hud_H__
