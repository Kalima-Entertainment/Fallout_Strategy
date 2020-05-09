#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;
class DynamicEntity;
class GenericPlayer;

#define RECT_MIN_AREA 5

enum class StatesMenu
{
	MAIN_MENU,
	SETTINGS,
	FACTION_SELECTION,
	PAUSE,
	NONE
};

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;


	StatesMenu GetMenuState();
	void SetMenuState(const StatesMenu& menu);

	void CheckWinner();

	//Creates rectangle and check wich entities are inside the are to create groups
	void RectangleSelection();

	void OnCommand(std::vector<std::string> command_parts);

public:

	int topleft;
	int topright;
	int bottomleft;
	int bottomright;
	bool create = false;
	StatesMenu menu_state = StatesMenu::NONE;
	GenericPlayer* players[4];

	std::string modules[4];

	//Rectangle Selection needed
	iPoint mouse_pos = { 0,0 };
	iPoint rectangle_origin = { 0,0 };
	int rectangle_width = 0;
	int rectangle_height = 0;
	bool win = false;
private:
	int SongPlaying = 0;
	int beaten_enemies;
};

#endif // __j1SCENE_H__
