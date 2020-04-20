#ifndef __UI_ELEMENT__
#define __UI_ELEMENT__

#include "j1Gui.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"

struct SDL_Texture;

enum UI_Type {

	Image,
	Label,

	button_start_game,
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
	button_christian,
	button_load_game,
	button_settings,
	button_settings_pause,
	button_credits,
	button_exit,
	button_pause_to_main,
	button_back,
	button_back_pause,
	button_cap,
	button_mute,
	button_fullscreen,
	button_win_lose_to_main,

	Slider_music,
	Slider_fx,
	Button_slider_music_left,
	Button_slider_music_right,
	Button_slider_fx_left,
	Button_slider_fx_right,

	resume_button,
	
	Brotherhood_ghaterer_button,
	Brotherhood_melee_button,
	Brotherhood_ranged_button,


	Vault_ghaterer_button,
	Vault_melee_button,
	Vault_ranged_button,

	Supermutant_ghaterer_button,
	Supermutant_melee_button,
	Supermutant_ranged_button,

	Ghouls_ghaterer_button,
	Ghouls_melee_button,
	Ghouls_ranged_button,

	InputBox,
	Boost_button,

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

	std::string font_text;

	bool destroy;

	bool drawable = true;

	//pointer to an element
	UI_element*	element_UI;

};

#endif //_!_UI_ELEMENT