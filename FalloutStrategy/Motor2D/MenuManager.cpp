#include "p2Defs.h"
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
#include "brofiler/Brofiler/Brofiler.h"
#include "SDL_mixer/include/SDL_mixer.h"

MenuManager::MenuManager()
{
	name = ("menu_manager");
	current_menu = last_menu = Menu::NO_MENU;

	
}

MenuManager::~MenuManager()
{
}

void MenuManager::CreateMenu(Menu menu) {

	switch (menu)
	{
	case Menu::MAIN_MENU:

		//Images															
		main_menu[0] = (j1Image*)App->gui->CreateImage(-5, 0, Image, { 1529, 0, 1502, 775 }, NULL, this);

		//Buttons
		main_menu[1] = (UI_Button*)App->gui->CreateButton(260, 560, button_new_game, { 1372,927,187,49 }, { 1372,987,187,49 }, { 1372,1046,187,49 }, NULL, this);
		main_menu[2] = (UI_Button*)App->gui->CreateButton(460, 560, button_load_game, { 1372,927,187,49 }, { 1372,987,187,49 }, { 1372,1046,187,49 }, NULL, this);
		main_menu[3] = (UI_Button*)App->gui->CreateButton(160, 620, button_settings, { 1372,927,187,49 }, { 1372,987,187,49 }, { 1372,1046,187,49 }, NULL, this);
		main_menu[4] = (UI_Button*)App->gui->CreateButton(360, 620, button_credits, { 1372,927,187,49 }, { 1372,987,187,49 }, { 1372,1046,187,49 }, NULL, this);
		main_menu[5] = (UI_Button*)App->gui->CreateButton(560, 620, button_exit, { 1372,927,187,49 }, { 1372,987,187,49 }, { 1372,1046,187,49 }, NULL, this);

		//Labels
		main_menu[6] = (UI_Label*)App->gui->CreateLabel(305, 575, Label, "NEW GAME", NULL, this, NULL, "StackedPixelSmall");
		main_menu[7] = (UI_Label*)App->gui->CreateLabel(505, 575, Label, "LOAD GAME", NULL, this, NULL, "StackedPixelSmall");
		main_menu[8] = (UI_Label*)App->gui->CreateLabel(210, 635, Label, "SETTINGS", NULL, this, NULL, "StackedPixelSmall");
		main_menu[9] = (UI_Label*)App->gui->CreateLabel(415, 635, Label, "CREDITS", NULL, this, NULL, "StackedPixelSmall");
		main_menu[10] = (UI_Label*)App->gui->CreateLabel(630, 635, Label, "EXIT", NULL, this, NULL, "StackedPixelSmall");



		App->render->camera.x = 0;
		App->render->camera.y = 0;

		last_menu = current_menu;
		current_menu = Menu::MAIN_MENU;
		break;
	case Menu::CREDITS:

		//Images
		credits_menu[0] = (j1Image*)App->gui->CreateImage(0, 0, Image, { 1287, 2430, 1281, 730 }, NULL, this);

		//Buttons
		credits_menu[1] = (UI_Button*)App->gui->CreateButton(65, 65, button_back_credits, { 1243,2014,73,78 }, { 1243,2102,73,79 }, { 1243,2188,73,71 }, NULL, this);
		credits_menu[2] = (UI_Button*)App->gui->CreateButton(65, 200, button_github_credits, { 2181,841,73,78 }, { 2181,919,73,78 }, { 2181,1005,73,78 }, NULL, this);
		credits_menu[3] = (UI_Button*)App->gui->CreateButton(65, 300, button_web_credits, { 2268,841,73,78 }, { 2268,919,73,78 }, { 2268,1005,73,78 }, NULL, this);
		credits_menu[4] = (UI_Button*)App->gui->CreateButton(65, 400, button_twitter_credits, { 2355,841,73,78 }, { 2355,919,73,78 }, { 2355,1005,73,78 }, NULL, this);

		last_menu = current_menu;
		current_menu = Menu::CREDITS;

		break;
	case Menu::SETTINGS:

		//Images
		settings_menu[0] = (j1Image*)App->gui->CreateImage(-5, 0, Image, { 1529, 0, 1502, 775 }, NULL, this);
		settings_menu[1] = (j1Image*)App->gui->CreateImage(310, 125, Image, { 0, 142, 658, 358 }, NULL, this);
		settings_menu[2] = (j1Image*)App->gui->CreateImage(420, 500, Image, { 1017, 352, 455, 190 }, NULL, this);
		settings_menu[3] = (j1Image*)App->gui->CreateImage(395, 275, Image, { 1674, 981, 20, 36 }, NULL, this);
		settings_menu[4] = (j1Image*)App->gui->CreateImage(505, 600, Image, { 1599, 1054, 77, 52 }, NULL, this);
		settings_menu[19] = (j1Image*)App->gui->CreateImage(740, 600, Image, { 1599, 1054, 77, 52 }, NULL, this);
		//Labels
		settings_menu[5] = (UI_Label*)App->gui->CreateLabel(450, 535, Label, "REFRESH RATE", NULL, this, NULL, "StackedPixelMedium");
		settings_menu[6] = (UI_Label*)App->gui->CreateLabel(360, 335, Label, "SOUND EFFECTS VOLUME", NULL, this, NULL);
		settings_menu[7] = (UI_Label*)App->gui->CreateLabel(360, 150, Label, "MUSIC VOLUME", NULL, this, NULL);
		settings_menu[8] = (UI_Label*)App->gui->CreateLabel(555, 50, Label, "SETTINGS", NULL, this, NULL);
		settings_menu[18] = (UI_Label*)App->gui->CreateLabel(685, 535, Label, "FULLSCREEN", NULL, this, NULL, "StackedPixelMedium");

		if (App->gui->fullscreen == true) {
			settings_menu[22] = (UI_Label*)App->gui->CreateLabel(756, 613, Label, "YES", NULL, this, NULL, "StackedPixelMedium");
		}
		else {
			settings_menu[22] = (UI_Label*)App->gui->CreateLabel(763, 613, Label, "NO", NULL, this, NULL, "StackedPixelMedium");
		}

		if (App->gui->cap == true) {
			settings_menu[21] = (UI_Label*)App->gui->CreateLabel(528, 613, Label, "30", NULL, this, NULL, "StackedPixelMedium");
		}
		else {
			settings_menu[21] = (UI_Label*)App->gui->CreateLabel(528, 613, Label, "60", NULL, this, NULL, "StackedPixelMedium");
		}

		//Music Slider
		settings_menu[9] = (UI_Slider*)App->gui->CreateSlider(465, 220, Slider_music, { 1834,842,329,27 }, { 1792,926,33,16 }, 400, NULL, this);
		settings_menu[10] = (UI_Button*)App->gui->CreateButton(435, 220, Button_slider_music_left, { 1671,880,26,25 }, { 1738,880,26,25 }, { 1738,880,26,25 }, NULL, this);
		settings_menu[11] = (UI_Button*)App->gui->CreateButton(787, 220, Button_slider_music_right, { 1704,880,26,25 }, { 1771,880,26,25 }, { 1771,880,26,25 }, NULL, this);

		//Fx Slider
		settings_menu[12] = (UI_Slider*)App->gui->CreateSlider(465, 405, Slider_fx, { 1834,842,329,27 }, { 1792,926,33,16 }, 400, NULL, this);
		settings_menu[13] = (UI_Button*)App->gui->CreateButton(435, 405, Button_slider_fx_left, { 1671,880,26,25 }, { 1738,880,26,25 }, { 1738,880,26,25 }, NULL, this);
		settings_menu[14] = (UI_Button*)App->gui->CreateButton(796, 405, Button_slider_fx_right, { 1704,880,26,25 }, { 1771,880,26,25 }, { 1771,880,26,25 }, NULL, this);

		//Buttons
		settings_menu[15] = (UI_Button*)App->gui->CreateButton(470, 615, button_cap, { 1671,922,25,26 }, { 1713,922,25,26 }, { 1713,922,25,26 }, NULL, this);
		settings_menu[16] = (UI_Button*)App->gui->CreateButton(360, 280, button_mute, { 1671,922,25,26 }, { 1713,922,25,26 }, { 1713,922,25,26 }, NULL, this);
		settings_menu[17] = (UI_Button*)App->gui->CreateButton(40, 40, button_back, { 1704,1054,48,46 }, { 1765,1054,48,46 }, { 1765,1054,48,46 }, NULL, this);
		settings_menu[20] = (UI_Button*)App->gui->CreateButton(705, 615, button_fullscreen, { 1671,922,25,26 }, { 1713,922,25,26 }, { 1713,922,25,26 }, NULL, this);

		last_menu = current_menu;
		current_menu = Menu::SETTINGS;

		break;

	case Menu::PAUSE_SETTINGS:

		//Images
		settings_menu[0] = (j1Image*)App->gui->CreateImage(-5, 0, Image, { 1529, 0, 1502, 775 }, NULL, this);
		settings_menu[1] = (j1Image*)App->gui->CreateImage(310, 125, Image, { 0, 537, 658, 358 }, NULL, this);
		settings_menu[2] = (j1Image*)App->gui->CreateImage(420, 500, Image, { 1017, 352, 455, 190 }, NULL, this);
		settings_menu[3] = (j1Image*)App->gui->CreateImage(395, 275, Image, { 1674, 981, 20, 36 }, NULL, this);
		settings_menu[4] = (j1Image*)App->gui->CreateImage(505, 600, Image, { 1599, 1054, 77, 52 }, NULL, this);
		settings_menu[19] = (j1Image*)App->gui->CreateImage(740, 600, Image, { 1599, 1054, 77, 52 }, NULL, this);
		//Labels
		settings_menu[5] = (UI_Label*)App->gui->CreateLabel(450, 535, Label, "REFRESH RATE", NULL, this, NULL, "StackedPixelMedium");
		settings_menu[6] = (UI_Label*)App->gui->CreateLabel(360, 335, Label, "SOUND EFFECTS VOLUME", NULL, this, NULL);
		settings_menu[7] = (UI_Label*)App->gui->CreateLabel(360, 150, Label, "MUSIC VOLUME", NULL, this, NULL);
		settings_menu[8] = (UI_Label*)App->gui->CreateLabel(555, 50, Label, "SETTINGS", NULL, this, NULL);
		settings_menu[18] = (UI_Label*)App->gui->CreateLabel(685, 535, Label, "FULLSCREEN", NULL, this, NULL, "StackedPixelMedium");

		if (App->gui->fullscreen == true) {
			settings_menu[22] = (UI_Label*)App->gui->CreateLabel(756, 613, Label, "YES", NULL, this, NULL, "StackedPixelMedium");
		}
		else {
			settings_menu[22] = (UI_Label*)App->gui->CreateLabel(763, 613, Label, "NO", NULL, this, NULL, "StackedPixelMedium");
		}

		if (App->gui->cap == true) {
			settings_menu[21] = (UI_Label*)App->gui->CreateLabel(528, 613, Label, "30", NULL, this, NULL, "StackedPixelMedium");
		}
		else {
			settings_menu[21] = (UI_Label*)App->gui->CreateLabel(528, 613, Label, "60", NULL, this, NULL, "StackedPixelMedium");
		}

		//Music Slider
		settings_menu[9] = (UI_Slider*)App->gui->CreateSlider(465, 220, Slider_music, { 1834,842,329,27 }, { 1792,926,33,16 }, 400, NULL, this);
		settings_menu[10] = (UI_Button*)App->gui->CreateButton(435, 220, Button_slider_music_left, { 1671,880,26,25 }, { 1738,880,26,25 }, { 1738,880,26,25 }, NULL, this);
		settings_menu[11] = (UI_Button*)App->gui->CreateButton(787, 220, Button_slider_music_right, { 1704,880,26,25 }, { 1771,880,26,25 }, { 1771,880,26,25 }, NULL, this);

		//Fx Slider
		settings_menu[12] = (UI_Slider*)App->gui->CreateSlider(465, 405, Slider_fx, { 1834,842,329,27 }, { 1792,926,33,16 }, 400, NULL, this);
		settings_menu[13] = (UI_Button*)App->gui->CreateButton(435, 405, Button_slider_fx_left, { 1671,880,26,25 }, { 1738,880,26,25 }, { 1738,880,26,25 }, NULL, this);
		settings_menu[14] = (UI_Button*)App->gui->CreateButton(796, 405, Button_slider_fx_right, { 1704,880,26,25 }, { 1771,880,26,25 }, { 1771,880,26,25 }, NULL, this);

		//Buttons
		settings_menu[15] = (UI_Button*)App->gui->CreateButton(470, 615, button_cap, { 1671,922,25,26 }, { 1713,922,25,26 }, { 1713,922,25,26 }, NULL, this);
		settings_menu[16] = (UI_Button*)App->gui->CreateButton(360, 280, button_mute, { 1671,922,25,26 }, { 1713,922,25,26 }, { 1713,922,25,26 }, NULL, this);
		settings_menu[17] = (UI_Button*)App->gui->CreateButton(40, 40, button_back_pause, { 1704,1054,48,46 }, { 1765,1054,48,46 }, { 1765,1054,48,46 }, NULL, this);
		settings_menu[20] = (UI_Button*)App->gui->CreateButton(705, 615, button_fullscreen, { 1671,922,25,26 }, { 1713,922,25,26 }, { 1713,922,25,26 }, NULL, this);

		last_menu = current_menu;
		current_menu = Menu::PAUSE_SETTINGS;

		break;

	case Menu::SELECT_FACTION:

		//Images
		select_faction_menu[0] = (j1Image*)App->gui->CreateImage(-5, 0, Image, { 1900, 1531, 1290, 842 }, NULL, this);

		//Buttons
		select_faction_menu[1] = (UI_Button*)App->gui->CreateButton(160, 200, button_select_ghoul, { 1900,1144,204,60 }, { 1900,1210,204,60 }, { 1900,1277,204,60 }, NULL, this);
		select_faction_menu[2] = (UI_Button*)App->gui->CreateButton(160, 290, button_select_vault, { 1900,1144,204,60 }, { 1900,1210,204,60 }, { 1900,1277,204,60 }, NULL, this);
		select_faction_menu[3] = (UI_Button*)App->gui->CreateButton(160, 380, button_select_brotherhood, { 1900,1144,204,60 }, { 1900,1210,204,60 }, { 1900,1277,204,60 }, NULL, this);
		select_faction_menu[4] = (UI_Button*)App->gui->CreateButton(160, 470, button_select_supermutant, { 1900,1144,204,60 }, { 1900,1210,204,60 }, { 1900,1277,204,60 }, NULL, this);
		select_faction_menu[5] = (UI_Button*)App->gui->CreateButton(120, 605, button_back_to_menu, { 1243,2014,73,78 }, { 1243,2102,73,79 }, { 1243,2188,73,71 }, NULL, this);
		//select_faction_menu[6] = (UI_Button*)App->gui->CreateButton(900, 605, button_start_game, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,244,64 }, NULL, this);

		//Labels
		select_faction_menu[6] = (UI_Label*)App->gui->CreateLabel(222, 215, Label, "GHOUL", NULL, this, NULL, "StackedPixelMedium");
		select_faction_menu[7] = (UI_Label*)App->gui->CreateLabel(220, 305, Label, "VAULT", NULL, this, NULL, "StackedPixelMedium");
		select_faction_menu[8] = (UI_Label*)App->gui->CreateLabel(175, 395, Label, "BROTHERHOOD", NULL, this, NULL, "StackedPixelMedium");
		select_faction_menu[9] = (UI_Label*)App->gui->CreateLabel(170, 485, Label, "SUPERMUTANT", NULL, this, NULL, "StackedPixelMedium");
		//select_faction_menu[10] = (UI_Label*)App->gui->CreateLabel(925, 620, Label, "START GAME", NULL, this, NULL);

		last_menu = current_menu;
		current_menu = Menu::SELECT_FACTION;

		break;
	case Menu::PAUSE_MENU:

		//Images
		pause_menu[0] = (j1Image*)App->gui->CreateImage(434, 183, Image, { 3079, 41, 334, 313 }, NULL, this);


		//Buttons
		pause_menu[1] = (UI_Button*)App->gui->CreateButton(480, 210, resume_button, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,244,64 }, NULL, this);
		pause_menu[2] = (UI_Button*)App->gui->CreateButton(480, 300, button_settings_pause, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,244,64 }, NULL, this);
		pause_menu[3] = (UI_Button*)App->gui->CreateButton(480, 390, button_pause_to_main, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,244,64 }, NULL, this);

		//Labels
		pause_menu[4] = (UI_Label*)App->gui->CreateLabel(540, 228, Label, "RESUME", NULL, this, NULL);
		pause_menu[5] = (UI_Label*)App->gui->CreateLabel(530, 318, Label, "SETTINGS", NULL, this, NULL);
		pause_menu[6] = (UI_Label*)App->gui->CreateLabel(515, 408, Label, "MAIN MENU", NULL, this, NULL);

		last_menu = current_menu;
		current_menu = Menu::PAUSE_MENU;

		break;
	case Menu::GUI:
		
		//Images
		gui_ingame[0] = (j1Image*)App->gui->CreateImage(0, 0, Image, { 0, 2428, 1290, 730 }, NULL, this);
		gui_ingame[1] = (UI_Label*)App->gui->CreateLabel(690, 7, Label, ":", NULL, this, NULL, "StackedPixel");
		timer_item = (UI_Label*)App->gui->CreateLabel(700, 7, Label, "0", NULL, this, NULL, "StackedPixel");
		timer_minutes = (UI_Label*)App->gui->CreateLabel(652, 7, Label, "0", NULL, this, NULL, "StackedPixel");

		last_menu = current_menu;
		current_menu = Menu::GUI;
		
		//Tutorial
		gui_ingame[2] = (j1Image*)App->gui->CreateImage(1205, 70, Image, { 1037, 2120, 74, 71 }, NULL, this);
		gui_ingame[3] = (UI_Button*)App->gui->CreateButton(1160, 70, info_button, { 973,2084,43,46 }, { 973,2135,43,46 }, { 973,2185,43,46 }, NULL, this);
		
		break;
	case Menu::RESOURCES:


		water = std::to_string(App->player->water);
		food = std::to_string(App->player->food);
		caps = std::to_string(App->player->caps);

		resources[0] = (UI_Label*)App->gui->CreateLabel(90, 17, Label, food, NULL, this, NULL, "StackedPixelSmall");
		resources[1] = (UI_Label*)App->gui->CreateLabel(260, 17, Label, water, NULL, this, NULL, "StackedPixelSmall");
		resources[2] = (UI_Label*)App->gui->CreateLabel(435, 17, Label, caps, NULL, this, NULL, "StackedPixelSmall");

		/*last_menu = current_menu;
		current_menu = Menu::RESOURCES;*/

		break;

	case Menu::TUTORIAL:

		//Images
		tutorial_screen = (j1Image*)App->gui->CreateImage(634, 133, Image, { 0, 140, 654, 355 }, NULL, this);
		tutorial[0] = (j1Image*)App->gui->CreateImage(840, 200, Image, { 1900,895,244,72 }, NULL, this);

		//Buttons
		tutorial[1] = (UI_Button*)App->gui->CreateButton(680, 360, control_button, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,244,64 }, NULL, this);
		tutorial[2] = (UI_Button*)App->gui->CreateButton(1000, 360, how_to_play_button, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,244,64 }, NULL, this);
		info_button2_element = (UI_Button*)App->gui->CreateButton(594, 143, info_button2, { 908,2084,43,46 }, { 908,2135,43,46 }, { 908,2185,43,46 }, NULL, this);

		//Text
		tutorial[3] = (UI_Label*)App->gui->CreateLabel(890, 220, Label, "TUTORIAL", NULL, this, NULL);
		tutorial[4] = (UI_Label*)App->gui->CreateLabel(710, 380, Label, "CONTROLS", NULL, this, NULL);
		tutorial[5] = (UI_Label*)App->gui->CreateLabel(1020, 380, Label, "HOW TO PLAY", NULL, this, NULL);

		break;

	case Menu::BUI_BASES:
		break;
	case Menu::COLLABORATORS_CREDITS:
		break;
	case Menu::NO_MENU:
		break;
	default:
		break;
	}
}
void MenuManager::CreateMenuFaction(Menu menu, FACTION faction, BUILDING_TYPE type) {

	switch (menu)
	{

		case Menu::BUI_BASES:

			switch (faction)
			{

				case FACTION::BROTHERHOOD:
					switch (type)
					{
						case BUILDING_TYPE::BASE:

							//Buttons
							brotherhood_base[0] = (UI_Button*)App->gui->CreateButton(1050, 585, Brotherhood_ghaterer_button, { 1355,2028,53,52 }, { 1355,2112,53,52 }, { 1355,2196,53,52 }, NULL, this);
							brotherhood_base[1] = (UI_Button*)App->gui->CreateButton(1050, 645, Boost_base_button, { 1355,2028,53,52 }, { 1355,2112,53,52 }, { 1355,2196,53,52 }, NULL, this);

							//Images
							brotherhood_base[2] = (j1Image*)App->gui->CreateImage(900, 590, Image, { 118, 1561, 122, 98 }, NULL, this);
							brotherhood_base[3] = (j1Image*)App->gui->CreateImage(1058, 595, Image, { 138, 1868, 33, 33 }, NULL, this);
							brotherhood_base[4] = (j1Image*)App->gui->CreateImage(1055, 660, Image, { 926, 1872, 37, 24 }, NULL, this);

							//Labels
							if (App->entities->gatherer_resource_limit[App->player->faction].upgrade_num < 4) {
								int cost = App->entities->gatherer_resource_limit[App->player->faction].first_price + App->entities->gatherer_resource_limit[App->player->faction].price_increment * App->entities->gatherer_resource_limit[App->player->faction].upgrade_num;
								brotherhood_base[5] = (UI_Label*)App->gui->CreateLabel(1120, 630, Label, std::to_string(cost * 2), NULL, this, NULL, "StackedPixelSmall");
							}
							else
								brotherhood_base[5] = (UI_Label*)App->gui->CreateLabel(1120, 630, Label, "MAX.", NULL, this, NULL, "StackedPixelSmall");


							//Labels
							//p2SString hh("%i", App->entities->); VIDA EDIFICIO
							//gui_ingame.meat_count = (UI_Label*)App->gui->CreateLabel(95, 10, Meat, hh, NULL, this, NULL);
							//gui_ingame.meat_count = (UI_Label*)App->gui->CreateLabel(95, 10, Meat, "Brotherhood Base", NULL, this, NULL);

							last_menu = current_menu;
							current_menu = Menu::BUI_BASES;

							break;
						case BUILDING_TYPE::BARRACK:

							//Buttons
							brotherhood_barrack[0] = (UI_Button*)App->gui->CreateButton(1050, 570, Brotherhood_melee_button, { 1355,2028,53,52 }, { 1355,2112,53,52 }, { 1355,2196,53,52 }, NULL, this);
							brotherhood_barrack[1] = (UI_Button*)App->gui->CreateButton(1115, 570, Brotherhood_ranged_button, { 1355,2028,53,52 }, { 1355,2112,53,52 }, { 1355,2196,53,52 }, NULL, this);
							brotherhood_barrack[2] = (UI_Button*)App->gui->CreateButton(1050, 620, Boost_barrack_button, { 1418,2028,153,41 }, { 1418,2112,153,41 }, { 1418,2197,153,41 }, NULL, this);

							//Images
							brotherhood_barrack[3] = (j1Image*)App->gui->CreateImage(890, 570, Image, { 17, 1560, 69, 115 }, NULL, this);
							brotherhood_barrack[4] = (j1Image*)App->gui->CreateImage(1060, 575, Image, { 28, 1869, 35, 33 }, NULL, this);
							brotherhood_barrack[5] = (j1Image*)App->gui->CreateImage(1120, 574, Image, { 83, 1866, 35, 36 }, NULL, this);
							brotherhood_barrack[6] = (j1Image*)App->gui->CreateImage(1060, 630, Image, { 823, 1872, 32, 22 }, NULL, this);

							//Labels
							if (App->entities->units_damage[App->player->faction].upgrade_num < 4) {
								int cost = App->entities->units_damage[App->player->faction].first_price + App->entities->units_damage[App->player->faction].price_increment * App->entities->units_damage[App->player->faction].upgrade_num;
								brotherhood_barrack[7] = (UI_Label*)App->gui->CreateLabel(1120, 630, Label, std::to_string(cost * 2), NULL, this, NULL, "StackedPixelSmall");
							}
							else
								brotherhood_barrack[7] = (UI_Label*)App->gui->CreateLabel(1120, 630, Label, "MAX", NULL, this, NULL, "StackedPixelSmall");

							last_menu = current_menu;
							current_menu = Menu::BUI_BASES;

							break;
						case BUILDING_TYPE::LAB:

							//Buttons
							brotherhood_lab[0] = (UI_Button*)App->gui->CreateButton(1050, 620, Boost_lab_button, { 1418,2028,153,41 }, { 1418,2112,153,41 }, { 1418,2197,153,41 }, NULL, this);
							brotherhood_lab[4] = (UI_Button*)App->gui->CreateButton(1050, 570, Boost_radar_button, { 1418,2028,153,41 }, { 1418,2112,153,41 }, { 1418,2197,153,41 }, NULL, this);
							brotherhood_lab[6] = (UI_Button*)App->gui->CreateButton(1050, 670, summon_mrhandy_button, { 1418,2028,153,41 }, { 1418,2112,153,41 }, { 1418,2197,153,41 }, NULL, this);
						
							//Images
							brotherhood_lab[1] = (j1Image*)App->gui->CreateImage(885, 570, Image, { 663, 1567, 108, 104 }, NULL, this);
							brotherhood_lab[2] = (j1Image*)App->gui->CreateImage(1060, 630, Image, { 877, 1874, 33, 20 }, NULL, this);
							
							brotherhood_lab[5] = (j1Image*)App->gui->CreateImage(1060, 575, Image, { 982, 1869, 31, 33 }, NULL, this);
							brotherhood_lab[7] = (j1Image*)App->gui->CreateImage(1060, 675, Image, { 1034, 1869, 35, 33 }, NULL, this);

							//Labels
							if (App->entities->units_creation_time[App->player->faction].upgrade_num < 4) {
								int cost = App->entities->units_creation_time[App->player->faction].first_price + App->entities->units_creation_time[App->player->faction].price_increment * App->entities->units_creation_time[App->player->faction].upgrade_num;
								brotherhood_lab[3] = (UI_Label*)App->gui->CreateLabel(1120, 630, Label, std::to_string(cost * 2), NULL, this, NULL, "StackedPixelSmall");
							}
							else
								brotherhood_lab[3] = (UI_Label*)App->gui->CreateLabel(1120, 630, Label, "MAX", NULL, this, NULL, "StackedPixelSmall");

							
							last_menu = current_menu;
							current_menu = Menu::BUI_BASES;

							break;
						default:
							break;
					}
					break;

				case FACTION::SUPERMUTANT:
					switch (type)
					{

						case BUILDING_TYPE::BASE:
							
							//Buttons
							supermutant_base[0] = (UI_Button*)App->gui->CreateButton(1050, 585, Supermutant_ghaterer_button, { 1355,2028,53,52 }, { 1355,2112,53,52 }, { 1355,2196,53,52 }, NULL, this);
							supermutant_base[1] = (UI_Button*)App->gui->CreateButton(1050, 645, Boost_base_button, { 1355,2028,53,52 }, { 1355,2112,53,52 }, { 1355,2196,53,52 }, NULL, this);

							//Images
							supermutant_base[2] = (j1Image*)App->gui->CreateImage(900, 590, Image, { 934, 1577, 129, 91 }, NULL, this);
							supermutant_base[3] = (j1Image*)App->gui->CreateImage(1060, 595, Image, { 191, 1868, 34, 32 }, NULL, this);
							supermutant_base[4] = (j1Image*)App->gui->CreateImage(1055, 660, Image, { 926, 1872, 37, 24 }, NULL, this);

							//Labels
							if (App->entities->gatherer_resource_limit[App->player->faction].upgrade_num < 4) {
								int cost = App->entities->gatherer_resource_limit[App->player->faction].first_price + App->entities->gatherer_resource_limit[App->player->faction].price_increment * App->entities->gatherer_resource_limit[App->player->faction].upgrade_num;
								supermutant_base[5] = (UI_Label*)App->gui->CreateLabel(1120, 630, Label, std::to_string(cost * 2), NULL, this, NULL, "StackedPixelSmall");
							}
							else
								supermutant_base[5] = (UI_Label*)App->gui->CreateLabel(1120, 630, Label, "MAX", NULL, this, NULL, "StackedPixelSmall");

							last_menu = current_menu;
							current_menu = Menu::BUI_BASES;

							break;
						case BUILDING_TYPE::BARRACK:

							//Buttons
							supermutant_barrack[0] = (UI_Button*)App->gui->CreateButton(1050, 585, Supermutant_melee_button, { 1355,2028,53,52 }, { 1355,2112,53,52 }, { 1355,2196,53,52 }, NULL, this);
							supermutant_barrack[1] = (UI_Button*)App->gui->CreateButton(1110, 585, Supermutant_ranged_button, { 1355,2028,53,52 }, { 1355,2112,53,52 }, { 1355,2196,53,52 }, NULL, this);
							supermutant_barrack[2] = (UI_Button*)App->gui->CreateButton(1050, 645, Boost_barrack_button, { 1355,2028,53,52 }, { 1355,2112,53,52 }, { 1355,2196,53,52 }, NULL, this);

							//Images
							supermutant_barrack[3] = (j1Image*)App->gui->CreateImage(890, 590, Image, { 801, 1549, 102, 119 }, NULL, this);
							supermutant_barrack[4] = (j1Image*)App->gui->CreateImage(1062, 595, Image, { 246, 1869, 34, 33 }, NULL, this);
							supermutant_barrack[5] = (j1Image*)App->gui->CreateImage(1117, 595, Image, { 301, 1865, 34, 35 }, NULL, this);
							supermutant_barrack[6] = (j1Image*)App->gui->CreateImage(1055, 660, Image, { 823, 1872, 32, 22 }, NULL, this);

							//Labels
							if (App->entities->units_damage[App->player->faction].upgrade_num < 4) {
								int cost = App->entities->units_damage[App->player->faction].first_price + App->entities->units_damage[App->player->faction].price_increment * App->entities->units_damage[App->player->faction].upgrade_num;
								supermutant_barrack[7] = (UI_Label*)App->gui->CreateLabel(1120, 630, Label, std::to_string(cost * 2), NULL, this, NULL, "StackedPixelSmall");
							}
							else
								supermutant_barrack[7] = (UI_Label*)App->gui->CreateLabel(1120, 630, Label, "MAX", NULL, this, NULL, "StackedPixelSmall");

							last_menu = current_menu;
							current_menu = Menu::BUI_BASES;

							break;
						case BUILDING_TYPE::LAB:

							//Buttons
		
							supermutant_lab[0] = (UI_Button*)App->gui->CreateButton(1050, 620, Boost_lab_button, { 1418,2028,153,41 }, { 1418,2112,153,41 }, { 1418,2197,153,41 }, NULL, this);
							supermutant_lab[4] = (UI_Button*)App->gui->CreateButton(1050, 570, Boost_radar_button, { 1418,2028,153,41 }, { 1418,2112,153,41 }, { 1418,2197,153,41 }, NULL, this);
							supermutant_lab[6] = (UI_Button*)App->gui->CreateButton(1050, 670, summon_mrhandy_button, { 1418,2028,153,41 }, { 1418,2112,153,41 }, { 1418,2197,153,41 }, NULL, this);
							

							//Images
							supermutant_lab[1] = (j1Image*)App->gui->CreateImage(885, 570, Image, { 663, 1567, 108, 104 }, NULL, this);
							supermutant_lab[2] = (j1Image*)App->gui->CreateImage(1060, 630, Image, { 877, 1874, 33, 20 }, NULL, this);
							
							supermutant_lab[5] = (j1Image*)App->gui->CreateImage(1060, 575, Image, { 982, 1869, 31, 33 }, NULL, this);
							supermutant_lab[7] = (j1Image*)App->gui->CreateImage(1060, 675, Image, { 1034, 1869, 35, 33 }, NULL, this);

							//Labels
							if (App->entities->units_creation_time[App->player->faction].upgrade_num < 4) {
								int cost = App->entities->units_creation_time[App->player->faction].first_price + App->entities->units_creation_time[App->player->faction].price_increment * App->entities->units_creation_time[App->player->faction].upgrade_num;
								supermutant_lab[3] = (UI_Label*)App->gui->CreateLabel(1120, 630, Label, std::to_string(cost * 2), NULL, this, NULL, "StackedPixelSmall");
							}
							else
								supermutant_lab[3] = (UI_Label*)App->gui->CreateLabel(1120, 630, Label, "MAX", NULL, this, NULL, "StackedPixelSmall");

							last_menu = current_menu;
							current_menu = Menu::BUI_BASES;

							break;
						default:
							break;
					}
					break;

				case FACTION::GHOUL:
					switch (type)
					{

						case BUILDING_TYPE::BASE:

							//Buttons
							ghoul_base[0] = (UI_Button*)App->gui->CreateButton(1050, 585, Ghouls_ghaterer_button, { 1355,2028,53,52 }, { 1355,2112,53,52 }, { 1355,2196,53,52 }, NULL, this);
							ghoul_base[1] = (UI_Button*)App->gui->CreateButton(1050, 645, Boost_base_button, { 1355,2028,53,52 }, { 1355,2112,53,52 }, { 1355,2196,53,52 }, NULL, this);
							

							//Images
							ghoul_base[2] = (j1Image*)App->gui->CreateImage(900, 590, Image, { 398, 1564, 99, 108 }, NULL, this);
							ghoul_base[3] = (j1Image*)App->gui->CreateImage(1064, 595, Image, { 624, 1866, 24, 34 }, NULL, this);
							ghoul_base[4] = (j1Image*)App->gui->CreateImage(1055, 660, Image, { 926, 1872, 37, 24 }, NULL, this);

							//Labels
							if (App->entities->gatherer_resource_limit[App->player->faction].upgrade_num < 4) {
								int cost = App->entities->gatherer_resource_limit[App->player->faction].first_price + App->entities->gatherer_resource_limit[App->player->faction].price_increment * App->entities->gatherer_resource_limit[App->player->faction].upgrade_num;
								ghoul_base[5] = (UI_Label*)App->gui->CreateLabel(1120, 630, Label, std::to_string(cost * 2), NULL, this, NULL, "StackedPixelSmall");
							}
							else
								ghoul_base[5] = (UI_Label*)App->gui->CreateLabel(1120, 630, Label, "MAX", NULL, this, NULL, "StackedPixelSmall");

							last_menu = current_menu;
							current_menu = Menu::BUI_BASES;

							break;
						case BUILDING_TYPE::BARRACK:

							//Buttons
							ghoul_barrack[0] = (UI_Button*)App->gui->CreateButton(1050, 570, Ghouls_melee_button, { 1355,2028,53,52 }, { 1355,2112,53,52 }, { 1355,2196,53,52 }, NULL, this);
							ghoul_barrack[1] = (UI_Button*)App->gui->CreateButton(1115, 570, Ghouls_ranged_button, { 1355,2028,53,52 }, { 1355,2112,53,52 }, { 1355,2196,53,52 }, NULL, this);
							ghoul_barrack[2] = (UI_Button*)App->gui->CreateButton(1050, 620, Boost_barrack_button, { 1418,2028,153,41 }, { 1418,2112,153,41 }, { 1418,2197,153,41 }, NULL, this);

							//Images
							ghoul_barrack[3] = (j1Image*)App->gui->CreateImage(900, 570, Image, { 527, 1563, 105, 105 }, NULL, this);
							ghoul_barrack[4] = (j1Image*)App->gui->CreateImage(1055, 575, Image, { 513, 1865, 33, 34 }, NULL, this);
							ghoul_barrack[5] = (j1Image*)App->gui->CreateImage(1110, 575, Image, { 571, 1866, 28, 33 }, NULL, this);
							ghoul_barrack[6] = (j1Image*)App->gui->CreateImage(1060, 630, Image, { 823, 1872, 32, 22 }, NULL, this);

							//Labels
							if (App->entities->units_damage[App->player->faction].upgrade_num < 4) {
								int cost = App->entities->units_damage[App->player->faction].first_price + App->entities->units_damage[App->player->faction].price_increment * App->entities->units_damage[App->player->faction].upgrade_num;
								ghoul_barrack[7] = (UI_Label*)App->gui->CreateLabel(1120, 630, Label, std::to_string(cost * 2), NULL, this, NULL, "StackedPixelSmall");
							}
							else
								ghoul_barrack[7] = (UI_Label*)App->gui->CreateLabel(1120, 630, Label, "MAX", NULL, this, NULL, "StackedPixelSmall");

							last_menu = current_menu;
							current_menu = Menu::BUI_BASES;

							break;
						case BUILDING_TYPE::LAB:

							//Buttons
							ghoul_lab[0] = (UI_Button*)App->gui->CreateButton(1050, 620, Boost_lab_button, { 1418,2028,153,41 }, { 1418,2112,153,41 }, { 1418,2197,153,41 }, NULL, this);
							ghoul_lab[4] = (UI_Button*)App->gui->CreateButton(1050, 570, Boost_radar_button, { 1418,2028,153,41 }, { 1418,2112,153,41 }, { 1418,2197,153,41 }, NULL, this);
							ghoul_lab[6] = (UI_Button*)App->gui->CreateButton(1050, 670, summon_mrhandy_button, { 1418,2028,153,41 }, { 1418,2112,153,41 }, { 1418,2197,153,41 }, NULL, this);
							

							//Images
							ghoul_lab[1] = (j1Image*)App->gui->CreateImage(900, 590, Image, { 536, 1741, 101, 73 }, NULL, this);
							ghoul_lab[2] = (j1Image*)App->gui->CreateImage(1060, 630, Image, { 877, 1874, 33, 20 }, NULL, this);
							
							ghoul_lab[5] = (j1Image*)App->gui->CreateImage(1060, 575, Image, { 982, 1869, 31, 33 }, NULL, this);
							ghoul_lab[7] = (j1Image*)App->gui->CreateImage(1060, 675, Image, { 1034, 1869, 35, 33 }, NULL, this);

							//Labels
							if (App->entities->units_creation_time[App->player->faction].upgrade_num < 4) {
								int cost = App->entities->units_creation_time[App->player->faction].first_price + App->entities->units_creation_time[App->player->faction].price_increment * App->entities->units_creation_time[App->player->faction].upgrade_num;
								ghoul_lab[3] = (UI_Label*)App->gui->CreateLabel(1120, 630, Label, std::to_string(cost * 2), NULL, this, NULL, "StackedPixelSmall");
							}
							else
								ghoul_lab[3] = (UI_Label*)App->gui->CreateLabel(1120, 630, Label, "MAX", NULL, this, NULL, "StackedPixelSmall");

							last_menu = current_menu;
							current_menu = Menu::BUI_BASES;

							break;
						default:
							break;
					}
					break;

				case FACTION::VAULT:
					switch (type)
					{
						case BUILDING_TYPE::BASE:

							//Buttons
							vault_base[0] = (UI_Button*)App->gui->CreateButton(1050, 585, Vault_ghaterer_button, { 1355,2028,53,52 }, { 1355,2112,53,52 }, { 1355,2196,53,52 }, NULL, this);
							vault_base[1] = (UI_Button*)App->gui->CreateButton(1050, 645, Boost_base_button, { 1355,2028,53,52 }, { 1355,2112,53,52 }, { 1355,2196,53,52 }, NULL, this);

							//Images
							vault_base[2] = (j1Image*)App->gui->CreateImage(900, 590, Image, { 319, 1710, 49, 113 }, NULL, this);
							vault_base[3] = (j1Image*)App->gui->CreateImage(1064, 595, Image, { 463, 1871, 25, 29 }, NULL, this);
							vault_base[4] = (j1Image*)App->gui->CreateImage(1055, 660, Image, { 926, 1872, 37, 24 }, NULL, this);

							//Labels
							if (App->entities->gatherer_resource_limit[App->player->faction].upgrade_num < 4) {
								int cost = App->entities->gatherer_resource_limit[App->player->faction].first_price + App->entities->gatherer_resource_limit[App->player->faction].price_increment * App->entities->gatherer_resource_limit[App->player->faction].upgrade_num;
								vault_base[5] = (UI_Label*)App->gui->CreateLabel(1120, 630, Label, std::to_string(cost * 2), NULL, this, NULL, "StackedPixelSmall");
							}
							else
								vault_base[5] = (UI_Label*)App->gui->CreateLabel(1120, 630, Label, "MAX", NULL, this, NULL, "StackedPixelSmall");

							last_menu = current_menu;
							current_menu = Menu::BUI_BASES;

							break;
						case BUILDING_TYPE::BARRACK:

							//Buttons
							vault_barrack[0] = (UI_Button*)App->gui->CreateButton(1050, 570, Vault_melee_button, { 1355,2028,53,52 }, { 1355,2112,53,52 }, { 1355,2196,53,52 }, NULL, this);
							vault_barrack[1] = (UI_Button*)App->gui->CreateButton(1115, 570, Vault_ranged_button, { 1355,2028,53,52 }, { 1355,2112,53,52 }, { 1355,2196,53,52 }, NULL, this);
							vault_barrack[2] = (UI_Button*)App->gui->CreateButton(1050, 620, Boost_barrack_button, { 1418,2028,153,41 }, { 1418,2112,153,41 }, { 1418,2197,153,41 }, NULL, this);

							//Images
							vault_barrack[3] = (j1Image*)App->gui->CreateImage(885, 570, Image, { 148, 1731, 122, 91 }, NULL, this);
							vault_barrack[4] = (j1Image*)App->gui->CreateImage(1055, 575, Image, { 406, 1866, 32, 34 }, NULL, this);
							vault_barrack[5] = (j1Image*)App->gui->CreateImage(1110, 575, Image, { 355, 1866, 30, 34 }, NULL, this);
							vault_barrack[6] = (j1Image*)App->gui->CreateImage(1060, 630, Image, { 823, 1872, 32, 22 }, NULL, this);


							//Labels
							if (App->entities->units_damage[App->player->faction].upgrade_num < 4) {
								int cost = App->entities->units_damage[App->player->faction].first_price + App->entities->units_damage[App->player->faction].price_increment * App->entities->units_damage[App->player->faction].upgrade_num;
								vault_barrack[7] = (UI_Label*)App->gui->CreateLabel(1120, 630, Label, std::to_string(cost * 2), NULL, this, NULL, "StackedPixelSmall");
								LOG("%i", cost);
							}
							else
								vault_barrack[7] = (UI_Label*)App->gui->CreateLabel(1120, 630, Label, "MAX", NULL, this, NULL, "StackedPixelSmall");

							last_menu = current_menu;
							current_menu = Menu::BUI_BASES;

							break;
						case BUILDING_TYPE::LAB:

							//Buttons
							vault_lab[0] = (UI_Button*)App->gui->CreateButton(1050, 620, Boost_lab_button, { 1418,2028,153,41 }, { 1418,2112,153,41 }, { 1418,2197,153,41 }, NULL, this);
							vault_lab[4] = (UI_Button*)App->gui->CreateButton(1050, 570, Boost_radar_button, { 1418,2028,153,41 }, { 1418,2112,153,41 }, { 1418,2197,153,41 }, NULL, this);
							vault_lab[6] = (UI_Button*)App->gui->CreateButton(1050, 670, summon_mrhandy_button, { 1418,2028,153,41 }, { 1418,2112,153,41 }, { 1418,2197,153,41 }, NULL, this);
							
							
							//Images
							vault_lab[1] = (j1Image*)App->gui->CreateImage(920, 570, Image, { 426, 1726, 72, 96 }, NULL, this);
							vault_lab[2] = (j1Image*)App->gui->CreateImage(1060, 630, Image, { 877, 1874, 33, 20 }, NULL, this);
							
							vault_lab[5] = (j1Image*)App->gui->CreateImage(1060, 575, Image, { 982, 1869, 31, 33 }, NULL, this);
							vault_lab[7] = (j1Image*)App->gui->CreateImage(1060, 675, Image, { 1034, 1869, 35, 33 }, NULL, this);

							//Labels
							if (App->entities->units_creation_time[App->player->faction].upgrade_num < 4) {
								int cost = App->entities->units_creation_time[App->player->faction].first_price + App->entities->units_creation_time[App->player->faction].price_increment * App->entities->units_creation_time[App->player->faction].upgrade_num;
								vault_lab[3] = (UI_Label*)App->gui->CreateLabel(1120, 630, Label, std::to_string(cost * 2), NULL, this, NULL, "StackedPixelSmall");
							}
							else
								vault_lab[3] = (UI_Label*)App->gui->CreateLabel(1120, 630, Label, "MAX", NULL, this, NULL, "StackedPixelSmall");
							
							last_menu = current_menu;
							current_menu = Menu::BUI_BASES;

							break;
						default:
							break;
					}
					break;

				case FACTION::NONE:
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
		break;
	case Menu::CREDITS:
		App->gui->DeleteArrayElements(credits_menu, 5);
		break;
	case Menu::SELECT_FACTION:
		App->gui->DeleteArrayElements(select_faction_menu, 10);
		break;
	case Menu::PAUSE_MENU:
		App->gui->DeleteArrayElements(pause_menu, 7);
		App->isPaused = false;
		break;
	case Menu::GUI:
		App->gui->DeleteArrayElements(gui_ingame, 4);
		App->gui->Delete_Element(timer_item);
		App->gui->Delete_Element(timer_minutes);
		break;
	case Menu::RESOURCES:
		App->gui->DeleteArrayElements(resources, 3);
		break;
	case Menu::TUTORIAL:
		App->gui->DeleteArrayElements(tutorial, 6);
		App->gui->Delete_Element(tutorial_screen);
		App->gui->Delete_Element(info_button2_element);

	default:
		break;
	}

	current_menu = last_menu;
}
void MenuManager::DestroyFaction(Menu menu, FACTION faction, BUILDING_TYPE type) {

	switch (menu)
	{

	case Menu::BUI_BASES:

		switch (faction)
		{
		case FACTION::ALL:
			switch (type)
			{

			case BUILDING_TYPE::ALL:
				App->gui->DeleteArrayElements(brotherhood_base, 6);
				App->gui->DeleteArrayElements(brotherhood_barrack, 8);
				App->gui->DeleteArrayElements(brotherhood_lab, 8);
				App->gui->DeleteArrayElements(supermutant_base, 6);
				App->gui->DeleteArrayElements(supermutant_barrack, 8);
				App->gui->DeleteArrayElements(supermutant_lab, 8);
				App->gui->DeleteArrayElements(ghoul_base, 6);
				App->gui->DeleteArrayElements(ghoul_barrack, 8);
				App->gui->DeleteArrayElements(ghoul_lab, 8);
				App->gui->DeleteArrayElements(vault_base, 6);
				App->gui->DeleteArrayElements(vault_barrack, 8);
				App->gui->DeleteArrayElements(vault_lab, 8);
				break;
			case BUILDING_TYPE::NONE:
				break;
			default:
				break;
			}
			break;

		case FACTION::BROTHERHOOD:
			switch (type)
			{
			case BUILDING_TYPE::BASE:
				App->gui->DeleteArrayElements(brotherhood_base, 6);
				break;
			case BUILDING_TYPE::BARRACK:
				App->gui->DeleteArrayElements(brotherhood_barrack, 8);
				break;
			case BUILDING_TYPE::LAB:
				App->gui->DeleteArrayElements(brotherhood_lab, 8);
				break;
			case BUILDING_TYPE::ALL:
				break;
			case BUILDING_TYPE::NONE:
				App->gui->Delete_Element(select_faction_photos[0]);
				App->gui->Delete_Element(select_faction_photos[1]);
				App->gui->Delete_Element(select_faction_photos[3]);
				break;
			default:
				break;
			}
			break;

		case FACTION::SUPERMUTANT:
			switch (type)
			{

			case BUILDING_TYPE::BASE:
				App->gui->DeleteArrayElements(supermutant_base, 6);
				break;
			case BUILDING_TYPE::BARRACK:
				App->gui->DeleteArrayElements(supermutant_barrack, 8);
				break;
			case BUILDING_TYPE::LAB:
				App->gui->DeleteArrayElements(supermutant_lab, 8);
				break;
			case BUILDING_TYPE::NONE:
				App->gui->Delete_Element(select_faction_photos[0]);
				App->gui->Delete_Element(select_faction_photos[1]);
				App->gui->Delete_Element(select_faction_photos[2]);
				break;
			default:
				break;
			}
			break;

		case FACTION::GHOUL:
			switch (type)
			{

			case BUILDING_TYPE::BASE:
				App->gui->DeleteArrayElements(ghoul_base, 6);
				break;
			case BUILDING_TYPE::BARRACK:
				App->gui->DeleteArrayElements(ghoul_barrack, 8);
				break;
			case BUILDING_TYPE::LAB:
				App->gui->DeleteArrayElements(ghoul_lab, 8);
				break;
			case BUILDING_TYPE::NONE:
				App->gui->Delete_Element(select_faction_photos[1]);
				App->gui->Delete_Element(select_faction_photos[2]);
				App->gui->Delete_Element(select_faction_photos[3]);
				break;
			default:
				break;
			}
			break;

		case FACTION::VAULT:
			switch (type)
			{
			case BUILDING_TYPE::BASE:
				App->gui->DeleteArrayElements(vault_base, 6);
				break;
			case BUILDING_TYPE::BARRACK:
				App->gui->DeleteArrayElements(vault_barrack, 8);
				break;
			case BUILDING_TYPE::LAB:
				App->gui->DeleteArrayElements(vault_lab, 8);
				break;
			case BUILDING_TYPE::NONE:
				App->gui->Delete_Element(select_faction_photos[0]);
				App->gui->Delete_Element(select_faction_photos[2]);
				App->gui->Delete_Element(select_faction_photos[3]);
				break;
			default:
				break;
			}
			break;

		case FACTION::NONE:
			break;

		default:
			break;

		}
		break;

	default:
		break;
	}

	current_menu = last_menu;
}