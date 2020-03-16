#ifndef __MENUMANAGER_H__
#define __MENUMANAGER_H__

#include "j1Module.h"

enum UI_Type;
class j1Image;
class UI_Slider;
class UI_Button;
class UI_Label;

struct UI_main_menu {

	j1Image* background = nullptr;
	j1Image* logo = nullptr;
	j1Image* panel = nullptr;

	UI_Label* new_game_text = nullptr;
	UI_Label* load_game_text = nullptr;
	UI_Label* settings_text = nullptr;
	UI_Label* credits_text = nullptr;
	UI_Label* exit_text = nullptr;

	UI_Button* new_game_button = nullptr;
	UI_Button* load_game_button = nullptr;
	UI_Button* settings_button = nullptr;
	UI_Button* credits_button = nullptr;
	UI_Button* exit_button = nullptr;
	

};

struct UI_credits {

	j1Image* background = nullptr;
	
};

class MenuManager :public j1Module {

public:

	MenuManager();
	~MenuManager();


public:
	
	void CreateMainMenu();
	void DestroyMainMenu();
	void CreateCredits();
	void DestroyCredits();

public:

	UI_main_menu	main_menu;
	UI_credits	credits_menu;
	
};


#endif // !__MENUMANAGER_H__