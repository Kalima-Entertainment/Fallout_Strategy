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
	COLLABORATORS_CREDITS,
	SELECT_FACTION,
	PAUSE_MENU,
	GUI,
	BUI_BASES,
	NO_MENU,
};

enum class FACTION {

	BROTHERHOOD,
	SUPERMUTANT,
	GHOUL,
	VAULT,
	ALL,
	NONE,
};

enum class BUILDING_TYPE {

	BASE,
	BARRACK,
	LAB,
	ALL,
	NONE,
};


class MenuManager :public j1Module {

public:

	MenuManager();
	~MenuManager();


public:

	void CreateMenu(Menu menu);
	void DestroyMenu(Menu menu);
	void DestroyFaction(Menu menu, FACTION faction, BUILDING_TYPE type);

	void CreateMainMenu();
	void CreateCredits();
	void CreateSettings();
	void CreateCollaboratorPicture();
	void CreateSelectFaction();
	void CreatePauseMenu();
	void CreatePauseSettings();
	void CreateGUI();

	void CreateBrotherHood_Base();
	void CreateBrotherHood_Barrack();
	void CreateBrotherHood_Lab();

	void CreateSuperMutants_Base();
	void CreateSuperMutants_Barrack();
	void CreateSuperMutants_Lab();


	void CreateGhouls_Base();
	void CreateGhouls_Barrack();
	void CreateGhouls_Lab();


	void CreateVault_Base();
	void CreateVault_Barrack();
	void CreateVault_Lab();



public:

	UI_element* main_menu[14];
	UI_element* settings_menu[18];
	UI_element* credits_menu[13];
	UI_element* collaborators[9];
	UI_element* select_faction_menu[12];
	UI_element* select_faction_photos[4];
	UI_element* pause_menu[7];
	UI_element* gui_ingame[4];




	UI_element* brotherhood_base[3];
	UI_element* brotherhood_barrack[5];
	UI_element* brotherhood_lab[1];

	UI_element* ghoul_base[3];
	UI_element* ghoul_barrack[5];
	UI_element* ghoul_lab[1];

	UI_element* vault_base[3];
	UI_element* vault_barrack[5];
	UI_element* vault_lab[1];

	UI_element* supermutant_base[3];
	UI_element* supermutant_barrack[5];
	UI_element* supermutant_lab[1];

	Menu current_menu;
	Menu last_menu;

	char collaborator = 'M';
};


#endif // !__MENUMANAGER_H__
