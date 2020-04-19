#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1Image.h"
#include "UI_Slider.h"
#include "UI_element.h"
#include "UI_Button.h"
#include "UI_Label.h"
#include "UI_InputText.h"
#include "brofiler/Brofiler/Brofiler.h"

j1Gui::j1Gui() : j1Module()
{
	name = ("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& config)
{
	LOG("Loading GUI atlas");
	bool ret = true;
	node = config;
	folder = node.child("folder").child_value();
	UI_file_name = config.child("ui").attribute("file1").as_string();

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	texture = App->tex->Load(PATH(folder.c_str(), UI_file_name.c_str()));

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{

	return true;
}

bool j1Gui::Update(float dt) {
	BROFILER_CATEGORY("GuiUpdate", Profiler::Color::Yellow)
		for (int i = 0; i < ui_element.size(); i++) {
			if (ui_element[i] != nullptr) {

				ui_element[i]->Update(dt);

			}
		}

	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) {

		debug_UI = !debug_UI;

	}

	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate() {
	BROFILER_CATEGORY("GuiPostUpdate", Profiler::Color::LightGreen)
		for (int i = 0; i < ui_element.size(); i++) {

			if (ui_element[i] != nullptr) {
				ui_element[i]->Draw();
			}
		}
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	for (int i = 0; i < ui_element.size(); i++)
	{
		Delete_Element(ui_element[i]);
	}

	return true;
}

void j1Gui::DeleteArrayElements(UI_element* array[], int size) {

	if (size != NULL) {
		for (int i = 0; i < size; i++)
		{
			Delete_Element(array[i]);
		}
	}
}

bool j1Gui::Delete_Element(UI_element* element) {

	for (int i = 0; i < ui_element.size(); i++)
	{
		if (element == ui_element[i]) {
			ui_element.erase(ui_element.begin() + i);
		}
	}

	return true;
}

// const getter for atlas
SDL_Texture* j1Gui::GetAtlas() const { return texture; }

// class Gui

UI_element* j1Gui::CreateButton(int x, int y, UI_Type type, SDL_Rect idle, SDL_Rect hover, SDL_Rect click, UI_element* parent, j1Module* Observer) {

	UI_Button* button = new UI_Button(x, y, type, idle, hover, click, parent, Observer);

	ui_element.push_back(button);

	return button;
}

UI_element* j1Gui::CreateImage(int x, int y, UI_Type type, SDL_Rect rect, UI_element* parent, j1Module* Observer)
{
	j1Image* image = new j1Image(x, y, type, rect, parent, Observer);

	ui_element.push_back(image);

	return image;
}

UI_element* j1Gui::CreateSlider(int x, int y, UI_Type type, SDL_Rect scrollbar, SDL_Rect button, float width, UI_element* parent, j1Module* Observer)
{
	UI_Slider* slider = new UI_Slider(x, y, type, scrollbar, button, width, parent, Observer);

	ui_element.push_back(slider);

	return slider;
}

UI_element* j1Gui::CreateLabel(int x, int y, UI_Type type, std::string text_input, UI_element* parent, j1Module* Observer, int* counter, std::string font)
{
	UI_Label* label = new UI_Label(x, y, type, text_input, parent, Observer, counter);

	label->SetLabelText(text_input, font);
	ui_element.push_back(label);

	return label;
}


UI_element* j1Gui::CreateInputBox(int x, int y, UI_Type type, std::string text_input, UI_element* parent, j1Module* Observer, std::string font)
{
	InputText* inputtext = new InputText(x, y, type, text_input, parent, Observer, font);

	ui_element.push_back(inputtext);

	return inputtext;
}
