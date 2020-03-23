#ifndef __UI_ELEMENT__
#define __UI_ELEMENT__

#include "j1Gui.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"

struct SDL_Texture;

enum UI_Type {

	button_back_to_menu,
	button_select_ghoul,
	button_select_brotherhood,
	button_select_supermutant,
	button_select_vault,
	button_new_game,
	button_back_credits,
	button_github_credits,
	button_web_credits,
	button_twitter_credits,
	button_marc,
	button_javi,
	button_pablo,
	button_german,
	button_macia,
	button_pol,
	button_silvino,
	button_cristian,
	button_load_game,
	button_settings,
	button_settings_pause,
	button_credits,
	button_exit,
	Button_settings,
	Button_restart,
	Button_info,
	Button_close,
	Button_close_game,
	Button_menu,

	Slider_music,
	Slider_fx,
	Button_slider_music_left,
	Button_slider_music_right,
	Button_slider_fx_left,
	Button_slider_fx_right,

	Mute,
	FPS,
	Volume_Panel,
	Cap_Panel,
	button_cap,
	button_mute,
	button_back,
	button_back_pause,

	Label,
	Label_timer,
	Image,
	Logo,
	Panel,
	text_new_game,
	text_load_game,
	text_settings,
	text_credits,
	text_exit,
	text_fx_volume,
	text_music_volume,
	text_fps,
	ghouls_label,

	resume_button,
	RESUME,
	SETTINGS,
	EXIT,

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