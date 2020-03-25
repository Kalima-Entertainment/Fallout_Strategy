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
#include "MainMenu.h"

MainMenu::MainMenu()
{
	name.create("main_menu");
}

MainMenu::~MainMenu() {}

bool MainMenu::Awake(pugi::xml_node& config)
{
	

	return true;
}

bool MainMenu::Start()
{
	App->audio->PlayMusic("audio/music/FalloutStrategyMainTheme.ogg", 4.0F);
	App->menu_manager->CreateMainMenu();

	return true;
}

bool MainMenu::Update(float dt)
{
	


	return true;
}

bool MainMenu::CleanUp()
{


	return true;
}