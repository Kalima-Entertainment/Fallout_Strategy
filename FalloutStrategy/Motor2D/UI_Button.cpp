#include "p2Log.h"
#include "UI_Button.h"
#include "j1Gui.h"
#include "UI_element.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1EntityManager.h"
#include "j1Entity.h"
#include "j1Pathfinding.h"
#include "MenuManager.h"
#include "j1Player.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Window.h"
#include "StaticEntity.h"
#include "j1Transition.h"
#include "j1Minimap.h"

#include "./brofiler/Brofiler/Brofiler.h"
#include "SDL_mixer\include\SDL_mixer.h"

UI_Button::UI_Button(int x, int y, UI_Type type, SDL_Rect idle, SDL_Rect hover, SDL_Rect click, UI_element* parent, j1Module* Observer) : UI_element(x, y, type, parent, Observer)
{
	this->pos.x = x;
	this->pos.y = y;
	this->Button_idle = idle;
	this->Button_hover = hover;
	this->Button_click = click;

	t = type;

	dimensions = idle;

	faction_image = nullptr;

	brotherhood_rect = { 2600, 2351, 580, 185 };
	vault_rect = { 2600, 2886, 656, 180 };
	ghoul_rect = { 2600, 835, 591, 180 };
	supermutant_rect = { 2600, 2649, 602, 181 };

	current_state = BUTTON_STATE::NONE;

	inHover = true;
}

bool UI_Button::CleanUp()
{
	return true;
}

bool UI_Button::Draw()
{
	texture = App->gui->GetAtlas();

	if (texture != nullptr)
	{
		App->render->Blit_UI(texture, pos.x, pos.y, &dimensions, SDL_FLIP_NONE, 0.0f);
	}

	if (App->gui->debug_UI == true) {

		SDL_Rect b{ pos.x, pos.y, Button_click.w, Button_click.h };

		App->render->DrawQuad(b, 255, 0, 0, 255, false, 0.0f);
	}

	return true;
}

bool UI_Button::Update(float dt)
{
	//if cursor is inside button rectangle
	if (IsIntersection() == true) {
		
		counter++;

		//rect is button hover
		dimensions = Button_hover;
		//LOG("%i", counter);

		//only plays fx once
		if (counter == 1) {
			App->audio->PlayFx(1, App->audio->hover_fx, 0);
		}


		if (t == button_select_ghoul)
		{
			if (inHover) {
				faction_image = (j1Image*)App->gui->CreateImage(510, 300, Image, ghoul_rect, NULL, this);
				current_state = BUTTON_STATE::HOVER;
				inHover = !inHover;
			}
		}
		else if (t == button_select_vault)
		{
			if (inHover) {
				faction_image = (j1Image*)App->gui->CreateImage(510, 300, Image, vault_rect, NULL, this);
				current_state = BUTTON_STATE::HOVER;
				inHover = !inHover;
			}
		}
		else if (t == button_select_brotherhood)
		{
			if (inHover) {
				faction_image = (j1Image*)App->gui->CreateImage(510, 300, Image, brotherhood_rect, NULL, this);
				current_state = BUTTON_STATE::HOVER;
				inHover = !inHover;
			}
		}
		else if (t == button_select_supermutant)
		{
			if (inHover) {
				faction_image = (j1Image*)App->gui->CreateImage(510, 300, Image, supermutant_rect, NULL, this);
				current_state = BUTTON_STATE::HOVER;
				inHover = !inHover;
			}
		}

		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT)==KEY_DOWN) {

			//rect is button pressed
			dimensions = Button_click;
			
			//only plays fx once
						
			if (observer) {
				observer->Callback(this);
			}

			if (t == button_new_game)
			{
				App->menu_manager->DestroyMenu(Menu::MAIN_MENU);
				App->menu_manager->CreateMenu(Menu::SELECT_FACTION);
				App->audio->PlayFx(1, App->audio->click_fx, 0);
			}
			else if (t == Button_slider_music_left) {
				App->gui->volume_up = 1;
				App->audio->PlayFx(1, App->audio->volume_fx, 0);
			}
			else if (t == Button_slider_music_right) {
				App->gui->volume_up = 2;
				App->audio->PlayFx(1, App->audio->volume_fx, 0);
			}
			else if (t == Button_slider_fx_left) {
				App->gui->fx_up = 1;
				App->audio->PlayFx(1, App->audio->volume_fx, 0);
			}
			else if (t == Button_slider_fx_right) {
				App->gui->fx_up = 2;
				App->audio->PlayFx(1, App->audio->volume_fx, 0);
			}
			else if (t == button_github_credits)
			{
				ShellExecuteA(NULL, "open", "https://github.com/Kalima-Entertainment/Fallout_Strategy", NULL, NULL, SW_SHOWNORMAL);
				App->audio->PlayFx(1, App->audio->click_fx, 0);
			}
			else if (t == button_twitter_credits)
			{
				ShellExecuteA(NULL, "open", "https://twitter.com/KalimaEntmt", NULL, NULL, SW_SHOWNORMAL);
				App->audio->PlayFx(1, App->audio->click_fx, 0);
			}
			else if (t == button_web_credits)
			{
				ShellExecuteA(NULL, "open", "https://kalima-entertainment.github.io/Fallout_Strategy/", NULL, NULL, SW_SHOWNORMAL);
				App->audio->PlayFx(1, App->audio->click_fx, 0);
			}
			else if (t == button_credits)
			{
				App->menu_manager->DestroyMenu(Menu::MAIN_MENU);
				App->menu_manager->CreateMenu(Menu::CREDITS);
				App->audio->PlayFx(1, App->audio->click_fx, 0);
			}
			else if (t == button_back_credits)
			{
				App->menu_manager->CreateMenu(Menu::MAIN_MENU);
				App->menu_manager->DestroyMenu(Menu::CREDITS);
				App->audio->PlayFx(1, App->audio->back_fx, 0);
			}
			else if (t == button_exit)
			{
				App->quitGame = true;
				App->audio->PlayFx(1, App->audio->click_fx, 0);
			}
			else if (t == button_settings) {

				App->menu_manager->DestroyMenu(Menu::MAIN_MENU);
				App->menu_manager->CreateMenu(Menu::SETTINGS);
				App->audio->PlayFx(1, App->audio->click_fx, 0);
			}
			else if (t == button_settings_pause) {

				App->menu_manager->DestroyMenu(Menu::PAUSE_MENU);
				App->menu_manager->CreateMenu(Menu::PAUSE_SETTINGS);
				App->audio->PlayFx(1, App->audio->click_fx, 0);
			}
			else if (t == button_back) {
				App->menu_manager->DestroyMenu(Menu::SETTINGS);
				App->menu_manager->CreateMenu(Menu::MAIN_MENU);
				
				App->audio->PlayFx(1, App->audio->back_fx, 0);
			}
			else if (t == button_back_pause) {
				App->menu_manager->DestroyMenu(Menu::PAUSE_SETTINGS);
				App->menu_manager->CreateMenu(Menu::PAUSE_MENU);
				App->audio->PlayFx(1, App->audio->back_fx, 0);
			}
			else if (t == resume_button) {
				App->menu_manager->DestroyMenu(Menu::PAUSE_MENU);
				App->scene->create = !App->scene->create;
				App->audio->PlayFx(1, App->audio->back_fx, 0);
			}
			else if (t == button_back_to_menu)
			{
				App->menu_manager->CreateMenu(Menu::MAIN_MENU);
				App->menu_manager->DestroyMenu(Menu::SELECT_FACTION);
				App->audio->PlayFx(1, App->audio->back_fx, 0);
				App->isPaused = false;
			}
			else if (t == button_pause_to_main)
			{
				App->menu_manager->DestroyMenu(Menu::PAUSE_MENU);
				App->menu_manager->CreateMenu(Menu::MAIN_MENU);
				App->audio->PlayFx(1, App->audio->back_fx, 0);
				App->gui->ingame = false;
				App->transition->StartTimer();
				App->transition->transition = true;
				App->transition->fadetimer.Start();
			}
			else if (t == button_cap) {
				
				App->gui->cap = !App->gui->cap;
				App->gui->Delete_Element(App->menu_manager->settings_menu[21]);

				if (App->gui->cap == false) {
					App->capped_ms = 1000 / 60;
					App->gui->Delete_Element(App->menu_manager->cap_label);
					App->menu_manager->cap_label = (UI_Label*)App->gui->CreateLabel(528, 613, Label, "60", NULL, this, NULL, "StackedPixelMedium");
				}
				else if (App->gui->cap == true) {
					App->capped_ms = 1000 / 30;
					App->gui->Delete_Element(App->menu_manager->cap_label);
					App->menu_manager->cap_label = (UI_Label*)App->gui->CreateLabel(528, 613, Label, "30", NULL, this, NULL, "StackedPixelMedium");
				}

			}
			else if (t == button_fullscreen) {
				
				App->gui->fullscreen = !App->gui->fullscreen;

				App->gui->Delete_Element(App->menu_manager->settings_menu[22]);

				if (App->gui->fullscreen == true) {
					App->win->ChangeFullScreen(true);
					App->gui->Delete_Element(App->menu_manager->fullscreen_label);
					App->menu_manager->fullscreen_label = (UI_Label*)App->gui->CreateLabel(756, 613, Label, "YES", NULL, this, NULL, "StackedPixelMedium");
				}
				else if (App->gui->fullscreen == false) {
					App->win->ChangeFullScreen(false);
					App->gui->Delete_Element(App->menu_manager->fullscreen_label);
					App->menu_manager->fullscreen_label = (UI_Label*)App->gui->CreateLabel(763, 613, Label, "NO", NULL, this, NULL, "StackedPixelMedium");
				}

			}
			else if (t == button_win_lose_to_main)
			{
				App->menu_manager->DestroyMenu(Menu::WIN_LOSE_SCENE);
				App->menu_manager->CreateMenu(Menu::MAIN_MENU);
				App->audio->PlayFx(1, App->audio->back_fx, 0);
				App->gui->ingame = false;
				App->transition->StartTimer();
				App->transition->transition = true;
				App->transition->fadetimer.Start();
				
			}
			//Spawn Gatherer from any faction
			else if (t == Ghouls_ghaterer_button || t == Vault_ghaterer_button || t == Supermutant_ghaterer_button || t == Brotherhood_ghaterer_button){
				App->audio->PlayFx(1, App->audio->character_fx, 0);
				
				//Select building to spawn
				StaticEntity* static_entity;
				if (App->player->selected_entity == nullptr)
					static_entity = (StaticEntity*)App->player->last_selected_entity;
				else
					static_entity = (StaticEntity*)App->player->selected_entity;
				static_entity->SpawnUnit(GATHERER);
			}
			//Spawn Melee from any faction
			else if (t == Ghouls_melee_button || t == Vault_melee_button || t == Supermutant_melee_button || t == Brotherhood_melee_button){
				App->audio->PlayFx(1, App->audio->character_fx, 0);

				StaticEntity* static_entity;
				if (App->player->selected_entity == nullptr)
					static_entity = (StaticEntity*)App->player->last_selected_entity;
				else
				static_entity = (StaticEntity*)App->player->selected_entity;

				static_entity->SpawnUnit(MELEE);
			}
			//Spawn Ranged from any faction
			else if (t == Ghouls_ranged_button || t == Vault_ranged_button || t == Supermutant_ranged_button || t == Brotherhood_ranged_button){
				App->audio->PlayFx(1, App->audio->character_fx, 0);

				StaticEntity* static_entity;
				if (App->player->selected_entity == nullptr)
					static_entity = (StaticEntity*)App->player->last_selected_entity;
				else
				static_entity = (StaticEntity*)App->player->selected_entity;

				static_entity->SpawnUnit(RANGED);
			}
			else if (t == button_select_ghoul)
			{
				App->gui->Delete_Element(faction_image);
				App->menu_manager->DestroyMenu(Menu::SELECT_FACTION);
				current_state = BUTTON_STATE::HOVER_EXIT;
				App->player->faction = GHOUL;
				App->audio->PlayFx(1, App->audio->character_fx, 0);
				App->gui->count = 0;
				App->gui->ingame = true;
				App->menu_manager->CreateMenu(Menu::GUI);
				App->menu_manager->CreateMenu(Menu::RESOURCES);
				App->transition->fadetimer.Start();
				App->transition->transition = true;
				App->entities->Enable();
							
			}
			else if (t == button_select_vault)
			{
				App->gui->Delete_Element(faction_image);
				App->menu_manager->DestroyMenu(Menu::SELECT_FACTION);
				App->player->faction = VAULT;
				App->audio->PlayFx(1, App->audio->character_fx, 0);
				App->gui->count = 0;
				App->gui->ingame = true;
				App->menu_manager->CreateMenu(Menu::GUI);
				App->menu_manager->CreateMenu(Menu::RESOURCES);
				App->transition->fadetimer.Start();
				App->transition->transition = true;
				App->entities->Enable();
			}
			else if (t == button_select_brotherhood)
			{
				App->gui->Delete_Element(faction_image);
				App->menu_manager->DestroyMenu(Menu::SELECT_FACTION);
				App->player->faction = BROTHERHOOD;
				App->audio->PlayFx(1, App->audio->character_fx, 0);
				App->gui->count = 0;
				App->gui->ingame = true;
				App->menu_manager->CreateMenu(Menu::GUI);
				App->menu_manager->CreateMenu(Menu::RESOURCES);
				App->transition->fadetimer.Start();
				App->transition->transition = true;				
				App->entities->Enable();
			}
			else if (t == button_select_supermutant)
			{
				App->gui->Delete_Element(faction_image);
				App->menu_manager->DestroyMenu(Menu::SELECT_FACTION);
				App->player->faction = MUTANT;
				App->audio->PlayFx(1, App->audio->character_fx, 0);
				App->gui->count = 0;
				App->gui->ingame = true;
				App->menu_manager->CreateMenu(Menu::GUI);
				App->menu_manager->CreateMenu(Menu::RESOURCES);
				App->transition->fadetimer.Start();
				App->transition->transition = true;
				App->entities->Enable();
			}
			else if (t == Boost_base_button)
			{
				StaticEntity* static_entity;
				if (App->player->selected_entity == nullptr) {
					static_entity = (StaticEntity*)App->player->last_selected_entity;
				}
				else{
					static_entity = (StaticEntity*)App->player->selected_entity;
				}
				static_entity->Upgrade(App->entities->base_resource_limit[static_entity->faction]);
				static_entity->Upgrade(App->entities->gatherer_resource_limit[static_entity->faction]);
			}
			else if (t == Boost_barrack_button)
			{
				StaticEntity* static_entity;
				if (App->player->selected_entity == nullptr) {
					static_entity = (StaticEntity*)App->player->last_selected_entity;
				}
				else {
					static_entity = (StaticEntity*)App->player->selected_entity;
				}
				static_entity->Upgrade(App->entities->units_damage[static_entity->faction]);
				static_entity->Upgrade(App->entities->units_speed[static_entity->faction]);
			}
			if (t == Boost_lab_button)
			{
				StaticEntity* static_entity;
				if (App->player->selected_entity == nullptr) {
					static_entity = (StaticEntity*)App->player->last_selected_entity;
				}
				else {
					static_entity = (StaticEntity*)App->player->selected_entity;
				}
				static_entity->Upgrade(App->entities->units_health[static_entity->faction]);
				static_entity->Upgrade(App->entities->units_creation_time[static_entity->faction]);
			}
			else if (t == Boost_radar_button) {
				App->minimap->EnableRadar();
			}
			 			 		  		  		 	   		
		}

		else {
			
			App->gui->volume_up = 0;
			App->gui->fx_up = 0;
		}

	}

	else {
		dimensions = Button_idle;
		counter = 0;
		current_state = BUTTON_STATE::HOVER_EXIT;
	}

	if (current_state == BUTTON_STATE::HOVER_EXIT) {
		App->gui->Delete_Element(faction_image);
		inHover = !inHover;
	}



	return true;

}

void UI_Button::DoNotShowElement(UI_element* element)
{
	element->drawable = false;
}
