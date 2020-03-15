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

	main_menu.background = (j1Image*)App->gui->CreateImage(0, 0, Image, { 2122, 0, 1479, 1077 }, NULL, this);
	main_menu.new_game_button = (UI_Button*)App->gui->CreateButton(700, 200, button_new_game, { 2218,1159,75,84 }, { 2218,1253,75,84 }, { 2218,1346,75,84 }, NULL, this);
}

void MenuManager::DestroyMainMenu()
{
	App->gui->Delete_Element(main_menu.background);
	App->gui->Delete_Element(main_menu.new_game_button);
}