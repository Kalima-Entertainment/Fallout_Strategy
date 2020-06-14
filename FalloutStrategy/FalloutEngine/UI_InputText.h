#pragma once

#include "UI_element.h"
#include <string.h>
#include "SDL\include\SDL.h"
class UI_Label;

class InputText : public UI_element
{
private:

public:

	InputText(int x, int y, UI_Type type, const std::string &text_input, UI_element* parent, j1Module* Observer, const std::string &font);
	~InputText();

	bool Update(float dt);

	UI_Type GetType();

public:

	bool InputText_Actived;
	int H, W;

	std::string text;

	SDL_Texture* texture;
	SDL_Rect r;
};
