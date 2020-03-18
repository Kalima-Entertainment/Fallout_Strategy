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

struct UI_credits {

	j1Image* background = nullptr;
	j1Image* marc_image = nullptr;
	j1Image* javi_image = nullptr;
	j1Image* pablo_image = nullptr;
	j1Image* german_image = nullptr;
	j1Image* macia_image = nullptr;
	j1Image* pol_image = nullptr;
	j1Image* silvino_image = nullptr;
	j1Image* cristian_image = nullptr;

	UI_Button* back_button = nullptr;
	UI_Button* github_button = nullptr;
	UI_Button* web_button = nullptr;
	UI_Button* twitter_button = nullptr;
	UI_Button* marc_button = nullptr;
	UI_Button* javi_button = nullptr;
	UI_Button* pablo_button = nullptr;
	UI_Button* german_button = nullptr;
	UI_Button* macia_button = nullptr;
	UI_Button* pol_button = nullptr;
	UI_Button* silvino_button = nullptr;
	UI_Button* cristian_button = nullptr;

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
	void CreateCollaboratorPicture();
	void DestroyAllCollaboratorsPictures();

public:

	UI_main_menu	main_menu;
	UI_credits	credits_menu;
	char collaborator = 'M';
	
};


#endif // !__MENUMANAGER_H__