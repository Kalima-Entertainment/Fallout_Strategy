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
	Boost_base_button,
	Boost_barrack_button,
	Boost_lab_button,
	Boost_radar_button,
	summon_mrhandy_button,

	//TUTORIAL
	control_button,
	how_to_play_button,
	info_button,
	info_button2,
	back_tutorial_controls,
	back_tutorial_how_to_play,
	how_to_play_next,
	how_to_play_previous,

	//Dialogs
	OPTION_A,
	OPTION_B,
	OPTION_C,
	ADVANCE_DIALOGS,
	FINISH_DIALOGS,

	//Quests
	quest_button,
	quest_button2,
	continue_button,

	//Radio
	Radio_button,
	Radio_off_button,
	RockFM_button,
	AyuwokiFM_button,
	SlimShadyFM_button,
	UltraRADIO_button,
	FiestaLatinaFM_button,
	FusionCoreFM_button,

	//Group Photos
	marc_photo,
	silvino_photo,
	pol_photo,
	german_photo,
	pablo_photo,
	javi_photo,
	christian_photo,
	macia_photo,

	UI_NONE
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

	virtual UI_Type GetType() { return UI_NONE; };

public:

	//UI elements position and dimensions
	iPoint pos;
	
	SDL_Rect dimensions;
	
	SDL_Texture* texture;

	iPoint mouse_pos;

	j1Module* observer = nullptr;

	std::string font_text;

	bool destroy;

	bool drawable;
	bool hover;

	//pointer to an element
	UI_element*	element_UI;

};

#endif //_!_UI_ELEMENT