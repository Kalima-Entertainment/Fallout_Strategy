#ifndef __UI_BUTTON__
#define __UI_BUTTON__

#include "UI_element.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Image.h"
#include "UI_Label.h"
#include <list>

struct SDL_Texture;

enum class BUTTON_STATE
{
	HOVER,
	HOVER_EXIT,
	NONE
};


class UI_Button : public UI_element, j1Module
{
public:

	UI_Button(int x, int y, UI_Type type, SDL_Rect idle, SDL_Rect hover, SDL_Rect click, UI_element* parent, j1Module* Observer);

	//Destructor
	~UI_Button();

	// Called each loop iteration
	bool Update(float dt);
	bool Start();
	// Called each loop iteration
	bool Draw();

	UI_Type GetType();

public:

	//Rects for button

	//idle when is not intersecting
	SDL_Rect Button_idle;

	//hover when is intersecting
	SDL_Rect Button_hover;

	//click when is clicking
	SDL_Rect Button_click;

	UI_Type t;

	void DoNotShowElement(UI_element*);

	j1Image* faction_image;

	BUTTON_STATE current_state;
	std::list<UI_element*> elements_to_show;

	bool inHover;
	bool fx_played;
	bool radio;
};


#endif //_!_UI_BUTTON