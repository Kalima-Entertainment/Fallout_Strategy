#include "p2Log.h"
#include "UI_Button.h"
#include "j1Gui.h"
#include "UI_element.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1EntityManager.h"
#include "j1Entity.h"
#include "j1Pathfinding.h"
#include "j1EasingAndSplines.h"
#include "MenuManager.h"
#include "j1Player.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Window.h"
#include "StaticEntity.h"
#include "j1Transition.h"
#include "j1Minimap.h"
#include "j1Hud.h"
#include "DialogManager.h"

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
	counter = 0;
	
}

UI_Button::~UI_Button() {
	texture = nullptr;
	observer = nullptr;
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
		else if ((t == OPTION_A) || (t == OPTION_B) || (t == OPTION_C)) {
			if (inHover) {
				App->render->DrawQuad({ pos.x, pos.y, 1135, 65 }, 255, 255, 255, 255, true, false);
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
			else if (t == button_load_game) {

				App->menu_manager->DestroyMenu(Menu::MAIN_MENU);
				App->audio->PlayFx(1, App->audio->click_fx, 0);
				App->player->faction = BROTHERHOOD;
				App->gui->count = 0;
				App->gui->ingame = true;
				App->gui->load = true;
				App->transition->fadetimer.Start();
				App->transition->transition = true;
				App->menu_manager->CreateMenu(Menu::GUI);
				App->menu_manager->CreateMenu(Menu::RESOURCES);
				App->hud->activateTimer = true;
				App->isPaused = false;
				App->entities->Enable();
				App->scene->load_game = true;
				//App->LoadGame("save_file.xml");
				
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
				App->menu_manager->DestroyMenu(Menu::CREDITS);
				App->menu_manager->CreateMenu(Menu::MAIN_MENU);
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
				App->hud->activateTimer = true;
				Mix_PauseMusic();
				App->isPaused = false;
			}
			else if (t == button_back_to_menu)
			{
				App->menu_manager->DestroyMenu(Menu::SELECT_FACTION);
				App->menu_manager->CreateMenu(Menu::MAIN_MENU);
				App->audio->PlayFx(1, App->audio->back_fx, 0);
				App->isPaused = false;
				Mix_PauseMusic();
			}
			else if (t == button_pause_to_main)
			{
				App->menu_manager->DestroyMenu(Menu::PAUSE_MENU);
				App->menu_manager->DestroyMenu(Menu::RESOURCES);
				App->menu_manager->DestroyMenu(Menu::TUTORIAL);
				App->menu_manager->DestroyMenu(Menu::GUI);
				App->menu_manager->DestroyMenu(Menu::QUEST);
				if ((App->player->selected_entity) && (!App->player->selected_entity->is_dynamic)) {
					App->menu_manager->DestroyFaction(Menu::BUI_BASES, App->player->selected_entity->faction, App->player->selected_entity->type);
				}
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

				App->minimap->CleanUp();
				App->minimap->Start();
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

				App->menu_manager->DestroyMenu(Menu::GATHERER_QUANTITY);
				App->menu_manager->CreateMenu(Menu::GATHERER_QUANTITY);

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

				App->menu_manager->DestroyMenu(Menu::MELEE_QUANTITY);
				App->menu_manager->CreateMenu(Menu::MELEE_QUANTITY);
		

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

				App->menu_manager->DestroyMenu(Menu::RANGED_QUANTITY);
				App->menu_manager->CreateMenu(Menu::RANGED_QUANTITY);
				

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
				/*static_entity->level++;*/
				if (App->player->faction == BROTHERHOOD) {
					App->gui->Delete_Element(App->menu_manager->brotherhood_base[5]);
					App->menu_manager->brotherhood_base[5] = (UI_Label*)App->gui->CreateLabel(750, 656, Label, std::to_string(App->menu_manager->cost * 2), NULL, this, NULL, "StackedPixelXS");
				}
				else if (App->player->faction == VAULT) {
					App->gui->Delete_Element(App->menu_manager->vault_base[5]);
					App->menu_manager->vault_base[5] = (UI_Label*)App->gui->CreateLabel(750, 656, Label, std::to_string(App->menu_manager->cost * 2), NULL, this, NULL, "StackedPixelXS");
				}
				else if (App->player->faction == MUTANT) {
					App->gui->Delete_Element(App->menu_manager->supermutant_base[5]);
					App->menu_manager->supermutant_base[5] = (UI_Label*)App->gui->CreateLabel(750, 656, Label, std::to_string(App->menu_manager->cost * 2), NULL, this, NULL, "StackedPixelXS");
				}
				else if (App->player->faction == GHOUL) {
					App->gui->Delete_Element(App->menu_manager->ghoul_base[5]);
					App->menu_manager->ghoul_base[5] = (UI_Label*)App->gui->CreateLabel(750, 656, Label, std::to_string(App->menu_manager->cost * 2), NULL, this, NULL, "StackedPixelXS");
				}
			
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
				//static_entity->level++;
				if (App->player->faction == BROTHERHOOD) {
					App->gui->Delete_Element(App->menu_manager->brotherhood_barrack[7]);
					App->menu_manager->brotherhood_barrack[7]= (UI_Label*)App->gui->CreateLabel(750, 656, Label, std::to_string(App->menu_manager->cost * 2), NULL, this, NULL, "StackedPixelXS");
				}
				else if (App->player->faction == VAULT) {
					App->gui->Delete_Element(App->menu_manager->vault_barrack[7]);
					App->menu_manager->vault_barrack[7] = (UI_Label*)App->gui->CreateLabel(750, 656, Label, std::to_string(App->menu_manager->cost * 2), NULL, this, NULL, "StackedPixelXS");
				}
				else if (App->player->faction == MUTANT) {
					App->gui->Delete_Element(App->menu_manager->supermutant_barrack[7]);
					App->menu_manager->supermutant_barrack[7] = (UI_Label*)App->gui->CreateLabel(750, 656, Label, std::to_string(App->menu_manager->cost * 2), NULL, this, NULL, "StackedPixelXS");
				}
				else if (App->player->faction == GHOUL) {
					App->gui->Delete_Element(App->menu_manager->ghoul_barrack[7]);
					App->menu_manager->ghoul_barrack[7] = (UI_Label*)App->gui->CreateLabel(750, 656, Label, std::to_string(App->menu_manager->cost * 2), NULL, this, NULL, "StackedPixelXS");
				}
			}
			else if (t == Boost_lab_button)
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
				/*static_entity->level++;*/
				if (App->player->faction == BROTHERHOOD) {
					App->gui->Delete_Element(App->menu_manager->brotherhood_lab[7]);
					App->menu_manager->brotherhood_lab[7] = (UI_Label*)App->gui->CreateLabel(750, 596, Label, std::to_string(App->menu_manager->cost * 2), NULL, this, NULL, "StackedPixelXS");
				}
				else if (App->player->faction == VAULT) {
					App->gui->Delete_Element(App->menu_manager->vault_lab[7]);
					App->menu_manager->vault_lab[7] = (UI_Label*)App->gui->CreateLabel(750, 596, Label, std::to_string(App->menu_manager->cost * 2), NULL, this, NULL, "StackedPixelXS");
				}
				else if (App->player->faction == MUTANT) {
					App->gui->Delete_Element(App->menu_manager->supermutant_lab[7]);
					App->menu_manager->supermutant_lab[7] = (UI_Label*)App->gui->CreateLabel(750, 596, Label, std::to_string(App->menu_manager->cost * 2), NULL, this, NULL, "StackedPixelXS");
				}
				else if (App->player->faction == GHOUL) {
					App->gui->Delete_Element(App->menu_manager->ghoul_lab[7]);
					App->menu_manager->ghoul_lab[7] = (UI_Label*)App->gui->CreateLabel(750, 596, Label, std::to_string(App->menu_manager->cost * 2), NULL, this, NULL, "StackedPixelXS");
				}
			}
			else if (t == Boost_radar_button) {
				if (App->player->caps >= App->entities->radar_cost) {
					App->minimap->EnableRadar();
					App->player->UpdateResourceData(Resource::CAPS, -App->entities->radar_cost);
				}
			}		
			else if (t == summon_mrhandy_button) {
				if (App->player->caps >= App->entities->mr_handy_cost) {
					App->player->laboratory->SpawnUnit(MR_HANDY, true);
					App->player->UpdateResourceData(Resource::CAPS, -App->entities->mr_handy_cost);
					//App->entities->CreateEntity(App->player->faction, MR_HANDY, 75, 75, App->player);

				}
			}
			else if (t == info_button) {
				if (App->scene->create_tutorial == false) {
					App->menu_manager->CreateMenu(Menu::TUTORIAL);
					App->audio->PlayFx(1, App->audio->character_fx, 0);
					App->scene->create_tutorial = true;
					App->gui->ingame = false;
				
					App->easing_splines->CreateSpline(&App->menu_manager->tutorial_screen->pos.x, App->menu_manager->tutorial_screen->pos.x - 700.0f, 1000, Spline_Type::EASE_IN_OUT_QUAD);
					App->easing_splines->CreateSpline(&App->menu_manager->tutorial[0]->pos.x, App->menu_manager->tutorial[0]->pos.x - 700.0f, 1000, Spline_Type::EASE_IN_OUT_QUAD);
					App->easing_splines->CreateSpline(&App->menu_manager->tutorial[1]->pos.x, App->menu_manager->tutorial[1]->pos.x - 700.0f, 1000, Spline_Type::EASE_IN_OUT_QUAD);
					App->easing_splines->CreateSpline(&App->menu_manager->tutorial[2]->pos.x, App->menu_manager->tutorial[2]->pos.x - 700.0f, 1000, Spline_Type::EASE_IN_OUT_QUAD);
					App->easing_splines->CreateSpline(&App->menu_manager->tutorial[3]->pos.x, App->menu_manager->tutorial[3]->pos.x - 700.0f, 1000, Spline_Type::EASE_IN_OUT_QUAD);
					App->easing_splines->CreateSpline(&App->menu_manager->tutorial[4]->pos.x, App->menu_manager->tutorial[4]->pos.x - 700.0f, 1000, Spline_Type::EASE_IN_OUT_QUAD);
					App->easing_splines->CreateSpline(&App->menu_manager->info_button2_element->pos.x, App->menu_manager->info_button2_element->pos.x - 700.0f, 1000, Spline_Type::EASE_IN_OUT_QUAD);

				}
			}
			else if (t == info_button2) {
				if (App->scene->create_tutorial == true) {
					App->easing_splines->CreateSpline(&App->menu_manager->tutorial_screen->pos.x, App->menu_manager->tutorial_screen->pos.x + 700.0f, 1000, Spline_Type::EASE_IN_OUT_QUAD);
					App->easing_splines->CreateSpline(&App->menu_manager->tutorial[0]->pos.x, App->menu_manager->tutorial[0]->pos.x + 700.0f, 1000, Spline_Type::EASE_IN_OUT_QUAD);
					App->easing_splines->CreateSpline(&App->menu_manager->tutorial[1]->pos.x, App->menu_manager->tutorial[1]->pos.x + 700.0f, 1000, Spline_Type::EASE_IN_OUT_QUAD);
					App->easing_splines->CreateSpline(&App->menu_manager->tutorial[2]->pos.x, App->menu_manager->tutorial[2]->pos.x + 700.0f, 1000, Spline_Type::EASE_IN_OUT_QUAD);
					App->easing_splines->CreateSpline(&App->menu_manager->tutorial[3]->pos.x, App->menu_manager->tutorial[3]->pos.x + 700.0f, 1000, Spline_Type::EASE_IN_OUT_QUAD);
					App->easing_splines->CreateSpline(&App->menu_manager->tutorial[4]->pos.x, App->menu_manager->tutorial[4]->pos.x + 700.0f, 1000, Spline_Type::EASE_IN_OUT_QUAD);
					if (App->menu_manager->tutorial[6] != nullptr) {
						App->easing_splines->CreateSpline(&App->menu_manager->tutorial[6]->pos.x, App->menu_manager->tutorial[6]->pos.x + 700.0f, 1000, Spline_Type::EASE_IN_OUT_QUAD);
					}
					if (App->menu_manager->tutorial[7] != nullptr) {
						App->easing_splines->CreateSpline(&App->menu_manager->tutorial[7]->pos.x, App->menu_manager->tutorial[7]->pos.x + 700.0f, 1000, Spline_Type::EASE_IN_OUT_QUAD);
					}
					App->easing_splines->CreateSpline(&App->menu_manager->info_button2_element->pos.x, App->menu_manager->info_button2_element->pos.x + 700.0f, 1000, Spline_Type::EASE_IN_OUT_QUAD);
					//App->menu_manager->DestroyMenu(Menu::TUTORIAL);
					App->menu_manager->DestroyMenu(Menu::HOW_TO_PLAY);
					App->gui->Delete_Element(App->menu_manager->tutorial[5]);
					App->audio->PlayFx(1, App->audio->back_fx, 0);
					App->scene->create_tutorial = false;
					App->gui->ingame = true;
				}

			}
			else if (t == control_button) {
				App->audio->PlayFx(1, App->audio->character_fx, 0);
				App->gui->DeleteArrayElements(App->menu_manager->tutorial, 5);
				App->menu_manager->tutorial[6] = (UI_Button*)App->gui->CreateButton(640, 435, back_tutorial_controls, { 973,2084,43,46 }, { 973,2135,43,46 }, { 973,2185,43,46 }, NULL, this);
				App->menu_manager->tutorial[5] = (j1Image*)App->gui->CreateImage(670, 160, Image, { 0, 2028, 605, 305 }, NULL, this);
				App->gui->ingame = false;
			}
			else if (t == back_tutorial_controls) {
				App->audio->PlayFx(1, App->audio->back_fx, 0);
				App->gui->Delete_Element(App->menu_manager->tutorial[5]);
				App->gui->Delete_Element(App->menu_manager->tutorial[6]);
				App->menu_manager->tutorial[0] = (UI_Button*)App->gui->CreateButton(680, 360, control_button, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,244,64 }, NULL, this);
				App->menu_manager->tutorial[1] = (UI_Button*)App->gui->CreateButton(1000, 360, how_to_play_button, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,244,64 }, NULL, this);
				App->menu_manager->tutorial[2] = (UI_Label*)App->gui->CreateLabel(890, 220, Label, "TUTORIAL", NULL, this, NULL);
				App->menu_manager->tutorial[3] = (UI_Label*)App->gui->CreateLabel(710, 380, Label, "CONTROLS", NULL, this, NULL);
				App->menu_manager->tutorial[4] = (UI_Label*)App->gui->CreateLabel(1020, 380, Label, "HOW TO PLAY", NULL, this, NULL);
			}
			else if (t == how_to_play_button) {
				App->gui->DeleteArrayElements(App->menu_manager->tutorial, 5);
				App->audio->PlayFx(1, App->audio->character_fx, 0);
				App->menu_manager->tutorial[7] = (UI_Button*)App->gui->CreateButton(640, 435, back_tutorial_how_to_play, { 973,2084,43,46 }, { 973,2135,43,46 }, { 973,2185,43,46 }, NULL, this);
				App->menu_manager->CreateMenu(Menu::HOW_TO_PLAY);
				App->gui->ingame = false;
			}
			else if (t == back_tutorial_how_to_play) {
				App->audio->PlayFx(1, App->audio->back_fx, 0);
				App->gui->DeleteArrayElements(App->menu_manager->how_to_play, 6);
				App->gui->Delete_Element(App->menu_manager->tutorial[7]);
				App->menu_manager->tutorial[0] = (UI_Button*)App->gui->CreateButton(680, 360, control_button, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,244,64 }, NULL, this);
				App->menu_manager->tutorial[1] = (UI_Button*)App->gui->CreateButton(1000, 360, how_to_play_button, { 1900,895,244,72 }, { 1900,974,244,72 }, { 1900,1054,244,64 }, NULL, this);
				App->menu_manager->tutorial[2] = (UI_Label*)App->gui->CreateLabel(890, 220, Label, "TUTORIAL", NULL, this, NULL);
				App->menu_manager->tutorial[3] = (UI_Label*)App->gui->CreateLabel(710, 380, Label, "CONTROLS", NULL, this, NULL);
				App->menu_manager->tutorial[4] = (UI_Label*)App->gui->CreateLabel(1020, 380, Label, "HOW TO PLAY", NULL, this, NULL);
			}
			else if (t == how_to_play_next) {
				App->menu_manager->DestroyMenu(Menu::HOW_TO_PLAY);
				App->menu_manager->how_to_play_counter++;
				if (App->menu_manager->how_to_play_counter > 3) {
					App->menu_manager->how_to_play_counter = 3;
				}
				App->menu_manager->CreateMenu(Menu::HOW_TO_PLAY);
				App->gui->ingame = false;
			}
			else if (t == how_to_play_previous) {
				App->menu_manager->DestroyMenu(Menu::HOW_TO_PLAY);
				App->menu_manager->how_to_play_counter--;
				if (App->menu_manager->how_to_play_counter < 0) {
					App->menu_manager->how_to_play_counter = 0;
				}
				App->menu_manager->CreateMenu(Menu::HOW_TO_PLAY);
				App->gui->ingame = false;
			}
			else if (t == quest_button) {

				if (App->gui->open == true) {

					App->audio->PlayFx(1, App->audio->character_fx, 0);
					App->easing_splines->CreateSpline(&App->menu_manager->quest[2]->pos.x, App->menu_manager->quest[2]->pos.x - 307, 1000, Spline_Type::EASE_IN_OUT_QUAD);
					if (App->menu_manager->quest[3] != nullptr) {
						App->easing_splines->CreateSpline(&App->menu_manager->quest[3]->pos.x, App->menu_manager->quest[3]->pos.x - 307, 1000, Spline_Type::EASE_IN_OUT_QUAD);
					}
					if (App->menu_manager->quest[4] != nullptr) {
						App->easing_splines->CreateSpline(&App->menu_manager->quest[4]->pos.x, App->menu_manager->quest[4]->pos.x - 307, 1000, Spline_Type::EASE_IN_OUT_QUAD);
					}
					if (App->menu_manager->quest[5] != nullptr) {
						App->easing_splines->CreateSpline(&App->menu_manager->quest[5]->pos.x, App->menu_manager->quest[5]->pos.x - 307, 1000, Spline_Type::EASE_IN_OUT_QUAD);
					}
					if (App->menu_manager->quest[6] != nullptr) {
						App->easing_splines->CreateSpline(&App->menu_manager->quest[6]->pos.x, App->menu_manager->quest[6]->pos.x - 307, 1000, Spline_Type::EASE_IN_OUT_QUAD);
					}
					if (App->menu_manager->quest[7] != nullptr) {
						App->easing_splines->CreateSpline(&App->menu_manager->quest[7]->pos.x, App->menu_manager->quest[7]->pos.x - 307, 1000, Spline_Type::EASE_IN_OUT_QUAD);
					}
					if (App->menu_manager->quest[8] != nullptr) {
						App->easing_splines->CreateSpline(&App->menu_manager->quest[8]->pos.x, App->menu_manager->quest[8]->pos.x - 307, 1000, Spline_Type::EASE_IN_OUT_QUAD);
					}
					App->easing_splines->CreateSpline(&App->menu_manager->quest[0]->pos.x, App->menu_manager->quest[0]->pos.x - 350, 1000, Spline_Type::EASE_IN_OUT_QUAD);
					App->easing_splines->CreateSpline(&App->menu_manager->quest[1]->pos.x, App->menu_manager->quest[1]->pos.x + 50, 1000, Spline_Type::EASE_IN_OUT_QUAD);

					App->gui->open = false;

				}
}
			else if (t == quest_button2) {
				
				if (App->gui->open == false) {

					App->audio->PlayFx(1, App->audio->character_fx, 0);
					App->easing_splines->CreateSpline(&App->menu_manager->quest[2]->pos.x, App->menu_manager->quest[2]->pos.x + 307, 1000, Spline_Type::EASE_IN_OUT_QUAD);
					if (App->menu_manager->quest[3] != nullptr) {
						App->easing_splines->CreateSpline(&App->menu_manager->quest[3]->pos.x, App->menu_manager->quest[3]->pos.x + 307, 1000, Spline_Type::EASE_IN_OUT_QUAD);
					}
					if (App->menu_manager->quest[4] != nullptr) {
						App->easing_splines->CreateSpline(&App->menu_manager->quest[4]->pos.x, App->menu_manager->quest[4]->pos.x + 307, 1000, Spline_Type::EASE_IN_OUT_QUAD);
					}
					if (App->menu_manager->quest[5] != nullptr) {
						App->easing_splines->CreateSpline(&App->menu_manager->quest[5]->pos.x, App->menu_manager->quest[5]->pos.x + 307, 1000, Spline_Type::EASE_IN_OUT_QUAD);
					}
					if (App->menu_manager->quest[6] != nullptr) {
						App->easing_splines->CreateSpline(&App->menu_manager->quest[6]->pos.x, App->menu_manager->quest[6]->pos.x + 307, 1000, Spline_Type::EASE_IN_OUT_QUAD);
					}
					if (App->menu_manager->quest[7] != nullptr) {
						App->easing_splines->CreateSpline(&App->menu_manager->quest[7]->pos.x, App->menu_manager->quest[7]->pos.x + 307, 1000, Spline_Type::EASE_IN_OUT_QUAD);
					}
					if (App->menu_manager->quest[8] != nullptr) {
						App->easing_splines->CreateSpline(&App->menu_manager->quest[8]->pos.x, App->menu_manager->quest[8]->pos.x + 307, 1000, Spline_Type::EASE_IN_OUT_QUAD);
					}
					if (App->menu_manager->quest[0] != nullptr) {
						App->easing_splines->CreateSpline(&App->menu_manager->quest[0]->pos.x, App->menu_manager->quest[0]->pos.x + 350, 1000, Spline_Type::EASE_IN_OUT_QUAD);
					}
					if (App->menu_manager->quest[1] != nullptr) {
						App->easing_splines->CreateSpline(&App->menu_manager->quest[1]->pos.x, App->menu_manager->quest[1]->pos.x - 50, 1000, Spline_Type::EASE_IN_OUT_QUAD);
					}

					App->gui->open = true;

				}
							
			}
			else if (t == continue_button) {
				App->gui->DeleteArrayElements(App->menu_manager->quest, 12);
			}
			else if (t == Radio_button) {
				App->menu_manager->CreateMenu(Menu::RADIO);
			}
			else if (t == RockFM_button)
			{
			Mix_PauseMusic();
			App->audio->PlayMusic("Assets/audio/music/RockFM/bon-jovi-its-my-life.ogg", 0.0F);
			}
			else if (t == SlimShadyFM_button)
			{
			Mix_PauseMusic();
			App->audio->PlayMusic("Assets/audio/music/SlimShadyFM/not-afraid-eminem.ogg", 0.0F);
			}
			else if (t == UltraRADIO_button)
			{
			Mix_PauseMusic();
			App->audio->PlayMusic("Assets/audio/music/Ultraradio/avicii-hey-brother.ogg", 0.0F);
			}
			else if (t == FiestaLatinaFM_button)
			{
			Mix_PauseMusic();
			App->audio->PlayMusic("Assets/audio/music/FiestalatinaFM/karol-g-nicki-minaj-tusa.ogg", 0.0F);
			}
			else if (t == AyuwokiFM_button)
			{
			Mix_PauseMusic();
			App->audio->PlayMusic("Assets/audio/music/AyuwokiFM/michael-jackson-billy-jean.ogg", 0.0F);
			}
			else if (t == FusionCoreFM_button)
			{
			Mix_PauseMusic();
			App->audio->PlayMusic("Assets/audio/music/FusioncoreFM/fallout-3-soundtrack-rhythm-for-you.ogg", 0.0F);
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

UI_Type UI_Button::GetType() {
	return t;
}