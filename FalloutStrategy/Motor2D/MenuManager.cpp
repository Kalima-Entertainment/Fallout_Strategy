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
	main_menu.background = (j1Image*)App->gui->CreateImage(-5, 0, Image, { 1526, 0, 1075, 783 }, NULL, this);
	main_menu.logo = (j1Image*)App->gui->CreateImage(40, 40, Logo, { 938, 903, 505, 233 }, NULL, this);
	main_menu.panel = (j1Image*)App->gui->CreateImage(570, 260, Panel, { 0, 0, 416, 448 }, NULL, this);
	
	//Labels
	main_menu.new_game_text = (UI_Label*)App->gui->CreateLabel(700, 300, text_new_game, "NEW GAME", NULL, this, NULL);
	main_menu.load_game_text = (UI_Label*)App->gui->CreateLabel(700, 377, text_load_game, "LOAD GAME", NULL, this, NULL);
	main_menu.settings_text = (UI_Label*)App->gui->CreateLabel(700, 454, text_settings, "SETTINGS", NULL, this, NULL);
	main_menu.credits_text = (UI_Label*)App->gui->CreateLabel(700, 531, text_credits, "CREDITS", NULL, this, NULL);
	main_menu.exit_text = (UI_Label*)App->gui->CreateLabel(700, 608, text_exit, "EXIT", NULL, this, NULL);

	//Buttons
	main_menu.new_game_button = (UI_Button*)App->gui->CreateButton(630, 295, button_new_game, { 1596,834,60,62 }, { 1596,901,60,62 }, { 1596,967,60,62 }, NULL, this);
	main_menu.load_game_button = (UI_Button*)App->gui->CreateButton(630, 373, button_load_game, { 1596,834,60,62 }, { 1596,901,60,62 }, { 1596,967,60,62 }, NULL, this);
	main_menu.settings_button = (UI_Button*)App->gui->CreateButton(630, 450, button_settings, { 1596,834,60,62 }, { 1596,901,60,62 }, { 1596,967,60,62 }, NULL, this);
	main_menu.credits_button = (UI_Button*)App->gui->CreateButton(630, 527, button_credits, { 1596,834,60,62 }, { 1596,901,60,62 }, { 1596,967,60,62 }, NULL, this);
	main_menu.exit_button = (UI_Button*)App->gui->CreateButton(630, 604, button_exit, { 1596,834,60,62 }, { 1596,901,60,62 }, { 1596,967,60,62 }, NULL, this);
}

void MenuManager::DestroyMainMenu()
{

	App->player->Enable();

	//Destroying images
	App->gui->Delete_Element(main_menu.background);
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
	settings_menu.background = (j1Image*)App->gui->CreateImage(-5, 0, Image, { 1526, 0, 1075, 783 }, NULL, this);
	settings_menu.panel_volume = (j1Image*)App->gui->CreateImage(190, 125, Volume_Panel, { 0, 537, 658, 358 }, NULL, this);
	settings_menu.panel_cap = (j1Image*)App->gui->CreateImage(300, 500, Cap_Panel, { 1017, 352, 455, 190 }, NULL, this);
	settings_menu.mute = (j1Image*)App->gui->CreateImage(275, 275, Mute, { 1674, 981, 20, 36 }, NULL, this);
	settings_menu.fps_image = (j1Image*)App->gui->CreateImage(475, 600, FPS, { 1599, 1054, 77, 52 }, NULL, this);

	//Labels
	settings_menu.fps = (UI_Label*)App->gui->CreateLabel(400, 525, text_fps, "REFRESH RATE", NULL, this, NULL);
	settings_menu.fx_volume = (UI_Label*)App->gui->CreateLabel(240, 335, text_fx_volume, "SOUND EFFECTS VOLUME", NULL, this, NULL);
	settings_menu.music_volume = (UI_Label*)App->gui->CreateLabel(240, 150, text_music_volume, "MUSIC VOLUME", NULL, this, NULL);
	settings_menu.settings_text = (UI_Label*)App->gui->CreateLabel(435, 50, text_settings, "SETTINGS", NULL, this, NULL);

	//Music Slider
	settings_menu.volume_music_slider = (UI_Slider*)App->gui->CreateSlider(345, 220, Slider_music, { 1834,842,329,27 }, { 1792,926,33,16 }, 400, NULL, this);
	settings_menu.left_music_button = (UI_Button*)App->gui->CreateButton(315, 220, Button_slider_music_left, { 1671,880,26,25 }, { 1738,880,26,25 }, { 1738,880,26,25 }, NULL, this);
	settings_menu.right_music_button = (UI_Button*)App->gui->CreateButton(676, 220, Button_slider_music_right, { 1704,880,26,25 }, { 1771,880,26,25 }, { 1771,880,26,25 }, NULL, this);

	//Fx Slider
	settings_menu.volume_fx_slider = (UI_Slider*)App->gui->CreateSlider(345, 405, Slider_fx, { 1834,842,329,27 }, { 1792,926,33,16 }, 400, NULL, this);
	settings_menu.left_fx_button = (UI_Button*)App->gui->CreateButton(315, 405, Button_slider_fx_left, { 1671,880,26,25 }, { 1738,880,26,25 }, { 1738,880,26,25 }, NULL, this);
	settings_menu.right_fx_button = (UI_Button*)App->gui->CreateButton(676, 405, Button_slider_fx_right, { 1704,880,26,25 }, { 1771,880,26,25 }, { 1771,880,26,25 }, NULL, this);

	//Buttons
	settings_menu.cap_button = (UI_Button*)App->gui->CreateButton(425, 615, button_cap, { 1671,922,25,26 }, { 1713,922,25,26 }, { 1713,922,25,26 }, NULL, this);
	settings_menu.mute_button = (UI_Button*)App->gui->CreateButton(240, 280, button_mute, { 1671,922,25,26 }, { 1713,922,25,26 }, { 1713,922,25,26 }, NULL, this);
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
	select_faction_menu.background = (j1Image*)App->gui->CreateImage(-5, 0, Image, { 1900, 1531, 1240, 842 }, NULL, this);
	select_faction_menu.ghouls = (j1Image*)App->gui->CreateImage(100, 300, Image, { 2492, 837, 168, 166 }, NULL, this);
	select_faction_menu.vault = (j1Image*)App->gui->CreateImage(310, 300, Image, { 2492, 1014, 177, 140 }, NULL, this);
	select_faction_menu.brotherhood = (j1Image*)App->gui->CreateImage(550, 300, Image, { 2492, 1161, 155, 180 }, NULL, this);
	select_faction_menu.supermutants = (j1Image*)App->gui->CreateImage(750, 300, Image, { 2492, 1354, 158, 158 }, NULL, this);

	//Buttons
	select_faction_menu.ghouls_button = (UI_Button*)App->gui->CreateButton(150, 200, button_select_ghoul, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,144,64 }, NULL, this);
	select_faction_menu.vault_button = (UI_Button*)App->gui->CreateButton(150, 300, button_select_vault, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,144,64 }, NULL, this);
	select_faction_menu.brotherhood_button = (UI_Button*)App->gui->CreateButton(150, 400, button_select_brotherhood, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,144,64 }, NULL, this);
	select_faction_menu.supermutants_button = (UI_Button*)App->gui->CreateButton(150, 500, button_select_supermutant, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,144,64 }, NULL, this);

	//Labels
	select_faction_menu.ghouls_label = (UI_Label*)App->gui->CreateLabel(230, 215, ghouls_label, "GHOUL", NULL, this, NULL);
	select_faction_menu.vault_label = (UI_Label*)App->gui->CreateLabel(228, 315, ghouls_label, "VAULT", NULL, this, NULL);
	select_faction_menu.brotherhood_label = (UI_Label*)App->gui->CreateLabel(170, 415, ghouls_label, "BROTHERHOOD", NULL, this, NULL);
	select_faction_menu.supermutants_label = (UI_Label*)App->gui->CreateLabel(164, 515, ghouls_label, "SUPERMUTANT", NULL, this, NULL);
}

void MenuManager::DestroySelectFaction()
{

	App->gui->Delete_Element(select_faction_menu.background);

}

void MenuManager::CreatePauseMenu()
{

	//Images
	pause_menu.panel = (j1Image*)App->gui->CreateImage(434, 213, Image, { 887, 0, 413, 294 }, NULL, this);


	//Buttons
	pause_menu.resume_button = (UI_Button*)App->gui->CreateButton(450, 240, resume_button, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,144,64 }, NULL, this);
	pause_menu.settings_button = (UI_Button*)App->gui->CreateButton(450, 330, button_settings_pause, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,144,64 }, NULL, this);
	pause_menu.exit_button = (UI_Button*)App->gui->CreateButton(450, 420, button_exit, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,144,64 }, NULL, this);

	//Labels
	pause_menu.resume_label = (UI_Label*)App->gui->CreateLabel(517, 258, RESUME, "RESUME", NULL, this, NULL);
	pause_menu.settings_label = (UI_Label*)App->gui->CreateLabel(495, 348, SETTINGS, "SETTINGS", NULL, this, NULL);
	pause_menu.exit_label = (UI_Label*)App->gui->CreateLabel(537, 438, EXIT, "EXIT", NULL, this, NULL);


}

void MenuManager::DestroyPauseMenu()
{

	//Destroying images
	App->gui->Delete_Element(pause_menu.panel);
	

	//Destroying buttons
	App->gui->Delete_Element(pause_menu.resume_button);
	App->gui->Delete_Element(pause_menu.settings_button);
	App->gui->Delete_Element(pause_menu.exit_button);

	//Destroying Labels
	App->gui->Delete_Element(pause_menu.resume_label);
	App->gui->Delete_Element(pause_menu.settings_label);
	App->gui->Delete_Element(pause_menu.exit_label);
	

}

void MenuManager::CreatePauseSettings()
{

	//Images
	settings_menu.background = (j1Image*)App->gui->CreateImage(-5, 0, Image, { 1526, 0, 1075, 783 }, NULL, this);
	settings_menu.panel_volume = (j1Image*)App->gui->CreateImage(190, 125, Volume_Panel, { 0, 537, 658, 358 }, NULL, this);
	settings_menu.panel_cap = (j1Image*)App->gui->CreateImage(300, 500, Cap_Panel, { 1017, 352, 455, 190 }, NULL, this);
	settings_menu.mute = (j1Image*)App->gui->CreateImage(275, 275, Mute, { 1674, 981, 20, 36 }, NULL, this);
	settings_menu.fps_image = (j1Image*)App->gui->CreateImage(475, 600, FPS, { 1599, 1054, 77, 52 }, NULL, this);

	//Labels
	settings_menu.fps = (UI_Label*)App->gui->CreateLabel(400, 525, text_fps, "REFRESH RATE", NULL, this, NULL);
	settings_menu.fx_volume = (UI_Label*)App->gui->CreateLabel(240, 335, text_fx_volume, "SOUND EFFECTS VOLUME", NULL, this, NULL);
	settings_menu.music_volume = (UI_Label*)App->gui->CreateLabel(240, 150, text_music_volume, "MUSIC VOLUME", NULL, this, NULL);
	settings_menu.settings_text = (UI_Label*)App->gui->CreateLabel(435, 50, text_settings, "SETTINGS", NULL, this, NULL);

	//Music Slider
	settings_menu.volume_music_slider = (UI_Slider*)App->gui->CreateSlider(345, 220, Slider_music, { 1834,842,329,27 }, { 1792,926,33,16 }, 400, NULL, this);
	settings_menu.left_music_button = (UI_Button*)App->gui->CreateButton(315, 220, Button_slider_music_left, { 1671,880,26,25 }, { 1738,880,26,25 }, { 1738,880,26,25 }, NULL, this);
	settings_menu.right_music_button = (UI_Button*)App->gui->CreateButton(676, 220, Button_slider_music_right, { 1704,880,26,25 }, { 1771,880,26,25 }, { 1771,880,26,25 }, NULL, this);

	//Fx Slider
	settings_menu.volume_fx_slider = (UI_Slider*)App->gui->CreateSlider(345, 405, Slider_fx, { 1834,842,329,27 }, { 1792,926,33,16 }, 400, NULL, this);
	settings_menu.left_fx_button = (UI_Button*)App->gui->CreateButton(315, 405, Button_slider_fx_left, { 1671,880,26,25 }, { 1738,880,26,25 }, { 1738,880,26,25 }, NULL, this);
	settings_menu.right_fx_button = (UI_Button*)App->gui->CreateButton(676, 405, Button_slider_fx_right, { 1704,880,26,25 }, { 1771,880,26,25 }, { 1771,880,26,25 }, NULL, this);

	//Buttons
	settings_menu.cap_button = (UI_Button*)App->gui->CreateButton(425, 615, button_cap, { 1671,922,25,26 }, { 1713,922,25,26 }, { 1713,922,25,26 }, NULL, this);
	settings_menu.mute_button = (UI_Button*)App->gui->CreateButton(240, 280, button_mute, { 1671,922,25,26 }, { 1713,922,25,26 }, { 1713,922,25,26 }, NULL, this);
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