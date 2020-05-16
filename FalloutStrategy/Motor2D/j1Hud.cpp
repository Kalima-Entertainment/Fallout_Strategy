#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Scene.h"
#include "j1Timer.h"
#include "j1Map.h"
#include "UI_Label.h"
#include "j1Audio.h"
#include "j1Gui.h"
#include "j1Hud.h"
#include "j1Fonts.h"
#include "MenuManager.h"
#include "j1EntityManager.h"
#include "j1Player.h"
#include "StaticEntity.h"
#include "j1Entity.h"

j1Hud::j1Hud() :j1Module()
{
	name.assign("hud");
}

bool j1Hud::Awake(pugi::xml_node& node)
{
	timer = 59;
	minutes = 14;
	draw_health = false;
	return true;
}

bool j1Hud::Start()
{

	activateTimer = false;
	
	return true;
}

bool j1Hud::PreUpdate()
{
	return true;
}

bool j1Hud::Update(float dt)
{

	//TIMER

	if (activateTimer)
	{
		if (timer_game.Read() >= 1000)
		{
			timer_game.Start();
			timer--;
		}
		if (timer == 0)
		{
			timer = 59;
			minutes -= 1;
		}

		sprintf_s(timer_text, 10, "%d", timer);
		sprintf_s(minutes_text, 10, "%d", minutes);

		if (timer < 10)
		{
			timer_text[2] = timer_text[1];
			timer_text[1] = timer_text[0];
			timer_text[0] = '0';
		}
		if (minutes < 10)
		{
			minutes_text[2] = minutes_text[1];
			minutes_text[1] = minutes_text[0];
			minutes_text[0] = '0';
		}

		App->menu_manager->timer_item->SetTextTimer(timer_text);

		App->menu_manager->timer_minutes->SetTextTimer(minutes_text);

		if (minutes == 0 && timer == 0) {
			activateTimer = false;
		}

	}

	if (minutes == 8 && timer == 30)
	{
		App->entities->SpawnAnimals();
	}

	


	return true;
}

bool j1Hud::PostUpdate()
{
	if (App->player->selected_entity != nullptr)
	{
		StaticEntity* static_entity = (StaticEntity*)App->player->selected_entity;
		//Create HUD for the building
		switch (static_entity->faction) {
		case GHOUL:
			if (static_entity->type == BASE) {

				//Health bar stats
				background_bar = { 530, 698, 122, 4 };
				foreground_bar = { 531, 699, static_entity->current_health / static_entity->max_health * background_bar.w, 4 };
				if (foreground_bar.w < 0) { foreground_bar.w = 0; }
				App->render->DrawQuad(background_bar, 0, 0, 0, 255, true, 0.0f);
				App->render->DrawQuad(foreground_bar, 18, 164, 62, 255, true, 0.0f);


			}
			else if (static_entity->type == BARRACK) {

				//Health bar stats
				background_bar = { 530, 698, 122, 4 };
				foreground_bar = { 531, 699, static_entity->current_health / static_entity->max_health * background_bar.w, 4 };
				if (foreground_bar.w < 0) { foreground_bar.w = 0; }
				App->render->DrawQuad(background_bar, 0, 0, 0, 255, true, 0.0f);
				App->render->DrawQuad(foreground_bar, 18, 164, 62, 255, true, 0.0f);


			}
			else if (static_entity->type == LABORATORY) {

				//Health bar stats
				background_bar = { 530, 698, 122, 4 };
				foreground_bar = { 531, 699, static_entity->current_health / static_entity->max_health * background_bar.w, 4 };
				if (foreground_bar.w < 0) { foreground_bar.w = 0; }
				App->render->DrawQuad(background_bar, 0, 0, 0, 255, true, 0.0f);
				App->render->DrawQuad(foreground_bar, 18, 164, 62, 255, true, 0.0f);


			}
			break;
		case BROTHERHOOD:
			
			if (static_entity->type == BASE) {

				//Health bar stats
				background_bar = { 530, 698, 122, 4 };
				foreground_bar = { 531, 699, static_entity->current_health / static_entity->max_health * background_bar.w, 4 };
				if (foreground_bar.w < 0) { foreground_bar.w = 0; }
				App->render->DrawQuad(background_bar, 255, 255, 255, 255, true, 0.0f);
				App->render->DrawQuad(foreground_bar, 18, 164, 62, 255, true, 0.0f);

				//Spawn bar
			
				SDL_Rect spawn_bar_background = { 900, 678, 170, 8 };
				SDL_Rect spawn_bar_foreground = { 901, 679, static_entity->time_left / static_entity->spawn_stack[0].spawn_seconds * spawn_bar_background.w, 8 };
				App->render->DrawQuad(spawn_bar_background, 255, 255, 255, 255, true, 0.0f);
				App->render->DrawQuad(spawn_bar_foreground, 230, 165, 30, 255, true, 0.0f);


			}
			else if (static_entity->type == BARRACK) {

				//Health bar stats
				background_bar = { 530, 698, 122, 4 };
				foreground_bar = { 531, 699, static_entity->current_health / static_entity->max_health * background_bar.w, 4 };
				if (foreground_bar.w < 0) { foreground_bar.w = 0; }
				App->render->DrawQuad(background_bar, 0, 0, 0, 255, true, 0.0f);
				App->render->DrawQuad(foreground_bar, 18, 164, 62, 255, true, 0.0f);


			}
			else if (static_entity->type == LABORATORY) {

				//Health bar stats
				background_bar = { 530, 698, 122, 4 };
				foreground_bar = { 531, 699, static_entity->current_health / static_entity->max_health * background_bar.w, 4 };
				if (foreground_bar.w < 0) { foreground_bar.w = 0; }
				App->render->DrawQuad(background_bar, 0, 0, 0, 255, true, 0.0f);
				App->render->DrawQuad(foreground_bar, 18, 164, 62, 255, true, 0.0f);

			}
			break;

		case VAULT:

			if (static_entity->type == BASE) {

				//Health bar stats
				background_bar = { 530, 698, 122, 4 };
				foreground_bar = { 531, 699, static_entity->current_health / static_entity->max_health * background_bar.w, 4 };
				if (foreground_bar.w < 0) { foreground_bar.w = 0; }
				App->render->DrawQuad(background_bar, 0, 0, 0, 255, true, 0.0f);
				App->render->DrawQuad(foreground_bar, 18, 164, 62, 255, true, 0.0f);

			}
			else if (static_entity->type == BARRACK) {

				//Health bar stats
				background_bar = { 530, 698, 122, 4 };
				foreground_bar = { 531, 699, static_entity->current_health / static_entity->max_health * background_bar.w, 4 };
				if (foreground_bar.w < 0) { foreground_bar.w = 0; }
				App->render->DrawQuad(background_bar, 0, 0, 0, 255, true, 0.0f);
				App->render->DrawQuad(foreground_bar, 18, 164, 62, 255, true, 0.0f);


			}
			else if (static_entity->type == LABORATORY) {

				//Health bar stats
				background_bar = { 530, 698, 122, 4 };
				foreground_bar = { 531, 699, static_entity->current_health / static_entity->max_health * background_bar.w, 4 };
				if (foreground_bar.w < 0) { foreground_bar.w = 0; }
				App->render->DrawQuad(background_bar, 0, 0, 0, 255, true, 0.0f);
				App->render->DrawQuad(foreground_bar, 18, 164, 62, 255, true, 0.0f);


			}
			break;
		case MUTANT:
			if (static_entity->type == BASE) {

				//Health bar stats
				background_bar = { 530, 698, 122, 4 };
				foreground_bar = { 531, 699, static_entity->current_health / static_entity->max_health * background_bar.w, 4 };
				if (foreground_bar.w < 0) { foreground_bar.w = 0; }
				App->render->DrawQuad(background_bar, 0, 0, 0, 255, true, 0.0f);
				App->render->DrawQuad(foreground_bar, 18, 164, 62, 255, true, 0.0f);


			}
			else if (static_entity->type == BARRACK) {

				//Health bar stats
				background_bar = { 530, 698, 122, 4 };
				foreground_bar = { 531, 699, static_entity->current_health / static_entity->max_health * background_bar.w, 4 };
				if (foreground_bar.w < 0) { foreground_bar.w = 0; }
				App->render->DrawQuad(background_bar, 0, 0, 0, 255, true, 0.0f);
				App->render->DrawQuad(foreground_bar, 18, 164, 62, 255, true, 0.0f);


			}
			else if (static_entity->type == LABORATORY) {

				//Health bar stats
				background_bar = { 530, 698, 122, 4 };
				foreground_bar = { 531, 699, static_entity->current_health / static_entity->max_health * background_bar.w, 4 };
				if (foreground_bar.w < 0) { foreground_bar.w = 0; }
				App->render->DrawQuad(background_bar, 0, 0, 0, 255, true, 0.0f);
				App->render->DrawQuad(foreground_bar, 18, 164, 62, 255, true, 0.0f);

			}
			break;
		}


	}
	else if (App->player->selected_entity == nullptr) {
		background_bar;
		foreground_bar;
		draw_health = false;
	}

	return true;
}

bool j1Hud::CleanUp()
{
	
	return true;
}

bool j1Hud::Load(pugi::xml_node& data)
{
	timer = data.child("timer").attribute("seconds").as_int();
	minutes = data.child("timer").attribute("minutes").as_int();

	
	return true;
}

bool j1Hud::Save(pugi::xml_node& data) const
{
	pugi::xml_node time = data.append_child("timer");

	time.append_attribute("seconds") = timer;
	time.append_attribute("minutes") = minutes;

	return true;
}






