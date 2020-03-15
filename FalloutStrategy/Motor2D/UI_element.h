#ifndef __UI_ELEMENT__
#define __UI_ELEMENT__

#include "j1Gui.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"

struct SDL_Texture;

enum UI_Type {

	Button_play,
	Button_settings,
	Button_restart,
	Button_info,
	Button_close,
	Button_close_game,
	Button_menu,
	Button_slider_music_left,
	Button_slider_music_right,
	Button_slider_fx_left,
	Button_slider_fx_right,
	Label,
	Label_timer,
	Image,
	Slider_music,
	Slider_fx,
	Heart,
	Timer,
	Coin,
};

class UI_element 
{
public:

	UI_element(int x, int y, UI_Type type, UI_element* parent, j1Module* Observer);

	//Destructor
	virtual ~UI_element() {}

	// Called each loop iteration
	virtual bool Update(float dt);

	// Called each loop iteration
	virtual bool Draw();

	// Called before quitting
	virtual bool CleanUp();

	bool IsIntersection();


public:

	//UI elements position and dimensions
	iPoint pos;
	SDL_Rect dimensions;
	
	SDL_Texture* texture;

	iPoint mouse_pos;

	j1Module* observer = nullptr;

	bool destroy;

	//pointer to an element
	UI_element*	element_UI;

};


#endif //_!_UI_ELEMENT