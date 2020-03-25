#ifndef __UI_SLIDER__
#define __UI_SLIDER__

#include "UI_element.h"
#include "j1Textures.h"
#include "SDL/include/SDL.h"
#include "p2SString.h"

struct SDL_Texture;


class UI_Slider : public UI_element
{
public:


	UI_Slider(int x, int y, UI_Type type, SDL_Rect scrollbar, SDL_Rect button, float width, UI_element* parent, j1Module* Observer);

	~UI_Slider();

	bool Draw();

	bool Update(float dt);

	bool Mouse_Is_Moving();

	float get_valors();

	void Debug();


public:

	iPoint new_mouse_pos_music;
	iPoint new_mouse_pos_fx;
	float mouse_position_in_button;

private:
	
	SDL_Rect Button_Scrollbar_volume, Button_Scrollbar_fx;
	SDL_Rect Scrollbar_volume, Scrollbar_fx;

	UI_Type t;
	float initial_v, max_v, actual_pos_v;
	float initial_f, max_f, actual_pos_f;

};

#endif //_!_UI_SLIDER_