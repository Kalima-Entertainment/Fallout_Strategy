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
	j1Image* image = nullptr;

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

struct UI_settings {

	UI_Label* fps = nullptr;
	UI_Label* fx_volume = nullptr;
	UI_Label* music_volume = nullptr;
	UI_Label* settings_text = nullptr;

	j1Image* background = nullptr;
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

struct UI_select_faction {

	j1Image* background = nullptr;
	j1Image* ghouls = nullptr;
	j1Image* brotherhood = nullptr;
	j1Image* supermutants = nullptr;
	j1Image* vault = nullptr;

	UI_Button* ghouls_button = nullptr;
	UI_Button* brotherhood_button = nullptr;
	UI_Button* supermutants_button = nullptr;
	UI_Button* vault_button = nullptr;
	UI_Button* back_button = nullptr;
	UI_Button* start_game_button = nullptr;

	UI_Label* ghouls_label = nullptr;
	UI_Label* brotherhood_label = nullptr;
	UI_Label* supermutants_label = nullptr;
	UI_Label* vault_label = nullptr;
	UI_Label* start_game_label = nullptr;
};

struct UI_pause_menu {

	j1Image* panel = nullptr;

	UI_Button* resume_button = nullptr;
	UI_Button* settings_button = nullptr;
	UI_Button* back_to_main_menu_button = nullptr;

	UI_Label* resume_label = nullptr;
	UI_Label* settings_label = nullptr;
	UI_Label* back_to_main_menu_label = nullptr;

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
	void CreateSelectFaction();
	void DestroySelectFaction();
	void CreatePauseMenu();
	void DestroyPauseMenu();
	void CreatePauseSettings();
	void DestroyPauseSettings();

public:

	UI_main_menu		main_menu;
	UI_credits			credits_menu;
	UI_select_faction	select_faction_menu;
	UI_settings			settings_menu;
	UI_pause_menu		pause_menu;

	char collaborator = 'M';
	

};


#endif // !__MENUMANAGER_H__