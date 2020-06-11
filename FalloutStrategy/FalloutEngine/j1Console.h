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
class UI_Label;

struct Command {
	std::string name;
	std::string description;
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
	void OnCommand(std::vector<std::string> command_parts);

	void CreateCommand(const std::string &name,const std::string &description, j1Module* callback);
	void ProcessCommand(std::string command);

	void AddLogText(std::string incoming_text);
	j1Module* FindModule(const std::string &command_parts);

	void CreateInterface();
	void DestroyInterface();

public:
	std::vector<std::string> log_record;
	std::vector<Command> command_vector;
	std::vector<UI_Label*> on_screen_log;
	InputText* input_box;
	int l;
	bool isVisible;
	SDL_Rect log_box;
	SDL_Rect command_background;
	bool CleanUpStarted;
	float last_dt;

	UI_Label* labelInputText;

private:
	SDL_Color log_box_color;
	SDL_Color input_box_color;
};

std::string ToLower(std::string string);

#endif // !_j1CONSOLE_H_

