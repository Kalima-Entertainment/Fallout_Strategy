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
	bool activateTimer;
	bool draw_health;
	uint timer;
	uint minutes;

	char timer_text[10];
	char minutes_text[15];
	
	int gatherer_amount;
	int melee_amount;
	int ranged_amount;
	
	bool finish_base;
	bool finish_barrack;
	bool finish_lab;
	int entity;
	
};



#endif // !__Hud_H__
