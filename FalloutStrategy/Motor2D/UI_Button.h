#ifndef __UI_BUTTON__
#define __UI_BUTTON__

#include "UI_element.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Image.h"

struct SDL_Texture;

class UI_Button : public UI_element, j1Module
{
public:

	UI_Button(int x, int y, UI_Type type, SDL_Rect idle, SDL_Rect hover, SDL_Rect click, UI_element* parent, j1Module* Observer);

	//Destructor
	~UI_Button() {}

	// Called each loop iteration
	bool Update(float dt);

	// Called each loop iteration
	bool Draw();

	// Called before quitting
	bool CleanUp();


public:

	//Rects for button

	//idle when is not intersecting
	SDL_Rect Button_idle;

	//hover when is intersecting
	SDL_Rect Button_hover;

	//click when is clicking
	SDL_Rect Button_click;

	UI_Type t;

	int click_fx;
	int hover_fx;
	int back_fx;
	int volume_fx;
	int members_fx;
	int counter;
	int character_fx;


};


#endif //_!_UI_BUTTON