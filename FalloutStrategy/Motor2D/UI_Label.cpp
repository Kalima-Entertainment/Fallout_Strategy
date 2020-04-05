#include "UI_Label.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "UI_element.h"
#include <string.h>

UI_Label::UI_Label(int x, int y, UI_Type type, std::string text_input, UI_element * parent, j1Module * Observer, int* counter) : UI_element(x, y, type, parent, Observer)
{
	t = type;
	timer_out = counter;
}

UI_Label::~UI_Label()
{
}

bool UI_Label::Update(float dt)
{

	
	/*if (t == Label) {

		timer_count = *timer_out;
		d = 100000;

		for (int i = 0; i < 6; i++) {

			timer[i] = (char)(timer_count/d);

			timer_count = timer_count%d;

			d = d / 10;

		}

		SetTextTimer(timer);

	}*/

	return true;
}

bool UI_Label::CleanUp()
{
	App->tex->UnLoad(texture);
	return true;
}

bool UI_Label::SetLabelText(std::string text_input, std::string font)
{
	bool ret = false;
	text = text_input;

	if (text.size() > 0){

		text_texture = App->font->Print(text.c_str(), { 244,244,244,255 }, font);
	}

	if (text_texture != nullptr)
	{

		uint w = 0, h = 0;

		App->tex->GetSize(text_texture, w, h);

		SDL_Rect rect = {0,0,(int)w,(int)h};

		texture = text_texture;

		this->text = text_input;
		this->dimensions = rect;

		ret = true;

	}


	return ret;
}

void UI_Label::SetTextTimer(const char* text)
{

	App->tex->UnLoad(texture);
	texture = App->font->Print(text, { 255,255,255,255 }, "StackedPixel");
	App->font->CalcSize(text, dimensions.w, dimensions.h);

}

bool UI_Label::Draw()
{
	if (texture != nullptr) {
		App->render->Blit_UI(texture, pos.x, pos.y, &dimensions, SDL_FLIP_NONE, 0.0f);
	}

	if (App->gui->debug_UI == true) {

		SDL_Rect lbl{ pos.x, pos.y, dimensions.w, dimensions.h };

		App->render->DrawQuad(lbl, 255, 0, 0, 255, false, 0.0f);
	}

	return true;
}
