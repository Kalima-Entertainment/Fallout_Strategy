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
#include "GenericPlayer.h"
#include "brofiler/Brofiler/Brofiler.h"

j1Hud::j1Hud() :j1Module()
{
	name.assign("hud");
}

bool j1Hud::Awake(pugi::xml_node& node)
{
	timer = 59;
	minutes = 14;
	draw_health = false;
	finish_base = false;
	finish_barrack = false;
	finish_lab = false;
	entity = 0;
	gatherer_amount = 0;
	melee_amount = 0;
	ranged_amount = 0;

	return true;
}

bool j1Hud::Start()
{
	//activateTimer = false;
	
	return true;
}

bool j1Hud::PreUpdate()
{
	return true;
}

bool j1Hud::Update(float dt) {
	BROFILER_CATEGORY("Hud Update", Profiler::Color::Aquamarine)
	//TIMER

	if (activateTimer)
	{
		if (timer_game.Read() >= 1000)
		{
			timer_game.Start();
			timer--;

			if (timer == 0)
			{
				timer = 59;
				minutes -= 1;
			}

			sprintf_s(timer_text, 10, "%i", timer);
			sprintf_s(minutes_text, 10, "%i", minutes);

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

	}
	if (minutes == 8 && timer == 30)
	{
		App->entities->SpawnAnimals();
	}

	return true;
}

bool j1Hud::PostUpdate() {
	BROFILER_CATEGORY("Hud PostUpdate", Profiler::Color::Red)
	if (App->player->selected_entity != nullptr)
	{
		StaticEntity* static_entity = (StaticEntity*)App->player->selected_entity;

		//Create HUD for the building
		switch (static_entity->faction) {
		case GHOUL:
			if (static_entity->type == BASE) {

				//Health bar stats
				SDL_Rect background_bar = { 530, 698, 122, 4 };
				SDL_Rect foreground_bar = { 531, 699, (float)static_entity->current_health / static_entity->max_health * background_bar.w, 4 };
				if (foreground_bar.w < 0) { foreground_bar.w = 0; }
				App->render->DrawQuad(background_bar, 255, 255, 255, 255, true, 0.0f);
				App->render->DrawQuad(foreground_bar, 18, 164, 62, 255, true, 0.0f);

				//Spawn bar

				SDL_Rect spawn_bar_background = { 995, 692, 75, 6 };

				App->render->DrawQuad(spawn_bar_background, 255, 255, 255, 255, true, 0.0f);

				if (static_entity->spawning) {

					if (static_entity->spawn_stack[0].type == GATHERER) {
						
						SDL_Rect spawn_bar_foreground = { 995, 692, static_entity->time_left / static_entity->spawn_stack[0].spawn_seconds * spawn_bar_background.w, 6 };
						App->render->DrawQuad(spawn_bar_foreground, 18, 164, 62, 255, true, 0.0f);
						
					}

					finish_base = true;

				}
				else if (static_entity->spawning == false && finish_base == true) {
					gatherer_amount--;
					App->menu_manager->DestroyMenu(Menu::GATHERER_QUANTITY);
					App->menu_manager->CreateMenu(Menu::GATHERER_QUANTITY);
					finish_base = false;
				}

				//TIMER BAG BOOST
				SDL_Rect spawn_bag_background = { 1092, 692, 75, 6 };
				App->render->DrawQuad(spawn_bag_background, 255, 255, 255, 255, true, 0.0f);

				if(static_entity->upgrading)
				{
					SDL_Rect spawn_bag_foreground = { 1092, 692,  static_entity->time_left_upgrade / static_entity->upgrade_stack.upgrade_seconds * spawn_bag_background.w, 6 };
					App->render->DrawQuad(spawn_bag_foreground, 18, 164, 62, 255, true, 0.0f);
				}

			}
			else if (static_entity->type == BARRACK) {

				//Health bar stats
				SDL_Rect background_bar = { 530, 698, 122, 4 };
				SDL_Rect foreground_bar = { 531, 699, (float)static_entity->current_health / static_entity->max_health * background_bar.w, 4 };
				if (foreground_bar.w < 0) { foreground_bar.w = 0; }
				App->render->DrawQuad(background_bar, 255, 255, 255, 255, true, 0.0f);
				App->render->DrawQuad(foreground_bar, 18, 164, 62, 255, true, 0.0f);

				//Spawn bar

				SDL_Rect spawn_bar_background = { 995, 692, 75, 6 };
				SDL_Rect spawn_bar_background2 = { 1092, 692, 75, 6 };

				App->render->DrawQuad(spawn_bar_background, 255, 255, 255, 255, true, 0.0f);
				App->render->DrawQuad(spawn_bar_background2, 255, 255, 255, 255, true, 0.0f);

				if (static_entity->spawning) {

					if (static_entity->spawn_stack[0].type == MELEE) {
						SDL_Rect spawn_bar_foreground = { 995, 692, static_entity->time_left / static_entity->spawn_stack[0].spawn_seconds * spawn_bar_background.w, 6 };
						App->render->DrawQuad(spawn_bar_foreground, 18, 164, 62, 255, true, 0.0f);
					}
					else if (static_entity->spawn_stack[0].type == RANGED) {
						SDL_Rect spawn_bar_foreground = { 1092, 692, static_entity->time_left / static_entity->spawn_stack[0].spawn_seconds * spawn_bar_background2.w, 6 };
						App->render->DrawQuad(spawn_bar_foreground, 18, 164, 62, 255, true, 0.0f);
					}
				}

				//TIMER SPEED BOOST
				SDL_Rect spawn_speed_background = { 1189, 692, 75, 6 };
				App->render->DrawQuad(spawn_speed_background, 255, 255, 255, 255, true, 0.0f);

				if (static_entity->upgrading)
				{
					SDL_Rect spawn_speed_foreground = { 1189, 692,  static_entity->time_left_upgrade / static_entity->upgrade_stack.upgrade_seconds * spawn_speed_background.w, 6 };
					App->render->DrawQuad(spawn_speed_foreground, 18, 164, 62, 255, true, 0.0f);
				}
			}
			else if (static_entity->type == LABORATORY) {
				//Health bar stats
				SDL_Rect background_bar = { 530, 698, 122, 4 };
				SDL_Rect foreground_bar = { 531, 699, (float)static_entity->current_health / static_entity->max_health * background_bar.w, 4 };
				if (foreground_bar.w < 0) { foreground_bar.w = 0; }
				App->render->DrawQuad(background_bar, 255, 255, 255, 255, true, 0.0f);
				App->render->DrawQuad(foreground_bar, 18, 164, 62, 255, true, 0.0f);

				//Spawn bar

				SDL_Rect spawn_bar_background = { 995, 692, 75, 6 };
				App->render->DrawQuad(spawn_bar_background, 255, 255, 255, 255, true, 0.0f);

				if (static_entity->spawning) {

					if (static_entity->spawn_stack[0].type == MR_HANDY) {
						SDL_Rect spawn_bar_foreground = { 995, 692, static_entity->time_left / static_entity->spawn_stack[0].spawn_seconds * spawn_bar_background.w, 6 };
						App->render->DrawQuad(spawn_bar_foreground, 18, 164, 62, 255, true, 0.0f);
					}
			
				}

				//TIMER SHIELD BOOST
				SDL_Rect spawn_shield_background = { 1189, 692, 75, 6 };
				App->render->DrawQuad(spawn_shield_background, 255, 255, 255, 255, true, 0.0f);

				if (static_entity->upgrading)
				{
					SDL_Rect spawn_shield_foreground = { 1189, 692,  static_entity->time_left_upgrade / static_entity->upgrade_stack.upgrade_seconds * spawn_shield_background.w, 6 };
					App->render->DrawQuad(spawn_shield_foreground, 18, 164, 62, 255, true, 0.0f);
				}
			}
			break;
		case BROTHERHOOD:
			
			if (static_entity->type == BASE) {

				//Health bar stats
				SDL_Rect background_bar = { 530, 698, 122, 4 };
				SDL_Rect foreground_bar = { 531, 699, (float)static_entity->current_health / static_entity->max_health * background_bar.w, 4 };
				if (foreground_bar.w < 0) { foreground_bar.w = 0; }
				App->render->DrawQuad(background_bar, 255, 255, 255, 255, true, 0.0f);
				App->render->DrawQuad(foreground_bar, 18, 164, 62, 255, true, 0.0f);

				//Spawn bar

				SDL_Rect spawn_bar_background = { 995, 692, 75, 6 };

				App->render->DrawQuad(spawn_bar_background, 255, 255, 255, 255, true, 0.0f);

				if (static_entity->spawning) {

					if (static_entity->spawn_stack[0].type == GATHERER) {
						
						SDL_Rect spawn_bar_foreground = { 995, 692, static_entity->time_left / static_entity->spawn_stack[0].spawn_seconds * spawn_bar_background.w, 6 };
						App->render->DrawQuad(spawn_bar_foreground, 18, 164, 62, 255, true, 0.0f);
						
					}

					finish_base = true;

				}
				else if (static_entity->spawning == false && finish_base == true) {
					gatherer_amount--;
					App->menu_manager->DestroyMenu(Menu::GATHERER_QUANTITY);
					App->menu_manager->CreateMenu(Menu::GATHERER_QUANTITY);
					finish_base = false;
				}

				//TIMER BAG BOOST
				SDL_Rect spawn_bag_background = { 1092, 692, 75, 6 };
				App->render->DrawQuad(spawn_bag_background, 255, 255, 255, 255, true, 0.0f);

				if (static_entity->upgrading)
				{
					SDL_Rect spawn_bag_foreground = { 1092, 692,  static_entity->time_left_upgrade / static_entity->upgrade_stack.upgrade_seconds * spawn_bag_background.w, 6 };
					App->render->DrawQuad(spawn_bag_foreground, 18, 164, 62, 255, true, 0.0f);
				}
			}
			else if (static_entity->type == BARRACK) {

				//Health bar stats
				SDL_Rect background_bar = { 530, 698, 122, 4 };
				SDL_Rect foreground_bar = { 531, 699, (float)static_entity->current_health / static_entity->max_health * background_bar.w, 4 };
				if (foreground_bar.w < 0) { foreground_bar.w = 0; }
				App->render->DrawQuad(background_bar, 255, 255, 255, 255, true, 0.0f);
				App->render->DrawQuad(foreground_bar, 18, 164, 62, 255, true, 0.0f);

				//Spawn bar

				SDL_Rect spawn_bar_background = { 995, 692, 75, 6 };
				SDL_Rect spawn_bar_background2 = { 1092, 692, 75, 6 };

				App->render->DrawQuad(spawn_bar_background, 255, 255, 255, 255, true, 0.0f);
				App->render->DrawQuad(spawn_bar_background2, 255, 255, 255, 255, true, 0.0f);

				if (static_entity->spawning) {

					if (static_entity->spawn_stack[0].type == MELEE) {
						SDL_Rect spawn_bar_foreground = { 995, 692, static_entity->time_left / static_entity->spawn_stack[0].spawn_seconds * spawn_bar_background.w, 6 };
						App->render->DrawQuad(spawn_bar_foreground, 18, 164, 62, 255, true, 0.0f);
					}
					else if (static_entity->spawn_stack[0].type == RANGED) {
						SDL_Rect spawn_bar_foreground = { 1092, 692, static_entity->time_left / static_entity->spawn_stack[0].spawn_seconds * spawn_bar_background2.w, 6 };
						App->render->DrawQuad(spawn_bar_foreground, 18, 164, 62, 255, true, 0.0f);
					}
				}
			
				//TIMER SPEED BOOST
				SDL_Rect spawn_speed_background = { 1189, 692, 75, 6 };
				App->render->DrawQuad(spawn_speed_background, 255, 255, 255, 255, true, 0.0f);

				if (static_entity->upgrading)
				{
					SDL_Rect spawn_speed_foreground = { 1189, 692,  static_entity->time_left_upgrade / static_entity->upgrade_stack.upgrade_seconds * spawn_speed_background.w, 6 };
					App->render->DrawQuad(spawn_speed_foreground, 18, 164, 62, 255, true, 0.0f);
				}
			}
			else if (static_entity->type == LABORATORY) {

				//Health bar stats
				SDL_Rect background_bar = { 530, 698, 122, 4 };
				SDL_Rect foreground_bar = { 531, 699, (float)static_entity->current_health / static_entity->max_health * background_bar.w, 4 };
				if (foreground_bar.w < 0) { foreground_bar.w = 0; }
				App->render->DrawQuad(background_bar, 255, 255, 255, 255, true, 0.0f);
				App->render->DrawQuad(foreground_bar, 18, 164, 62, 255, true, 0.0f);

				//Spawn bar

				SDL_Rect spawn_bar_background = { 995, 692, 75, 6 };
				App->render->DrawQuad(spawn_bar_background, 255, 255, 255, 255, true, 0.0f);

				if (static_entity->spawning) {

					if (static_entity->spawn_stack[0].type == MR_HANDY) {
						SDL_Rect spawn_bar_foreground = { 995, 692, static_entity->time_left / static_entity->spawn_stack[0].spawn_seconds * spawn_bar_background.w, 6 };
						App->render->DrawQuad(spawn_bar_foreground, 18, 164, 62, 255, true, 0.0f);
					}

				}

				//TIMER SHIELD BOOST
				SDL_Rect spawn_shield_background = { 1189, 692, 75, 6 };
				App->render->DrawQuad(spawn_shield_background, 255, 255, 255, 255, true, 0.0f);

				if (static_entity->upgrading)
				{
					SDL_Rect spawn_shield_foreground = { 1189, 692,  static_entity->time_left_upgrade / static_entity->upgrade_stack.upgrade_seconds * spawn_shield_background.w, 6 };
					App->render->DrawQuad(spawn_shield_foreground, 18, 164, 62, 255, true, 0.0f);
				}
			}
			break;

		case VAULT:

			if (static_entity->type == BASE) {

				//Health bar stats
				SDL_Rect background_bar = { 530, 698, 122, 4 };
				SDL_Rect foreground_bar = { 531, 699, (float)static_entity->current_health / static_entity->max_health * background_bar.w, 4 };
				if (foreground_bar.w < 0) { foreground_bar.w = 0; }
				App->render->DrawQuad(background_bar, 255, 255, 255, 255, true, 0.0f);
				App->render->DrawQuad(foreground_bar, 18, 164, 62, 255, true, 0.0f);

				//Spawn bar

				SDL_Rect spawn_bar_background = { 995, 692, 75, 6 };

				App->render->DrawQuad(spawn_bar_background, 255, 255, 255, 255, true, 0.0f);

				if (static_entity->spawning) {

					if (static_entity->spawn_stack[0].type == GATHERER) {

						SDL_Rect spawn_bar_foreground = { 995, 692, static_entity->time_left / static_entity->spawn_stack[0].spawn_seconds * spawn_bar_background.w, 6 };
						App->render->DrawQuad(spawn_bar_foreground, 18, 164, 62, 255, true, 0.0f);

					}

					finish_base = true;

				}
				else if (static_entity->spawning == false && finish_base == true) {
					gatherer_amount--;
					App->menu_manager->DestroyMenu(Menu::GATHERER_QUANTITY);
					App->menu_manager->CreateMenu(Menu::GATHERER_QUANTITY);
					finish_base = false;
				}

				//TIMER BAG BOOST
				SDL_Rect spawn_bag_background = { 1092, 692, 75, 6 };
				App->render->DrawQuad(spawn_bag_background, 255, 255, 255, 255, true, 0.0f);

				if (static_entity->upgrading)
				{
					SDL_Rect spawn_bag_foreground = { 1092, 692,  static_entity->time_left_upgrade / static_entity->upgrade_stack.upgrade_seconds * spawn_bag_background.w, 6 };
					App->render->DrawQuad(spawn_bag_foreground, 18, 164, 62, 255, true, 0.0f);
				}
			}
			else if (static_entity->type == BARRACK) {

				//Health bar stats
				SDL_Rect background_bar = { 530, 698, 122, 4 };
				SDL_Rect foreground_bar = { 531, 699, (float)static_entity->current_health / static_entity->max_health * background_bar.w, 4 };
				if (foreground_bar.w < 0) { foreground_bar.w = 0; }
				App->render->DrawQuad(background_bar, 255, 255, 255, 255, true, 0.0f);
				App->render->DrawQuad(foreground_bar, 18, 164, 62, 255, true, 0.0f);

				//Spawn bar

				SDL_Rect spawn_bar_background = { 995, 692, 75, 6 };
				SDL_Rect spawn_bar_background2 = { 1092, 692, 75, 6 };

				App->render->DrawQuad(spawn_bar_background, 255, 255, 255, 255, true, 0.0f);
				App->render->DrawQuad(spawn_bar_background2, 255, 255, 255, 255, true, 0.0f);

				if (static_entity->spawning) {

					if (static_entity->spawn_stack[0].type == MELEE) {
						SDL_Rect spawn_bar_foreground = { 995, 692, static_entity->time_left / static_entity->spawn_stack[0].spawn_seconds * spawn_bar_background.w, 6 };
						App->render->DrawQuad(spawn_bar_foreground, 18, 164, 62, 255, true, 0.0f);
					}
					else if (static_entity->spawn_stack[0].type == RANGED) {
						SDL_Rect spawn_bar_foreground = { 1092, 692, static_entity->time_left / static_entity->spawn_stack[0].spawn_seconds * spawn_bar_background2.w, 6 };
						App->render->DrawQuad(spawn_bar_foreground, 18, 164, 62, 255, true, 0.0f);
					}
				}

				//TIMER SPEED BOOST
				SDL_Rect spawn_speed_background = { 1189, 692, 75, 6 };
				App->render->DrawQuad(spawn_speed_background, 255, 255, 255, 255, true, 0.0f);

				if (static_entity->upgrading)
				{
					SDL_Rect spawn_speed_foreground = { 1189, 692,  static_entity->time_left_upgrade / static_entity->upgrade_stack.upgrade_seconds * spawn_speed_background.w, 6 };
					App->render->DrawQuad(spawn_speed_foreground, 18, 164, 62, 255, true, 0.0f);
				}
			}
			else if (static_entity->type == LABORATORY) {

				//Health bar stats
				SDL_Rect background_bar = { 530, 698, 122, 4 };
				SDL_Rect foreground_bar = { 531, 699, (float)static_entity->current_health / static_entity->max_health * background_bar.w, 4 };
				if (foreground_bar.w < 0) { foreground_bar.w = 0; }
				App->render->DrawQuad(background_bar, 255, 255, 255, 255, true, 0.0f);
				App->render->DrawQuad(foreground_bar, 18, 164, 62, 255, true, 0.0f);

				//Spawn bar

				SDL_Rect spawn_bar_background = { 995, 692, 75, 6 };
				App->render->DrawQuad(spawn_bar_background, 255, 255, 255, 255, true, 0.0f);

				if (static_entity->spawning) {

					if (static_entity->spawn_stack[0].type == MR_HANDY) {
						SDL_Rect spawn_bar_foreground = { 995, 692, static_entity->time_left / static_entity->spawn_stack[0].spawn_seconds * spawn_bar_background.w, 6 };
						App->render->DrawQuad(spawn_bar_foreground, 18, 164, 62, 255, true, 0.0f);
					}

				}

				//TIMER SHIELD BOOST
				SDL_Rect spawn_shield_background = { 1189, 692, 75, 6 };
				App->render->DrawQuad(spawn_shield_background, 255, 255, 255, 255, true, 0.0f);

				if (static_entity->upgrading)
				{
					SDL_Rect spawn_shield_foreground = { 1189, 692,  static_entity->time_left_upgrade / static_entity->upgrade_stack.upgrade_seconds * spawn_shield_background.w, 6 };
					App->render->DrawQuad(spawn_shield_foreground, 18, 164, 62, 255, true, 0.0f);
				}

			}
			break;
		case MUTANT:
			if (static_entity->type == BASE) {

				//Health bar stats
				SDL_Rect background_bar = { 530, 698, 122, 4 };
				SDL_Rect foreground_bar = { 531, 699, (float)static_entity->current_health / static_entity->max_health * background_bar.w, 4 };
				if (foreground_bar.w < 0) { foreground_bar.w = 0; }
				App->render->DrawQuad(background_bar, 255, 255, 255, 255, true, 0.0f);
				App->render->DrawQuad(foreground_bar, 18, 164, 62, 255, true, 0.0f);

				//Spawn bar

				SDL_Rect spawn_bar_background = { 995, 692, 75, 6 };

				App->render->DrawQuad(spawn_bar_background, 255, 255, 255, 255, true, 0.0f);

				if (static_entity->spawning) {

					if (static_entity->spawn_stack[0].type == GATHERER) {

						SDL_Rect spawn_bar_foreground = { 995, 692, static_entity->time_left / static_entity->spawn_stack[0].spawn_seconds * spawn_bar_background.w, 6 };
						App->render->DrawQuad(spawn_bar_foreground, 18, 164, 62, 255, true, 0.0f);

					}

					finish_base = true;

				}
				else if (static_entity->spawning == false && finish_base == true) {
					gatherer_amount--;
					App->menu_manager->DestroyMenu(Menu::GATHERER_QUANTITY);
					App->menu_manager->CreateMenu(Menu::GATHERER_QUANTITY);
					finish_base = false;
				}

				//TIMER BAG BOOST
				SDL_Rect spawn_bag_background = { 1092, 692, 75, 6 };
				App->render->DrawQuad(spawn_bag_background, 255, 255, 255, 255, true, 0.0f);

				if (static_entity->upgrading)
				{
					SDL_Rect spawn_bag_foreground = { 1092, 692,  static_entity->time_left_upgrade / static_entity->upgrade_stack.upgrade_seconds * spawn_bag_background.w, 6 };
					App->render->DrawQuad(spawn_bag_foreground, 18, 164, 62, 255, true, 0.0f);
				}
			}
			else if (static_entity->type == BARRACK) {

				//Health bar stats
				SDL_Rect background_bar = { 530, 698, 122, 4 };
				SDL_Rect foreground_bar = { 531, 699, (float)static_entity->current_health / static_entity->max_health * background_bar.w, 4 };
				if (foreground_bar.w < 0) { foreground_bar.w = 0; }
				App->render->DrawQuad(background_bar, 255, 255, 255, 255, true, 0.0f);
				App->render->DrawQuad(foreground_bar, 18, 164, 62, 255, true, 0.0f);

				//Spawn bar

				SDL_Rect spawn_bar_background = { 995, 692, 75, 6 };
				SDL_Rect spawn_bar_background2 = { 1092, 692, 75, 6 };

				App->render->DrawQuad(spawn_bar_background, 255, 255, 255, 255, true, 0.0f);
				App->render->DrawQuad(spawn_bar_background2, 255, 255, 255, 255, true, 0.0f);

				if (static_entity->spawning) {

					if(static_entity->spawn_stack[0].type == MELEE){
					SDL_Rect spawn_bar_foreground = { 995, 692, static_entity->time_left / static_entity->spawn_stack[0].spawn_seconds * spawn_bar_background.w, 6 };
					App->render->DrawQuad(spawn_bar_foreground, 18, 164, 62, 255, true, 0.0f);
					entity = 1;
					}
					
					else if (static_entity->spawn_stack[0].type == RANGED) {
					SDL_Rect spawn_bar_foreground = { 1092, 692, static_entity->time_left / static_entity->spawn_stack[0].spawn_seconds * spawn_bar_background2.w, 6 };
					App->render->DrawQuad(spawn_bar_foreground, 18, 164, 62, 255, true, 0.0f);
					entity = 2;
					}

					finish_barrack = true;
				}

				if (static_entity->spawning == false && finish_barrack == true) {
					
						if (entity == 1) {
							
							App->menu_manager->DestroyMenu(Menu::MELEE_QUANTITY);
							App->menu_manager->CreateMenu(Menu::MELEE_QUANTITY);
							finish_barrack = false;
							entity = 0;
						}
						else if (entity == 2) {
						
							App->menu_manager->DestroyMenu(Menu::RANGED_QUANTITY);
							App->menu_manager->CreateMenu(Menu::RANGED_QUANTITY);
							finish_barrack = false;
							entity = 0;
						}
					
				}

				//TIMER SPEED BOOST
				SDL_Rect spawn_speed_background = { 1189, 692, 75, 6 };
				App->render->DrawQuad(spawn_speed_background, 255, 255, 255, 255, true, 0.0f);

				if (static_entity->upgrading)
				{
					SDL_Rect spawn_speed_foreground = { 1189, 692,  static_entity->time_left_upgrade / static_entity->upgrade_stack.upgrade_seconds * spawn_speed_background.w, 6 };
					App->render->DrawQuad(spawn_speed_foreground, 18, 164, 62, 255, true, 0.0f);
				}


			}
			else if (static_entity->type == LABORATORY) {

				//Health bar stats
				SDL_Rect background_bar = { 530, 698, 122, 4 };
				SDL_Rect foreground_bar = { 531, 699, (float)static_entity->current_health / static_entity->max_health * background_bar.w, 4 };
				if (foreground_bar.w < 0) { foreground_bar.w = 0; }
				App->render->DrawQuad(background_bar, 255, 255, 255, 255, true, 0.0f);
				App->render->DrawQuad(foreground_bar, 18, 164, 62, 255, true, 0.0f);
				//Spawn bar

				SDL_Rect spawn_bar_background = { 995, 692, 75, 6 };
				App->render->DrawQuad(spawn_bar_background, 255, 255, 255, 255, true, 0.0f);

				if (static_entity->spawning) {

					if (static_entity->spawn_stack[0].type == MR_HANDY) {
						SDL_Rect spawn_bar_foreground = { 995, 692, static_entity->time_left / static_entity->spawn_stack[0].spawn_seconds * spawn_bar_background.w, 6 };
						App->render->DrawQuad(spawn_bar_foreground, 18, 164, 62, 255, true, 0.0f);
					}

				}

				//TIMER SHIELD BOOST
				SDL_Rect spawn_shield_background = { 1189, 692, 75, 6 };
				App->render->DrawQuad(spawn_shield_background, 255, 255, 255, 255, true, 0.0f);

				if (static_entity->upgrading)
				{
					SDL_Rect spawn_shield_foreground = { 1189, 692,  static_entity->time_left_upgrade / static_entity->upgrade_stack.upgrade_seconds * spawn_shield_background.w, 6 };
					App->render->DrawQuad(spawn_shield_foreground, 18, 164, 62, 255, true, 0.0f);
				}
			}
			break;
		}


	}
	else if (App->player->selected_entity == nullptr) {
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






