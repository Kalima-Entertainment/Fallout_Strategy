#include "j1Console.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Window.h"
#include "UI_Label.h"
#include "p2Log.h"
#include "j1EntityManager.h"
#include "j1Gui.h"
#include "UI_InputText.h"
#include "brofiler/Brofiler/Brofiler.h"
#include "j1Player.h"

j1Console::j1Console() : j1Module() {
	isVisible = false;
	input_box = nullptr;
	CleanUpStarted = false;
	l = 0;
	name = "console";
}

j1Console::~j1Console() {}

bool j1Console::Awake(pugi::xml_node& config) {
	bool ret = true;
	pugi::xml_node log_box_node = config.child("log_box");
	pugi::xml_node input_box_node = config.child("input_box");

	//log box color
	log_box_color.r = log_box_node.attribute("r").as_int();
	log_box_color.g = log_box_node.attribute("g").as_int();
	log_box_color.b = log_box_node.attribute("b").as_int();
	log_box_color.a = log_box_node.attribute("a").as_int();

	//input box color
	input_box_color.r = input_box_node.attribute("r").as_int();
	input_box_color.g = input_box_node.attribute("g").as_int();
	input_box_color.b = input_box_node.attribute("b").as_int();
	input_box_color.a = input_box_node.attribute("a").as_int();

	return ret;
}

bool j1Console::Start() {
	bool ret = true;
	AddLogText("Console started");
	uint width, height;
	App->win->GetWindowSize(width, height);
	log_box = { -App->render->camera.x, -App->render->camera.y, (int)width, 350 };
	command_background = { -App->render->camera.x, log_box.h, (int)width, 40 };

	CreateCommand("help", "list all console commands", this);
	CreateCommand("fps", "Change FPS cap", this);

	return ret;
}

bool j1Console::PreUpdate() {
	bool ret = true;
	return ret;
}

bool j1Console::Update(float dt) {
	BROFILER_CATEGORY("ConsoleUpdate", Profiler::Color::MediumPurple)
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_GRAVE) == KEY_DOWN)
	{
		if (isVisible){
			DestroyInterface();
			isVisible = false;
			App->isPaused = false;
		}
		else {
			CreateInterface();
			isVisible = true;
			App->isPaused = true;
		}
	}

	last_dt = dt;

	return ret;
}

bool j1Console::PostUpdate() {
	BROFILER_CATEGORY("GuiPostUpdate", Profiler::Color::DarkViolet)
		bool ret = true;

	if (isVisible)
	{
		//background 
		log_box.x = command_background.x = -App->render->camera.x;
		log_box.y = -App->render->camera.y;
		command_background.y = log_box.y + log_box.h;
		App->render->DrawQuad(log_box, log_box_color.r, log_box_color.g, log_box_color.b, log_box_color.a);
		App->render->DrawQuad(command_background, input_box_color.r, input_box_color.g, input_box_color.b, input_box_color.a);
		App->render->DrawQuad(command_background, 255, 255, 255, 255, false);
		//log text
		for (int i = 0; i < on_screen_log.size(); i++)
		{
			on_screen_log[i]->Draw();
		}
		input_box->Update(last_dt);
		input_box->Draw();
	}
	return ret;
}

bool j1Console::CleanUp() {
	bool ret = true;

	log_record.clear();
	command_vector.clear();
	on_screen_log.clear();
	input_box = nullptr;

	return ret;
}

void j1Console::AddLogText(std::string incoming_text) {
	
	log_record.push_back(incoming_text);

	if (isVisible) {
		DestroyInterface();
		if (log_record.size() > MAX_LOG_RECORD)
		{
			log_record.erase(log_record.begin());
		}
		CreateInterface();
	}
	else {
		if (log_record.size() > MAX_LOG_RECORD)
		{
			log_record.erase(log_record.begin());
		}
	}
}

void j1Console::CreateInterface() {
	int font_size = 14;
	int x_margin = 14;
	int spacing = 6;
	int j = 1;

	for (int i = log_record.size() -1; i >= 0; i--)
	{
		on_screen_log.push_back((UI_Label*)App->gui->CreateLabel(0 + x_margin, log_box.h - (j * (font_size + spacing)) -spacing, Label, log_record[i].c_str(), NULL, this, NULL, "OpenSans-Light"));
		j++;
	}

	input_box = (InputText*)App->gui->CreateInputBox(x_margin, log_box.h , InputBox, " ", NULL, this, "StackedPixelSmall");
}

void j1Console::DestroyInterface() {

	for (int i = 0; i < on_screen_log.size(); i++)
	{
		App->gui->Delete_Element(on_screen_log[i]);
		on_screen_log[i] = nullptr;
	}

	if (input_box != nullptr) {
		App->gui->Delete_Element(input_box);
		input_box = nullptr;
	}

	on_screen_log.clear();
}

void j1Console::CreateCommand(std::string name, std::string description, j1Module* callback) {
	Command command;

	command.name = name;
	command.description = description;
	command.callback = callback;

	//check the command doesn't already exist 
	for (int i = 0; i < command_vector.size(); i++)
	{
		if (command_vector[i].name == name)
			return;
	}

	//if it doesn't exist add it to the command list
	command_vector.push_back(command);
}

void j1Console::ProcessCommand(std::string command_text) {
	AddLogText(command_text);
	command_text = ToLower(command_text);

	//break the command in parts
	std::vector<std::string> command_parts;
	int cut_beginning = 0;
	for (int i = 0; i < command_text.size(); i++)
	{
		if (command_text[i] == ' ') {
			command_parts.push_back(command_text.substr(cut_beginning, i-cut_beginning));
			cut_beginning = i+1;
		}
	}
	command_parts.push_back(command_text.substr(cut_beginning, command_text.size() - cut_beginning));
	
	//check if the command has been created
	j1Module* command_callback;
	command_callback = FindModule(command_parts[0]);

	//if yes send it to its creator
	if (command_callback != nullptr)
		command_callback->OnCommand(command_parts);
	else
		AddLogText("Invalid command");
}

j1Module* j1Console::FindModule(std::string command_beginning) {
	for (int i = 0; i < command_vector.size(); i++)
	{
		if (command_vector[i].name == command_beginning)
			return command_vector[i].callback;
	}
	return nullptr;
}

void j1Console::OnCommand(std::vector<std::string> command_parts) {
	std::string command_beginning = command_parts[0];
	
	if (command_beginning == "help") {
		for (int i = 0; i < command_vector.size(); i++)
		{
			std::string command_and_description = command_vector[i].name;
			command_and_description.append(": ").append(command_vector[i].description);
			AddLogText(command_and_description);
		}
	}

	if (command_beginning == "fps") {
		int framerate_cap = std::stoi(command_parts[1].c_str());
		if (framerate_cap < 30)
		{
			AddLogText("Framerate cap too low");
		}
		else if (framerate_cap > 120)
		{
			App->console->AddLogText("Framerate cap too high");
		}
		else
		{
			LOG("Framerate cap changed to: %i", std::stoi(command_parts[1].c_str()));
			App->capped_ms = 1000 / std::stoi(command_parts[1].c_str());
		}
	}
}

std::string ToLower(std::string string) {
	for (int i = 0; i < string.size(); i++)
	{
		string[i] = tolower(string[i]);
	}
	return string;
}