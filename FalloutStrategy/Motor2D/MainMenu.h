#ifndef __MAINMENU_H__
#define __MAINMENU_H__

#include "j1Module.h"

class MainMenu : public j1Module
{
public:
	
	MainMenu();
	~MainMenu();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool Update(float dt);
	bool CleanUp();

public:

};
#endif
