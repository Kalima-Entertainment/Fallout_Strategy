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
	//command_input = nullptr;
	//current_consulting_command = nullptr;
}

j1Console::~j1Console() {}

bool j1Console::Awake(pugi::xml_node& config) {
	bool ret = true;

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
	//CreateCommand("list", (j1Module*)this, "List all console commands");
	//App->console->CreateCommand("quit", (j1Module*)this, "Quit the game");
	//App->console->CreateCommand("fps_", (j1Module*)this, "Change FPS cap");

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

	if (isVisible) {
		if ((App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)) {
			//DestroyInterface();
		}

		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) {
		}

		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN) {
		}

		last_dt = dt;
	}
	return ret;
}

bool j1Console::PostUpdate() {
	BROFILER_CATEGORY("GuiPostUpdate", Profiler::Color::DarkViolet)
		bool ret = true;

	if (isVisible)
	{
		//Draw console

		//background 
		log_box.x = command_background.x = -App->render->camera.x;
		log_box.y = -App->render->camera.y;
		command_background.y = log_box.y + log_box.h;
		App->render->DrawQuad(log_box, 0, 115, 25, 225);
		App->render->DrawQuad(command_background, 0, 80, 10, 245);

		//log text
		for (int i = 0; i < on_screen_log.size(); i++)
		{
			on_screen_log[i]->Draw();
		}
		input_box->Update(last_dt);
	}
	return ret;
}

bool j1Console::CleanUp() {
	bool ret = true;

	log_record.clear();

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
	int j = 0;

	for (int i = log_record.size(); i > 0; i--)
	{
		on_screen_log.push_back((UI_Label*)App->gui->CreateLabel(0 + x_margin, log_box.h - (j * (font_size + spacing)) -spacing, Label, log_record[i].c_str(), NULL, this, NULL, "OpenSans-Light"));
		j++;
	}

	input_box = (InputText*)App->gui->CreateInputBox(x_margin, log_box.h , InputBox, " ", NULL, this);
}

void j1Console::DestroyInterface() {
	for (int i = 0; i < on_screen_log.size(); i++)
	{
		App->gui->Delete_Element(on_screen_log[i]);
	}
	App->gui->Delete_Element(input_box);
	input_box = nullptr;
	on_screen_log.clear();
}

void j1Console::CreateCommand(std::string name, std::string description, j1Module* callback) {
	Command command;

	command.name = name;
	command.description = description;
	command.callback = callback;

	command_vector.push_back(command);
}

void j1Console::ProcessCommand(std::string command_text) {
	AddLogText(command_text);

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
			App->capped_ms = 1000 / std::stoi(command_parts[1].c_str());
		}
	}
}

/*
void j1Console::CreateCommand(const char* g_command, j1Module* g_callback, const char* explanation) {
	j1Command* command = new j1Command(g_command, g_callback, explanation);
	commands.add(command);
}
*/

/*

void j1Console::CheckCommand(p2SString command) {
	char given_initial_three[5] = "0000";
	char listed_initial_three[5] = "0000";
	char* given_command = (char*)command.GetString();


	for (p2List_item<j1Command*>* item = commands.start; item != nullptr; item = item->next)
	{
		if (item->data->text == command)
		{
			item->data->callback->OnCommand(command);
			return;
		}

		for (int i = 0; i < 4; i++)
		{
			given_initial_three[i] = command.GetString()[i];
			listed_initial_three[i] = item->data->text.GetString()[i];
		}

		if (strcmp(given_initial_three, listed_initial_three) == 0)
		{
			item->data->callback->OnCommand(command);
			return;
		}
	}

	LOG("Invalid command");
}

void j1Console::OnCommand(p2SString command) {
	char fps_string[8] = "fps_120";
	char initial_three[5] = "0000";

	if (command == "list")
	{
		for (p2List_item<j1Command*>* item = commands.start; item != nullptr; item = item->next)
		{
			//LOG(("%s : %s \n", item->data->text.GetString(), item->data->explanation);
			char string[200];
			strcpy_s(string, item->data->text.GetString());
			strcat_s(string, " : ");
			strcat_s(string, item->data->explanation);
			AddLogText(string);
		}
		return;
	}

	if (command == "quit") {
		App->quit = true;
		return;
	}

	for (int i = 0; i < 3; i++)
	{
		initial_three[i] = command.lowercased().GetString()[i];
	}

	if (strcmp(initial_three, "fps") == 0);
	{
		int fps = 0;
		if (command.Length() == 7)
		{
			char fps_char[3];
			fps_char[0] = command.GetString()[4];
			fps_char[1] = command.GetString()[5];
			fps_char[2] = command.GetString()[6];

			int units = (int)fps_char[2] - 48;
			int dozens = (int)fps_char[1] - 48;
			int hundreds = (int)fps_char[0] - 48;
			fps = units + dozens * 10 + hundreds * 100;
		}
		else if (command.Length() == 6)
		{
			char fps_char[2];
			fps_char[0] = command.GetString()[4];
			fps_char[1] = command.GetString()[5];
			int units = (int)fps_char[1] - 48;
			int dozens = (int)fps_char[0] - 48;
			fps = units + dozens * 10;
		}
		else
		{
			LOG("Invalid framerate cap number");
			return;
		}

		if (fps < 30) {
			LOG("Too low framerate cap");
		}
		else if (fps > 120)
		{
			LOG("Too high framerate cap");
		}
		else
		{
			App->framerate_cap = fps;
			LOG("Framerate cap changed to %i", fps);
		}
	}
}

*/