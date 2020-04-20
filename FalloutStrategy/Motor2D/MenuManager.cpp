#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
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

MenuManager::MenuManager()
{
	name = ("menu_manager");
	current_menu = last_menu = Menu::NO_MENU;
}

MenuManager::~MenuManager()
{
}

void MenuManager::CreateMainMenu()
{
	
	App->player->Disable();

	//Images															
	main_menu[0] = (j1Image*)App->gui->CreateImage(-5, 0, Image, { 1529, 0, 1502, 775 }, NULL, this);
	main_menu[1] = (j1Image*)App->gui->CreateImage(0, 350, Image, { 0, 1016, 871, 516 }, NULL, this);
	main_menu[2] = (j1Image*)App->gui->CreateImage(40, 40, Image, { 938, 903, 505, 233 }, NULL, this);
	main_menu[3] = (j1Image*)App->gui->CreateImage(827, 260, Image, { 0, 0, 416, 448 }, NULL, this);

	//Labels
	main_menu[4] = (UI_Label*)App->gui->CreateLabel(957, 310, Label, "NEW GAME", NULL, this, NULL);
	main_menu[5] = (UI_Label*)App->gui->CreateLabel(957, 387, Label, "LOAD GAME", NULL, this, NULL);
	main_menu[6] = (UI_Label*)App->gui->CreateLabel(957, 464, Label, "SETTINGS", NULL, this, NULL);
	main_menu[7] = (UI_Label*)App->gui->CreateLabel(957, 541, Label, "CREDITS", NULL, this, NULL);
	main_menu[8] = (UI_Label*)App->gui->CreateLabel(957, 618, Label, "EXIT", NULL, this, NULL);

	//Buttons
	main_menu[9] = (UI_Button*)App->gui->CreateButton(887, 295, button_new_game, { 1596,834,60,62 }, { 1596,901,60,62 }, { 1596,967,60,62 }, NULL, this);
	main_menu[10] = (UI_Button*)App->gui->CreateButton(887, 373, button_load_game, { 1596,834,60,62 }, { 1596,901,60,62 }, { 1596,967,60,62 }, NULL, this);
	main_menu[11] = (UI_Button*)App->gui->CreateButton(887, 450, button_settings, { 1596,834,60,62 }, { 1596,901,60,62 }, { 1596,967,60,62 }, NULL, this);
	main_menu[12] = (UI_Button*)App->gui->CreateButton(887, 527, button_credits, { 1596,834,60,62 }, { 1596,901,60,62 }, { 1596,967,60,62 }, NULL, this);
	main_menu[13] = (UI_Button*)App->gui->CreateButton(887, 604, button_exit, { 1596,834,60,62 }, { 1596,901,60,62 }, { 1596,967,60,62 }, NULL, this);

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	current_menu = Menu::MAIN_MENU;
}

void MenuManager::CreateCredits()
{
	//Images
	credits_menu[0] = (j1Image*)App->gui->CreateImage(0, 0, Image, { 1287, 2430, 1281, 730 }, NULL, this);

	//Buttons
	credits_menu[1] = (UI_Button*)App->gui->CreateButton(65, 65, button_back_credits, { 1243,2014,73,78 }, { 1243,2102,73,79 }, { 1243,2188,73,71 }, NULL, this);
	credits_menu[2] = (UI_Button*)App->gui->CreateButton(750, 600, button_github_credits, { 2181,841,73,78 }, { 2181,919,73,78 }, { 2181,1005,73,78 }, NULL, this);
	credits_menu[3] = (UI_Button*)App->gui->CreateButton(850, 600, button_web_credits, { 2268,841,73,78 }, { 2268,919,73,78 }, { 2268,1005,73,78 }, NULL, this);
	credits_menu[4] = (UI_Button*)App->gui->CreateButton(950, 600, button_twitter_credits, { 2355,841,73,78 }, { 2355,919,73,78 }, { 2355,1005,73,78 }, NULL, this);
	credits_menu[5] = (UI_Button*)App->gui->CreateButton(450, 130, button_marc, { 0,455,166,17 }, { 0,455,166,17 }, { 0,455,166,17 }, NULL, this);
	credits_menu[6] = (UI_Button*)App->gui->CreateButton(450, 175, button_javi, { 0,455,166,17 }, { 0,455,166,17 }, { 0,455,166,17 }, NULL, this);
	credits_menu[7] = (UI_Button*)App->gui->CreateButton(450, 220, button_pablo, { 0,455,166,17 }, { 0,455,166,17 }, { 0,455,166,17 }, NULL, this);
	credits_menu[8] = (UI_Button*)App->gui->CreateButton(450, 265, button_german, { 0,455,166,17 }, { 0,455,166,17 }, { 0,455,166,17 }, NULL, this);
	credits_menu[9] = (UI_Button*)App->gui->CreateButton(450, 310, button_macia, { 0,455,166,17 }, { 0,455,166,17 }, { 0,455,166,17 }, NULL, this);
	credits_menu[10] = (UI_Button*)App->gui->CreateButton(450, 355, button_pol, { 0,455,166,17 }, { 0,455,166,17 }, { 0,455,166,17 }, NULL, this);
	credits_menu[11] = (UI_Button*)App->gui->CreateButton(450, 400, button_silvino, { 0,455,166,17 }, { 0,455,166,17 }, { 0,455,166,17 }, NULL, this);
	credits_menu[12] = (UI_Button*)App->gui->CreateButton(450, 445, button_christian, { 0,455,166,17 }, { 0,455,166,17 }, { 0,455,166,17 }, NULL, this);

	current_menu = Menu::CREDITS;

}

void MenuManager::CreateSettings()
{

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
	settings_menu[17] = (UI_Button*)App->gui->CreateButton(40, 40, button_back, { 1704,1054,48,46 }, { 1765,1054,48,46 }, { 1765,1054,48,46 }, NULL, this);
	settings_menu[20] = (UI_Button*)App->gui->CreateButton(705, 615, button_fullscreen, { 1671,922,25,26 }, { 1713,922,25,26 }, { 1713,922,25,26 }, NULL, this);
	last_menu = current_menu;
	current_menu = Menu::SETTINGS;
}

void MenuManager::CreateCollaboratorPicture()
{
	switch (collaborator)
	{
	case 'M':
		collaborators[0] = (j1Image*)App->gui->CreateImage(815, 130, Image, { 1230, 1545, 138, 138 }, NULL, this);
		break;
	case 'J':
		collaborators[1] = (j1Image*)App->gui->CreateImage(815, 130, Image, { 1392, 1545, 138, 138 }, NULL, this);
		break;
	case 'P':
		collaborators[2] = (j1Image*)App->gui->CreateImage(815, 130, Image, { 1563, 1545, 138, 138 }, NULL, this);
		break;
	case 'G':
		collaborators[3] = (j1Image*)App->gui->CreateImage(815, 130, Image, { 1719, 1539, 138, 138 }, NULL, this);
		break;
	case 'D':
		collaborators[4] = (j1Image*)App->gui->CreateImage(815, 130, Image, { 1230, 1690, 138, 138 }, NULL, this);
		break;
	case 'K':
		collaborators[5] = (j1Image*)App->gui->CreateImage(815, 130, Image, { 1392, 1690, 138, 138 }, NULL, this);
		break;
	case 'S':
		collaborators[6] = (j1Image*)App->gui->CreateImage(815, 130, Image, { 1563, 1690, 138, 138 }, NULL, this);
		break;
	case 'C':
		collaborators[7] = (j1Image*)App->gui->CreateImage(815, 130, Image, { 1719, 1690, 138, 138 }, NULL, this);
		break;
	default:
		collaborators[8] = (j1Image*)App->gui->CreateImage(815, 130, Image, { 1230, 1545, 138, 138 }, NULL, this);
	}

	last_menu = current_menu;
	current_menu = Menu::COLLABORATORS_CREDITS;

}

void MenuManager::CreateSelectFaction()
{
	//Images
	select_faction_menu[0] = (j1Image*)App->gui->CreateImage(-5, 0, Image, { 1900, 1531, 1290, 842 }, NULL, this);

	//Buttons
	select_faction_menu[1] = (UI_Button*)App->gui->CreateButton(200, 200, button_select_ghoul, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,244,64 }, NULL, this);
	select_faction_menu[2] = (UI_Button*)App->gui->CreateButton(200, 300, button_select_vault, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,244,64 }, NULL, this);
	select_faction_menu[3] = (UI_Button*)App->gui->CreateButton(200, 400, button_select_brotherhood, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,244,64 }, NULL, this);
	select_faction_menu[4] = (UI_Button*)App->gui->CreateButton(200, 500, button_select_supermutant, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,244,64 }, NULL, this);
	select_faction_menu[5] = (UI_Button*)App->gui->CreateButton(120, 605, button_back_to_menu, { 1243,2014,73,78 }, { 1243,2102,73,79 }, { 1243,2188,73,71 }, NULL, this);
	//select_faction_menu[6] = (UI_Button*)App->gui->CreateButton(900, 605, button_start_game, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,244,64 }, NULL, this);

	//Labels
	select_faction_menu[6] = (UI_Label*)App->gui->CreateLabel(280, 215, Label, "GHOUL", NULL, this, NULL);
	select_faction_menu[7] = (UI_Label*)App->gui->CreateLabel(278, 315, Label, "VAULT", NULL, this, NULL);
	select_faction_menu[8] = (UI_Label*)App->gui->CreateLabel(220, 415, Label, "BROTHERHOOD", NULL, this, NULL);
	select_faction_menu[9] = (UI_Label*)App->gui->CreateLabel(214, 515, Label, "SUPERMUTANT", NULL, this, NULL);
	//select_faction_menu[10] = (UI_Label*)App->gui->CreateLabel(925, 620, Label, "START GAME", NULL, this, NULL);

	last_menu = current_menu;
	current_menu = Menu::SELECT_FACTION;
}

void MenuManager::CreatePauseMenu()
{

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

	current_menu = Menu::PAUSE_MENU;
}

void MenuManager::CreatePauseSettings()
{

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
		settings_menu[22] = (UI_Label*)App->gui->CreateLabel(763, 613, Label, "YES", NULL, this, NULL, "StackedPixelMedium");
	}
	else{
		settings_menu[22] = (UI_Label*)App->gui->CreateLabel(763, 613, Label, "NO", NULL, this, NULL, "StackedPixelMedium");
	}

	if (App->gui->cap == true) {
		settings_menu[21] = (UI_Label*)App->gui->CreateLabel(528, 613, Label, "30", NULL, this, NULL, "StackedPixelMedium");
	}
	else{
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
	current_menu = Menu::SETTINGS;
}

void MenuManager::CreateGUI()
{

	//Images
	gui_ingame[0] = (j1Image*)App->gui->CreateImage(0, 0, Image, { 0, 2428, 1290, 730 }, NULL, this);

	//Labels
	//settings_menu.fps = (UI_Label*)App->gui->CreateLabel(520, 525, text_fps, "REFRESH RATE", NULL, this, NULL);
	//settings_menu.fx_volume = (UI_Label*)App->gui->CreateLabel(360, 335, text_fx_volume, "SOUND EFFECTS VOLUME", NULL, this, NULL);
	//settings_menu.music_volume = (UI_Label*)App->gui->CreateLabel(360, 150, text_music_volume, "MUSIC VOLUME", NULL, this, NULL);
	last_menu = current_menu;
	current_menu = Menu::GUI;
}

void MenuManager::CreateResources() {

	std::string water = std::to_string(App->player->water);
	std::string food = std::to_string(App->player->food);
	std::string caps = std::to_string(App->player->caps);

	resources[0] = (UI_Label*)App->gui->CreateLabel(75, 15, Label, food, NULL, this, NULL, "StackedPixelSmall");
	resources[1] = (UI_Label*)App->gui->CreateLabel(220, 15, Label, water, NULL, this, NULL, "StackedPixelSmall");
	resources[2] = (UI_Label*)App->gui->CreateLabel(365, 15, Label, caps, NULL, this, NULL, "StackedPixelSmall");

}

void MenuManager::CreateBrotherHood_Base()
{

	//Buttons
	brotherhood_base[0] = (UI_Button*)App->gui->CreateButton(1050, 570, Brotherhood_ghaterer_button, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);
	brotherhood_base[1] = (UI_Button*)App->gui->CreateButton(1050, 620, Boost_button, { 1418,2028,153,41 }, { 1418,2112,153,41 }, { 1418,2197,153,41 }, NULL, this);

	//Images
	brotherhood_base[2] = (j1Image*)App->gui->CreateImage(880, 575, Image, { 118, 1561, 122, 98 }, NULL, this);
	brotherhood_base[3] = (j1Image*)App->gui->CreateImage(1055, 575, Image, { 138, 1868, 33, 33 }, NULL, this);
	brotherhood_base[4] = (j1Image*)App->gui->CreateImage(1060, 628, Image, { 926, 1872, 37, 24 }, NULL, this);

	//Labels
	//p2SString hh("%i", App->entities->); VIDA EDIFICIO
	//gui_ingame.meat_count = (UI_Label*)App->gui->CreateLabel(95, 10, Meat, hh, NULL, this, NULL);
	//gui_ingame.meat_count = (UI_Label*)App->gui->CreateLabel(95, 10, Meat, "Brotherhood Base", NULL, this, NULL);

	last_menu = current_menu;
	current_menu = Menu::BUI_BASES;

}

void MenuManager::CreateBrotherHood_Barrack() {

	//Buttons
	brotherhood_barrack[0] = (UI_Button*)App->gui->CreateButton(1050, 570, Brotherhood_melee_button, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);
	brotherhood_barrack[1] = (UI_Button*)App->gui->CreateButton(1105, 570, Brotherhood_ranged_button, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);
	brotherhood_barrack[2] = (UI_Button*)App->gui->CreateButton(1050, 620, Boost_button, { 1418,2028,153,41 }, { 1418,2112,153,41 }, { 1418,2197,153,41 }, NULL, this);

	//Images
	brotherhood_barrack[3] = (j1Image*)App->gui->CreateImage(885, 570, Image, { 17, 1560, 69, 115 }, NULL, this);
	brotherhood_barrack[4] = (j1Image*)App->gui->CreateImage(1055, 575, Image, { 28, 1869, 35, 33 }, NULL, this);
	brotherhood_barrack[5] = (j1Image*)App->gui->CreateImage(1110, 574, Image, { 83, 1866, 35, 36 }, NULL, this);
	brotherhood_barrack[6] = (j1Image*)App->gui->CreateImage(1060, 630, Image, { 823, 1872, 32, 22 }, NULL, this);

	last_menu = current_menu;
	current_menu = Menu::BUI_BASES;
}

void MenuManager::CreateBrotherHood_Lab() {

	//Buttons
	//bases_hud.lab_create_brotherhood = (UI_Button*)App->gui->CreateButton(1050, 600, Brotherhood_melee, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);
	//bases_hud.lab_create_brotherhood = (UI_Button*)App->gui->CreateButton(1105, 600, Brotherhood_ranged, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);
	brotherhood_lab[0] = (UI_Button*)App->gui->CreateButton(1050, 620, Boost_button, { 1418,2028,153,41 }, { 1418,2112,153,41 }, { 1418,2197,153,41 }, NULL, this);

	//Images
	brotherhood_lab[1] = (j1Image*)App->gui->CreateImage(885, 570, Image, { 663, 1567, 108, 104 }, NULL, this);
	brotherhood_lab[2] = (j1Image*)App->gui->CreateImage(1060, 630, Image, { 877, 1874, 33, 20 }, NULL, this);
	//bases_hud.brotherhood_melee = (j1Image*)App->gui->CreateImage(1055, 605, Brotherhood_melee, { 28, 1869, 35, 33 }, NULL, this);
	//bases_hud.brotherhood_ranged = (j1Image*)App->gui->CreateImage(1110, 605, Brotherhood_ranged, { 83, 1866, 35, 36 }, NULL, this);
	//bases_hud.brotherhood_melee = (j1Image*)App->gui->CreateImage(1055, 605, Brotherhood_melee, { 28, 1869, 35, 33 }, NULL, this);
	//bases_hud.brotherhood_ranged = (j1Image*)App->gui->CreateImage(1110, 605, Brotherhood_ranged, { 83, 1866, 35, 36 }, NULL, this);
	//bases_hud.brotherhood_melee = (j1Image*)App->gui->CreateImage(1055, 605, Brotherhood_melee, { 28, 1869, 35, 33 }, NULL, this);
	last_menu = current_menu;
	current_menu = Menu::BUI_BASES;
}


//Supermutants

void MenuManager::CreateSuperMutants_Base() {

	//Buttons
	supermutant_base[0] = (UI_Button*)App->gui->CreateButton(1050, 570, Supermutant_ghaterer_button, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);
	supermutant_base[1] = (UI_Button*)App->gui->CreateButton(1050, 620, Boost_button, { 1418,2028,153,41 }, { 1418,2112,153,41 }, { 1418,2197,153,41 }, NULL, this);

	//Images
	supermutant_base[2] = (j1Image*)App->gui->CreateImage(880, 575, Image, { 934, 1577, 129, 91 }, NULL, this);
	supermutant_base[3] = (j1Image*)App->gui->CreateImage(1055, 575, Image, { 191, 1868, 34, 32 }, NULL, this);
	supermutant_base[4] = (j1Image*)App->gui->CreateImage(1060, 628, Image, { 926, 1872, 37, 24 }, NULL, this);

	last_menu = current_menu;
	current_menu = Menu::BUI_BASES;

}
void MenuManager::CreateSuperMutants_Barrack() {

	//Buttons
	supermutant_barrack[0] = (UI_Button*)App->gui->CreateButton(1050, 570, Supermutant_melee_button, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);
	supermutant_barrack[1] = (UI_Button*)App->gui->CreateButton(1105, 570, Supermutant_ranged_button, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);
	supermutant_barrack[2] = (UI_Button*)App->gui->CreateButton(1050, 620, Boost_button, { 1418,2028,153,41 }, { 1418,2112,153,41 }, { 1418,2197,153,41 }, NULL, this);

	//Images
	supermutant_barrack[3] = (j1Image*)App->gui->CreateImage(890, 560, Image, { 801, 1549, 102, 119 }, NULL, this);
	supermutant_barrack[4] = (j1Image*)App->gui->CreateImage(1055, 575, Image, { 246, 1869, 34, 33 }, NULL, this);
	supermutant_barrack[5] = (j1Image*)App->gui->CreateImage(1110, 573, Image, { 301, 1865, 34, 35 }, NULL, this);
	supermutant_barrack[6] = (j1Image*)App->gui->CreateImage(1060, 630, Image, { 823, 1872, 32, 22 }, NULL, this);

	last_menu = current_menu;
	current_menu = Menu::BUI_BASES;

}
void MenuManager::CreateSuperMutants_Lab() {

	//Buttons
	//bases_hud.lab_create_brotherhood = (UI_Button*)App->gui->CreateButton(1050, 600, Brotherhood_melee, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);
	//bases_hud.lab_create_brotherhood = (UI_Button*)App->gui->CreateButton(1105, 600, Brotherhood_ranged, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);
	supermutant_lab[0] = (UI_Button*)App->gui->CreateButton(1050, 620, Boost_button, { 1418,2028,153,41 }, { 1418,2112,153,41 }, { 1418,2197,153,41 }, NULL, this);

	//Images
	supermutant_lab[1] = (j1Image*)App->gui->CreateImage(885, 570, Image, { 663, 1567, 108, 104 }, NULL, this);
	supermutant_lab[2] = (j1Image*)App->gui->CreateImage(1060, 630, Image, { 877, 1874, 33, 20 }, NULL, this);
	//bases_hud.brotherhood_melee = (j1Image*)App->gui->CreateImage(1055, 605, Brotherhood_melee, { 28, 1869, 35, 33 }, NULL, this);
	//bases_hud.brotherhood_ranged = (j1Image*)App->gui->CreateImage(1110, 605, Brotherhood_ranged, { 83, 1866, 35, 36 }, NULL, this);
	//bases_hud.brotherhood_melee = (j1Image*)App->gui->CreateImage(1055, 605, Brotherhood_melee, { 28, 1869, 35, 33 }, NULL, this);
	//bases_hud.brotherhood_ranged = (j1Image*)App->gui->CreateImage(1110, 605, Brotherhood_ranged, { 83, 1866, 35, 36 }, NULL, this);
	//bases_hud.brotherhood_melee = (j1Image*)App->gui->CreateImage(1055, 605, Brotherhood_melee, { 28, 1869, 35, 33 }, NULL, this);

	last_menu = current_menu;
	current_menu = Menu::BUI_BASES;

}

//Ghouls

void MenuManager::CreateGhouls_Base() {

	//Buttons
	ghoul_base[0] = (UI_Button*)App->gui->CreateButton(1050, 570, Ghouls_ghaterer_button, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);
	ghoul_base[1] = (UI_Button*)App->gui->CreateButton(1050, 620, Boost_button, { 1418,2028,153,41 }, { 1418,2112,153,41 }, { 1418,2197,153,41 }, NULL, this);

	//Images
	ghoul_base[2] = (j1Image*)App->gui->CreateImage(900, 575, Image, { 398, 1564, 99, 108 }, NULL, this);
	ghoul_base[3] = (j1Image*)App->gui->CreateImage(1060, 575, Image, { 624, 1866, 24, 34 }, NULL, this);
	ghoul_base[4] = (j1Image*)App->gui->CreateImage(1060, 628, Image, { 926, 1872, 37, 24 }, NULL, this);

	last_menu = current_menu;
	current_menu = Menu::BUI_BASES;

}
void MenuManager::CreateGhouls_Barrack() {

	//Buttons
	ghoul_barrack[0] = (UI_Button*)App->gui->CreateButton(1050, 570, Ghouls_melee_button, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);
	ghoul_barrack[1] = (UI_Button*)App->gui->CreateButton(1105, 570, Ghouls_ranged_button, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);
	ghoul_barrack[2] = (UI_Button*)App->gui->CreateButton(1050, 620, Boost_button, { 1418,2028,153,41 }, { 1418,2112,153,41 }, { 1418,2197,153,41 }, NULL, this);

	//Images
	ghoul_barrack[3] = (j1Image*)App->gui->CreateImage(900, 570, Image, { 527, 1563, 105, 105 }, NULL, this);
	ghoul_barrack[4] = (j1Image*)App->gui->CreateImage(1055, 575, Image, { 513, 1865, 33, 34 }, NULL, this);
	ghoul_barrack[5] = (j1Image*)App->gui->CreateImage(1110, 575, Image, { 571, 1866, 28, 33 }, NULL, this);
	ghoul_barrack[6] = (j1Image*)App->gui->CreateImage(1060, 630, Image, { 823, 1872, 32, 22 }, NULL, this);

	last_menu = current_menu;
	current_menu = Menu::BUI_BASES;

}
void MenuManager::CreateGhouls_Lab() {

	//Buttons
	//bases_hud.lab_create_brotherhood = (UI_Button*)App->gui->CreateButton(1050, 600, Brotherhood_melee, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);
	//bases_hud.lab_create_brotherhood = (UI_Button*)App->gui->CreateButton(1105, 600, Brotherhood_ranged, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);
	ghoul_lab[0] = (UI_Button*)App->gui->CreateButton(1050, 620, Boost_button, { 1418,2028,153,41 }, { 1418,2112,153,41 }, { 1418,2197,153,41 }, NULL, this);

	//Images
	ghoul_lab[1] = (j1Image*)App->gui->CreateImage(900, 590, Image, { 536, 1741, 101, 73 }, NULL, this);
	ghoul_lab[2] = (j1Image*)App->gui->CreateImage(1060, 630, Image, { 877, 1874, 33, 20 }, NULL, this);
	//bases_hud.brotherhood_melee = (j1Image*)App->gui->CreateImage(1055, 605, Brotherhood_melee, { 28, 1869, 35, 33 }, NULL, this);
	//bases_hud.brotherhood_ranged = (j1Image*)App->gui->CreateImage(1110, 605, Brotherhood_ranged, { 83, 1866, 35, 36 }, NULL, this);
	//bases_hud.brotherhood_melee = (j1Image*)App->gui->CreateImage(1055, 605, Brotherhood_melee, { 28, 1869, 35, 33 }, NULL, this);
	//bases_hud.brotherhood_ranged = (j1Image*)App->gui->CreateImage(1110, 605, Brotherhood_ranged, { 83, 1866, 35, 36 }, NULL, this);
	//bases_hud.brotherhood_melee = (j1Image*)App->gui->CreateImage(1055, 605, Brotherhood_melee, { 28, 1869, 35, 33 }, NULL, this);

	last_menu = current_menu;
	current_menu = Menu::BUI_BASES;

}


//Vault
void MenuManager::CreateVault_Base() {

	//Buttons
	vault_base[0] = (UI_Button*)App->gui->CreateButton(1050, 570, Vault_ghaterer_button, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);
	vault_base[1] = (UI_Button*)App->gui->CreateButton(1050, 620, Boost_button, { 1418,2028,153,41 }, { 1418,2112,153,41 }, { 1418,2197,153,41 }, NULL, this);
	
	//Images
	vault_base[2] = (j1Image*)App->gui->CreateImage(930, 569, Image, { 319, 1710, 49, 113 }, NULL, this);
	vault_base[3] = (j1Image*)App->gui->CreateImage(1058, 579, Image, { 463, 1871, 25, 29 }, NULL, this);
	vault_base[4] = (j1Image*)App->gui->CreateImage(1060, 628, Image, { 926, 1872, 37, 24 }, NULL, this);

	last_menu = current_menu;
	current_menu = Menu::BUI_BASES;

}
void MenuManager::CreateVault_Barrack() {

	//Buttons
	vault_barrack[0] = (UI_Button*)App->gui->CreateButton(1050, 570, Vault_melee_button, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);
	vault_barrack[1] = (UI_Button*)App->gui->CreateButton(1105, 570, Vault_ranged_button, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);
	vault_barrack[2] = (UI_Button*)App->gui->CreateButton(1050, 620, Boost_button, { 1418,2028,153,41 }, { 1418,2112,153,41 }, { 1418,2197,153,41 }, NULL, this);

	//Images
	vault_barrack[3] = (j1Image*)App->gui->CreateImage(885, 570, Image, { 148, 1731, 122, 91 }, NULL, this);
	vault_barrack[4] = (j1Image*)App->gui->CreateImage(1055, 575, Image, { 406, 1866, 32, 34 }, NULL, this);
	vault_barrack[5] = (j1Image*)App->gui->CreateImage(1110, 575, Image, { 355, 1866, 30, 34 }, NULL, this);
	vault_barrack[6] = (j1Image*)App->gui->CreateImage(1060, 630, Image, { 823, 1872, 32, 22 }, NULL, this);

	last_menu = current_menu;
	current_menu = Menu::BUI_BASES;

}

void MenuManager::CreateVault_Lab() {

	//Buttons
	//bases_hud.lab_create_brotherhood = (UI_Button*)App->gui->CreateButton(1050, 600, Brotherhood_melee, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);
	//bases_hud.lab_create_brotherhood = (UI_Button*)App->gui->CreateButton(1105, 600, Brotherhood_ranged, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);
	vault_lab[0] = (UI_Button*)App->gui->CreateButton(1050, 620, Boost_button, { 1418,2028,153,41 }, { 1418,2112,153,41 }, { 1418,2197,153,41 }, NULL, this);

	//Images
	vault_lab[1] = (j1Image*)App->gui->CreateImage(920, 570, Image, { 426, 1726, 72, 96 }, NULL, this);
	vault_lab[2] = (j1Image*)App->gui->CreateImage(1060, 630, Image, { 877, 1874, 33, 20 }, NULL, this);
	//bases_hud.brotherhood_melee = (j1Image*)App->gui->CreateImage(1055, 605, Brotherhood_melee, { 28, 1869, 35, 33 }, NULL, this);
	//bases_hud.brotherhood_ranged = (j1Image*)App->gui->CreateImage(1110, 605, Brotherhood_ranged, { 83, 1866, 35, 36 }, NULL, this);
	//bases_hud.brotherhood_melee = (j1Image*)App->gui->CreateImage(1055, 605, Brotherhood_melee, { 28, 1869, 35, 33 }, NULL, this);
	//bases_hud.brotherhood_ranged = (j1Image*)App->gui->CreateImage(1110, 605, Brotherhood_ranged, { 83, 1866, 35, 36 }, NULL, this);
	//bases_hud.brotherhood_melee = (j1Image*)App->gui->CreateImage(1055, 605, Brotherhood_melee, { 28, 1869, 35, 33 }, NULL, this);

	last_menu = current_menu;
	current_menu = Menu::BUI_BASES;

}

void MenuManager::CreateWinLoseScene() {

	//Images+
	if(App->scene->win)
	{
		win_scene[0] = (j1Image*)App->gui->CreateImage(0, 0, Image, { 0, 3190, 1280, 720 }, NULL, this);
	}
	else
	{
		win_scene[0] = (j1Image*)App->gui->CreateImage(0, 0, Image, { 1281, 3190, 1280, 720 }, NULL, this);
	}

	//Buttons
	win_scene[1] = (UI_Button*)App->gui->CreateButton(108, 260, button_exit, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,244,64 }, NULL, this);
	win_scene[2] = (UI_Button*)App->gui->CreateButton(108, 350, button_win_lose_to_main, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,244,64 }, NULL, this);

	//Labels
	win_scene[3] = (UI_Label*)App->gui->CreateLabel(195, 278, Label, "EXIT", NULL, this, NULL);
	win_scene[4] = (UI_Label*)App->gui->CreateLabel(140, 368, Label, "MAIN MENU", NULL, this, NULL);
	
	last_menu = current_menu;
	current_menu = Menu::WIN_LOSE_SCENE;

}

void MenuManager::DestroyMenu(Menu menu) {

	switch (menu)
	{

	case Menu::MAIN_MENU:
		App->gui->DeleteArrayElements(main_menu, 14);
		break;
	case Menu::SETTINGS:
		App->gui->DeleteArrayElements(settings_menu, 23);
		break;
	case Menu::CREDITS:
		App->gui->DeleteArrayElements(credits_menu, 13);
		break;
	case Menu::COLLABORATORS_CREDITS:
		App->gui->DeleteArrayElements(collaborators, 9);
		break;
	case Menu::SELECT_FACTION:
		App->gui->DeleteArrayElements(select_faction_menu, 10);
		break;
	case Menu::PAUSE_MENU:
		App->gui->DeleteArrayElements(pause_menu, 7);
		App->isPaused = false;
		break;
	case Menu::GUI:
		App->gui->DeleteArrayElements(gui_ingame, 1);
		break;
	case Menu::RESOURCES:
		App->gui->DeleteArrayElements(resources, 3);
		break;
	case Menu::WIN_LOSE_SCENE:
		App->gui->DeleteArrayElements(win_scene, 5);
		

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
				App->gui->DeleteArrayElements(brotherhood_base, 5);
				App->gui->DeleteArrayElements(brotherhood_barrack, 7);
				App->gui->DeleteArrayElements(brotherhood_lab, 3);
				App->gui->DeleteArrayElements(supermutant_base, 5);
				App->gui->DeleteArrayElements(supermutant_barrack, 7);
				App->gui->DeleteArrayElements(supermutant_lab, 3);
				App->gui->DeleteArrayElements(ghoul_base, 5);
				App->gui->DeleteArrayElements(ghoul_barrack, 7);
				App->gui->DeleteArrayElements(ghoul_lab, 3);
				App->gui->DeleteArrayElements(vault_base, 5);
				App->gui->DeleteArrayElements(vault_barrack, 7);
				App->gui->DeleteArrayElements(vault_lab, 3);
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
				App->gui->DeleteArrayElements(brotherhood_base, 5);
				break;
			case BUILDING_TYPE::BARRACK:
				App->gui->DeleteArrayElements(brotherhood_barrack, 7);
				break;
			case BUILDING_TYPE::LAB:
				App->gui->DeleteArrayElements(brotherhood_lab, 3);
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
				App->gui->DeleteArrayElements(supermutant_base, 5);
				break;
			case BUILDING_TYPE::BARRACK:
				App->gui->DeleteArrayElements(supermutant_barrack, 7);
				break;
			case BUILDING_TYPE::LAB:
				App->gui->DeleteArrayElements(supermutant_lab, 3);
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
				App->gui->DeleteArrayElements(ghoul_base, 5);
				break;
			case BUILDING_TYPE::BARRACK:
				App->gui->DeleteArrayElements(ghoul_barrack, 7);
				break;
			case BUILDING_TYPE::LAB:
				App->gui->DeleteArrayElements(ghoul_lab, 3);
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
				App->gui->DeleteArrayElements(vault_base, 5);
				break;
			case BUILDING_TYPE::BARRACK:
				App->gui->DeleteArrayElements(vault_barrack, 7);
				break;
			case BUILDING_TYPE::LAB:
				App->gui->DeleteArrayElements(vault_lab, 3);
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