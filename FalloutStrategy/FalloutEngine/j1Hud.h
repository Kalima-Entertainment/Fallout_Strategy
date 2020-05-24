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

	SDL_Rect background_bar;
	SDL_Rect foreground_bar;
	bool draw_health;
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
	char minutes_text[15];
	
	int gatherer_amount = 1;
	int melee_amount = 1;
	int ranged_amount = 1;
	
	bool finish_base;
	bool finish_barrack;
	bool finish_lab;
	int entity;
	
};



#endif // !__Hud_H__