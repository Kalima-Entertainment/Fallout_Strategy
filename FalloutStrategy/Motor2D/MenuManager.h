#ifndef __MENUMANAGER_H__
#define __MENUMANAGER_H__

#include "j1Module.h"
#include <vector>

enum UI_Type;
class j1Image;
class UI_Slider;
class UI_Button;
class UI_Label;

#define MAX_UI_ELEMENTS 60

enum class Menu {
	MAIN_MENU,
	SETTINGS,
	CREDITS,
	SELECT_FACTION,
	PAUSE_MENU,
	GUI,
	BUI_BASES, 
	NO_MENU
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

struct UI_GUI {

	
	j1Image* ingame_background = nullptr;

	j1Image* nukas_label = nullptr;
	j1Image* water_label = nullptr;
	j1Image* meat_label = nullptr;

	UI_Label* nukas_count = nullptr;
	UI_Label* water_count = nullptr;
	UI_Label* meat_count = nullptr;
	int count = 0;

	

};

struct UI_GUI_Bases {

	j1Image* brotherhood_base = nullptr;
	j1Image* brotherhood_barrack = nullptr;
	j1Image* brotherhood_lab = nullptr;
	j1Image* brotherhood_melee = nullptr;
	j1Image* brotherhood_gatherer = nullptr;
	j1Image* brotherhood_ranged = nullptr;
	UI_Button* base_create_brotherhood = nullptr;
	UI_Button* lab_create_brotherhood = nullptr;
	UI_Button* barrack_create_brotherhood = nullptr;

	j1Image* ghouls_base = nullptr;
	j1Image* ghouls_barrack = nullptr;
	j1Image* ghouls_lab = nullptr;
	j1Image* ghouls_melee = nullptr;
	j1Image* ghouls_gatherer = nullptr;
	j1Image* ghouls_ranged = nullptr;
	UI_Button* base_create_ghouls = nullptr;
	UI_Button* lab_create_ghouls = nullptr;
	UI_Button* barrack_melee_create_ghouls = nullptr;
	UI_Button* barrack_ranged_create_ghouls = nullptr;

	j1Image* vault_base = nullptr;
	j1Image* vault_barrack = nullptr;
	j1Image* vault_lab = nullptr;
	j1Image* vault_gatherer = nullptr;
	j1Image* vault_melee = nullptr;
	j1Image* vault_ranged = nullptr;
	UI_Button* base_create_vault = nullptr;
	UI_Button* lab_create_vault = nullptr;
	UI_Button* barrack_create_vault = nullptr;

	j1Image* supermutant_base = nullptr;
	j1Image* supermutant_barrack = nullptr;
	j1Image* supermutant_lab = nullptr;
	j1Image* supermutant_gatherer = nullptr;
	j1Image* supermutant_melee = nullptr;
	j1Image* supermutant_ranged = nullptr;
	UI_Button* base_create_supermutant = nullptr;
	UI_Button* lab_create_supermutant = nullptr;
	UI_Button* barrack_create_supermutant = nullptr;


	UI_Button* craft_boost_brotherhood = nullptr;
	UI_Button* damage_boost_brotherhood = nullptr;
	UI_Button* velocity_boost_brotherhood = nullptr;
	UI_Button* shield_boost_brotherhood = nullptr;
	UI_Button* resource_boost_brotherhood = nullptr;

	UI_Button* craft_boost_ghouls = nullptr;
	UI_Button* damage_boost_ghouls = nullptr;
	UI_Button* velocity_boost_ghouls = nullptr;
	UI_Button* shield_boost_ghouls = nullptr;
	UI_Button* resource_boost_ghouls = nullptr;

	UI_Button* craft_boost_vault = nullptr;
	UI_Button* damage_boost_vault = nullptr;
	UI_Button* velocity_boost_vault = nullptr;
	UI_Button* shield_boost_vault = nullptr;
	UI_Button* resource_boost_vault = nullptr;

	UI_Button* craft_boost_supermutant = nullptr;
	UI_Button* damage_boost_supermutant = nullptr;
	UI_Button* velocity_boost_supermutant = nullptr;
	UI_Button* shield_boost_supermutant = nullptr;
	UI_Button* resource_boost_supermutant = nullptr;
};


class MenuManager :public j1Module {

public:

	MenuManager();
	~MenuManager();


public:
	
	void CreateMenu(Menu menu);
	void DestroyMenu(Menu menu);

	void CreateMainMenu();
	void CreateCredits();
	void DestroyCredits();
	void CreateSettings();
	void CreateCollaboratorPicture();
	void DestroyAllCollaboratorsPictures();
	void CreateSelectFaction();
	void DestroySelectFaction();
	void CreatePauseMenu();
	void DestroyPauseMenu();
	void CreatePauseSettings();
	
	void CreateGUI();
	void DestroyGUI();
	
	void DestroyAllBuildings();
	void CreateBrotherHood_Base();
	void DestroyBrotherHood_Base();
	void CreateBrotherHood_Barrack();
	void DestroyBrotherHood_Barrack();
	void CreateBrotherHood_Lab();
	void DestroyBrotherHood_Lab();

	void CreateSuperMutants_Base();
	void DestroySuperMutants_Base();
	void CreateSuperMutants_Barrack();
	void DestroySuperMutants_Barrack();
	void CreateSuperMutants_Lab();
	void DestroySuperMutants_Lab();

	void CreateGhouls_Base();
	void DestroyGhouls_Base();
	void CreateGhouls_Barrack();
	void DestroyGhouls_Barrack();
	void CreateGhouls_Lab();
	void DestroyGhouls_Lab();

	void CreateVault_Base();
	void DestroyVault_Base();
	void CreateVault_Barrack();
	void DestroyVault_Barrack();
	void CreateVault_Lab();
	void DestroyVault_Lab();


public:

	UI_credits			credits_menu;
	UI_select_faction	select_faction_menu;
	UI_pause_menu		pause_menu;
	UI_GUI				gui_ingame;
	UI_GUI_Bases		bases_hud;
	

	//UI_element	
	
	UI_element* main_menu[14];
	UI_element* settings_menu[18];
	//UI_element* credits_menu[18];
	//UI_element* select_faction_menu[16];
	//UI_element* pause_menu[];
	//UI_element* gui_ingame[];
	//UI_element* bases_hud[];

	Menu current_menu;
	Menu last_menu;

	char collaborator = 'M';
};


#endif // !__MENUMANAGER_H__