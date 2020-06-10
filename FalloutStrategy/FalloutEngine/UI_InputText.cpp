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
#include "p2Log.h"
#include "j1Console.h"

InputText::InputText(int x, int y, UI_Type type, const std::string &text_input, UI_element* parent, j1Module* Observer,const std::string &font) : UI_element(x, y, type, parent, Observer) {

	InputText_Actived = false;
	texture = nullptr;
	type = InputBox;
	this->pos.x = x;
	this->pos.y = y;
	H = 0;
	W = 0;
	text.assign(text_input);
	//text = text_input;
	font_text = font;
	texture = App->font->Print(text.data(), { 255,255,255,255 }, font_text);
	r = { 0,0,0,0 };
}

InputText::~InputText() {
	observer = nullptr;
}


bool InputText::Update(float dt) {

	if (InputText_Actived) {
	
		App->render->DrawQuad({ pos.x + r.w,pos.y,3,30 }, 255, 255, 255, 255, true, false);
		
		if (App->input->isPresed) {
			text += App->input->newLetter;
			App->font->CalcSize(text.data(), r.w, r.h);
			texture = App->font->Print(text.data(), {255,255,255,255}, font_text);
			App->input->isPresed = false;
		}

		
		if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN && !text.empty()) {
			
				text.pop_back();
				
				if(text.size()>0){
					App->font->CalcSize(text.data(), r.w, r.h);
					texture = App->font->Print(text.data(), {255, 255, 255, 255}, font_text);
				}
		
		}
		
		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
			if (text.size() == 0)
				text = ("Please introduce text");

			if (observer == dynamic_cast<j1Module*>(App->console)) {
				App->console->ProcessCommand(text.data());
			}
		}

		if(!text.empty()){
			App->render->Blit_UI(texture, pos.x, pos.y, &r, SDL_FLIP_NONE, 0.0f);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_GRAVE) == KEY_DOWN || App->console->isVisible) {
		InputText_Actived = !InputText_Actived;
	}

	return true;
}
