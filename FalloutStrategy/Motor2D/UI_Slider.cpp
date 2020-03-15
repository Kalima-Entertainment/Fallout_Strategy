#include "p2Log.h"
#include "j1Audio.h"
#include "j1Gui.h"
#include "UI_Button.h"
#include "UI_Slider.h"
#include "j1Render.h"
#include "j1Map.h"

#include "./brofiler/Brofiler/Brofiler.h"

UI_Slider::UI_Slider(int x, int y, UI_Type type, SDL_Rect scrollbar, SDL_Rect button, float width, UI_element* parent, j1Module* Observer) : UI_element(x, y, type, parent, Observer)
{
	texture = App->gui->GetAtlas();
	
	dimensions.w = button.w + 150;
	dimensions.h = button.h + 25;

	//App->audio->Change_Volume_Music(get_valors());

	t = type;

	if (t == Slider_music) {
		
		initial_v = x;
		Scrollbar_volume = scrollbar;

		max_v = initial_v + (Scrollbar_volume.w + 34);

		actual_pos_v = x + 89;

		Button_Scrollbar_volume = button;

		//actual_pos_v = ((App->audio->Get_Music_Volume()*(max_v - initial_v)) / 128) + initial_v;
		//App->audio->Change_Volume_Music(get_valors());
	}
	if (t == Slider_fx) {
		
		initial_f = x;
		Scrollbar_fx = scrollbar;

		max_f = initial_f + (Scrollbar_fx.w + 34);

		actual_pos_f = x + 89;

		Button_Scrollbar_fx = button;
		//actual_pos_f = ((App->audio->Get_Music_Volume()*(max_f - initial_f)) / 128) + initial_f;
		//App->audio->Change_Volume_FX(get_valors());
	
	}

}

UI_Slider::~UI_Slider() {
}

bool UI_Slider::Update(float dt)
{
	

	if (IsIntersection() == true) {

		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
			
			/*if (mouse_position_in_button == -1) {
				
				App->input->GetMousePosition(new_mouse_pos.x, new_mouse_pos.y);
				mouse_position_in_button = new_mouse_pos.x;

			}*/
			
			Mouse_Is_Moving();

			if (t == Slider_music) {
				//App->audio->Change_Volume_Music(get_valors());
			}
			if (t == Slider_fx) {
				//App->audio->Change_Volume_FX(get_valors());
			}
	
		}
		
	}

	/*else {

		mouse_position_in_button = -1;

	}*/
	
	//LOG("%i", App->gui->volume_up);
	
	if (t == Slider_music) {

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->gui->volume_up == 1) {

			if (actual_pos_v > initial_v) {

				//App->audio->Change_Volume_Music(get_valors());
				actual_pos_v -= 10;

			}

		}

		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || App->gui->volume_up == 2) {

			if (actual_pos_v < initial_v + 178) {

				actual_pos_v += 10;
				//App->audio->Change_Volume_Music(get_valors());

			}

		}

	}

	if (t == Slider_fx) {

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->gui->fx_up == 1) {

			if (actual_pos_f > initial_f) {

				//App->audio->Change_Volume_FX(get_valors());
				actual_pos_f -= 10;

			}

		}

		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || App->gui->fx_up == 2) {

			if (actual_pos_f < initial_f + 178) {

				actual_pos_f += 10;
				//App->audio->Change_Volume_FX(get_valors());

			}

		}

	}
	
	//LOG("%f", current_point);
		
	return true;
}

bool UI_Slider::Mouse_Is_Moving() {

	if (t == Slider_music) {
		App->input->GetMousePosition(new_mouse_pos_music.x, new_mouse_pos_music.y);
		actual_pos_v = new_mouse_pos_music.x;
	}

	if (t == Slider_fx) {
		App->input->GetMousePosition(new_mouse_pos_fx.x, new_mouse_pos_fx.y);
		actual_pos_f = new_mouse_pos_fx.x;
	}
		
	/*if (new_mouse_pos.x < actual_pos) {
		actual_pos = initial;
		return false;
	}

	if (new_mouse_pos.x > max) {

		actual_pos = max;
		return false;
	}*/

	return true;

}

bool UI_Slider::Draw() {

	texture = App->gui->GetAtlas();
	
	if (t == Slider_music) {
		App->render->Blit_UI(texture, pos.x, pos.y, &Scrollbar_volume, SDL_FLIP_NONE, 0.0f);
		App->render->Blit_UI(texture, actual_pos_v, pos.y - 4, &Button_Scrollbar_volume, SDL_FLIP_NONE, 0.0f);
		Debug();
	}

	if (t == Slider_fx) {
		App->render->Blit_UI(texture, pos.x, pos.y, &Scrollbar_fx, SDL_FLIP_NONE, 0.0f);
		App->render->Blit_UI(texture, actual_pos_f, pos.y - 4, &Button_Scrollbar_fx, SDL_FLIP_NONE, 0.0f);
		Debug();
	}
	
	return true;

}

float UI_Slider::get_valors()
{
	
	if (t == Slider_music) {
		int max_valor = max_v - initial_v;
		float min_valor = max_valor / 100.0f;
		float current;

		current = actual_pos_v - initial_v;

		float ret = current / min_valor;

		//LOG("%f", ret);

		return ret;
	}

	if (t == Slider_fx) {
		int max_valor = max_f - initial_f;
		float min_valor = max_valor / 100.0f;
		float current;

		current = actual_pos_f - initial_f;

		float ret = current / min_valor;

		//LOG("%f", ret);

		return ret;
	}

}

void UI_Slider::Debug() {

	//F8 debug
	
	if (t == Slider_music) {
		
		if (App->gui->debug_UI == true) {

			SDL_Rect bar{ pos.x, pos.y, Scrollbar_volume.w, Scrollbar_volume.h };

			App->render->DrawQuad(bar, 255, 0, 0, 255, false, 0.0f);

			SDL_Rect button_scrollbar{ actual_pos_v, pos.y - 4, Button_Scrollbar_volume.w, Button_Scrollbar_volume.h };

			App->render->DrawQuad(button_scrollbar, 255, 0, 0, 255, false, 0.0f);
		}

	}

	else if (t == Slider_fx) {
		
		if (App->gui->debug_UI == true) {

			SDL_Rect bar{ pos.x, pos.y, Scrollbar_fx.w, Scrollbar_fx.h };

			App->render->DrawQuad(bar, 255, 0, 0, 255, false, 0.0f);

			SDL_Rect button_scrollbar2{ actual_pos_f, pos.y - 4, Button_Scrollbar_fx.w, Button_Scrollbar_fx.h };

			App->render->DrawQuad(button_scrollbar2, 255, 0, 0, 255, false, 0.0f);
		}
	}

}