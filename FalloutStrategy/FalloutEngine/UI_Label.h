#ifndef __UI_LABEL_H__
#define __UI_LABEL_H__

#include "UI_element.h"
#include <string>
#include "p2List.h"
#include "SDL\include\SDL.h"

class UI_Label : public UI_element
{
public:

	UI_Label(int x, int y, UI_Type type, std::string text_input, UI_element * parent, j1Module * Observer, int* counter);
	~UI_Label();

	bool Update(float dt);

	bool SetLabelText(std::string text_input, std::string font = "StackedPixel");
	void SetTextTimer(const char* text);

	bool Draw();

public:

	std::string text;
	SDL_Texture* text_texture;
	UI_Type t;

	//Timer
	int d;
	char timer[7] = "";
	int* timer_out;
	int timer_count;

};

#endif //_!_UI_LABEL__