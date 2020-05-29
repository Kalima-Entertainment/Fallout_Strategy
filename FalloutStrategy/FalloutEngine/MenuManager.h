#ifndef __MENUMANAGER_H__
#define __MENUMANAGER_H__

#include "j1Module.h"
#include <vector>

enum UI_Type;
class j1Image;
class UI_Slider;
class UI_Button;
class UI_Label;
enum Faction;
enum EntityType;

#define MAX_UI_ELEMENTS 60

enum class Menu {
	
	MAIN_MENU,
	CREDITS,
	SETTINGS,
	PAUSE_SETTINGS,
	SELECT_FACTION,
	PAUSE_MENU,
	GUI,
	RESOURCES,
	TUTORIAL,
	HOW_TO_PLAY,
	BUI_BASES,
	COLLABORATORS_CREDITS,
	DIALOG,
	QUEST,
	GATHERER_QUANTITY,
	MELEE_QUANTITY,
	RANGED_QUANTITY,
	NO_MENU,
};

class MenuManager :public j1Module {

public:

	MenuManager();
	~MenuManager();


public:

	void CreateMenu(Menu menu);
	void CreateMenuFaction(Menu menu, Faction faction, EntityType type);
	void DestroyMenu(Menu menu);
	void DestroyFaction(Menu menu, Faction faction, EntityType type);

public:

	UI_element* main_menu[11];
	UI_element* settings_menu[23];
	UI_element* credits_menu[5];
	UI_element* collaborators[9];
	UI_element* select_faction_menu[10];
	UI_element* select_faction_photos[4];
	UI_element* pause_menu[7];
	UI_element* gui_ingame[3];
	UI_element* tutorial[8];
	UI_element* how_to_play[6];
	UI_element* tutorial_screen;
	UI_element* info_button2_element;
	UI_element* quest_button2_element;
	UI_Label*	timer_item;
	UI_Label*	timer_minutes;
	UI_element* resources[3];
	UI_element* quest[12];

	UI_element* brotherhood_base[14];
	UI_element* brotherhood_barrack[21];
	UI_element* brotherhood_lab[19];

	UI_element* ghoul_base[14];
	UI_element* ghoul_barrack[21];
	UI_element* ghoul_lab[19];

	UI_element* vault_base[14];
	UI_element* vault_barrack[21];
	UI_element* vault_lab[19];

	UI_element* supermutant_base[14];
	UI_element* supermutant_barrack[21];
	UI_element* supermutant_lab[19];

	UI_element* win_scene[5];

	UI_element* dialogs[11];

	UI_Label* cap_label;
	UI_Label* fullscreen_label;
	
	UI_Label* gatherer_label;
	UI_Label* melee_label;
	UI_Label* ranged_label;

	Menu current_menu;
	Menu last_menu;

	Faction current_building_faction;
	EntityType current_building_type;

	int how_to_play_counter;
	int cost;

	std::string water;
	std::string food;
	std::string caps;

	std::string gatherer_quantity;
	std::string melee_quantity;
	std::string ranged_quantity;

};


#endif // !__MENUMANAGER_H__
