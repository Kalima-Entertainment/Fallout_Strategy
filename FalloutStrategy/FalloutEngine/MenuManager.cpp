﻿#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include <string>
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1Gui.h"
#include "j1Scene.h"
#include "j1EntityManager.h"
#include "j1Entity.h"
#include "DynamicEntity.h"
#include "UI_Button.h"
#include "UI_Slider.h"
#include "UI_Label.h"
#include "j1Image.h"
#include "MenuManager.h"
#include "j1Player.h"
#include "p2Defs.h"
#include "UI_InputText.h"
#include "DialogManager.h"
#include "j1Hud.h"
#include "j1Minimap.h"
#include "GenericPlayer.h"
#include "StaticEntity.h"

#include "brofiler/Brofiler/Brofiler.h"
#include "SDL_mixer/include/SDL_mixer.h"

MenuManager::MenuManager()
{
	name = ("menu_manager");
	current_menu = last_menu = Menu::NO_MENU;
	current_building_faction = NO_FACTION;
	current_building_type = NO_TYPE;
	cost = 0;
	cap_label = nullptr;
	fullscreen_label = nullptr;
	gatherer_label = nullptr;
	melee_label = nullptr;
	ranged_label = nullptr;
	
	for(int i = 0; i <= 10; i++) {
		main_menu[i] = nullptr;
	}
	for(int i = 0; i <= 22; i++) {
		settings_menu[i] = nullptr;
	}
	for(int i = 0; i <= 4; i++) {
		credits_menu[i] = nullptr;
	}
	for(int i = 0; i <= 8; i++) {
		collaborators[i] = nullptr;
	}
	for(int i = 0; i <= 9; i++) {
		select_faction_menu[i] = nullptr;
	}
	for(int i = 0; i <= 7; i++) {
		group_photos[i] = nullptr;
	}
	for(int i = 0; i <= 6; i++) {
		pause_menu[i] = nullptr;
	}
	for(int i = 0; i <= 3; i++) {
		gui_ingame[i] = nullptr;
	}
	for(int i = 0; i <= 7; i++) {
		tutorial[i] = nullptr;
	}
	for(int i = 0; i <= 5; i++) {
		how_to_play[i] = nullptr;
	}
	for(int i = 0; i <= 2; i++) {
		resources[i] = nullptr;
	}
	for(int i = 0; i <= 11; i++) {
		quest[i] = nullptr;
	}
	for (int i = 0; i <= 5; i++) {
		radio[i] = nullptr;
	}

	tutorial_screen = nullptr;
	info_button2_element = nullptr;
	quest_button2_element = nullptr;
	timer_item = nullptr;
	timer_minutes = nullptr;
	cap_label = nullptr;
	fullscreen_label = nullptr;

	gatherer_label = nullptr;
	melee_label = nullptr;
	ranged_label = nullptr;

	how_to_play_counter = 0;

	for(int i = 0; i <= 13; i++) {
		brotherhood_base[i] = nullptr;
	}
	for(int i = 0; i <= 20; i++) {
		brotherhood_barrack[i] = nullptr;
	}
	for(int i = 0; i <= 18; i++) {
		brotherhood_lab[i] = nullptr;
	}


	for(int i = 0; i <= 13; i++) {
		ghoul_base[i] = nullptr;
	}
	for(int i = 0; i <= 20; i++) {
		ghoul_barrack[i] = nullptr;
	}
	for(int i = 0; i <= 18; i++) {
		ghoul_lab[i] = nullptr;
	}


	for(int i = 0; i <= 13; i++) {
		vault_base[i] = nullptr;
	}
	for(int i = 0; i <= 20; i++) {
		vault_barrack[i] = nullptr;
	}
	for(int i = 0; i <= 18; i++) {
		vault_lab[i] = nullptr;
	}


	for(int i = 0; i <= 13; i++) {
		supermutant_base[i] = nullptr;
	}
	for(int i = 0; i <= 20; i++) {
		supermutant_barrack[i] = nullptr;
	}
	for(int i = 0; i <= 18; i++) {
		supermutant_lab[i] = nullptr;
	}

	for(int i = 0; i <= 4; i++) {
		win_scene[i] = nullptr;
	}
	for(int i = 0; i <= 10; i++) {
		dialogs[i] = nullptr;
	}


	
	
}

MenuManager::~MenuManager()
{
}

void MenuManager::CreateMenu(Menu menu) {

	switch (menu)
	{
	case Menu::MAIN_MENU:

		//Images															
		main_menu[0] = dynamic_cast<j1Image*>(App->gui->CreateImage(-5, 0, Image, { 1529, 0, 1502, 775 }, NULL, this));

		//Buttons
		main_menu[1] = dynamic_cast<UI_Button*>(App->gui->CreateButton(260, 560, button_new_game, { 1372,927,187,49 }, { 1372,987,187,49 }, { 1372,1046,187,49 }, NULL, this));
		main_menu[2] = dynamic_cast<UI_Button*>(App->gui->CreateButton(460, 560, button_load_game, { 1372,927,187,49 }, { 1372,987,187,49 }, { 1372,1046,187,49 }, NULL, this));
		main_menu[3] = dynamic_cast<UI_Button*>(App->gui->CreateButton(160, 620, button_settings, { 1372,927,187,49 }, { 1372,987,187,49 }, { 1372,1046,187,49 }, NULL, this));
		main_menu[4] = dynamic_cast<UI_Button*>(App->gui->CreateButton(360, 620, button_credits, { 1372,927,187,49 }, { 1372,987,187,49 }, { 1372,1046,187,49 }, NULL, this));
		main_menu[5] = dynamic_cast<UI_Button*>(App->gui->CreateButton(560, 620, button_exit, { 1372,927,187,49 }, { 1372,987,187,49 }, { 1372,1046,187,49 }, NULL, this));

		//Labels
		main_menu[6] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(305, 575, Label, "NEW GAME", NULL, this, "StackedPixelSmall"));
		main_menu[7] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(505, 575, Label, "LOAD GAME", NULL, this, "StackedPixelSmall"));
		main_menu[8] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(210, 635, Label, "SETTINGS", NULL, this, "StackedPixelSmall"));
		main_menu[9] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(415, 635, Label, "CREDITS", NULL, this, "StackedPixelSmall"));
		main_menu[10] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(630, 635, Label, "EXIT", NULL, this, "StackedPixelSmall"));

		App->render->camera.x = 0;
		App->render->camera.y = 0;

		last_menu = current_menu;
		current_menu = Menu::MAIN_MENU;
		break;

	case Menu::CREDITS:

		//Images
		credits_menu[0] = dynamic_cast<j1Image*>(App->gui->CreateImage(0, 0, Image, { 1287, 2430, 1281, 730 }, NULL, this));

		//Buttons
		credits_menu[1] = dynamic_cast<UI_Button*>(App->gui->CreateButton(65, 65, button_back_credits, { 1243,2014,73,78 }, { 1243,2102,73,79 }, { 1243,2188,73,71 }, NULL, this));
		credits_menu[2] = dynamic_cast<UI_Button*>(App->gui->CreateButton(65, 200, button_github_credits, { 2181,841,73,78 }, { 2181,919,73,78 }, { 2181,1005,73,78 }, NULL, this));
		credits_menu[3] = dynamic_cast<UI_Button*>(App->gui->CreateButton(65, 300, button_web_credits, { 2268,841,73,78 }, { 2268,919,73,78 }, { 2268,1005,73,78 }, NULL, this));
		credits_menu[4] = dynamic_cast<UI_Button*>(App->gui->CreateButton(65, 400, button_twitter_credits, { 2355,841,73,78 }, { 2355,919,73,78 }, { 2355,1005,73,78 }, NULL, this));

		group_photos[0] = dynamic_cast<UI_Button*>(App->gui->CreateButton(450, 127, marc_photo, { 11, 541, 166, 25 }, { 11, 541,166, 25 }, { 11, 541, 166, 25 }, NULL, this));
		group_photos[1] = dynamic_cast<UI_Button*>(App->gui->CreateButton(450, 172, javi_photo, { 11, 541, 166, 25 }, { 11, 541, 166, 25 }, { 11, 541, 166, 25 }, NULL, this));
		group_photos[2] = dynamic_cast<UI_Button*>(App->gui->CreateButton(450, 217, pablo_photo, { 11, 541, 166, 25 }, { 11, 541, 166, 25 }, { 11, 541, 166, 25 }, NULL, this));
		group_photos[3] = dynamic_cast<UI_Button*>(App->gui->CreateButton(450, 262, german_photo, { 11, 541, 166, 25 }, { 11, 541, 166, 25 }, { 11, 541, 166, 25 }, NULL, this));
		group_photos[4] = dynamic_cast<UI_Button*>(App->gui->CreateButton(450, 307, macia_photo, { 11, 541, 166, 25 }, { 11, 541, 166, 25 }, { 11, 541, 166, 25 }, NULL, this));
		group_photos[5] = dynamic_cast<UI_Button*>(App->gui->CreateButton(450, 352, pol_photo, { 11, 541, 166, 25 }, { 11, 541, 166, 25 }, { 11, 541, 166, 25 }, NULL, this));
		group_photos[6] = dynamic_cast<UI_Button*>(App->gui->CreateButton(450, 397, silvino_photo, { 11, 541, 166, 25 }, { 11, 541, 166, 25 }, { 11, 541, 166, 25 }, NULL, this));
		group_photos[7] = dynamic_cast<UI_Button*>(App->gui->CreateButton(450, 442, christian_photo, { 11, 541, 166, 25 }, { 11, 541, 166, 25 }, { 11, 541, 166, 25 }, NULL, this));

		last_menu = current_menu;
		current_menu = Menu::CREDITS;

		break;
	case Menu::SETTINGS:

		//Images
		settings_menu[0] = dynamic_cast<j1Image*>(App->gui->CreateImage(-5, 0, Image, { 1529, 0, 1502, 775 }, NULL, this));
		settings_menu[1] = dynamic_cast<j1Image*>(App->gui->CreateImage(310, 125, Image, { 0, 142, 658, 358 }, NULL, this));
		settings_menu[2] = dynamic_cast<j1Image*>(App->gui->CreateImage(420, 500, Image, { 1017, 352, 455, 190 }, NULL, this));
		settings_menu[3] = dynamic_cast<j1Image*>(App->gui->CreateImage(395, 275, Image, { 1674, 981, 20, 36 }, NULL, this));
		settings_menu[4] = dynamic_cast<j1Image*>(App->gui->CreateImage(505, 600, Image, { 1599, 1054, 77, 52 }, NULL, this));
		settings_menu[19] =dynamic_cast<j1Image*>(App->gui->CreateImage(740, 600, Image, { 1599, 1054, 77, 52 }, NULL, this));
		//Labels
		settings_menu[5] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(450, 535, Label, "REFRESH RATE", NULL, this, "StackedPixelMedium"));
		settings_menu[6] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(360, 335, Label, "SOUND EFFECTS VOLUME", NULL, this,"StackedPixel" ));
		settings_menu[7] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(360, 150, Label, "MUSIC VOLUME", NULL, this, "StackedPixel"));
		settings_menu[8] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(555, 50, Label, "SETTINGS", NULL, this, "StackedPixel"));
		settings_menu[18]= dynamic_cast<UI_Label*>(App->gui->CreateLabel(685, 535, Label, "FULLSCREEN", NULL, this, "StackedPixelMedium"));

		if (App->gui->fullscreen == true) {
			settings_menu[22] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(756, 613, Label, "YES", NULL, this, "StackedPixelMedium"));
		}
		else {
			settings_menu[22] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(763, 613, Label, "NO", NULL, this, "StackedPixelMedium"));
		}

		if (App->gui->cap == true) {
			settings_menu[21] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(528, 613, Label, "30", NULL, this,"StackedPixelMedium"));
		}
		else {
			settings_menu[21] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(528, 613, Label, "60", NULL, this, "StackedPixelMedium"));
		}

		//Music Slider
		settings_menu[9] =  dynamic_cast<UI_Slider*>(App->gui->CreateSlider(465, 220, Slider_music, { 1834,842,329,27 }, { 1792,926,33,16 }, 400, NULL, this));
		settings_menu[10] = dynamic_cast<UI_Button*>(App->gui->CreateButton(435, 220, Button_slider_music_left, { 1671,880,26,25 }, { 1738,880,26,25 }, { 1738,880,26,25 }, NULL, this));
		settings_menu[11] = dynamic_cast<UI_Button*>(App->gui->CreateButton(787, 220, Button_slider_music_right, { 1704,880,26,25 }, { 1771,880,26,25 }, { 1771,880,26,25 }, NULL, this));

		//Fx Slider
		settings_menu[12] = dynamic_cast<UI_Slider*>(App->gui->CreateSlider(465, 405, Slider_fx, { 1834,842,329,27 }, { 1792,926,33,16 }, 400, NULL, this));
		settings_menu[13] = dynamic_cast<UI_Button*>(App->gui->CreateButton(435, 405, Button_slider_fx_left, { 1671,880,26,25 }, { 1738,880,26,25 }, { 1738,880,26,25 }, NULL, this));
		settings_menu[14] = dynamic_cast<UI_Button*>(App->gui->CreateButton(796, 405, Button_slider_fx_right, { 1704,880,26,25 }, { 1771,880,26,25 }, { 1771,880,26,25 }, NULL, this));

		//Buttons
		settings_menu[15] = dynamic_cast<UI_Button*>(App->gui->CreateButton(470, 615, button_cap, { 1671,922,25,26 }, { 1713,922,25,26 }, { 1713,922,25,26 }, NULL, this));
		settings_menu[16] = dynamic_cast<UI_Button*>(App->gui->CreateButton(360, 280, button_mute, { 1671,922,25,26 }, { 1713,922,25,26 }, { 1713,922,25,26 }, NULL, this));
		settings_menu[17] = dynamic_cast<UI_Button*>(App->gui->CreateButton(40, 40, button_back, { 1704,1054,48,46 }, { 1765,1054,48,46 }, { 1765,1054,48,46 }, NULL, this));
		settings_menu[20] = dynamic_cast<UI_Button*>(App->gui->CreateButton(705, 615, button_fullscreen, { 1671,922,25,26 }, { 1713,922,25,26 }, { 1713,922,25,26 }, NULL, this));

		last_menu = current_menu;
		current_menu = Menu::SETTINGS;

		break;

	case Menu::PAUSE_SETTINGS:

		//Images
		settings_menu[0] = dynamic_cast<j1Image*>(App->gui->CreateImage(-5, 0, Image, { 1529, 0, 1502, 775 }, NULL, this));
		settings_menu[1] = dynamic_cast<j1Image*>(App->gui->CreateImage(310, 125, Image, { 0, 142, 658, 358 }, NULL, this));
		settings_menu[2] = dynamic_cast<j1Image*>(App->gui->CreateImage(420, 500, Image, { 1017, 352, 455, 190 }, NULL, this));
		settings_menu[3] = dynamic_cast<j1Image*>(App->gui->CreateImage(395, 275, Image, { 1674, 981, 20, 36 }, NULL, this));
		settings_menu[4] = dynamic_cast<j1Image*>(App->gui->CreateImage(505, 600, Image, { 1599, 1054, 77, 52 }, NULL, this));
		settings_menu[19]= dynamic_cast<j1Image*>(App->gui->CreateImage(740, 600, Image, { 1599, 1054, 77, 52 }, NULL, this));
		//Labels
		settings_menu[5] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(450, 535, Label, "REFRESH RATE", NULL, this, "StackedPixelMedium"));
		settings_menu[6] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(360, 335, Label, "SOUND EFFECTS VOLUME", NULL, this, "StackedPixel"));
		settings_menu[7] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(360, 150, Label, "MUSIC VOLUME", NULL, this, "StackedPixel"));
		settings_menu[8] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(555, 50, Label, "SETTINGS", NULL, this, "StackedPixel"));
		settings_menu[18]= dynamic_cast<UI_Label*>(App->gui->CreateLabel(685, 535, Label, "FULLSCREEN", NULL, this, "StackedPixelMedium"));

		if (App->gui->fullscreen == true) {
			settings_menu[22] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(756, 613, Label, "YES", NULL, this, "StackedPixelMedium"));
		}
		else {
			settings_menu[22] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(763, 613, Label, "NO", NULL, this, "StackedPixelMedium"));
		}

		if (App->gui->cap == true) {
			settings_menu[21] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(528, 613, Label, "30", NULL, this, "StackedPixelMedium"));
		}
		else {
			settings_menu[21] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(528, 613, Label, "60", NULL, this, "StackedPixelMedium"));
		}

		//Music Slider
		settings_menu[9] = dynamic_cast<UI_Slider*>(App->gui->CreateSlider(465, 220, Slider_music, { 1834,842,329,27 }, { 1792,926,33,16 }, 400, NULL, this));
		settings_menu[10] = dynamic_cast<UI_Button*>(App->gui->CreateButton(435, 220, Button_slider_music_left, { 1671,880,26,25 }, { 1738,880,26,25 }, { 1738,880,26,25 }, NULL, this));
		settings_menu[11] = dynamic_cast<UI_Button*>(App->gui->CreateButton(787, 220, Button_slider_music_right, { 1704,880,26,25 }, { 1771,880,26,25 }, { 1771,880,26,25 }, NULL, this));

		//Fx Slider
		settings_menu[12] = dynamic_cast<UI_Slider*>(App->gui->CreateSlider(465, 405, Slider_fx, { 1834,842,329,27 }, { 1792,926,33,16 }, 400, NULL, this));
		settings_menu[13] = dynamic_cast<UI_Button*>(App->gui->CreateButton(435, 405, Button_slider_fx_left, { 1671,880,26,25 }, { 1738,880,26,25 }, { 1738,880,26,25 }, NULL, this));
		settings_menu[14] = dynamic_cast<UI_Button*>(App->gui->CreateButton(796, 405, Button_slider_fx_right, { 1704,880,26,25 }, { 1771,880,26,25 }, { 1771,880,26,25 }, NULL, this));

		//Buttons
		settings_menu[15] = dynamic_cast<UI_Button*>(App->gui->CreateButton(470, 615, button_cap, { 1671,922,25,26 }, { 1713,922,25,26 }, { 1713,922,25,26 }, NULL, this));
		settings_menu[16] = dynamic_cast<UI_Button*>(App->gui->CreateButton(360, 280, button_mute, { 1671,922,25,26 }, { 1713,922,25,26 }, { 1713,922,25,26 }, NULL, this));
		settings_menu[17] = dynamic_cast<UI_Button*>(App->gui->CreateButton(40, 40, button_back_pause, { 1704,1054,48,46 }, { 1765,1054,48,46 }, { 1765,1054,48,46 }, NULL, this));
		settings_menu[20] = dynamic_cast<UI_Button*>(App->gui->CreateButton(705, 615, button_fullscreen, { 1671,922,25,26 }, { 1713,922,25,26 }, { 1713,922,25,26 }, NULL, this));

		last_menu = current_menu;
		current_menu = Menu::PAUSE_SETTINGS;

		App->minimap->Hide();

		break;

	case Menu::SELECT_FACTION:
		//Images
		select_faction_menu[0] = dynamic_cast<j1Image*>(App->gui->CreateImage(-5, 0, Image, { 1900, 1531, 1290, 842 }, NULL, this));

		//Buttons
		select_faction_menu[1] = dynamic_cast<UI_Button*>(App->gui->CreateButton(160, 200, button_select_ghoul, { 1900,1144,204,60 }, { 1900,1210,204,60 }, { 1900,1277,204,60 }, NULL, this));
		select_faction_menu[2] = dynamic_cast<UI_Button*>(App->gui->CreateButton(160, 290, button_select_vault, { 1900,1144,204,60 }, { 1900,1210,204,60 }, { 1900,1277,204,60 }, NULL, this));
		select_faction_menu[3] = dynamic_cast<UI_Button*>(App->gui->CreateButton(160, 380, button_select_brotherhood, { 1900,1144,204,60 }, { 1900,1210,204,60 }, { 1900,1277,204,60 }, NULL, this));
		select_faction_menu[4] = dynamic_cast<UI_Button*>(App->gui->CreateButton(160, 470, button_select_supermutant, { 1900,1144,204,60 }, { 1900,1210,204,60 }, { 1900,1277,204,60 }, NULL, this));
		select_faction_menu[5] = dynamic_cast<UI_Button*>(App->gui->CreateButton(40, 190, button_back_to_menu, { 1243,2014,73,78 }, { 1243,2102,73,79 }, { 1243,2188,73,71 }, NULL, this));

		//Labels
		select_faction_menu[6] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(222, 215, Label, "GHOUL", NULL, this, "StackedPixelMedium"));
		select_faction_menu[7] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(220, 305, Label, "VAULT", NULL, this, "StackedPixelMedium"));
		select_faction_menu[8] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(175, 395, Label, "BROTHERHOOD", NULL, this, "StackedPixelMedium"));
		select_faction_menu[9] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(170, 485, Label, "SUPERMUTANT", NULL, this, "StackedPixelMedium"));

		last_menu = current_menu;
		current_menu = Menu::SELECT_FACTION;

		break;
	case Menu::PAUSE_MENU:
		//Images
		pause_menu[0] = dynamic_cast<j1Image*>(App->gui->CreateImage(434, 183, Image, { 3079, 41, 334, 313 }, NULL, this));

		//Buttons
		pause_menu[1] = dynamic_cast<UI_Button*>(App->gui->CreateButton(480, 210, resume_button, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,244,64 }, NULL, this));
		pause_menu[2] = dynamic_cast<UI_Button*>(App->gui->CreateButton(480, 300, button_settings_pause, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,244,64 }, NULL, this));
		pause_menu[3] = dynamic_cast<UI_Button*>(App->gui->CreateButton(480, 390, button_pause_to_main, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,244,64 }, NULL, this));

		//Labels
		pause_menu[4] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(540, 228, Label, "RESUME", NULL, this, "StackedPixel"));
		pause_menu[5] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(530, 318, Label, "SETTINGS", NULL, this, "StackedPixel"));
		pause_menu[6] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(515, 408, Label, "MAIN MENU", NULL, this, "StackedPixel"));

		last_menu = current_menu;
		current_menu = Menu::PAUSE_MENU;

		break;
	case Menu::GUI:
		//Images
		gui_ingame[0] = dynamic_cast<j1Image*>(App->gui->CreateImage(0, 0, Image, { 0, 2428, 1290, 730 }, NULL, this));
		gui_ingame[1] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(690, 7, Label, ":", NULL, this, "StackedPixel"));
		
		//Tutorial
		gui_ingame[2] = dynamic_cast<j1Image*>(App->gui->CreateImage(1205, 70, Image, { 1037, 2120, 74, 71 }, NULL, this));
		gui_ingame[3] = dynamic_cast<UI_Button*>(App->gui->CreateButton(1160, 70, info_button, { 973,2084,43,46 }, { 973,2135,43,46 }, { 973,2185,43,46 }, NULL, this));
		gui_ingame[4] = dynamic_cast<UI_Button*>(App->gui->CreateButton(1205, 0, Radio_button, { 1043,1998,74,71 }, { 966,1998,74,71 }, { 889,1998,74,71 }, NULL, this));
		gui_ingame[5] = dynamic_cast<UI_Button*>(App->gui->CreateButton(1180, 0, Radio_off_button, { 1671,922,25,26 }, { 1713,922,25,26 }, { 1713,922,25,26 }, NULL, this));

		timer_item = dynamic_cast<UI_Label*>(App->gui->CreateLabel(700, 7, Label, "0", NULL, this, "StackedPixel"));
		timer_minutes = dynamic_cast<UI_Label*>(App->gui->CreateLabel(652, 7, Label, "0", NULL, this, "StackedPixel"));

		last_menu = current_menu;
		current_menu = Menu::GUI;
		break;

	case Menu::RESOURCES:

		water = std::to_string(App->player->water);
		food = std::to_string(App->player->food);
		caps = std::to_string(App->player->caps);

		resources[0] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(80, 15, Label, food, NULL, this, "StackedPixelSmall"));
		resources[1] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(250, 15, Label, water, NULL, this, "StackedPixelSmall"));
		resources[2] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(420, 15, Label, caps, NULL, this, "StackedPixelSmall"));

		/*last_menu = current_menu;
		current_menu = Menu::RESOURCES;*/

		break;

	case Menu::GATHERER_QUANTITY:

		if (App->player->selected_entity != nullptr)
		{

			StaticEntity* static_entity_gatherer = dynamic_cast<StaticEntity*>(App->player->selected_entity);

			gatherer_quantity = std::to_string(static_entity_gatherer->GetUnitsInStack(GATHERER));

			gatherer_label = dynamic_cast<UI_Label*>(App->gui->CreateLabel(1010, 668, Label, gatherer_quantity, NULL, this, "StackedPixelSmall"));

		}

		break; 

	case Menu::RANGED_QUANTITY:

		if (App->player->selected_entity != nullptr)
		{

			StaticEntity* static_entity_ranged = dynamic_cast<StaticEntity*>(App->player->selected_entity);

			ranged_quantity = std::to_string(static_entity_ranged->GetUnitsInStack(RANGED));

			ranged_label = dynamic_cast<UI_Label*>(App->gui->CreateLabel(1107, 668, Label, ranged_quantity, NULL, this, "StackedPixelSmall"));

		}

		break;

	case Menu::MELEE_QUANTITY:
		
		if (App->player->selected_entity != nullptr)
		{

			StaticEntity* static_entity_melee = dynamic_cast<StaticEntity*>(App->player->selected_entity);

			melee_quantity = std::to_string(static_entity_melee->GetUnitsInStack(MELEE));

			melee_label = dynamic_cast<UI_Label*>(App->gui->CreateLabel(1010, 668, Label, melee_quantity, NULL, this, "StackedPixelSmall"));

		}

		break;

	case Menu::TUTORIAL:

		//Images
		tutorial_screen = dynamic_cast<j1Image*>(App->gui->CreateImage(1334, 133, Image, { 0, 140, 654, 355 }, NULL, this));
		
		//Buttons
		tutorial[0] = dynamic_cast<UI_Button*>(App->gui->CreateButton(1380, 360, control_button, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,244,64 }, NULL, this));
		tutorial[1] = dynamic_cast<UI_Button*>(App->gui->CreateButton(1700, 360, how_to_play_button, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,244,64 }, NULL, this));
		info_button2_element = dynamic_cast<UI_Button*>(App->gui->CreateButton(1294, 143, info_button2, { 908,2084,43,46 }, { 908,2135,43,46 }, { 908,2185,43,46 }, NULL, this));
		
		//Text
		tutorial[2] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(1600, 220, Label, "TUTORIAL", NULL, this));
		tutorial[3] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(1430, 375, Label, "CONTROLS", NULL, this));
		tutorial[4] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(1722, 375, Label, "HOW TO PLAY", NULL, this));
		tutorial[5] = nullptr;
		tutorial[6] = nullptr;
		tutorial[7] = nullptr;
		
		last_menu = current_menu;
		current_menu = Menu::PAUSE_MENU;

		break; 

	case Menu::HOW_TO_PLAY:
		how_to_play[0] = dynamic_cast<UI_Button*>(App->gui->CreateButton(920, 470, how_to_play_previous, { 1670,788,42,41 }, { 1773,788,42,41 }, { 1670,788,42,41 }, NULL, this));
		how_to_play[1] = dynamic_cast<UI_Button*>(App->gui->CreateButton(970, 470, how_to_play_next, { 1719,788,42,41 }, { 1823,788,42,41 }, { 1719,788,42,41 }, NULL, this));

		if (how_to_play_counter == 0) {
			how_to_play[2] = dynamic_cast<j1Image*>(App->gui->CreateImage(690, 160, Image, { 2869, 1188, 542, 296 }, NULL, this));
		}
		if (how_to_play_counter == 1) {
			how_to_play[3] = dynamic_cast<j1Image*>(App->gui->CreateImage(680, 157, Image, { 1183, 1557, 581, 322 }, NULL, this));
		}
		if (how_to_play_counter == 2) {
			how_to_play[4] = dynamic_cast<j1Image*>(App->gui->CreateImage(720, 165, Image, { 2231, 1152, 470, 286 }, NULL, this));
		}
		if (how_to_play_counter == 3) {
			how_to_play[5] = dynamic_cast<j1Image*>(App->gui->CreateImage(671, 180, Image, { 1240, 1190, 590, 255 }, NULL, this));
		}
		if (how_to_play_counter == 4) {
			how_to_play[6] = dynamic_cast<j1Image*>(App->gui->CreateImage(690, 165, Image, { 971, 35, 535, 287 }, NULL, this));
		}
		
		last_menu = current_menu;
		current_menu = Menu::PAUSE_MENU;

		break;

	case Menu::BUI_BASES:
		break;
	case Menu::COLLABORATORS_CREDITS:
		break;
	case Menu::DIALOG:
		dialogs[0] = App->gui->CreateLabel(App->dialog_manager->statement_box.x + 10, App->dialog_manager->statement_box.y + 30, Label, App->dialog_manager->dialogs[0]->statement, NULL, this, "StackedPixelSmall");
		dialogs[1] = App->gui->CreateLabel(App->dialog_manager->option_box[0].x + 10, App->dialog_manager->option_box[0].y + 20, Label, App->dialog_manager->dialogs[0]->options[0], NULL, this, "StackedPixelSmall");
		dialogs[2] = App->gui->CreateLabel(App->dialog_manager->option_box[1].x + 10, App->dialog_manager->option_box[1].y + 20, Label, App->dialog_manager->dialogs[0]->options[1], NULL, this, "StackedPixelSmall");
		dialogs[3] = App->gui->CreateLabel(App->dialog_manager->option_box[2].x + 10, App->dialog_manager->option_box[2].y + 20, Label, App->dialog_manager->dialogs[0]->options[2], NULL, this, "StackedPixelSmall");

		dialogs[4] = App->gui->CreateButton(App->dialog_manager->option_box[0].x, App->dialog_manager->option_box[0].y, OPTION_A, { 0,540,App->dialog_manager->option_box[0].w,App->dialog_manager->option_box[0].h },
			{ 0,540,App->dialog_manager->option_box[0].w,App->dialog_manager->option_box[0].h }, { 0,540,App->dialog_manager->option_box[0].w,App->dialog_manager->option_box[0].h }, NULL, App->dialog_manager);

		dialogs[5] = App->gui->CreateButton(App->dialog_manager->option_box[1].x, App->dialog_manager->option_box[1].y, OPTION_B, { 0,540,App->dialog_manager->option_box[1].w,App->dialog_manager->option_box[1].h },
			{ 0,540,App->dialog_manager->option_box[1].w,App->dialog_manager->option_box[1].h }, { 0,540,App->dialog_manager->option_box[1].w,App->dialog_manager->option_box[1].h }, NULL, App->dialog_manager);

		dialogs[6] = App->gui->CreateButton(App->dialog_manager->option_box[2].x, App->dialog_manager->option_box[2].y, OPTION_C, { 0,540,App->dialog_manager->option_box[2].w,App->dialog_manager->option_box[2].h },
			{ 0,540,App->dialog_manager->option_box[2].w,App->dialog_manager->option_box[2].h }, { 0,540,App->dialog_manager->option_box[2].w,App->dialog_manager->option_box[2].h }, NULL, App->dialog_manager);

		dialogs[7] = App->gui->CreateButton(1180, 315, ADVANCE_DIALOGS, { 1156, 2014, 75, 78 }, { 1156, 2101, 75, 78 }, { 1156, 2187, 75, 78 }, nullptr, App->dialog_manager);

		dialogs[8] = App->gui->CreateButton(25, 315, FINISH_DIALOGS, { 2441, 842,71,76 }, { 2441, 922,71,76 }, { 2441, 1004,71,76 }, nullptr, App->dialog_manager);

		dialogs[9] = App->gui->CreateImage(470, 20, Image, { 3078,42, 335, 312 }, nullptr, nullptr);

		if (App->player->faction == VAULT) dialogs[10] = App->gui->CreateImage(480, 110, Image, { 2948, 2909, 310, 134 }, nullptr, nullptr);
		else if (App->player->faction == BROTHERHOOD) dialogs[10] = App->gui->CreateImage(560, 90, Image, { 3025, 2352, 155, 181 }, nullptr, nullptr);
		else if (App->player->faction == MUTANT) dialogs[10] = App->gui->CreateImage(555, 105, Image, { 3023, 2658, 158, 158 }, nullptr, nullptr);
		else if (App->player->faction == GHOUL) dialogs[10] = App->gui->CreateImage(550, 100, Image, { 3027, 845, 167, 166 }, nullptr, nullptr);

		last_menu = current_menu;
		current_menu = Menu::DIALOG;

		break;

	case Menu::QUEST:
		quest[0] = dynamic_cast<UI_Button*>(App->gui->CreateButton(307, 100, quest_button, { 973,2084,43,47 }, { 973,2135,43,47 }, { 973,2185,43,47 }, NULL, this));
		quest[1] = dynamic_cast<UI_Button*>(App->gui->CreateButton(-50, 146, quest_button2, { 909,2084,43,47 }, { 909,2135,43,47 }, { 909,2185,43,47 }, NULL, this));
		quest[2] = dynamic_cast<j1Image*>(App->gui->CreateImage(0, 100, Image, { 3105, 597, 307, 199 }, NULL, this));
		
		quest[3] = nullptr;
		quest[4] = nullptr;
		quest[5] = nullptr;

		quest[6] = nullptr;
		quest[7] = nullptr;
		quest[8] = nullptr; 
		
		quest[9] = nullptr;
		quest[10] = nullptr;
		quest[11] = nullptr;

		break;

	case Menu::RADIO:

		radio[0] = dynamic_cast<UI_Button*>(App->gui->CreateButton( 600, 125, RockFM_button, { 1391,2312,114,114 }, { 1055,2312,114,114 }, { 1712,2312,114,114 }, NULL, this));
		radio[1] = dynamic_cast<UI_Button*>(App->gui->CreateButton( 450, 225, AyuwokiFM_button, { 1283,2312,114,114 }, { 941,2312,114,114 }, { 1606,2312,114,114 }, NULL, this));
		radio[2] = dynamic_cast<UI_Button*>(App->gui->CreateButton( 750, 225, SlimShadyFM_button, { 1499,2312,114,114 }, { 1169,2312,114,114 }, { 1818,2312,114,114 }, NULL, this));
		radio[3] = dynamic_cast<UI_Button*>(App->gui->CreateButton( 450, 375, FiestaLatinaFM_button, { 2274,2312,114,114 }, { 1932,2312,114,114 }, { 602,2312,114,114 }, NULL, this));
		radio[4] = dynamic_cast<UI_Button*>(App->gui->CreateButton( 750, 375, FusionCoreFM_button, { 2492,2312,114,114 }, { 2160,2312,114,114 }, { 823,2312,114,114 }, NULL, this));
		radio[5] = dynamic_cast<UI_Button*>(App->gui->CreateButton( 600, 475, UltraRADIO_button, { 2383,2312,114,114 }, { 2046,2312,114,114 }, { 712,2312,114,114 }, NULL, this));

		break;
	case Menu::NO_MENU:
		break;
	default:
		break;
	}
}
void MenuManager::CreateMenuFaction(Menu menu, Faction faction, EntityType type) {

	switch (menu)
	{
		case Menu::BUI_BASES:

			switch (faction)
			{
			case VAULT:
				current_building_faction = VAULT;
				switch (type)
				{
				case BASE:
					current_building_type = BASE;
					//Buttons
					vault_base[0] = dynamic_cast<UI_Button*>(App->gui->CreateButton(670, 593, Vault_ghaterer_button, { 1355,2028,149,52 }, { 1355,2112,149,52 }, { 1355,2196,149,52 }, NULL, this));
					vault_base[1] = dynamic_cast<UI_Button*>(App->gui->CreateButton(670, 653, Boost_base_button, { 1355,2028,149,52 }, { 1355,2112,149,52 }, { 1355,2196,149,52 }, NULL, this));

					//Images
					vault_base[2] = dynamic_cast<j1Image*>(App->gui->CreateImage(563, 590, Image, { 319, 1710, 49, 113 }, NULL, this));
					vault_base[3] = dynamic_cast<j1Image*>(App->gui->CreateImage(685, 600, Image, { 463, 1871, 25, 29 }, NULL, this));
					vault_base[4] = dynamic_cast<j1Image*>(App->gui->CreateImage(675, 665, Image, { 926, 1872, 37, 24 }, NULL, this));

					//Labels
					if (App->entities->gatherer_resource_limit[App->player->faction].upgrade_num < 4) {
						 cost = App->entities->gatherer_resource_limit[App->player->faction].first_price + App->entities->gatherer_resource_limit[App->player->faction].price_increment * App->entities->gatherer_resource_limit[App->player->faction].upgrade_num;
						
						vault_base[5] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 656, Label, std::to_string(cost * 2), NULL, this, "StackedPixelXS"));
						
					}
					else
						vault_base[5] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 656, Label, "MAX", NULL, this, "StackedPixelXS"));

					vault_base[6] = dynamic_cast<j1Image*>(App->gui->CreateImage(1020, 600, Image, { 382, 1930, 21, 57 }, NULL, this));
					vault_base[7] = dynamic_cast<j1Image*>(App->gui->CreateImage(1102, 600, Image, { 104, 2338, 49, 55 }, NULL, this));

					vault_base[8] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(995, 668, Label, "x", NULL, this, "StackedPixelSmall"));

					vault_base[9] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 596, Label, "0", NULL, this, "StackedPixelXS"));
					vault_base[10] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 614, Label, "40", NULL, this, "StackedPixelXS"));
					vault_base[11] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 629, Label, "0", NULL, this, "StackedPixelXS"));

					vault_base[12] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 674, Label, "0", NULL, this,"StackedPixelXS"));
					vault_base[13] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 689, Label, "0", NULL, this, "StackedPixelXS"));

					//Draw initial quantity
					CreateMenu(Menu::GATHERER_QUANTITY);

					last_menu = current_menu;
					current_menu = Menu::BUI_BASES;

					break;
				case BARRACK:
					current_building_type = BARRACK;
					//Buttons
					vault_barrack[0] = dynamic_cast<UI_Button*>(App->gui->CreateButton(670, 593, Vault_melee_button, { 1355,2028,149,52 }, { 1355,2112,149,52 }, { 1355,2196,149,52 }, NULL, this));
					vault_barrack[1] = dynamic_cast<UI_Button*>(App->gui->CreateButton(830, 593, Vault_ranged_button, { 1355,2028,149,52 }, { 1355,2112,149,52 }, { 1355,2196,149,52 }, NULL, this));
					vault_barrack[2] = dynamic_cast<UI_Button*>(App->gui->CreateButton(670, 653, Boost_barrack_button, { 1355,2028,149,52 }, { 1355,2112,149,52 }, { 1355,2196,149,52 }, NULL, this));

					//Images
					vault_barrack[3] = dynamic_cast<j1Image*>(App->gui->CreateImage(530, 600, Image, { 148, 1731, 122, 91 }, NULL, this));
					vault_barrack[4] = dynamic_cast<j1Image*>(App->gui->CreateImage(680, 600, Image, { 406, 1866, 32, 34 }, NULL, this));
					vault_barrack[5] = dynamic_cast<j1Image*>(App->gui->CreateImage(840, 600, Image, { 355, 1866, 30, 34 }, NULL, this));
					vault_barrack[6] = dynamic_cast<j1Image*>(App->gui->CreateImage(675, 665, Image, { 823, 1872, 32, 22 }, NULL, this));

					//Labels
					if (App->entities->units_damage[App->player->faction].upgrade_num < 4) {
						 cost = App->entities->units_damage[App->player->faction].first_price + App->entities->units_damage[App->player->faction].price_increment * App->entities->units_damage[App->player->faction].upgrade_num;
						vault_barrack[7] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 656, Label, std::to_string(cost * 2), NULL, this, "StackedPixelXS"));
					}
					else
						vault_barrack[7] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 656, Label, "MAX", NULL, this, "StackedPixelXS"));

					//Carta
					vault_barrack[8] = dynamic_cast<j1Image*>(App->gui->CreateImage(1110, 600, Image, { 296, 1930, 44, 61 }, NULL, this));
					vault_barrack[9] = dynamic_cast<j1Image*>(App->gui->CreateImage(1015, 600, Image, { 340, 1930, 34, 65 }, NULL, this));
					vault_barrack[10] = dynamic_cast<j1Image*>(App->gui->CreateImage(1200, 610, Image, { 0, 2338, 41, 46 }, NULL, this));

					//Cost
					vault_barrack[11] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(995, 668, Label, "x", NULL, this, "StackedPixelSmall"));
					vault_barrack[12] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(1092, 668, Label, "x", NULL, this, "StackedPixelSmall"));

					vault_barrack[13] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 596, Label, "0", NULL, this, "StackedPixelXS"));		//Nukas
					vault_barrack[14] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 613, Label, "60", NULL, this, "StackedPixelXS"));	//Water
					vault_barrack[15] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 629, Label, "60", NULL, this, "StackedPixelXS"));	//Food
					
					vault_barrack[16] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 673, Label, "0", NULL, this, "StackedPixelXS"));		//Water
					vault_barrack[17] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 689, Label, "0", NULL, this, "StackedPixelXS"));		//Food

					vault_barrack[18] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(910, 596, Label, "0", NULL, this, "StackedPixelXS"));		//Nukas
					vault_barrack[19] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(910, 613, Label, "80", NULL, this, "StackedPixelXS"));	//Water
					vault_barrack[20] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(910, 629, Label, "80", NULL, this, "StackedPixelXS"));	//Food

					//Draw initial quantity
					CreateMenu(Menu::RANGED_QUANTITY);
					CreateMenu(Menu::MELEE_QUANTITY);

					last_menu = current_menu;
					current_menu = Menu::BUI_BASES;

					break;

				case LABORATORY:
					current_building_type = LABORATORY;
					//Buttons
					vault_lab[0] = dynamic_cast<UI_Button*>(App->gui->CreateButton(670, 593, Boost_lab_button, { 1355,2028,149,52 }, { 1355,2112,149,52 }, { 1355,2196,149,52 }, NULL, this));
					vault_lab[1] = dynamic_cast<UI_Button*>(App->gui->CreateButton(830, 593, Boost_radar_button, { 1355,2028,149,52 }, { 1355,2112,149,52 }, { 1355,2196,149,52 }, NULL, this));
					vault_lab[2] = dynamic_cast<UI_Button*>(App->gui->CreateButton(670, 653, summon_mrhandy_button, { 1355,2028,149,52 }, { 1355,2112,149,52 }, { 1355,2196,149,52 }, NULL, this));

					//Images
					vault_lab[3] = dynamic_cast<j1Image*>(App->gui->CreateImage(555, 600, Image, { 426, 1726, 72, 96 }, NULL, this));
					vault_lab[4] = dynamic_cast<j1Image*>(App->gui->CreateImage(677, 607, Image, { 877, 1874, 33, 20 }, NULL, this));

					vault_lab[5] = dynamic_cast<j1Image*>(App->gui->CreateImage(840, 600, Image, { 982, 1869, 31, 33 }, NULL, this));
					vault_lab[6] = dynamic_cast<j1Image*>(App->gui->CreateImage(680, 660, Image, { 1034, 1869, 35, 33 }, NULL, this));

					//Labels
					if (App->entities->units_creation_time[App->player->faction].upgrade_num < 4) {
						cost = App->entities->units_creation_time[App->player->faction].first_price + App->entities->units_creation_time[App->player->faction].price_increment * App->entities->units_creation_time[App->player->faction].upgrade_num;
						
						vault_lab[7] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 596, Label, std::to_string(cost * 2), NULL, this, "StackedPixelXS"));
					}
					else
						vault_lab[7] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 596, Label, "MAX", NULL, this, "StackedPixelXS"));

					//Carta
					vault_lab[8] = dynamic_cast<j1Image*>(App->gui->CreateImage(995, 590, Image, { 228, 2328, 73, 90 }, NULL, this));
					vault_lab[9] = dynamic_cast<j1Image*>(App->gui->CreateImage(1102, 600, Image, { 164, 2336, 55, 58 }, NULL, this));
					vault_lab[10] = dynamic_cast<j1Image*>(App->gui->CreateImage(1205, 600, Image, { 52, 2338, 45, 51 }, NULL, this));

					//Cost
					vault_lab[11] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 613, Label, "0", NULL, this, "StackedPixelXS"));
					vault_lab[12] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 629, Label, "0", NULL, this, "StackedPixelXS"));

					vault_lab[13] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 656, Label, "1000", NULL, this, "StackedPixelXS"));
					vault_lab[14] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 673, Label, "0", NULL, this, "StackedPixelXS"));
					vault_lab[15] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 689, Label, "0", NULL, this, "StackedPixelXS"));

					vault_lab[16] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(910, 596, Label, "100", NULL, this, "StackedPixelXS"));
					vault_lab[17] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(910, 613, Label, "0", NULL, this, "StackedPixelXS"));
					vault_lab[18] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(910, 629, Label, "0", NULL, this, "StackedPixelXS"));

					last_menu = current_menu;
					current_menu = Menu::BUI_BASES;

					break;
				default:
					break;
				}
				break;

				case BROTHERHOOD:
					current_building_faction = BROTHERHOOD;
					switch (type)
					{
						case BASE:
							current_building_type = BASE;
							//Buttons
							brotherhood_base[0] = dynamic_cast<UI_Button*>(App->gui->CreateButton(670, 593, Brotherhood_ghaterer_button, { 1355,2028,149,52 }, { 1355,2112,149,52 }, { 1355,2196,149,52 }, NULL, this));
							brotherhood_base[1] = dynamic_cast<UI_Button*>(App->gui->CreateButton(670, 653, Boost_base_button, { 1355,2028,149,52 }, { 1355,2112,149,52 }, { 1355,2196,149,52 }, NULL, this));

							//Images
							brotherhood_base[2] = dynamic_cast<j1Image*>(App->gui->CreateImage(530, 589, Image, { 118, 1561, 122, 98 }, NULL, this));
							brotherhood_base[3] = dynamic_cast<j1Image*>(App->gui->CreateImage(682, 600, Image, { 138, 1868, 33, 33 }, NULL, this));
							brotherhood_base[4] = dynamic_cast<j1Image*>(App->gui->CreateImage(675, 665, Image, { 926, 1872, 37, 24 }, NULL, this));

							//Labels
							if (App->entities->gatherer_resource_limit[App->player->faction].upgrade_num < 4) {
								 cost = App->entities->gatherer_resource_limit[App->player->faction].first_price + App->entities->gatherer_resource_limit[App->player->faction].price_increment * App->entities->gatherer_resource_limit[App->player->faction].upgrade_num;
								
								brotherhood_base[5] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 656, Label, std::to_string(cost * 2), NULL, this, "StackedPixelXS"));
							}
							else
								brotherhood_base[5] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 656, Label, "MAX", NULL, this, "StackedPixelXS"));
								

							brotherhood_base[6] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(995, 668, Label, "x", NULL, this, "StackedPixelSmall"));
							
							//Draw initial quantity
							CreateMenu(Menu::GATHERER_QUANTITY);

							//Carts
							brotherhood_base[7] = dynamic_cast<j1Image*>(App->gui->CreateImage(1008, 599, Image, { 242, 1929, 45, 75 }, NULL, this));
							brotherhood_base[8] = dynamic_cast<j1Image*>(App->gui->CreateImage(1102, 610, Image, { 104, 2338, 49, 55 }, NULL, this));

							//Cost
							brotherhood_base[9] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 596, Label, "0", NULL, this, "StackedPixelXS"));   //Nukas
							brotherhood_base[10] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 614, Label, "50", NULL, this, "StackedPixelXS")); //Water
							brotherhood_base[11] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 629, Label, "0", NULL, this, "StackedPixelXS"));  //Food
							
							brotherhood_base[12] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 674, Label, "0", NULL, this, "StackedPixelXS")); //Water
							brotherhood_base[13] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 689, Label, "0", NULL, this, "StackedPixelXS"));  //Food


							last_menu = current_menu;
							current_menu = Menu::BUI_BASES;

							break;
						case BARRACK:
							current_building_type = BARRACK;
							//Buttons
							brotherhood_barrack[0] = dynamic_cast<UI_Button*>(App->gui->CreateButton(670, 593, Brotherhood_melee_button, { 1355,2028,149,52 }, { 1355,2112,149,52 }, { 1355,2196,149,52 }, NULL, this));
							brotherhood_barrack[1] = dynamic_cast<UI_Button*>(App->gui->CreateButton(830, 593, Brotherhood_ranged_button, { 1355,2028,149,52 }, { 1355,2112,149,52 }, { 1355,2196,149,52 }, NULL, this));
							brotherhood_barrack[2] = dynamic_cast<UI_Button*>(App->gui->CreateButton(670, 653, Boost_barrack_button, { 1355,2028,149,52 }, { 1355,2112,149,52 }, { 1355,2196,149,52 }, NULL, this));

							//Images
							brotherhood_barrack[3] = dynamic_cast<j1Image*>(App->gui->CreateImage(555, 590, Image, { 17, 1560, 69, 115 }, NULL, this));
							brotherhood_barrack[4] = dynamic_cast<j1Image*>(App->gui->CreateImage(682, 600, Image, { 28, 1869, 35, 33 }, NULL, this));
							brotherhood_barrack[5] = dynamic_cast<j1Image*>(App->gui->CreateImage(840, 600, Image, { 83, 1866, 35, 36 }, NULL, this));
							brotherhood_barrack[6] = dynamic_cast<j1Image*>(App->gui->CreateImage(675, 665, Image, { 823, 1872, 32, 22 }, NULL, this));

							//Labels
							if (App->entities->units_damage[App->player->faction].upgrade_num < 4) {
								cost = App->entities->units_damage[App->player->faction].first_price + App->entities->units_damage[App->player->faction].price_increment * App->entities->units_damage[App->player->faction].upgrade_num;
								brotherhood_barrack[7] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 656, Label, std::to_string(cost * 2), NULL, this, "StackedPixelXS"));
							}
							else
								brotherhood_barrack[7] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 656, Label, "MAX", NULL, this, "StackedPixelXS"));

							//Carta
							brotherhood_barrack[8] = dynamic_cast<j1Image*>(App->gui->CreateImage(1010, 600, Image, { 147, 1930, 41, 72 }, NULL, this));
							brotherhood_barrack[9] = dynamic_cast<j1Image*>(App->gui->CreateImage(1110, 600, Image, { 194, 1930, 40, 73 }, NULL, this));
							brotherhood_barrack[10] = dynamic_cast<j1Image*>(App->gui->CreateImage(1200, 610, Image, { 0, 2338, 41, 46 }, NULL, this));

							brotherhood_barrack[11] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(995, 668, Label, "x", NULL, this, "StackedPixelSmall"));
							brotherhood_barrack[12] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(1092, 668, Label, "x", NULL, this, "StackedPixelSmall"));

							//Cost
							brotherhood_barrack[13] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 596, Label, "0", NULL, this, "StackedPixelXS")); //Nukas
							brotherhood_barrack[14] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 613, Label, "100", NULL, this, "StackedPixelXS"));	//Water
							brotherhood_barrack[15] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 629, Label, "80", NULL, this, "StackedPixelXS"));	//Food

							brotherhood_barrack[16] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 673, Label, "0", NULL, this, "StackedPixelXS"));
							brotherhood_barrack[17] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 689, Label, "0", NULL, this, "StackedPixelXS"));

							brotherhood_barrack[18] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(910, 596, Label, "0", NULL, this, "StackedPixelXS"));
							brotherhood_barrack[19] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(910, 613, Label, "100", NULL, this, "StackedPixelXS"));
							brotherhood_barrack[20] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(910, 629, Label, "100", NULL, this, "StackedPixelXS"));

							//Draw initial quantity
							CreateMenu(Menu::RANGED_QUANTITY);
							CreateMenu(Menu::MELEE_QUANTITY);

							last_menu = current_menu;
							current_menu = Menu::BUI_BASES;

							break;
						case LABORATORY:
							current_building_type = LABORATORY;
							//Buttons
							brotherhood_lab[0] = dynamic_cast<UI_Button*>(App->gui->CreateButton(670, 593, Boost_lab_button, { 1355,2028,149,52 }, { 1355,2112,149,52 }, { 1355,2196,149,52 }, NULL, this));
							brotherhood_lab[1] = dynamic_cast<UI_Button*>(App->gui->CreateButton(830, 593, Boost_radar_button, { 1355,2028,149,52 }, { 1355,2112,149,52 }, { 1355,2196,149,52 }, NULL, this));
							brotherhood_lab[2] = dynamic_cast<UI_Button*>(App->gui->CreateButton(670, 653, summon_mrhandy_button, { 1355,2028,149,52 }, { 1355,2112,149,52 }, { 1355,2196,149,52 }, NULL, this));

							//Images
							brotherhood_lab[3] = dynamic_cast<j1Image*>(App->gui->CreateImage(530, 590, Image, { 663, 1567, 108, 104 }, NULL, this));// building
							
							brotherhood_lab[4] = dynamic_cast<j1Image*>(App->gui->CreateImage(677, 607, Image, { 877, 1874, 33, 20 }, NULL, this));//boost
							brotherhood_lab[5] = dynamic_cast<j1Image*>(App->gui->CreateImage(840, 600, Image, { 982, 1869, 31, 33 }, NULL, this));//radar
							brotherhood_lab[6] = dynamic_cast<j1Image*>(App->gui->CreateImage(680, 660, Image, { 1034, 1869, 35, 33 }, NULL, this));//misterhandy

							//Labels
							if (App->entities->units_creation_time[App->player->faction].upgrade_num < 4) {
								 cost = App->entities->units_creation_time[App->player->faction].first_price + App->entities->units_creation_time[App->player->faction].price_increment * App->entities->units_creation_time[App->player->faction].upgrade_num;
								brotherhood_lab[7] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 596, Label, std::to_string(cost * 2), NULL, this, "StackedPixelXS"));
							}
							else
								brotherhood_lab[7] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 596, Label, "MAX", NULL, this, "StackedPixelXS"));

							//Carta
							brotherhood_lab[8] = dynamic_cast<j1Image*>(App->gui->CreateImage(995, 590, Image, { 228, 2328, 73, 90 }, NULL, this));
							brotherhood_lab[9] = dynamic_cast<j1Image*>(App->gui->CreateImage(1102, 600, Image, { 164, 2336, 55, 58 }, NULL, this));
							brotherhood_lab[10] = dynamic_cast<j1Image*>(App->gui->CreateImage(1205, 600, Image, { 52, 2338, 45, 51 }, NULL, this));

							//Cost
							brotherhood_lab[11] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 613, Label, "0", NULL, this, "StackedPixelXS"));
							brotherhood_lab[12] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 629, Label, "0", NULL, this, "StackedPixelXS"));

							brotherhood_lab[13] =  dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 656, Label, "1000", NULL, this, "StackedPixelXS"));
							brotherhood_lab[14] =  dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 673, Label, "0", NULL, this, "StackedPixelXS"));
							brotherhood_lab[15] =  dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 689, Label, "0", NULL, this, "StackedPixelXS"));
												   
							brotherhood_lab[16] =  dynamic_cast<UI_Label*>(App->gui->CreateLabel(910, 596, Label, "100", NULL, this, "StackedPixelXS"));
							brotherhood_lab[17] =  dynamic_cast<UI_Label*>(App->gui->CreateLabel(910, 613, Label, "0", NULL, this, "StackedPixelXS"));
							brotherhood_lab[18] =  dynamic_cast<UI_Label*>(App->gui->CreateLabel(910, 629, Label, "0", NULL, this, "StackedPixelXS"));

							last_menu = current_menu;
							current_menu = Menu::BUI_BASES;

							break;
						default:
							break;
					}
					break;

				case MUTANT:
					current_building_faction = MUTANT;
					switch (type)
					{
						case BASE:
							current_building_type = BASE;
							//Buttons
							supermutant_base[0] = dynamic_cast<UI_Button*>(App->gui->CreateButton(670, 593, Supermutant_ghaterer_button, { 1355,2028,149,52 }, { 1355,2112,149,52 }, { 1355,2196,149,52 }, NULL, this));
							supermutant_base[1] = dynamic_cast<UI_Button*>(App->gui->CreateButton(670, 653, Boost_base_button, { 1355,2028,149,52 }, { 1355,2112,149,52 }, { 1355,2196,149,52 }, NULL, this));

							//Images
							supermutant_base[2] = dynamic_cast<j1Image*>(App->gui->CreateImage(530, 600, Image, { 934, 1577, 129, 91 }, NULL, this));
							supermutant_base[3] = dynamic_cast<j1Image*>(App->gui->CreateImage(682, 600, Image, { 191, 1868, 34, 32 }, NULL, this));
							supermutant_base[4] = dynamic_cast<j1Image*>(App->gui->CreateImage(675, 665, Image, { 926, 1872, 37, 24 }, NULL, this));

							//Labels
							if (App->entities->gatherer_resource_limit[App->player->faction].upgrade_num < 4) {
								cost = App->entities->gatherer_resource_limit[App->player->faction].first_price + App->entities->gatherer_resource_limit[App->player->faction].price_increment * App->entities->gatherer_resource_limit[App->player->faction].upgrade_num;

								supermutant_base[5] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 656, Label, std::to_string(cost * 2), NULL, this, "StackedPixelXS"));

							}
							else
								supermutant_base[5] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 656, Label, "MAX", NULL, this, "StackedPixelXS"));

							supermutant_base[6] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(995, 668, Label, "x", NULL, this, "StackedPixelSmall"));
							

							//Carts
							supermutant_base[7] = dynamic_cast<j1Image*>(App->gui->CreateImage(1008, 600, Image, { 92, 1956, 53, 47 }, NULL, this));
							supermutant_base[8] = dynamic_cast<j1Image*>(App->gui->CreateImage(1102, 600, Image, { 104, 2338, 49, 55 }, NULL, this));

							//Cost
							supermutant_base[9] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 596, Label, "0", NULL, this, "StackedPixelXS"));   //Nukas
							supermutant_base[10] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 614, Label, "50", NULL, this, "StackedPixelXS")); //Water
							supermutant_base[11] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 629, Label, "0", NULL, this, "StackedPixelXS"));  //Food
							
							supermutant_base[12] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 674, Label, "0", NULL, this, "StackedPixelXS")); //Water
							supermutant_base[13] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 689, Label, "0", NULL, this, "StackedPixelXS"));  //Food


							//Draw initial quantity
							CreateMenu(Menu::GATHERER_QUANTITY);
							
							last_menu = current_menu;
							current_menu = Menu::BUI_BASES;
							break;

						case BARRACK:
							current_building_type = BARRACK;
							//Buttons
							supermutant_barrack[0] = dynamic_cast<UI_Button*>(App->gui->CreateButton(670, 593, Supermutant_melee_button, { 1355,2028,149,52 }, { 1355,2112,149,52 }, { 1355,2196,149,52 }, NULL, this));
							supermutant_barrack[1] = dynamic_cast<UI_Button*>(App->gui->CreateButton(830, 593, Supermutant_ranged_button, { 1355,2028,149,52 }, { 1355,2112,149,52 }, { 1355,2196,149,52 }, NULL, this));
							supermutant_barrack[2] = dynamic_cast<UI_Button*>(App->gui->CreateButton(670, 653, Boost_barrack_button, { 1355,2028,149,52 }, { 1355,2112,149,52 }, { 1355,2196,149,52 }, NULL, this));

							//Images
							supermutant_barrack[3] =  dynamic_cast<j1Image*>(App->gui->CreateImage(530, 590, Image, { 6, 1729, 113, 95 }, NULL, this));
							supermutant_barrack[4] =  dynamic_cast<j1Image*>(App->gui->CreateImage(682, 600, Image, { 246, 1869, 34, 33 }, NULL, this));
							supermutant_barrack[5] =  dynamic_cast<j1Image*>(App->gui->CreateImage(840, 600, Image, { 301, 1865, 34, 35 }, NULL, this));
							supermutant_barrack[6] =  dynamic_cast<j1Image*>(App->gui->CreateImage(675, 665, Image, { 823, 1872, 32, 22 }, NULL, this));

							//Labels
							if (App->entities->units_damage[App->player->faction].upgrade_num < 4) {
								cost = App->entities->units_damage[App->player->faction].first_price + App->entities->units_damage[App->player->faction].price_increment * App->entities->units_damage[App->player->faction].upgrade_num;
								supermutant_barrack[7] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 656, Label, std::to_string(cost * 2), NULL, this, "StackedPixelXS"));
							}
							else
								supermutant_barrack[7] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 656, Label, "MAX", NULL, this, "StackedPixelXS"));

							//Carta
							supermutant_barrack[8] = dynamic_cast<j1Image*>(App->gui->CreateImage(1010, 600, Image, { 47, 1930, 44, 80 }, NULL, this));
							supermutant_barrack[9] = dynamic_cast<j1Image*>(App->gui->CreateImage(1110, 600, Image, { 0, 1930, 44, 80 }, NULL, this));
							supermutant_barrack[10] = dynamic_cast<j1Image*>(App->gui->CreateImage(1200, 610, Image, { 0, 2338, 41, 46 }, NULL, this));

							supermutant_barrack[11] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(995, 668, Label, "x", NULL, this,"StackedPixelSmall"));
							supermutant_barrack[12] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(1092, 668, Label, "x", NULL, this, "StackedPixelSmall"));

							//Cost
							supermutant_barrack[13] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 596, Label, "0", NULL, this, "StackedPixelXS")); //Nukas
							supermutant_barrack[14] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 613, Label, "100", NULL, this, "StackedPixelXS"));	//Water
							supermutant_barrack[15] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 629, Label, "100", NULL, this, "StackedPixelXS"));	//Food

							supermutant_barrack[16] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 673, Label, "0", NULL, this, "StackedPixelXS"));
							supermutant_barrack[17] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 689, Label, "0", NULL, this, "StackedPixelXS"));

							supermutant_barrack[18] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(910, 596, Label, "0", NULL, this, "StackedPixelXS"));
							supermutant_barrack[19] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(910, 613, Label, "100", NULL, this, "StackedPixelXS"));
							supermutant_barrack[20] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(910, 629, Label, "120", NULL, this, "StackedPixelXS"));

							//Draw initial quantity
							CreateMenu(Menu::RANGED_QUANTITY);
							CreateMenu(Menu::MELEE_QUANTITY);

							last_menu = current_menu;
							current_menu = Menu::BUI_BASES;

							break;
						case LABORATORY:
							current_building_type = LABORATORY;
							//Buttons
							supermutant_lab[0] = dynamic_cast<UI_Button*>(App->gui->CreateButton(670, 593, Boost_lab_button, { 1355,2028,149,52 }, { 1355,2112,149,52 }, { 1355,2196,149,52 }, NULL, this));
							supermutant_lab[1] = dynamic_cast<UI_Button*>(App->gui->CreateButton(830, 593, Boost_radar_button, { 1355,2028,149,52 }, { 1355,2112,149,52 }, { 1355,2196,149,52 }, NULL, this));
							supermutant_lab[2] = dynamic_cast<UI_Button*>(App->gui->CreateButton(670, 653, summon_mrhandy_button, { 1355,2028,149,52 }, { 1355,2112,149,52 }, { 1355,2196,149,52 }, NULL, this));
							
							//Images
							supermutant_lab[3] = dynamic_cast<j1Image*>(App->gui->CreateImage(540, 590, Image, { 801, 1549, 102, 119 }, NULL, this));
							supermutant_lab[4] = dynamic_cast<j1Image*>(App->gui->CreateImage(677, 607, Image, { 877, 1874, 33, 20 }, NULL, this));
							
							supermutant_lab[5] = dynamic_cast<j1Image*>(App->gui->CreateImage(840, 600, Image, { 982, 1869, 31, 33 }, NULL, this));
							supermutant_lab[6] = dynamic_cast<j1Image*>(App->gui->CreateImage(680, 660, Image, { 1034, 1869, 35, 33 }, NULL, this));

							//Labels
							if (App->entities->units_creation_time[App->player->faction].upgrade_num < 4) {
								 cost = App->entities->units_creation_time[App->player->faction].first_price + App->entities->units_creation_time[App->player->faction].price_increment * App->entities->units_creation_time[App->player->faction].upgrade_num;
								supermutant_lab[7] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 596, Label, std::to_string(cost * 2), NULL, this, "StackedPixelXS"));
							}
							else
								supermutant_lab[7] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 596, Label, "MAX", NULL, this, "StackedPixelXS"));

							//Carta
							supermutant_lab[8] = dynamic_cast<j1Image*>(App->gui->CreateImage(995, 590, Image, { 228, 2328, 73, 90 }, NULL, this));
							supermutant_lab[9] = dynamic_cast<j1Image*>(App->gui->CreateImage(1102, 600, Image, { 164, 2336, 55, 58 }, NULL, this));
							supermutant_lab[10] = dynamic_cast<j1Image*>(App->gui->CreateImage(1205, 600, Image, { 52, 2338, 45, 51 }, NULL, this));

							//Cost
							supermutant_lab[11] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 613, Label, "0", NULL, this, "StackedPixelXS"));
							supermutant_lab[12] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 629, Label, "0", NULL, this, "StackedPixelXS"));
												 
							supermutant_lab[13] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 656, Label, "1000", NULL, this, "StackedPixelXS"));
							supermutant_lab[14] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 673, Label, "0", NULL, this, "StackedPixelXS"));
							supermutant_lab[15] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 689, Label, "0", NULL, this, "StackedPixelXS"));
												 
							supermutant_lab[16] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(910, 596, Label, "100", NULL, this, "StackedPixelXS"));
							supermutant_lab[17] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(910, 613, Label, "0", NULL, this, "StackedPixelXS"));
							supermutant_lab[18] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(910, 629, Label, "0", NULL, this, "StackedPixelXS"));

							last_menu = current_menu;
							current_menu = Menu::BUI_BASES;

							break;
						default:
							break;
					}
					break;

				case GHOUL:
					current_building_faction = GHOUL;
					switch (type)
					{
						case BASE:
							current_building_type = BASE;
							//Buttons
							ghoul_base[0] = dynamic_cast<UI_Button*>(App->gui->CreateButton(670, 593, Ghouls_ghaterer_button, { 1355,2028,149,52 }, { 1355,2112,149,52 }, { 1355,2196,149,52 }, NULL, this));
							ghoul_base[1] = dynamic_cast<UI_Button*>(App->gui->CreateButton(670, 653, Boost_base_button, { 1355,2028,149,52 }, { 1355,2112,149,52 }, { 1355,2196,149,52 }, NULL, this));
							

							//Images
							ghoul_base[2] = dynamic_cast<j1Image*>(App->gui->CreateImage(540, 590, Image, { 398, 1564, 99, 108 }, NULL, this));
							ghoul_base[3] = dynamic_cast<j1Image*>(App->gui->CreateImage(685, 600, Image, { 624, 1866, 24, 34 }, NULL, this));
							ghoul_base[4] = dynamic_cast<j1Image*>(App->gui->CreateImage(675, 665, Image, { 926, 1872, 37, 24 }, NULL, this));

							//Labels
							if (App->entities->gatherer_resource_limit[App->player->faction].upgrade_num < 4) {
								 cost = App->entities->gatherer_resource_limit[App->player->faction].first_price + App->entities->gatherer_resource_limit[App->player->faction].price_increment * App->entities->gatherer_resource_limit[App->player->faction].upgrade_num;
								ghoul_base[5] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 656, Label, std::to_string(cost * 2), NULL, this, "StackedPixelXS"));

							}
							else
								ghoul_base[5] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 656, Label, "MAX", NULL, this, "StackedPixelXS"));
							

							ghoul_base[6] = dynamic_cast<j1Image*>(App->gui->CreateImage(1019, 600, Image, { 412, 1930, 28, 62 }, NULL, this));
							ghoul_base[7] = dynamic_cast<j1Image*>(App->gui->CreateImage(1102, 600, Image, { 104, 2338, 49, 55 }, NULL, this));

							ghoul_base[8] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(995, 668, Label, "x", NULL, this, "StackedPixelSmall"));

							//Cost
							ghoul_base[9] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 596, Label, "0", NULL, this, "StackedPixelXS"));		//Nukas
							ghoul_base[10] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 613, Label, "40", NULL, this, "StackedPixelXS"));	//Water
							ghoul_base[11] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 629, Label, "0", NULL, this, "StackedPixelXS"));	//Food
							ghoul_base[12] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 673, Label, "0", NULL, this, "StackedPixelXS"));	//Water
							ghoul_base[13] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 689, Label, "0", NULL, this, "StackedPixelXS"));	//Food

							//Draw initial quantity
							CreateMenu(Menu::GATHERER_QUANTITY);

							last_menu = current_menu;
							current_menu = Menu::BUI_BASES;

							break;
						case BARRACK:
							current_building_type = BARRACK;
							//Buttons
							ghoul_barrack[0] = dynamic_cast<UI_Button*>(App->gui->CreateButton(670, 593, Ghouls_melee_button, { 1355,2028,149,52 }, { 1355,2112,149,52 }, { 1355,2196,149,52 }, NULL, this));
							ghoul_barrack[1] = dynamic_cast<UI_Button*>(App->gui->CreateButton(830, 593, Ghouls_ranged_button, { 1355,2028,149,52 }, { 1355,2112,149,52 }, { 1355,2196,149,52 }, NULL, this));
							ghoul_barrack[2] = dynamic_cast<UI_Button*>(App->gui->CreateButton(670, 653, Boost_barrack_button, { 1355,2028,149,52 }, { 1355,2112,149,52 }, { 1355,2196,149,52 }, NULL, this));

							//Images
							ghoul_barrack[3] = dynamic_cast<j1Image*>(App->gui->CreateImage(540, 590, Image, { 527, 1563, 105, 105 }, NULL, this));
							ghoul_barrack[4] = dynamic_cast<j1Image*>(App->gui->CreateImage(680, 600, Image, { 513, 1865, 33, 34 }, NULL, this));
							ghoul_barrack[5] = dynamic_cast<j1Image*>(App->gui->CreateImage(840, 600, Image, { 571, 1866, 28, 33 }, NULL, this));
							ghoul_barrack[6] = dynamic_cast<j1Image*>(App->gui->CreateImage(675, 665, Image, { 823, 1872, 32, 22 }, NULL, this));

							//Labels
							if (App->entities->units_damage[App->player->faction].upgrade_num < 4) {
								 cost = App->entities->units_damage[App->player->faction].first_price + App->entities->units_damage[App->player->faction].price_increment * App->entities->units_damage[App->player->faction].upgrade_num;
								
								ghoul_barrack[7] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 656, Label, std::to_string(cost * 2), NULL, this, "StackedPixelXS"));
							}
							else
								ghoul_barrack[7] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 656, Label, "MAX", NULL, this, "StackedPixelXS"));

							ghoul_barrack[8] = dynamic_cast<j1Image*>(App->gui->CreateImage(1020, 600, Image, { 486, 1930, 22, 67 }, NULL, this));
							ghoul_barrack[9] = dynamic_cast<j1Image*>(App->gui->CreateImage(1110, 600, Image, { 442, 1930, 42, 60 }, NULL, this));
							ghoul_barrack[10] = dynamic_cast<j1Image*>(App->gui->CreateImage(1200, 610, Image, { 0, 2338, 41, 46 }, NULL, this));

							ghoul_barrack[11] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(995, 668, Label, "x", NULL, this, "StackedPixelSmall"));
							ghoul_barrack[12] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(1092, 668, Label, "x", NULL, this, "StackedPixelSmall"));

							//Cost
							ghoul_barrack[13] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 596, Label, "0", NULL, this, "StackedPixelXS"));		//Nukas
							ghoul_barrack[14] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 613, Label, "80", NULL, this, "StackedPixelXS"));	//Water
							ghoul_barrack[15] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 629, Label, "60", NULL, this, "StackedPixelXS"));	//Food

							ghoul_barrack[16] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 673, Label, "0", NULL, this, "StackedPixelXS"));	  //Water
							ghoul_barrack[17] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 689, Label, "0", NULL, this, "StackedPixelXS"));	  //Food

							ghoul_barrack[18] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(910, 596, Label, "0", NULL, this, "StackedPixelXS"));		//Nukas
							ghoul_barrack[19] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(910, 613, Label, "80", NULL, this, "StackedPixelXS"));	//Water
							ghoul_barrack[20] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(910, 629, Label, "80", NULL, this, "StackedPixelXS"));	//Food

							//Draw initial quantity
							CreateMenu(Menu::RANGED_QUANTITY);
							CreateMenu(Menu::MELEE_QUANTITY);

							last_menu = current_menu;
							current_menu = Menu::BUI_BASES;

							break;
						case LABORATORY:
							current_building_type = LABORATORY;
							//Buttons
							ghoul_lab[0] = dynamic_cast<UI_Button*>(App->gui->CreateButton(670, 593, Boost_lab_button, { 1355,2028,149,52 }, { 1355,2112,149,52 }, { 1355,2196,149,52 }, NULL, this));
							ghoul_lab[1] = dynamic_cast<UI_Button*>(App->gui->CreateButton(830, 593, Boost_radar_button, { 1355,2028,149,52 }, { 1355,2112,149,52 }, { 1355,2196,149,52 }, NULL, this));
							ghoul_lab[2] = dynamic_cast<UI_Button*>(App->gui->CreateButton(670, 653, summon_mrhandy_button, { 1355,2028,149,52 }, { 1355,2112,149,52 }, { 1355,2196,149,52 }, NULL, this));
							

							//Images
							ghoul_lab[3] = dynamic_cast<j1Image*>(App->gui->CreateImage(540, 615, Image, { 536, 1741, 101, 73 }, NULL, this));
							ghoul_lab[4] = dynamic_cast<j1Image*>(App->gui->CreateImage(677, 607, Image, { 877, 1874, 33, 20 }, NULL, this));
							
							ghoul_lab[5] = dynamic_cast<j1Image*>(App->gui->CreateImage(840, 600, Image, { 982, 1869, 31, 33 }, NULL, this));
							ghoul_lab[6] = dynamic_cast<j1Image*>(App->gui->CreateImage(680, 660, Image, { 1034, 1869, 35, 33 }, NULL, this));

							//Labels
							if (App->entities->units_creation_time[App->player->faction].upgrade_num < 4) {
								 cost = App->entities->units_creation_time[App->player->faction].first_price + App->entities->units_creation_time[App->player->faction].price_increment * App->entities->units_creation_time[App->player->faction].upgrade_num;
								ghoul_lab[7] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 596, Label, std::to_string(cost * 2), NULL, this, "StackedPixelXS"));
							}
							else
								ghoul_lab[7] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 596, Label, "MAX", NULL, this, "StackedPixelXS"));

							//Carta
							ghoul_lab[8] = dynamic_cast<j1Image*>(App->gui->CreateImage(995, 590, Image, { 228, 2328, 73, 90 }, NULL, this));
							ghoul_lab[9] = dynamic_cast<j1Image*>(App->gui->CreateImage(1102, 600, Image, { 164, 2336, 55, 58 }, NULL, this));
							ghoul_lab[10] = dynamic_cast<j1Image*>(App->gui->CreateImage(1205, 600, Image, { 52, 2338, 45, 51 }, NULL, this));

							ghoul_lab[11] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 613, Label, "0", NULL, this, "StackedPixelXS"));
							ghoul_lab[12] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 629, Label, "0", NULL, this, "StackedPixelXS"));
						
							ghoul_lab[13] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 656, Label, "1000", NULL, this, "StackedPixelXS"));
							ghoul_lab[14] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 673, Label, "0", NULL, this, "StackedPixelXS"));
							ghoul_lab[15] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(750, 689, Label, "0", NULL, this, "StackedPixelXS"));
							
							ghoul_lab[16] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(910, 596, Label, "100", NULL, this, "StackedPixelXS"));
							ghoul_lab[17] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(910, 613, Label, "0", NULL, this, "StackedPixelXS"));
							ghoul_lab[18] = dynamic_cast<UI_Label*>(App->gui->CreateLabel(910, 629, Label, "0", NULL, this, "StackedPixelXS"));

							last_menu = current_menu;
							current_menu = Menu::BUI_BASES;

							break;
						default:
							break;
					}
					break;

				default:
					break;

			}
			break;

		default:
			break;
	}
}

void MenuManager::DestroyMenu(Menu menu) {

	switch (menu)
	{
	case Menu::MAIN_MENU:
		App->gui->DeleteArrayElements(main_menu, 11);
		break;
	case Menu::SETTINGS:
		App->gui->DeleteArrayElements(settings_menu, 23);
		App->gui->Delete_Element(cap_label);
		App->gui->Delete_Element(fullscreen_label);
		break;
	case Menu::PAUSE_SETTINGS:
		App->gui->DeleteArrayElements(settings_menu, 23);
		App->gui->Delete_Element(cap_label);
		App->gui->Delete_Element(fullscreen_label);
		App->minimap->Show();
		break;
	case Menu::CREDITS:
		App->gui->DeleteArrayElements(credits_menu, 5);
		App->gui->DeleteArrayElements(group_photos, 8);
		break;
	case Menu::SELECT_FACTION:
		App->gui->DeleteArrayElements(select_faction_menu, 10);
		break;
	case Menu::PAUSE_MENU:
		App->gui->DeleteArrayElements(pause_menu, 7);
		break;
	case Menu::GUI:
		App->gui->DeleteArrayElements(gui_ingame, 6);
		App->gui->Delete_Element(timer_item);
		App->gui->Delete_Element(timer_minutes);
		break;
	case Menu::RESOURCES:
		App->gui->DeleteArrayElements(resources, 3);
		break;
	case Menu::RANGED_QUANTITY:
		App->gui->Delete_Element(ranged_label);
		break;
	case Menu::GATHERER_QUANTITY:
		App->gui->Delete_Element(gatherer_label);
		break;
	case Menu::MELEE_QUANTITY:
		App->gui->Delete_Element(melee_label);
		break;
	case Menu::TUTORIAL:
		App->gui->DeleteArrayElements(tutorial, 8);
		App->gui->Delete_Element(tutorial_screen);
		App->gui->Delete_Element(info_button2_element);
		break;
	case Menu::HOW_TO_PLAY:
		App->gui->DeleteArrayElements(how_to_play, 7);
		break;
	case Menu::DIALOG:
		App->gui->DeleteArrayElements(dialogs, 11);
		App->minimap->Show();
		break;
	case Menu::QUEST:
		App->gui->DeleteArrayElements(quest, 12);
		break;
	case Menu::RADIO:
		App->gui->DeleteArrayElements(radio, 6);
		break;

	default:
		break;
	}

	current_menu = last_menu;
}
void MenuManager::DestroyFaction(Menu menu, Faction faction, EntityType type) {

	switch (menu)
	{
	case Menu::BUI_BASES:

		switch (faction)
		{
		case BROTHERHOOD:
			switch (type)
			{
			case BASE:
				App->gui->DeleteArrayElements(brotherhood_base, 14);
				App->gui->Delete_Element(gatherer_label);
			
				break;
			case BARRACK:
				App->gui->DeleteArrayElements(brotherhood_barrack, 21);
				App->gui->Delete_Element(melee_label);
				App->gui->Delete_Element(ranged_label);
				break;
			case LABORATORY:
				App->gui->DeleteArrayElements(brotherhood_lab, 19);
				break;
			default:
				break;
			}
			break;

		case MUTANT:
			switch (type)
			{
			case BASE:
				App->gui->DeleteArrayElements(supermutant_base, 14);
				App->gui->Delete_Element(gatherer_label);
				break;
			case BARRACK:
				App->gui->DeleteArrayElements(supermutant_barrack, 21);
				App->gui->Delete_Element(melee_label);
				App->gui->Delete_Element(ranged_label);
				break;
			case LABORATORY:
				App->gui->DeleteArrayElements(supermutant_lab, 19);
				break;
			default:
				break;
			}
			break;

		case GHOUL:
			switch (type){
			case BASE:
				App->gui->DeleteArrayElements(ghoul_base, 14);
				App->gui->Delete_Element(gatherer_label);
				break;
			case BARRACK:
				App->gui->DeleteArrayElements(ghoul_barrack, 21);
				App->gui->Delete_Element(melee_label);
				App->gui->Delete_Element(ranged_label);
				break;
			case LABORATORY:
				App->gui->DeleteArrayElements(ghoul_lab, 19);
				break;
			default:
				break;
			}
			break;

		case VAULT:
			switch (type)
			{
			case BASE:
				App->gui->DeleteArrayElements(vault_base, 14);
				App->gui->Delete_Element(gatherer_label);
				break;
			case BARRACK:
				App->gui->DeleteArrayElements(vault_barrack, 21);
				App->gui->Delete_Element(melee_label);
				App->gui->Delete_Element(ranged_label);
				break;
			case LABORATORY:
				App->gui->DeleteArrayElements(vault_lab, 19);
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;

	default:
		break;
	}

	//current_menu = last_menu;
}