#ifndef __MAINMENU_H__
#define __MAINMENU_H__

#include "j1Module.h"
#include "Animation.h"
#include "j1App.h"

class MainMenu : public j1Module
{
public:
	
	MainMenu();
	~MainMenu();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	bool MainMenu::LoadAnimations();

private:

	Animation* loader;
	Animation	animationTitle;
	SDL_Texture* title_tex;
	float last_dt;
};
#endif
