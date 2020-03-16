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

UI_Button::UI_Button(int x, int y, UI_Type type, SDL_Rect idle, SDL_Rect hover, SDL_Rect click, UI_element * parent, j1Module * Observer) : UI_element(x, y, type, parent, Observer)
{
	this->pos.x = x;
	this->pos.y = y;
	this->Button_idle = idle;
	this->Button_hover = hover;
	this->Button_click = click;

	t = type;

	dimensions = idle;
	click_fx = App->audio->LoadFx("audio/fx/Button_click.wav");
	hover_fx = App->audio->LoadFx("audio/fx/Button_hover.wav");
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

		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT)==KEY_DOWN) {

			//rect is button pressed
			dimensions = Button_click;
			
			//only plays fx once
			App->audio->PlayFx(click_fx, 0);

			if (observer) {
				observer->Callback(this);
			}

			if (t == Button_slider_music_left) {
				App->gui->volume_up = 1;
			}

			else if (t == Button_slider_music_right) {
				App->gui->volume_up = 2;
			}

			if (t == Button_slider_fx_left) {
				App->gui->fx_up = 1;
			}

			else if (t == Button_slider_fx_right) {
				App->gui->fx_up = 2;
			}

			if (t == button_new_game) 
			{
				App->menu_manager->DestroyMainMenu();
				
				
			}
			
			if (t == button_credits)
			{
				App->menu_manager->DestroyMainMenu();
				App->menu_manager->CreateCredits();
			}

			if (t == button_back_credits)
			{
				App->menu_manager->CreateMainMenu();
				App->menu_manager->DestroyCredits();
			}

			if(t == button_exit)
			{
				App->quitGame = true;
			}

			if (t == Button_close) {
				LOG("HOLA");
				
				
				
			}

			if (t == Button_menu) {
				
				
			}

			if (t == Button_close_game) {
				
			}

			if (t == button_github_credits) 
			{
				ShellExecuteA(NULL, "open", "https://github.com/Kalima-Entertainment/Fallout_Strategy", NULL, NULL, SW_SHOWNORMAL);
			}

			if (t == button_twitter_credits)
			{
				ShellExecuteA(NULL, "open", "https://twitter.com/KalimaEntmt", NULL, NULL, SW_SHOWNORMAL);
			}

			if (t == Button_restart) {
				
				

			}

			if (t == Button_settings) {

				
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
	}

	return true;

}
