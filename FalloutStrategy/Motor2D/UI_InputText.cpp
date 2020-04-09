#include "UI_InputText.h"
#include "UI_element.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"
#include "UI_Label.h"
#include "j1Image.h"
#include "j1Input.h"

InputText::InputText(int x, int y, UI_Type type, std::string text_input, UI_element* parent, j1Module* Observer, std::string font) : UI_element(x, y, type, parent, Observer) {

	type = InputBox;
	this->pos.x = x;
	this->pos.y = y;

	labelInputText = (UI_Label*)App->gui->CreateLabel(x, y, Label, text_input, NULL, NULL, NULL, font);

	font_text = font;
}

InputText::~InputText() {}


bool InputText::Update(float dt) {

	if (InputText_Actived) {

		App->render->DrawQuad({ pos.x + r.w,pos.y,3,30 }, 255, 255, 255, 255);
		if (App->input->isPresed) {
			text += App->input->newLetter;
			App->font->CalcSize(text.data(), r.w, r.h);
			texture = App->font->Print(text.data(), {255,255,255,255}, font_text);
			App->input->isPresed = false;
		}

		if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN) {
			if (text.size() > 0)
			{
				text.pop_back();
				App->font->CalcSize(text.data(), r.w, r.h);
				texture = App->font->Print(text.data(), {255, 255, 255, 255}, font_text);
			}
		}

		if(text.size() > 0)
			App->render->Blit(texture, pos.x, pos.y, &r);
	}

	if (App->input->GetKey(SDL_SCANCODE_GRAVE) == KEY_DOWN) {
		InputText_Actived = !InputText_Actived;
		if (InputText_Actived) {
			labelInputText->SetLabelText("", font_text);
			SDL_StartTextInput();
		}
	}

	return true;
}