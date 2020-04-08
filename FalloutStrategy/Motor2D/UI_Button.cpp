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
#include "Player.h"
#include "j1Scene.h"
#include "j1Map.h"

#include "./brofiler/Brofiler/Brofiler.h"

UI_Button::UI_Button(int x, int y, UI_Type type, SDL_Rect idle, SDL_Rect hover, SDL_Rect click, UI_element* parent, j1Module* Observer) : UI_element(x, y, type, parent, Observer)
{
	this->pos.x = x;
	this->pos.y = y;
	this->Button_idle = idle;
	this->Button_hover = hover;
	this->Button_click = click;

	t = type;

	dimensions = idle;
	click_fx = App->audio->LoadFx("audio/fx/UI Sounds/Butn_Arrow.wav");
	hover_fx = App->audio->LoadFx("audio/fx/UI Sounds/Butn_Slider.wav");
	back_fx = App->audio->LoadFx("audio/fx/UI Sounds/Butn_ReadyOff.wav");
	volume_fx = App->audio->LoadFx("audio/fx/UI Sounds/Butn_Text.wav");
	members_fx = App->audio->LoadFx("audio/fx/UI Sounds/Butn_Skill.wav");

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

			App->audio->PlayFx(hover_fx, 0);
		}


		if (t == button_select_ghoul) 
		{
			ghoul_image = (j1Image*)App->gui->CreateImage(940, 300, Image, { 2492, 837, 168, 166 }, NULL, this);
			ghoul_image->hover = true;
			elements_to_show.push_back(ghoul_image);
			current_state = BUTTON_STATE::HOVER;
		}

		if (t == button_select_vault) 
		{
			vault_image = (j1Image*)App->gui->CreateImage(860, 300, Image, { 2492, 1013, 309, 134 }, NULL, this);
			vault_image->hover = true;
			elements_to_show.push_back(vault_image);
			current_state = BUTTON_STATE::HOVER;
		}

		if (t == button_select_brotherhood) 
		{
			brotherhood_image = (j1Image*)App->gui->CreateImage(950, 300, Image, { 2492, 1161, 155, 180 }, NULL, this);
			brotherhood_image->hover = true;
			elements_to_show.push_back(brotherhood_image);
			current_state = BUTTON_STATE::HOVER;
		}

		if (t == button_select_supermutant) 
		{
			supermutant_image = (j1Image*)App->gui->CreateImage(950, 300, Image, { 2492, 1354, 158, 158 }, NULL, this);
			supermutant_image->hover = true;
			elements_to_show.push_back(supermutant_image);
			current_state = BUTTON_STATE::HOVER;
		}

		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT)==KEY_DOWN) {

			//rect is button pressed
			dimensions = Button_click;
			
			//only plays fx once
						
			if (observer) {
				observer->Callback(this);
			}

			if (t == Button_slider_music_left) {
				App->gui->volume_up = 1;
				App->audio->PlayFx(volume_fx, 0);
			}

			else if (t == Button_slider_music_right) {
				App->gui->volume_up = 2;
				App->audio->PlayFx(volume_fx, 0);
			}

			if (t == Button_slider_fx_left) {
				App->gui->fx_up = 1;
				App->audio->PlayFx(volume_fx, 0);
			}

			else if (t == Button_slider_fx_right) {
				App->gui->fx_up = 2;
				App->audio->PlayFx(volume_fx, 0);
			}

			if (t == button_new_game) 
			{
				App->menu_manager->DestroyMenu(Menu::MAIN_MENU);
				App->menu_manager->CreateSelectFaction();
				App->audio->PlayFx(click_fx, 0);
			}
			
			if (t == button_credits)
			{
				App->menu_manager->DestroyMenu(Menu::MAIN_MENU);
				App->menu_manager->CreateCredits();
				App->audio->PlayFx(click_fx, 0);
			}

			if (t == button_back_credits)
			{
				App->menu_manager->CreateMainMenu();
				App->menu_manager->DestroyMenu(Menu::CREDITS);
				App->menu_manager->DestroyMenu(Menu::COLLABORATORS_CREDITS);
				App->audio->PlayFx(back_fx, 0);
			}

			if (t == button_marc)
			{
				App->menu_manager->DestroyMenu(Menu::COLLABORATORS_CREDITS);
				App->menu_manager->collaborator = 'M';
				App->menu_manager->CreateCollaboratorPicture();
				App->audio->PlayFx(members_fx, 0);
			}

			if (t == button_javi)
			{
				App->menu_manager->DestroyMenu(Menu::COLLABORATORS_CREDITS);
				App->menu_manager->collaborator = 'J';
				App->menu_manager->CreateCollaboratorPicture();
				App->audio->PlayFx(members_fx, 0);
			}

			if (t == button_pablo)
			{
				App->menu_manager->DestroyMenu(Menu::COLLABORATORS_CREDITS);
				App->menu_manager->collaborator = 'P';
				App->menu_manager->CreateCollaboratorPicture();
				App->audio->PlayFx(members_fx, 0);
			}

			if (t == button_german)
			{
				App->menu_manager->DestroyMenu(Menu::COLLABORATORS_CREDITS);
				App->menu_manager->collaborator = 'G';
				App->menu_manager->CreateCollaboratorPicture();
				App->audio->PlayFx(members_fx, 0);
			}

			if (t == button_macia)
			{
				App->menu_manager->DestroyMenu(Menu::COLLABORATORS_CREDITS);
				App->menu_manager->collaborator = 'D';
				App->menu_manager->CreateCollaboratorPicture();
				App->audio->PlayFx(members_fx, 0);
			}

			if (t == button_pol)
			{
				App->menu_manager->DestroyMenu(Menu::COLLABORATORS_CREDITS);
				App->menu_manager->collaborator = 'K';
				App->menu_manager->CreateCollaboratorPicture();
				App->audio->PlayFx(members_fx, 0);
			}

			if (t == button_silvino)
			{
				App->menu_manager->DestroyMenu(Menu::COLLABORATORS_CREDITS);
				App->menu_manager->collaborator = 'S';
				App->menu_manager->CreateCollaboratorPicture();
				App->audio->PlayFx(members_fx, 0);
			}

			if (t == button_christian)
			{
				App->menu_manager->DestroyMenu(Menu::COLLABORATORS_CREDITS);
				App->menu_manager->collaborator = 'C';
				App->menu_manager->CreateCollaboratorPicture();
				App->audio->PlayFx(members_fx, 0);
			}

			if(t == button_exit)
			{
				App->quitGame = true;
				App->audio->PlayFx(click_fx, 0);
			}

			if (t == button_github_credits) 
			{
				ShellExecuteA(NULL, "open", "https://github.com/Kalima-Entertainment/Fallout_Strategy", NULL, NULL, SW_SHOWNORMAL);
				App->audio->PlayFx(click_fx, 0);
			}

			if (t == button_twitter_credits)
			{
				ShellExecuteA(NULL, "open", "https://twitter.com/KalimaEntmt", NULL, NULL, SW_SHOWNORMAL);
				App->audio->PlayFx(click_fx, 0);
			}

			if (t == button_settings) {

				App->menu_manager->DestroyMenu(Menu::MAIN_MENU);
				App->menu_manager->CreateSettings();
				App->audio->PlayFx(click_fx, 0);
			}

			if (t == button_settings_pause) {

				App->menu_manager->DestroyMenu(Menu::PAUSE_MENU);
				App->menu_manager->CreatePauseSettings();
				App->audio->PlayFx(click_fx, 0);
			}

			if (t == button_back) {
				App->menu_manager->DestroyMenu(Menu::SETTINGS);
				App->menu_manager->CreateMainMenu();
				App->audio->PlayFx(back_fx, 0);
			}

			if (t == button_back_pause) {
				App->menu_manager->DestroyMenu(Menu::SETTINGS);
				App->menu_manager->CreatePauseMenu();
				App->audio->PlayFx(back_fx, 0);
			}

			if (t == resume_button) {
				App->menu_manager->DestroyMenu(Menu::PAUSE_MENU);
				App->scene->create = false;
				App->audio->PlayFx(back_fx, 0);
			}

			if(t == button_back_to_menu)
			{
				App->menu_manager->CreateMainMenu();
				App->menu_manager->DestroyMenu(Menu::SELECT_FACTION);
				App->audio->PlayFx(back_fx, 0);
			}
	
			if (t == button_start_game)
			{
				App->menu_manager->DestroyMenu(Menu::SELECT_FACTION);
				App->audio->PlayFx(back_fx, 0);
				App->gui->count = 0;
				App->player->Enable();
				App->menu_manager->CreateGUI();
			}

			if (t == button_pause_to_main)
			{
				App->menu_manager->DestroyMenu(Menu::PAUSE_MENU);
				App->menu_manager->CreateMainMenu();
				App->audio->PlayFx(back_fx, 0);
			}

			//Spawn Gatherer from any faction
			if (t == Ghouls_ghaterer_button || t == Vault_ghaterer_button || t == Supermutant_ghaterer_button || t == Brotherhood_ghaterer_button)
				App->entities->CreateEntity(App->player->selected_entity->faction, GATHERER, App->player->selected_entity->spawnPosition.x, App->player->selected_entity->spawnPosition.y);

			//Spawn Melee from any faction
			if (t == Ghouls_melee_button || t == Vault_melee_button || t == Supermutant_melee_button || t == Brotherhood_melee_button)
				App->entities->CreateEntity(App->player->selected_entity->faction, MELEE, App->player->selected_entity->spawnPosition.x, App->player->selected_entity->spawnPosition.y);
			
			//Spawn Ranged from any faction
			if (t == Ghouls_ranged_button || t == Vault_ranged_button || t == Supermutant_ranged_button || t == Brotherhood_ranged_button)
				App->entities->CreateEntity(App->player->selected_entity->faction, RANGED, App->player->selected_entity->spawnPosition.x, App->player->selected_entity->spawnPosition.y);
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
	

	if(current_state == BUTTON_STATE::HOVER_EXIT)
	{
		for (std::list<UI_element*>::iterator item = elements_to_show.begin(); item != elements_to_show.end(); ++item) {

			if ((*item)->hover == true)
			{
				AddElementToShow((*item));
			}
		}
	}
	
	
	return true;

}

void UI_Button::AddElementToShow(UI_element* element)
{
	element->drawable = false;
}