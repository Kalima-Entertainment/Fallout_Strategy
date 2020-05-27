#pragma once

#include "UI_element.h"
#include <string.h>
#include "SDL\include\SDL.h"
class UI_Label;

class InputText : public UI_element
{
private:

public:

	InputText(int x, int y, UI_Type type, std::string text_input, UI_element* parent, j1Module* Observer, std::string font);
	~InputText();

	bool Update(float dt);

public:

	bool InputText_Actived;
	int H, W;

	std::string text;

	SDL_Texture* texture;
	SDL_Rect r;

	UI_Label* labelInputText;

};
