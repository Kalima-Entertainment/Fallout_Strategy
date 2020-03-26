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

MenuManager::MenuManager()
{
	name.create("menu_manager");
}

MenuManager::~MenuManager()
{
}


void MenuManager::CreateMainMenu()
{

	App->player->Disable();

	//Images															
	main_menu.background = (j1Image*)App->gui->CreateImage(-5, 0, Image, { 1529, 0, 1502, 775 }, NULL, this);
	main_menu.image = (j1Image*)App->gui->CreateImage(0, 350, Image1, { 0, 1016, 871, 516 }, NULL, this);
	main_menu.logo = (j1Image*)App->gui->CreateImage(40, 40, Logo, { 938, 903, 505, 233 }, NULL, this);
	main_menu.panel = (j1Image*)App->gui->CreateImage(827, 260, Panel, { 0, 0, 416, 448 }, NULL, this);
	
	//Labels
	main_menu.new_game_text = (UI_Label*)App->gui->CreateLabel(957,310, text_new_game, "NEW GAME", NULL, this, NULL);
	main_menu.load_game_text = (UI_Label*)App->gui->CreateLabel(957, 387, text_load_game, "LOAD GAME", NULL, this, NULL);
	main_menu.settings_text = (UI_Label*)App->gui->CreateLabel(957, 464, text_settings, "SETTINGS", NULL, this, NULL);
	main_menu.credits_text = (UI_Label*)App->gui->CreateLabel(957, 541, text_credits, "CREDITS", NULL, this, NULL);
	main_menu.exit_text = (UI_Label*)App->gui->CreateLabel(957, 618, text_exit, "EXIT", NULL, this, NULL);

	//Buttons
	main_menu.new_game_button = (UI_Button*)App->gui->CreateButton(887, 295, button_new_game, { 1596,834,60,62 }, { 1596,901,60,62 }, { 1596,967,60,62 }, NULL, this);
	main_menu.load_game_button = (UI_Button*)App->gui->CreateButton(887, 373, button_load_game, { 1596,834,60,62 }, { 1596,901,60,62 }, { 1596,967,60,62 }, NULL, this);
	main_menu.settings_button = (UI_Button*)App->gui->CreateButton(887, 450, button_settings, { 1596,834,60,62 }, { 1596,901,60,62 }, { 1596,967,60,62 }, NULL, this);
	main_menu.credits_button = (UI_Button*)App->gui->CreateButton(887, 527, button_credits, { 1596,834,60,62 }, { 1596,901,60,62 }, { 1596,967,60,62 }, NULL, this);
	main_menu.exit_button = (UI_Button*)App->gui->CreateButton(887, 604, button_exit, { 1596,834,60,62 }, { 1596,901,60,62 }, { 1596,967,60,62 }, NULL, this);

}

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

void MenuManager::CreateCredits()
{
	//Images
	credits_menu.background = (j1Image*)App->gui->CreateImage(-90, 0, Image, { 0, 1536, 1207, 837 }, NULL, this);

	//Buttons
	credits_menu.back_button = (UI_Button*)App->gui->CreateButton(65, 65, button_back_credits, { 1704,1054,48,46 }, { 1765,1054,48,46 }, { 1765,1054,48,46 }, NULL, this);
	credits_menu.github_button = (UI_Button*)App->gui->CreateButton(110, 550, button_github_credits, { 2181,841,73,78 }, { 2181,919,73,78 }, { 2181,1005,73,78 }, NULL, this);
	credits_menu.web_button = (UI_Button*)App->gui->CreateButton(230, 550, button_web_credits, { 2268,841,73,78 }, { 2268,919,73,78 }, { 2268,1005,73,78 }, NULL, this);
	credits_menu.twitter_button = (UI_Button*)App->gui->CreateButton(350, 550, button_twitter_credits, { 2355,841,73,78 }, { 2355,919,73,78 }, { 2355,1005,73,78 }, NULL, this);
	credits_menu.marc_button = (UI_Button*)App->gui->CreateButton(290, 160, button_marc, { 0,455,166,17 }, { 0,455,166,17 }, { 0,455,166,17 }, NULL, this);
	credits_menu.javi_button = (UI_Button*)App->gui->CreateButton(290, 205, button_javi, { 0,455,166,17 }, { 0,455,166,17 }, { 0,455,166,17 }, NULL, this);
	credits_menu.pablo_button = (UI_Button*)App->gui->CreateButton(290, 250, button_pablo, { 0,455,166,17 }, { 0,455,166,17 }, { 0,455,166,17 }, NULL, this);
	credits_menu.german_button = (UI_Button*)App->gui->CreateButton(290, 295, button_german, { 0,455,166,17 }, { 0,455,166,17 }, { 0,455,166,17 }, NULL, this);
	credits_menu.macia_button = (UI_Button*)App->gui->CreateButton(290, 340, button_macia, { 0,455,166,17 }, { 0,455,166,17 }, { 0,455,166,17 }, NULL, this);
	credits_menu.pol_button = (UI_Button*)App->gui->CreateButton(290, 385, button_pol, { 0,455,166,17 }, { 0,455,166,17 }, { 0,455,166,17 }, NULL, this);
	credits_menu.silvino_button = (UI_Button*)App->gui->CreateButton(290, 430, button_silvino, { 0,455,166,17 }, { 0,455,166,17 }, { 0,455,166,17 }, NULL, this);
	credits_menu.cristian_button = (UI_Button*)App->gui->CreateButton(290, 475, button_cristian, { 0,455,166,17 }, { 0,455,166,17 }, { 0,455,166,17 }, NULL, this);
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
	settings_menu.background = (j1Image*)App->gui->CreateImage(-5, 0, Image, { 1529, 0, 1502, 775 }, NULL, this);
	settings_menu.panel_volume = (j1Image*)App->gui->CreateImage(310, 125, Volume_Panel, { 0, 537, 658, 358 }, NULL, this);
	settings_menu.panel_cap = (j1Image*)App->gui->CreateImage(420, 500, Cap_Panel, { 1017, 352, 455, 190 }, NULL, this);
	settings_menu.mute = (j1Image*)App->gui->CreateImage(395, 275, Mute, { 1674, 981, 20, 36 }, NULL, this);
	settings_menu.fps_image = (j1Image*)App->gui->CreateImage(595, 600, FPS, { 1599, 1054, 77, 52 }, NULL, this);

	//Labels
	
	settings_menu.fps = (UI_Label*)App->gui->CreateLabel(520, 525, text_fps, "REFRESH RATE", NULL, this, NULL);
	settings_menu.fx_volume = (UI_Label*)App->gui->CreateLabel(360, 335, text_fx_volume, "SOUND EFFECTS VOLUME", NULL, this, NULL);
	settings_menu.music_volume = (UI_Label*)App->gui->CreateLabel(360, 150, text_music_volume, "MUSIC VOLUME", NULL, this, NULL);
	settings_menu.settings_text = (UI_Label*)App->gui->CreateLabel(555, 50, text_settings, "SETTINGS", NULL, this, NULL);

	//Music Slider
	settings_menu.volume_music_slider = (UI_Slider*)App->gui->CreateSlider(465, 220, Slider_music, { 1834,842,329,27 }, { 1792,926,33,16 }, 400, NULL, this);
	settings_menu.left_music_button = (UI_Button*)App->gui->CreateButton(435, 220, Button_slider_music_left, { 1671,880,26,25 }, { 1738,880,26,25 }, { 1738,880,26,25 }, NULL, this);
	settings_menu.right_music_button = (UI_Button*)App->gui->CreateButton(787, 220, Button_slider_music_right, { 1704,880,26,25 }, { 1771,880,26,25 }, { 1771,880,26,25 }, NULL, this);

	//Fx Slider
	settings_menu.volume_fx_slider = (UI_Slider*)App->gui->CreateSlider(465, 405, Slider_fx, { 1834,842,329,27 }, { 1792,926,33,16 }, 400, NULL, this);
	settings_menu.left_fx_button = (UI_Button*)App->gui->CreateButton(435, 405, Button_slider_fx_left, { 1671,880,26,25 }, { 1738,880,26,25 }, { 1738,880,26,25 }, NULL, this);
	settings_menu.right_fx_button = (UI_Button*)App->gui->CreateButton(796, 405, Button_slider_fx_right, { 1704,880,26,25 }, { 1771,880,26,25 }, { 1771,880,26,25 }, NULL, this);

	//Buttons
	settings_menu.cap_button = (UI_Button*)App->gui->CreateButton(545, 615, button_cap, { 1671,922,25,26 }, { 1713,922,25,26 }, { 1713,922,25,26 }, NULL, this);
	settings_menu.mute_button = (UI_Button*)App->gui->CreateButton(360, 280, button_mute, { 1671,922,25,26 }, { 1713,922,25,26 }, { 1713,922,25,26 }, NULL, this);
	settings_menu.back_button = (UI_Button*)App->gui->CreateButton(40, 40, button_back, { 1704,1054,48,46 }, { 1765,1054,48,46 }, { 1765,1054,48,46 }, NULL, this);


}

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

void MenuManager::CreateCollaboratorPicture()
{
	switch (collaborator) 
	{
	case 'M':
		credits_menu.marc_image = (j1Image*)App->gui->CreateImage(660, 150, Image, { 1230, 1545, 138, 138 }, NULL, this);
		break;
	case 'J':
		credits_menu.javi_image = (j1Image*)App->gui->CreateImage(660, 150, Image, { 1392, 1545, 138, 138 }, NULL, this);
		break;
	case 'P':
		credits_menu.pablo_image = (j1Image*)App->gui->CreateImage(660, 150, Image, { 1563, 1545, 138, 138 }, NULL, this);
		break;
	case 'G':
		credits_menu.german_image = (j1Image*)App->gui->CreateImage(660, 150, Image, { 1719, 1539, 138, 138 }, NULL, this);
		break;
	case 'D':
		credits_menu.macia_image = (j1Image*)App->gui->CreateImage(660, 150, Image, { 1230, 1690, 138, 138 }, NULL, this);
		break;
	case 'K':
		credits_menu.pol_image = (j1Image*)App->gui->CreateImage(660, 150, Image, { 1392, 1690, 138, 138 }, NULL, this);
		break;
	case 'S':
		credits_menu.silvino_image = (j1Image*)App->gui->CreateImage(660, 150, Image, { 1563, 1690, 138, 138 }, NULL, this);
		break;
	case 'C':
		credits_menu.cristian_image = (j1Image*)App->gui->CreateImage(660, 150, Image, { 1719, 1690, 138, 138 }, NULL, this);
		break;
	default:
		credits_menu.marc_image = (j1Image*)App->gui->CreateImage(660, 150, Image, { 1230, 1545, 138, 138 }, NULL, this);
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
	pause_menu.panel = (j1Image*)App->gui->CreateImage(434, 213, Image, { 3079, 41, 334, 313 }, NULL, this);


	//Buttons
	pause_menu.resume_button = (UI_Button*)App->gui->CreateButton(480, 240, resume_button, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,144,64 }, NULL, this);
	pause_menu.settings_button = (UI_Button*)App->gui->CreateButton(480, 330, button_settings_pause, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,144,64 }, NULL, this);
	pause_menu.back_to_main_menu_button = (UI_Button*)App->gui->CreateButton(480, 420, button_pause_to_main, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,144,64 }, NULL, this);

	//Labels
	pause_menu.resume_label = (UI_Label*)App->gui->CreateLabel(540, 258, RESUME, "RESUME", NULL, this, NULL);
	pause_menu.settings_label = (UI_Label*)App->gui->CreateLabel(525, 348, SETTINGS, "SETTINGS", NULL, this, NULL);
	pause_menu.back_to_main_menu_label = (UI_Label*)App->gui->CreateLabel(515, 438, EXIT, "MAIN MENU", NULL, this, NULL);


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
	

}

void MenuManager::CreatePauseSettings()
{

	//Images
	settings_menu.background = (j1Image*)App->gui->CreateImage(-5, 0, Image, { 1529, 0, 1502, 775 }, NULL, this);
	settings_menu.panel_volume = (j1Image*)App->gui->CreateImage(310, 125, Volume_Panel, { 0, 537, 658, 358 }, NULL, this);
	settings_menu.panel_cap = (j1Image*)App->gui->CreateImage(420, 500, Cap_Panel, { 1017, 352, 455, 190 }, NULL, this);
	settings_menu.mute = (j1Image*)App->gui->CreateImage(395, 275, Mute, { 1674, 981, 20, 36 }, NULL, this);
	settings_menu.fps_image = (j1Image*)App->gui->CreateImage(595, 600, FPS, { 1599, 1054, 77, 52 }, NULL, this);

	//Labels
	settings_menu.fps = (UI_Label*)App->gui->CreateLabel(520, 525, text_fps, "REFRESH RATE", NULL, this, NULL);
	settings_menu.fx_volume = (UI_Label*)App->gui->CreateLabel(360, 335, text_fx_volume, "SOUND EFFECTS VOLUME", NULL, this, NULL);
	settings_menu.music_volume = (UI_Label*)App->gui->CreateLabel(360, 150, text_music_volume, "MUSIC VOLUME", NULL, this, NULL);
	settings_menu.settings_text = (UI_Label*)App->gui->CreateLabel(555, 50, text_settings, "SETTINGS", NULL, this, NULL);

	//Music Slider
	settings_menu.volume_music_slider = (UI_Slider*)App->gui->CreateSlider(465, 220, Slider_music, { 1834,842,329,27 }, { 1792,926,33,16 }, 400, NULL, this);
	settings_menu.left_music_button = (UI_Button*)App->gui->CreateButton(435, 220, Button_slider_music_left, { 1671,880,26,25 }, { 1738,880,26,25 }, { 1738,880,26,25 }, NULL, this);
	settings_menu.right_music_button = (UI_Button*)App->gui->CreateButton(796, 220, Button_slider_music_right, { 1704,880,26,25 }, { 1771,880,26,25 }, { 1771,880,26,25 }, NULL, this);

	//Fx Slider
	settings_menu.volume_fx_slider = (UI_Slider*)App->gui->CreateSlider(465, 405, Slider_fx, { 1834,842,329,27 }, { 1792,926,33,16 }, 400, NULL, this);
	settings_menu.left_fx_button = (UI_Button*)App->gui->CreateButton(435, 405, Button_slider_fx_left, { 1671,880,26,25 }, { 1738,880,26,25 }, { 1738,880,26,25 }, NULL, this);
	settings_menu.right_fx_button = (UI_Button*)App->gui->CreateButton(796, 405, Button_slider_fx_right, { 1704,880,26,25 }, { 1771,880,26,25 }, { 1771,880,26,25 }, NULL, this);

	//Buttons
	settings_menu.cap_button = (UI_Button*)App->gui->CreateButton(545, 615, button_cap, { 1671,922,25,26 }, { 1713,922,25,26 }, { 1713,922,25,26 }, NULL, this);
	settings_menu.mute_button = (UI_Button*)App->gui->CreateButton(360, 280, button_mute, { 1671,922,25,26 }, { 1713,922,25,26 }, { 1713,922,25,26 }, NULL, this);
	settings_menu.back_button = (UI_Button*)App->gui->CreateButton(40, 40, button_back_pause, { 1704,1054,48,46 }, { 1765,1054,48,46 }, { 1765,1054,48,46 }, NULL, this);


}

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

void MenuManager::CreateGUI()
{

	//Images
	gui_ingame.nukas_background = (j1Image*)App->gui->CreateImage(0, 0, Nukas, { 1275, 586, 113, 69 }, NULL, this);
	gui_ingame.water_background = (j1Image*)App->gui->CreateImage(114, 0, Water, { 1275, 586, 113, 69 }, NULL, this);
	gui_ingame.meat_background = (j1Image*)App->gui->CreateImage(228, 0, Meat, { 1275, 586, 113, 69 }, NULL, this);

	gui_ingame.nukas = (j1Image*)App->gui->CreateImage(15, 30, Nukas, { 1285, 678, 29, 30 }, NULL, this);
	gui_ingame.water = (j1Image*)App->gui->CreateImage(128, 30, Water, { 1329, 676, 15, 30 }, NULL, this);
	gui_ingame.meat = (j1Image*)App->gui->CreateImage(240, 35, Meat, { 1356, 682, 30, 20 }, NULL, this);

	gui_ingame.nukas_label = (j1Image*)App->gui->CreateImage(20, 8, Nukas, { 1285, 725, 43, 14 }, NULL, this);
	gui_ingame.water_label = (j1Image*)App->gui->CreateImage(135, 8, Water, { 1340, 727, 43, 14 }, NULL, this);
	gui_ingame.meat_label = (j1Image*)App->gui->CreateImage(253, 8, Meat, { 1407, 727, 34, 14 }, NULL, this);

	if (gui_ingame.count == 0) {
		
		p2SString cc("%i", App->player->caps);
		p2SString ww("%i", App->player->water);
		p2SString ff("%i", App->player->food);
		
		gui_ingame.nukas_count = (UI_Label*)App->gui->CreateLabel(82, 29, Nukas, cc, NULL, this, NULL);
		gui_ingame.water_count = (UI_Label*)App->gui->CreateLabel(193, 29, Water, ww, NULL, this, NULL);
		gui_ingame.meat_count = (UI_Label*)App->gui->CreateLabel(307, 29, Meat, ff, NULL, this, NULL);

		gui_ingame.count++;
	}

	//Labels
	//settings_menu.fps = (UI_Label*)App->gui->CreateLabel(520, 525, text_fps, "REFRESH RATE", NULL, this, NULL);
	//settings_menu.fx_volume = (UI_Label*)App->gui->CreateLabel(360, 335, text_fx_volume, "SOUND EFFECTS VOLUME", NULL, this, NULL);
	//settings_menu.music_volume = (UI_Label*)App->gui->CreateLabel(360, 150, text_music_volume, "MUSIC VOLUME", NULL, this, NULL);

}

void MenuManager::DestroyGUI()
{

	//Destroying images
	App->gui->Delete_Element(gui_ingame.nukas);
	App->gui->Delete_Element(gui_ingame.water);
	App->gui->Delete_Element(gui_ingame.meat);
	App->gui->Delete_Element(gui_ingame.nukas_background);
	App->gui->Delete_Element(gui_ingame.water_background);
	App->gui->Delete_Element(gui_ingame.meat_background);

	//Destroying Labels
	//App->gui->Delete_Element(settings_menu.fps);
	//App->gui->Delete_Element(settings_menu.music_volume);
	//App->gui->Delete_Element(settings_menu.fx_volume);

}