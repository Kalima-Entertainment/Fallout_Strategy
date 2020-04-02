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
#include "Player.h"
#include "p2Defs.h"
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
	main_menu[1] = (j1Image*)App->gui->CreateImage(0, 350, Image1, { 0, 1016, 871, 516 }, NULL, this);
	main_menu[2] = (j1Image*)App->gui->CreateImage(40, 40, Logo, { 938, 903, 505, 233 }, NULL, this);
	main_menu[3] = (j1Image*)App->gui->CreateImage(827, 260, Panel, { 0, 0, 416, 448 }, NULL, this);
	
	//Labels
	main_menu[4] = (UI_Label*)App->gui->CreateLabel(957,310, text_new_game, "NEW GAME", NULL, this, NULL);
	main_menu[5] = (UI_Label*)App->gui->CreateLabel(957, 387, text_load_game, "LOAD GAME", NULL, this, NULL);
	main_menu[6] = (UI_Label*)App->gui->CreateLabel(957, 464, text_settings, "SETTINGS", NULL, this, NULL);
	main_menu[7] = (UI_Label*)App->gui->CreateLabel(957, 541, text_credits, "CREDITS", NULL, this, NULL);
	main_menu[8] = (UI_Label*)App->gui->CreateLabel(957, 618, text_exit, "EXIT", NULL, this, NULL);

	//Buttons
	main_menu[9] = (UI_Button*)App->gui->CreateButton(887, 295, button_new_game, { 1596,834,60,62 }, { 1596,901,60,62 }, { 1596,967,60,62 }, NULL, this);
	main_menu[10] = (UI_Button*)App->gui->CreateButton(887, 373, button_load_game, { 1596,834,60,62 }, { 1596,901,60,62 }, { 1596,967,60,62 }, NULL, this);
	main_menu[11] = (UI_Button*)App->gui->CreateButton(887, 450, button_settings, { 1596,834,60,62 }, { 1596,901,60,62 }, { 1596,967,60,62 }, NULL, this);
	main_menu[12] = (UI_Button*)App->gui->CreateButton(887, 527, button_credits, { 1596,834,60,62 }, { 1596,901,60,62 }, { 1596,967,60,62 }, NULL, this);
	main_menu[13] = (UI_Button*)App->gui->CreateButton(887, 604, button_exit, { 1596,834,60,62 }, { 1596,901,60,62 }, { 1596,967,60,62 }, NULL, this);

	current_menu = Menu::MAIN_MENU;
}

/*
void MenuManager::DestroyMainMenu()
{

	App->player->Enable();

	//Destroying images
	App->gui->Delete_Element(main_menu.background);
	App->gui->Delete_Element(main_menu.image);
	App->gui->Delete_Element(main_menu.logo);
	App->gui->Delete_Element(main_menu.panel);

	//Destroying labels
	App->gui->Delete_Element(main_menu.new_game_text);
	App->gui->Delete_Element(main_menu.load_game_text);
	App->gui->Delete_Element(main_menu.settings_text);
	App->gui->Delete_Element(main_menu.credits_text);
	App->gui->Delete_Element(main_menu.exit_text);

	//Destroying buttons
	App->gui->Delete_Element(main_menu.new_game_button);
	App->gui->Delete_Element(main_menu.load_game_button);
	App->gui->Delete_Element(main_menu.settings_button);
	App->gui->Delete_Element(main_menu.credits_button);
	App->gui->Delete_Element(main_menu.exit_button);
}
*/

void MenuManager::CreateCredits()
{
	//Images
	credits_menu.background = (j1Image*)App->gui->CreateImage(0, 0, Image, { 1287, 2430, 1281, 730 }, NULL, this);

	//Buttons
	credits_menu.back_button = (UI_Button*)App->gui->CreateButton(65, 65, button_back_credits, { 1243,2014,73,78 }, { 1243,2102,73,79 }, { 1243,2188,73,71 }, NULL, this);
	credits_menu.github_button = (UI_Button*)App->gui->CreateButton(750, 600, button_github_credits, { 2181,841,73,78 }, { 2181,919,73,78 }, { 2181,1005,73,78 }, NULL, this);
	credits_menu.web_button = (UI_Button*)App->gui->CreateButton(850, 600, button_web_credits, { 2268,841,73,78 }, { 2268,919,73,78 }, { 2268,1005,73,78 }, NULL, this);
	credits_menu.twitter_button = (UI_Button*)App->gui->CreateButton(950, 600, button_twitter_credits, { 2355,841,73,78 }, { 2355,919,73,78 }, { 2355,1005,73,78 }, NULL, this);
	credits_menu.marc_button = (UI_Button*)App->gui->CreateButton(450, 130, button_marc, { 0,455,166,17 }, { 0,455,166,17 }, { 0,455,166,17 }, NULL, this);
	credits_menu.javi_button = (UI_Button*)App->gui->CreateButton(450, 175, button_javi, { 0,455,166,17 }, { 0,455,166,17 }, { 0,455,166,17 }, NULL, this);
	credits_menu.pablo_button = (UI_Button*)App->gui->CreateButton(450, 220, button_pablo, { 0,455,166,17 }, { 0,455,166,17 }, { 0,455,166,17 }, NULL, this);
	credits_menu.german_button = (UI_Button*)App->gui->CreateButton(450, 265, button_german, { 0,455,166,17 }, { 0,455,166,17 }, { 0,455,166,17 }, NULL, this);
	credits_menu.macia_button = (UI_Button*)App->gui->CreateButton(450, 310, button_macia, { 0,455,166,17 }, { 0,455,166,17 }, { 0,455,166,17 }, NULL, this);
	credits_menu.pol_button = (UI_Button*)App->gui->CreateButton(450, 355, button_pol, { 0,455,166,17 }, { 0,455,166,17 }, { 0,455,166,17 }, NULL, this);
	credits_menu.silvino_button = (UI_Button*)App->gui->CreateButton(450, 400, button_silvino, { 0,455,166,17 }, { 0,455,166,17 }, { 0,455,166,17 }, NULL, this);
	credits_menu.cristian_button = (UI_Button*)App->gui->CreateButton(450, 445, button_cristian, { 0,455,166,17 }, { 0,455,166,17 }, { 0,455,166,17 }, NULL, this);
}

void MenuManager::DestroyCredits()
{
	//Destroying Images
	App->gui->Delete_Element(credits_menu.background);
	
	//Destroying Buttons
	App->gui->Delete_Element(credits_menu.back_button);
	App->gui->Delete_Element(credits_menu.github_button);
	App->gui->Delete_Element(credits_menu.web_button);
	App->gui->Delete_Element(credits_menu.twitter_button);
	App->gui->Delete_Element(credits_menu.marc_button);
	App->gui->Delete_Element(credits_menu.javi_button);
	App->gui->Delete_Element(credits_menu.pablo_button);
	App->gui->Delete_Element(credits_menu.german_button);
	App->gui->Delete_Element(credits_menu.macia_button);
	App->gui->Delete_Element(credits_menu.pol_button);
	App->gui->Delete_Element(credits_menu.silvino_button);
	App->gui->Delete_Element(credits_menu.cristian_button);

	DestroyAllCollaboratorsPictures();
}

void MenuManager::CreateSettings()
{

	//Images
	settings_menu[0] = (j1Image*)App->gui->CreateImage(-5, 0, Image, { 1529, 0, 1502, 775 }, NULL, this);
	settings_menu[1] = (j1Image*)App->gui->CreateImage(310, 125, Volume_Panel, { 0, 537, 658, 358 }, NULL, this);
	settings_menu[2] = (j1Image*)App->gui->CreateImage(420, 500, Cap_Panel, { 1017, 352, 455, 190 }, NULL, this);
	settings_menu[3] = (j1Image*)App->gui->CreateImage(395, 275, Mute, { 1674, 981, 20, 36 }, NULL, this);
	settings_menu[4] = (j1Image*)App->gui->CreateImage(595, 600, FPS, { 1599, 1054, 77, 52 }, NULL, this);

	//Labels
	settings_menu[5] = (UI_Label*)App->gui->CreateLabel(520, 525, text_fps, "REFRESH RATE", NULL, this, NULL);
	settings_menu[6] = (UI_Label*)App->gui->CreateLabel(360, 335, text_fx_volume, "SOUND EFFECTS VOLUME", NULL, this, NULL);
	settings_menu[7] = (UI_Label*)App->gui->CreateLabel(360, 150, text_music_volume, "MUSIC VOLUME", NULL, this, NULL);
	settings_menu[8] = (UI_Label*)App->gui->CreateLabel(555, 50, text_settings, "SETTINGS", NULL, this, NULL);

	//Music Slider
	settings_menu[9] = (UI_Slider*)App->gui->CreateSlider(465, 220, Slider_music, { 1834,842,329,27 }, { 1792,926,33,16 }, 400, NULL, this);
	settings_menu[10] = (UI_Button*)App->gui->CreateButton(435, 220, Button_slider_music_left, { 1671,880,26,25 }, { 1738,880,26,25 }, { 1738,880,26,25 }, NULL, this);
	settings_menu[11] = (UI_Button*)App->gui->CreateButton(787, 220, Button_slider_music_right, { 1704,880,26,25 }, { 1771,880,26,25 }, { 1771,880,26,25 }, NULL, this);

	//Fx Slider
	settings_menu[12] = (UI_Slider*)App->gui->CreateSlider(465, 405, Slider_fx, { 1834,842,329,27 }, { 1792,926,33,16 }, 400, NULL, this);
	settings_menu[13] = (UI_Button*)App->gui->CreateButton(435, 405, Button_slider_fx_left, { 1671,880,26,25 }, { 1738,880,26,25 }, { 1738,880,26,25 }, NULL, this);
	settings_menu[14] = (UI_Button*)App->gui->CreateButton(796, 405, Button_slider_fx_right, { 1704,880,26,25 }, { 1771,880,26,25 }, { 1771,880,26,25 }, NULL, this);

	//Buttons
	settings_menu[15] = (UI_Button*)App->gui->CreateButton(545, 615, button_cap, { 1671,922,25,26 }, { 1713,922,25,26 }, { 1713,922,25,26 }, NULL, this);
	settings_menu[16] = (UI_Button*)App->gui->CreateButton(360, 280, button_mute, { 1671,922,25,26 }, { 1713,922,25,26 }, { 1713,922,25,26 }, NULL, this);
	settings_menu[17] = (UI_Button*)App->gui->CreateButton(40, 40, button_back, { 1704,1054,48,46 }, { 1765,1054,48,46 }, { 1765,1054,48,46 }, NULL, this);

	last_menu = current_menu;
	current_menu = Menu::SETTINGS;
}

/*
void MenuManager::DestroySettings()
{
	//Destroying images
	App->gui->Delete_Element(settings_menu.background);
	App->gui->Delete_Element(settings_menu.mute);
	App->gui->Delete_Element(settings_menu.panel_volume);
	App->gui->Delete_Element(settings_menu.panel_cap);
	App->gui->Delete_Element(settings_menu.fps_image);

	//Destroying Sliders
	App->gui->Delete_Element(settings_menu.volume_music_slider);
	App->gui->Delete_Element(settings_menu.volume_fx_slider);
	App->gui->Delete_Element(settings_menu.left_fx_button);
	App->gui->Delete_Element(settings_menu.right_fx_button);
	App->gui->Delete_Element(settings_menu.left_music_button);
	App->gui->Delete_Element(settings_menu.right_music_button);

	//Destroying buttons
	App->gui->Delete_Element(settings_menu.cap_button);
	App->gui->Delete_Element(settings_menu.mute_button);
	App->gui->Delete_Element(settings_menu.back_button);

	//Destroying Labels
	App->gui->Delete_Element(settings_menu.fps);
	App->gui->Delete_Element(settings_menu.music_volume);
	App->gui->Delete_Element(settings_menu.fx_volume);
	App->gui->Delete_Element(settings_menu.settings_text);

}
*/

void MenuManager::CreateCollaboratorPicture()
{
	switch (collaborator) 
	{
	case 'M':
		credits_menu.marc_image = (j1Image*)App->gui->CreateImage(815, 130, Image, { 1230, 1545, 138, 138 }, NULL, this);
		break;
	case 'J':
		credits_menu.javi_image = (j1Image*)App->gui->CreateImage(815, 130, Image, { 1392, 1545, 138, 138 }, NULL, this);
		break;
	case 'P':
		credits_menu.pablo_image = (j1Image*)App->gui->CreateImage(815, 130, Image, { 1563, 1545, 138, 138 }, NULL, this);
		break;
	case 'G':
		credits_menu.german_image = (j1Image*)App->gui->CreateImage(815, 130, Image, { 1719, 1539, 138, 138 }, NULL, this);
		break;
	case 'D':
		credits_menu.macia_image = (j1Image*)App->gui->CreateImage(815, 130, Image, { 1230, 1690, 138, 138 }, NULL, this);
		break;
	case 'K':
		credits_menu.pol_image = (j1Image*)App->gui->CreateImage(815, 130, Image, { 1392, 1690, 138, 138 }, NULL, this);
		break;
	case 'S':
		credits_menu.silvino_image = (j1Image*)App->gui->CreateImage(815, 130, Image, { 1563, 1690, 138, 138 }, NULL, this);
		break;
	case 'C':
		credits_menu.cristian_image = (j1Image*)App->gui->CreateImage(815, 130, Image, { 1719, 1690, 138, 138 }, NULL, this);
		break;
	default:
		credits_menu.marc_image = (j1Image*)App->gui->CreateImage(815, 130, Image, { 1230, 1545, 138, 138 }, NULL, this);
	}
	

}

void MenuManager::DestroyAllCollaboratorsPictures()
{

	App->gui->Delete_Element(credits_menu.marc_image);
	App->gui->Delete_Element(credits_menu.javi_image);
	App->gui->Delete_Element(credits_menu.pablo_image);
	App->gui->Delete_Element(credits_menu.german_image);
	App->gui->Delete_Element(credits_menu.macia_image);
	App->gui->Delete_Element(credits_menu.pol_image);
	App->gui->Delete_Element(credits_menu.silvino_image);
	App->gui->Delete_Element(credits_menu.cristian_image);

}

void MenuManager::CreateSelectFaction()
{
	//Images
	select_faction_menu.background = (j1Image*)App->gui->CreateImage(-5, 0, Image, { 1900, 1531, 1290, 842 }, NULL, this);
	
	//Buttons
	select_faction_menu.ghouls_button = (UI_Button*)App->gui->CreateButton(200, 200, button_select_ghoul, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,144,64 }, NULL, this);
	select_faction_menu.vault_button = (UI_Button*)App->gui->CreateButton(200, 300, button_select_vault, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,144,64 }, NULL, this);
	select_faction_menu.brotherhood_button = (UI_Button*)App->gui->CreateButton(200, 400, button_select_brotherhood, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,144,64 }, NULL, this);
	select_faction_menu.supermutants_button = (UI_Button*)App->gui->CreateButton(200, 500, button_select_supermutant, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,144,64 }, NULL, this);
	select_faction_menu.back_button = (UI_Button*)App->gui->CreateButton(120, 605, button_back_to_menu, { 1243,2014,73,78 }, { 1243,2102,73,79 }, { 1243,2188,73,71 }, NULL, this);
	select_faction_menu.start_game_button = (UI_Button*)App->gui->CreateButton(900, 605, button_start_game, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,144,64 }, NULL, this);
	
	//Labels
	select_faction_menu.ghouls_label = (UI_Label*)App->gui->CreateLabel(280, 215, ghouls_label, "GHOUL", NULL, this, NULL);
	select_faction_menu.vault_label = (UI_Label*)App->gui->CreateLabel(278, 315, ghouls_label, "VAULT", NULL, this, NULL);
	select_faction_menu.brotherhood_label = (UI_Label*)App->gui->CreateLabel(220, 415, ghouls_label, "BROTHERHOOD", NULL, this, NULL);
	select_faction_menu.supermutants_label = (UI_Label*)App->gui->CreateLabel(214, 515, ghouls_label, "SUPERMUTANT", NULL, this, NULL);
	select_faction_menu.start_game_label = (UI_Label*)App->gui->CreateLabel(925, 620, start_game_label, "START GAME", NULL, this, NULL);


}

void MenuManager::DestroySelectFaction()
{

	App->gui->Delete_Element(select_faction_menu.background);
	App->gui->Delete_Element(select_faction_menu.ghouls);
	App->gui->Delete_Element(select_faction_menu.vault);
	App->gui->Delete_Element(select_faction_menu.brotherhood);
	App->gui->Delete_Element(select_faction_menu.supermutants);

	App->gui->Delete_Element(select_faction_menu.ghouls_button);
	App->gui->Delete_Element(select_faction_menu.vault_button);
	App->gui->Delete_Element(select_faction_menu.brotherhood_button);
	App->gui->Delete_Element(select_faction_menu.supermutants_button);
	App->gui->Delete_Element(select_faction_menu.start_game_button);
	App->gui->Delete_Element(select_faction_menu.back_button);

	App->gui->Delete_Element(select_faction_menu.ghouls_label);
	App->gui->Delete_Element(select_faction_menu.vault_label);
	App->gui->Delete_Element(select_faction_menu.brotherhood_label);
	App->gui->Delete_Element(select_faction_menu.supermutants_label);
	App->gui->Delete_Element(select_faction_menu.start_game_label);

}

void MenuManager::CreatePauseMenu()
{

	//Images
	pause_menu.panel = (j1Image*)App->gui->CreateImage(434, 183, Image, { 3079, 41, 334, 313 }, NULL, this);


	//Buttons
	pause_menu.resume_button = (UI_Button*)App->gui->CreateButton(480, 210, resume_button, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,144,64 }, NULL, this);
	pause_menu.settings_button = (UI_Button*)App->gui->CreateButton(480, 300, button_settings_pause, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,144,64 }, NULL, this);
	pause_menu.back_to_main_menu_button = (UI_Button*)App->gui->CreateButton(480, 390, button_pause_to_main, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,144,64 }, NULL, this);

	//Labels
	pause_menu.resume_label = (UI_Label*)App->gui->CreateLabel(540, 228, RESUME, "RESUME", NULL, this, NULL);
	pause_menu.settings_label = (UI_Label*)App->gui->CreateLabel(530, 318, SETTINGS, "SETTINGS", NULL, this, NULL);
	pause_menu.back_to_main_menu_label = (UI_Label*)App->gui->CreateLabel(515, 408, EXIT, "MAIN MENU", NULL, this, NULL);

	last_menu = current_menu;
	current_menu = Menu::PAUSE_MENU;
}

void MenuManager::DestroyPauseMenu()
{

	//Destroying images
	App->gui->Delete_Element(pause_menu.panel);
	

	//Destroying buttons
	App->gui->Delete_Element(pause_menu.resume_button);
	App->gui->Delete_Element(pause_menu.settings_button);
	App->gui->Delete_Element(pause_menu.back_to_main_menu_button);

	//Destroying Labels
	App->gui->Delete_Element(pause_menu.resume_label);
	App->gui->Delete_Element(pause_menu.settings_label);
	App->gui->Delete_Element(pause_menu.back_to_main_menu_label);
	
	last_menu = current_menu;
	current_menu = Menu::NO_MENU;
}

void MenuManager::CreatePauseSettings()
{

	//Images
	settings_menu[0] = (j1Image*)App->gui->CreateImage(-5, 0, Image, { 1529, 0, 1502, 775 }, NULL, this);
	settings_menu[1] = (j1Image*)App->gui->CreateImage(310, 125, Volume_Panel, { 0, 537, 658, 358 }, NULL, this);
	settings_menu[2] = (j1Image*)App->gui->CreateImage(420, 500, Cap_Panel, { 1017, 352, 455, 190 }, NULL, this);
	settings_menu[3] = (j1Image*)App->gui->CreateImage(395, 275, Mute, { 1674, 981, 20, 36 }, NULL, this);
	settings_menu[4] = (j1Image*)App->gui->CreateImage(595, 600, FPS, { 1599, 1054, 77, 52 }, NULL, this);

	//Labels
	settings_menu[5] = (UI_Label*)App->gui->CreateLabel(520, 525, text_fps, "REFRESH RATE", NULL, this, NULL);
	settings_menu[6] = (UI_Label*)App->gui->CreateLabel(360, 335, text_fx_volume, "SOUND EFFECTS VOLUME", NULL, this, NULL);
	settings_menu[7] = (UI_Label*)App->gui->CreateLabel(360, 150, text_music_volume, "MUSIC VOLUME", NULL, this, NULL);
	settings_menu[8] = (UI_Label*)App->gui->CreateLabel(555, 50, text_settings, "SETTINGS", NULL, this, NULL);

	//Music Slider
	settings_menu[9] = (UI_Slider*)App->gui->CreateSlider(465, 220, Slider_music, { 1834,842,329,27 }, { 1792,926,33,16 }, 400, NULL, this);
	settings_menu[10] = (UI_Button*)App->gui->CreateButton(435, 220, Button_slider_music_left, { 1671,880,26,25 }, { 1738,880,26,25 }, { 1738,880,26,25 }, NULL, this);
	settings_menu[11] = (UI_Button*)App->gui->CreateButton(796, 220, Button_slider_music_right, { 1704,880,26,25 }, { 1771,880,26,25 }, { 1771,880,26,25 }, NULL, this);

	//Fx Slider
	settings_menu[12] = (UI_Slider*)App->gui->CreateSlider(465, 405, Slider_fx, { 1834,842,329,27 }, { 1792,926,33,16 }, 400, NULL, this);
	settings_menu[13] = (UI_Button*)App->gui->CreateButton(435, 405, Button_slider_fx_left, { 1671,880,26,25 }, { 1738,880,26,25 }, { 1738,880,26,25 }, NULL, this);
	settings_menu[14] = (UI_Button*)App->gui->CreateButton(796, 405, Button_slider_fx_right, { 1704,880,26,25 }, { 1771,880,26,25 }, { 1771,880,26,25 }, NULL, this);

	//Buttons
	settings_menu[15] = (UI_Button*)App->gui->CreateButton(545, 615, button_cap, { 1671,922,25,26 }, { 1713,922,25,26 }, { 1713,922,25,26 }, NULL, this);
	settings_menu[16] = (UI_Button*)App->gui->CreateButton(360, 280, button_mute, { 1671,922,25,26 }, { 1713,922,25,26 }, { 1713,922,25,26 }, NULL, this);
	settings_menu[17] = (UI_Button*)App->gui->CreateButton(40, 40, button_back_pause, { 1704,1054,48,46 }, { 1765,1054,48,46 }, { 1765,1054,48,46 }, NULL, this);

	last_menu = current_menu;
	current_menu = Menu::SETTINGS;
}

/*
void MenuManager::DestroyPauseSettings()
{

	//Destroying images
	App->gui->Delete_Element(settings_menu.background);
	App->gui->Delete_Element(settings_menu.mute);
	App->gui->Delete_Element(settings_menu.panel_volume);
	App->gui->Delete_Element(settings_menu.panel_cap);
	App->gui->Delete_Element(settings_menu.fps_image);

	//Destroying Sliders
	App->gui->Delete_Element(settings_menu.volume_music_slider);
	App->gui->Delete_Element(settings_menu.volume_fx_slider);
	App->gui->Delete_Element(settings_menu.left_fx_button);
	App->gui->Delete_Element(settings_menu.right_fx_button);
	App->gui->Delete_Element(settings_menu.left_music_button);
	App->gui->Delete_Element(settings_menu.right_music_button);

	//Destroying buttons
	App->gui->Delete_Element(settings_menu.cap_button);
	App->gui->Delete_Element(settings_menu.mute_button);
	App->gui->Delete_Element(settings_menu.back_button);

	//Destroying Labels
	App->gui->Delete_Element(settings_menu.fps);
	App->gui->Delete_Element(settings_menu.music_volume);
	App->gui->Delete_Element(settings_menu.fx_volume);
	App->gui->Delete_Element(settings_menu.settings_text);

}
*/

void MenuManager::CreateGUI()
{

	//Images
	gui_ingame.ingame_background = (j1Image*)App->gui->CreateImage(0, 0, Nukas, { 0, 2428, 1290, 730 }, NULL, this);

	if (App->gui->count == 0) {
		
		cc = std::to_string(App->player->caps);
		ww = std::to_string(App->player->water);
		ff = std::to_string(App->player->food);
		
		gui_ingame.meat_count = (UI_Label*)App->gui->CreateLabel(95, 10, Meat, ff, NULL, this, NULL);
		gui_ingame.water_count = (UI_Label*)App->gui->CreateLabel(240, 10, Water, ww, NULL, this, NULL);
		gui_ingame.nukas_count = (UI_Label*)App->gui->CreateLabel(385, 10, Nukas, cc, NULL, this, NULL);

		App->gui->count++;
	}

	//Labels
	//settings_menu.fps = (UI_Label*)App->gui->CreateLabel(520, 525, text_fps, "REFRESH RATE", NULL, this, NULL);
	//settings_menu.fx_volume = (UI_Label*)App->gui->CreateLabel(360, 335, text_fx_volume, "SOUND EFFECTS VOLUME", NULL, this, NULL);
	//settings_menu.music_volume = (UI_Label*)App->gui->CreateLabel(360, 150, text_music_volume, "MUSIC VOLUME", NULL, this, NULL);

}
void MenuManager::DestroyGUI()
{

	//Destroying images
	App->gui->Delete_Element(gui_ingame.ingame_background);
	
	
	//Destroying Labels
	App->gui->Delete_Element(gui_ingame.meat_count);
	App->gui->Delete_Element(gui_ingame.water_count);
	App->gui->Delete_Element(gui_ingame.nukas_count);

}

//Brotherhood

void MenuManager::DestroyAllBuildings() {
	//Ghouls
	DestroyGhouls_Base();
	DestroyGhouls_Barrack();
	DestroyGhouls_Lab();

	//Mutants
	DestroySuperMutants_Base();
	DestroySuperMutants_Barrack();
	DestroySuperMutants_Lab();

	//Vaults
	DestroyVault_Base();
	DestroyVault_Barrack();
	DestroyVault_Lab();

	//Brotherhood
	DestroyBrotherHood_Base();
	DestroyBrotherHood_Barrack();
	DestroyBrotherHood_Lab();
}

void MenuManager::CreateBrotherHood_Base()
{

	//Buttons
	bases_hud.base_create_brotherhood = (UI_Button*)App->gui->CreateButton(1050, 600, Brotherhood_ghaterer_button, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);

	//Images
	bases_hud.brotherhood_base = (j1Image*)App->gui->CreateImage(880, 575, Brotherhood_base, { 118, 1561, 122, 98 }, NULL, this);
	bases_hud.brotherhood_gatherer = (j1Image*)App->gui->CreateImage(1055, 605, Brotherhood_ghaterer, { 138, 1868, 33, 33 }, NULL, this);

	//Labels
	//p2SString hh("%i", App->entities->); VIDA EDIFICIO
	//gui_ingame.meat_count = (UI_Label*)App->gui->CreateLabel(95, 10, Meat, hh, NULL, this, NULL);
	//gui_ingame.meat_count = (UI_Label*)App->gui->CreateLabel(95, 10, Meat, "Brotherhood Base", NULL, this, NULL);

}
void MenuManager::DestroyBrotherHood_Base()
{

	App->gui->Delete_Element(bases_hud.base_create_brotherhood);
	App->gui->Delete_Element(bases_hud.brotherhood_base);
	App->gui->Delete_Element(bases_hud.brotherhood_gatherer);

}

void MenuManager::CreateBrotherHood_Barrack() {

	//Buttons
	bases_hud.barrack_create_brotherhood = (UI_Button*)App->gui->CreateButton(1050, 600, Brotherhood_melee, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);
	bases_hud.barrack_create_brotherhood = (UI_Button*)App->gui->CreateButton(1105, 600, Brotherhood_ranged, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);

	//Images
	bases_hud.brotherhood_barrack = (j1Image*)App->gui->CreateImage(885, 570, Brotherhood_barrack, { 17, 1560, 69, 115 }, NULL, this);
	bases_hud.brotherhood_melee = (j1Image*)App->gui->CreateImage(1055, 605, Brotherhood_melee, { 28, 1869, 35, 33 }, NULL, this);
	bases_hud.brotherhood_ranged = (j1Image*)App->gui->CreateImage(1110, 605, Brotherhood_ranged, { 83, 1866, 35, 36 }, NULL, this);

}
void MenuManager::DestroyBrotherHood_Barrack() {

	App->gui->Delete_Element(bases_hud.barrack_create_brotherhood);
	App->gui->Delete_Element(bases_hud.barrack_create_brotherhood);
	App->gui->Delete_Element(bases_hud.brotherhood_barrack);
	App->gui->Delete_Element(bases_hud.brotherhood_melee);
	App->gui->Delete_Element(bases_hud.brotherhood_ranged);

}

void MenuManager::CreateBrotherHood_Lab() {

	//Buttons
	//bases_hud.lab_create_brotherhood = (UI_Button*)App->gui->CreateButton(1050, 600, Brotherhood_melee, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);
	//bases_hud.lab_create_brotherhood = (UI_Button*)App->gui->CreateButton(1105, 600, Brotherhood_ranged, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);

	//Images
	bases_hud.brotherhood_lab = (j1Image*)App->gui->CreateImage(885, 570, Brotherhood_lab, { 663, 1567, 108, 104 }, NULL, this);
	//bases_hud.brotherhood_melee = (j1Image*)App->gui->CreateImage(1055, 605, Brotherhood_melee, { 28, 1869, 35, 33 }, NULL, this);
	//bases_hud.brotherhood_ranged = (j1Image*)App->gui->CreateImage(1110, 605, Brotherhood_ranged, { 83, 1866, 35, 36 }, NULL, this);
	//bases_hud.brotherhood_melee = (j1Image*)App->gui->CreateImage(1055, 605, Brotherhood_melee, { 28, 1869, 35, 33 }, NULL, this);
	//bases_hud.brotherhood_ranged = (j1Image*)App->gui->CreateImage(1110, 605, Brotherhood_ranged, { 83, 1866, 35, 36 }, NULL, this);
	//bases_hud.brotherhood_melee = (j1Image*)App->gui->CreateImage(1055, 605, Brotherhood_melee, { 28, 1869, 35, 33 }, NULL, this);

}
void MenuManager::DestroyBrotherHood_Lab() {

	App->gui->Delete_Element(bases_hud.brotherhood_lab);

}

//Supermutants

void MenuManager::CreateSuperMutants_Base() {

	//Buttons
	bases_hud.base_create_supermutant = (UI_Button*)App->gui->CreateButton(1050, 600, Supermutant_ghaterer_button, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);

	//Images
	bases_hud.supermutant_base = (j1Image*)App->gui->CreateImage(880, 575, Supermutant_base, { 934, 1577, 129, 91 }, NULL, this);
	bases_hud.supermutant_gatherer = (j1Image*)App->gui->CreateImage(1055, 605, Supermutant_ghaterer, { 191, 1868, 34, 32 }, NULL, this);

}
void MenuManager::DestroySuperMutants_Base() {

	App->gui->Delete_Element(bases_hud.base_create_supermutant);
	App->gui->Delete_Element(bases_hud.supermutant_base);
	App->gui->Delete_Element(bases_hud.supermutant_gatherer);

}

void MenuManager::CreateSuperMutants_Barrack() {

	//Buttons
	bases_hud.barrack_create_supermutant = (UI_Button*)App->gui->CreateButton(1050, 600, Supermutant_melee_button, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);
	bases_hud.barrack_create_supermutant = (UI_Button*)App->gui->CreateButton(1105, 600, Supermutant_ranged_button, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);

	//Images
	bases_hud.supermutant_barrack = (j1Image*)App->gui->CreateImage(885, 570, Supermutant_barrack, { 801, 1549, 102, 119 }, NULL, this);
	bases_hud.supermutant_melee = (j1Image*)App->gui->CreateImage(1055, 605, Supermutant_melee, { 246, 1869, 34, 33 }, NULL, this);
	bases_hud.supermutant_ranged = (j1Image*)App->gui->CreateImage(1110, 605, Supermutant_ranged, { 301, 1865, 34, 35 }, NULL, this);
	
}
void MenuManager::DestroySuperMutants_Barrack() {

	App->gui->Delete_Element(bases_hud.barrack_create_supermutant);
	App->gui->Delete_Element(bases_hud.barrack_create_supermutant);
	App->gui->Delete_Element(bases_hud.supermutant_barrack);
	App->gui->Delete_Element(bases_hud.supermutant_melee);
	App->gui->Delete_Element(bases_hud.supermutant_ranged);

}

void MenuManager::CreateSuperMutants_Lab() {

	//Buttons
	//bases_hud.lab_create_brotherhood = (UI_Button*)App->gui->CreateButton(1050, 600, Brotherhood_melee, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);
	//bases_hud.lab_create_brotherhood = (UI_Button*)App->gui->CreateButton(1105, 600, Brotherhood_ranged, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);

	//Images
	bases_hud.supermutant_lab = (j1Image*)App->gui->CreateImage(885, 570, Supermutant_lab, { 663, 1567, 108, 104 }, NULL, this);
	//bases_hud.brotherhood_melee = (j1Image*)App->gui->CreateImage(1055, 605, Brotherhood_melee, { 28, 1869, 35, 33 }, NULL, this);
	//bases_hud.brotherhood_ranged = (j1Image*)App->gui->CreateImage(1110, 605, Brotherhood_ranged, { 83, 1866, 35, 36 }, NULL, this);
	//bases_hud.brotherhood_melee = (j1Image*)App->gui->CreateImage(1055, 605, Brotherhood_melee, { 28, 1869, 35, 33 }, NULL, this);
	//bases_hud.brotherhood_ranged = (j1Image*)App->gui->CreateImage(1110, 605, Brotherhood_ranged, { 83, 1866, 35, 36 }, NULL, this);
	//bases_hud.brotherhood_melee = (j1Image*)App->gui->CreateImage(1055, 605, Brotherhood_melee, { 28, 1869, 35, 33 }, NULL, this);

}
void MenuManager::DestroySuperMutants_Lab() {

	App->gui->Delete_Element(bases_hud.supermutant_lab);
}

//Ghouls

void MenuManager::CreateGhouls_Base() {

	//Buttons
	bases_hud.base_create_ghouls = (UI_Button*)App->gui->CreateButton(1050, 600, Ghouls_ghaterer_button, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);

	//Images
	bases_hud.brotherhood_base = (j1Image*)App->gui->CreateImage(880, 575, Ghouls_base, { 398, 1564, 99, 108 }, NULL, this);
	bases_hud.brotherhood_gatherer = (j1Image*)App->gui->CreateImage(1060, 605, Ghouls_ghaterer, { 624, 1866, 24, 34 }, NULL, this);

}
void MenuManager::DestroyGhouls_Base() {

	App->gui->Delete_Element(bases_hud.base_create_ghouls);
	App->gui->Delete_Element(bases_hud.brotherhood_base);
	App->gui->Delete_Element(bases_hud.brotherhood_gatherer);

}

void MenuManager::CreateGhouls_Barrack() {

	//Buttons
	bases_hud.barrack_melee_create_ghouls = (UI_Button*)App->gui->CreateButton(1050, 600, Ghouls_melee_button, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);
	bases_hud.barrack_ranged_create_ghouls = (UI_Button*)App->gui->CreateButton(1105, 600, Ghouls_ranged_button, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);

	//Images
	bases_hud.ghouls_barrack = (j1Image*)App->gui->CreateImage(885, 570, Ghouls_barrack, { 270, 1568, 97, 99 }, NULL, this);
	bases_hud.ghouls_melee = (j1Image*)App->gui->CreateImage(1055, 605, Ghouls_melee, { 513, 1865, 33, 34 }, NULL, this);
	bases_hud.ghouls_ranged = (j1Image*)App->gui->CreateImage(1110, 605, Ghouls_ranged, { 571, 1866, 28, 33 }, NULL, this);

}
void MenuManager::DestroyGhouls_Barrack() {

	App->gui->Delete_Element(bases_hud.barrack_melee_create_ghouls);
	App->gui->Delete_Element(bases_hud.barrack_ranged_create_ghouls);
	App->gui->Delete_Element(bases_hud.ghouls_barrack);
	App->gui->Delete_Element(bases_hud.ghouls_melee);
	App->gui->Delete_Element(bases_hud.ghouls_ranged);

}

void MenuManager::CreateGhouls_Lab() {

	//Buttons
	//bases_hud.lab_create_brotherhood = (UI_Button*)App->gui->CreateButton(1050, 600, Brotherhood_melee, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);
	//bases_hud.lab_create_brotherhood = (UI_Button*)App->gui->CreateButton(1105, 600, Brotherhood_ranged, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);

	//Images
	bases_hud.ghouls_lab = (j1Image*)App->gui->CreateImage(885, 570, Ghouls_lab, { 527, 1563, 105, 105 }, NULL, this);
	//bases_hud.brotherhood_melee = (j1Image*)App->gui->CreateImage(1055, 605, Brotherhood_melee, { 28, 1869, 35, 33 }, NULL, this);
	//bases_hud.brotherhood_ranged = (j1Image*)App->gui->CreateImage(1110, 605, Brotherhood_ranged, { 83, 1866, 35, 36 }, NULL, this);
	//bases_hud.brotherhood_melee = (j1Image*)App->gui->CreateImage(1055, 605, Brotherhood_melee, { 28, 1869, 35, 33 }, NULL, this);
	//bases_hud.brotherhood_ranged = (j1Image*)App->gui->CreateImage(1110, 605, Brotherhood_ranged, { 83, 1866, 35, 36 }, NULL, this);
	//bases_hud.brotherhood_melee = (j1Image*)App->gui->CreateImage(1055, 605, Brotherhood_melee, { 28, 1869, 35, 33 }, NULL, this);

}
void MenuManager::DestroyGhouls_Lab() {

	App->gui->Delete_Element(bases_hud.ghouls_lab);

}

//Vault

void MenuManager::CreateVault_Base() {

	//Buttons
	bases_hud.base_create_vault = (UI_Button*)App->gui->CreateButton(1050, 600, Vault_ghaterer_button, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);

	//Images
	bases_hud.brotherhood_base = (j1Image*)App->gui->CreateImage(890, 569, Vault_base, { 319, 1710, 49, 113 }, NULL, this);
	bases_hud.brotherhood_gatherer = (j1Image*)App->gui->CreateImage(1058, 609, Vault_ghaterer, { 463, 1871, 25, 29 }, NULL, this);

}
void MenuManager::DestroyVault_Base() {

	App->gui->Delete_Element(bases_hud.base_create_vault);
	App->gui->Delete_Element(bases_hud.brotherhood_base);
	App->gui->Delete_Element(bases_hud.brotherhood_gatherer);

}

void MenuManager::CreateVault_Barrack() {

	//Buttons
	bases_hud.barrack_create_vault = (UI_Button*)App->gui->CreateButton(1050, 600, Vault_melee_button, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);
	bases_hud.barrack_create_vault = (UI_Button*)App->gui->CreateButton(1105, 600, Vault_ranged_button, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);

	//Images
	bases_hud.vault_barrack = (j1Image*)App->gui->CreateImage(885, 570, Vault_barrack, { 148, 1731, 122, 91 }, NULL, this);
	bases_hud.vault_melee = (j1Image*)App->gui->CreateImage(1055, 605, Vault_melee, { 406, 1866, 32, 34 }, NULL, this);
	bases_hud.vault_ranged = (j1Image*)App->gui->CreateImage(1110, 605, Vault_ranged, { 355, 1866, 30, 34 }, NULL, this);

}
void MenuManager::DestroyVault_Barrack() {

	App->gui->Delete_Element(bases_hud.barrack_create_vault);
	App->gui->Delete_Element(bases_hud.barrack_create_vault);
	App->gui->Delete_Element(bases_hud.vault_barrack);
	App->gui->Delete_Element(bases_hud.vault_melee);
	App->gui->Delete_Element(bases_hud.vault_ranged);

}

void MenuManager::CreateVault_Lab() {

	//Buttons
	//bases_hud.lab_create_brotherhood = (UI_Button*)App->gui->CreateButton(1050, 600, Brotherhood_melee, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);
	//bases_hud.lab_create_brotherhood = (UI_Button*)App->gui->CreateButton(1105, 600, Brotherhood_ranged, { 1355,2027,45,42 }, { 1355,2111,45,42 }, { 1355,2196,45,42 }, NULL, this);

	//Images
	bases_hud.brotherhood_lab = (j1Image*)App->gui->CreateImage(885, 570, Vault_lab, { 426, 1726, 72, 96 }, NULL, this);
	//bases_hud.brotherhood_melee = (j1Image*)App->gui->CreateImage(1055, 605, Brotherhood_melee, { 28, 1869, 35, 33 }, NULL, this);
	//bases_hud.brotherhood_ranged = (j1Image*)App->gui->CreateImage(1110, 605, Brotherhood_ranged, { 83, 1866, 35, 36 }, NULL, this);
	//bases_hud.brotherhood_melee = (j1Image*)App->gui->CreateImage(1055, 605, Brotherhood_melee, { 28, 1869, 35, 33 }, NULL, this);
	//bases_hud.brotherhood_ranged = (j1Image*)App->gui->CreateImage(1110, 605, Brotherhood_ranged, { 83, 1866, 35, 36 }, NULL, this);
	//bases_hud.brotherhood_melee = (j1Image*)App->gui->CreateImage(1055, 605, Brotherhood_melee, { 28, 1869, 35, 33 }, NULL, this);

}

void MenuManager::DestroyVault_Lab() {

	App->gui->Delete_Element(bases_hud.brotherhood_lab);

}

void MenuManager::DestroyMenu(Menu menu) {
	switch (menu)
	{
	case Menu::MAIN_MENU:
		App->gui->DeleteArrayElements(main_menu, 14);
		break;
	case Menu::SETTINGS:
		App->gui->DeleteArrayElements(settings_menu, 18);
		break;
	case Menu::CREDITS:
		break;
	case Menu::SELECT_FACTION:
		break;
	case Menu::PAUSE_MENU:
		break;
	case Menu::GUI:
		break;
	case Menu::BUI_BASES:
		break;
	default:
		break;
	}

	current_menu = last_menu;
}