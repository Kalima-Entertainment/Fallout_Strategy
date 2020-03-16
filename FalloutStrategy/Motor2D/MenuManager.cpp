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
	credits_menu.web_button = (UI_Button*)App->gui->CreateButton(350, 550, button_twitter_credits, { 2355,841,73,78 }, { 2355,919,73,78 }, { 2355,1005,73,78 }, NULL, this);
}

void MenuManager::DestroyCredits()
{
	//Destroying Images
	App->gui->Delete_Element(credits_menu.background);
	
	//Destroying Buttons
	App->gui->Delete_Element(credits_menu.back_button);
	App->gui->Delete_Element(credits_menu.github_button);
	App->gui->Delete_Element(credits_menu.web_button);
}