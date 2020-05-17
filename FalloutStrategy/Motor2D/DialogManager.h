#ifndef _DIALOG_MANAGER_H
#define _DIALOG_MANAGER_H
#include "j1Module.h"
#include <string>
#include <vector>
#include "SDL_image/include/SDL_image.h"

class UI_Button;

struct Dialog {
	std::string statement;
	std::string options[3];
	std::string response[3];
};

class DialogManager : public j1Module
{
public:
	DialogManager();
	~DialogManager();

	bool Start();
	bool Awake(pugi::xml_node&);
	bool Update(float dt);
	bool PostUpdate();

private:
	bool LoadDialogs();

public:
	std::vector<Dialog*> dialogs;
	SDL_Rect background_box;
	SDL_Rect statement_box;
	SDL_Rect option_box[3];
	SDL_Rect borders[5];
};


#endif // !_DIALOG_MANAGER_H
