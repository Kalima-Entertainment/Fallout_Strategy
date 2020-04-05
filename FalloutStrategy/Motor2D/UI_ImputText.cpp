#include "UI_ImputText.h"
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

ImputText::ImputText(int x, int y, UI_Type type, p2SString text_input, UI_element* parent, j1Module* Observer) : UI_element(x, y, type, parent, Observer) {

	type = InputBox;
	this->pos.x = x;
	this->pos.y = y;

	labelInputText = (UI_Label*)App->gui->CreateLabel(x, y, Label, text_input, this, NULL, nullptr);


}

ImputText::~ImputText() {

}


bool ImputText::Update(float dt) {
	
	if (InputText_Actived) {	
		
		App->render->DrawQuad({ pos.x + r.w,pos.y,3,30 }, 255, 255, 255, 255);
		if (App->input->isPresed) {
			text += App->input->newLetter;
			App->font->CalcSize(text.data(), r.w, r.h);
			texture = App->font->Print(text.data());
			App->input->isPresed = false;
		}

		if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN) {
			text.pop_back();
			App->font->CalcSize(text.data(), r.w, r.h);
			texture = App->font->Print(text.data());
		}
		App->render->Blit(texture, pos.x, pos.y, &r);
	}

	if (App->input->GetKey(SDL_SCANCODE_GRAVE) == KEY_DOWN) {
		InputText_Actived = !InputText_Actived;
		if (InputText_Actived) {
			labelInputText->SetLabelText("");
			SDL_StartTextInput();

		}
	}

	return true;
}
