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
	
	
	UI_Label* fps = nullptr;
	UI_Label* fx_volume = nullptr;
	UI_Label* music_volume = nullptr;

	j1Image* panel_volume = nullptr;
	j1Image* mute = nullptr;
	j1Image* panel_cap = nullptr;
	j1Image* fps_image = nullptr;

	UI_Slider* volume_music_slider = nullptr;
	UI_Button* left_music_button = nullptr;
	UI_Button* right_music_button = nullptr;
	
	UI_Slider* volume_fx_slider = nullptr;
	UI_Button* left_fx_button = nullptr;
	UI_Button* right_fx_button = nullptr;

	UI_Button* cap_button = nullptr;
	UI_Button* mute_button = nullptr;
	UI_Button* back_button = nullptr;
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
	void CreateSettings();
	void DestroySettings();

public:

	UI_main_menu	main_menu;
	
	
};


#endif // !__MENUMANAGER_H__