#ifndef _j1CONSOLE_H_
#define _j1CONSOLE_H_
#include "j1Module.h"
#include "SDL_image/include/SDL_image.h"
#include <vector>
#include <string>

#define MAX_LOG_RECORD 20

class UI_Label;
struct SDL_Rect;
class InputText;

struct Command {
	char* text;
	j1Module* callback;
};

class j1Console : public j1Module
{
public:
	j1Console();
	~j1Console();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	//void OnCommand(p2SString command);

	void AddLogText(std::string incoming_text);

	void CreateInterface();
	void DestroyInterface();

	//void CreateCommand(const char* command, j1Module* callback, const char* explanation = "No explanation given");
	//void CheckCommand(p2SString command);

public:
	std::vector<std::string> log_record;
	std::vector<UI_Label*> on_screen_log;
	InputText* input_box;
	int l;
	bool isVisible;
	SDL_Rect log_box;
	SDL_Rect command_background;
	bool CleanUpStarted;
	float last_dt;
	//GuiInputText* command_input;
	//p2List<p2SString> input_commands;
	//p2List<j1Command*> commands;
	//p2List_item<p2SString>* current_consulting_command;
};

#endif // !_j1CONSOLE_H_

