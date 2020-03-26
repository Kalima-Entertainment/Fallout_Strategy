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
#include "brofiler/Brofiler/Brofiler.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
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
	folder.create(node.child("folder").child_value());
	UI_file_name = config.child("ui").attribute("file1").as_string();

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	texture = App->tex->Load(PATH(folder.GetString(),UI_file_name.GetString()));

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	
	return true;
}

bool j1Gui::Update(float dt) {

	

	for (int i = 0; i < ui_element.count(); i++) {

		if (ui_element.At(i) != nullptr) {

			ui_element.At(i)->data->Draw();
			ui_element.At(i)->data->Update(dt);

		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) {

		debug_UI = !debug_UI;

	}

	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	

	return true;
}


// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");
	
	p2List_item<UI_element*>* element = ui_element.start;
	
	while (element != nullptr)
	{
		ui_element.del(element);
		delete element->data;
		element = element->next;
	} 

	return true;
}

bool j1Gui::Delete_Element(UI_element* element) {
	
	ui_element.find(element);
	p2List_item<UI_element*>* item = nullptr;
	for (item = ui_element.start; item; item = item->next)
	{
		if (item->data == element)
		{
			ui_element.del(item);
		}
	}

	
	return true;
}

// const getter for atlas
SDL_Texture* j1Gui::GetAtlas() const { return texture; }

// class Gui

UI_element* j1Gui::CreateButton(int x, int y, UI_Type type, SDL_Rect idle, SDL_Rect hover, SDL_Rect click, UI_element* parent, j1Module* Observer) {
	
	UI_Button* button = new UI_Button(x, y, type, idle, hover, click, parent, Observer);

	ui_element.add(button);

	return button;
}

UI_element* j1Gui::CreateImage(int x, int y, UI_Type type, SDL_Rect rect, UI_element* parent, j1Module* Observer)
{
	j1Image* image = new j1Image(x, y, type, rect, parent, Observer);

	ui_element.add(image);

	return image;
}

UI_element* j1Gui::CreateSlider(int x, int y, UI_Type type, SDL_Rect scrollbar, SDL_Rect button, float width, UI_element* parent, j1Module* Observer)
{
	UI_Slider* slider = new UI_Slider(x, y, type, scrollbar, button, width, parent, Observer);

	ui_element.add(slider);

	return slider;
}


UI_element* j1Gui::CreateLabel(int x, int y, UI_Type type, p2SString text_input, UI_element* parent, j1Module* Observer, int* counter)
{
	UI_Label* label = new UI_Label(x, y, type, text_input, parent, Observer, counter);

	label->SetLabelText(text_input);
	ui_element.add(label);

	return label;
}

