#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;
class DynamicEntity;
class GenericPlayer;
class Deathclaw;
#define RECT_MIN_AREA 15

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

	bool create;
	bool load_game;
	bool create_tutorial;
	StatesMenu menu_state;
	GenericPlayer* players[4];
	Deathclaw* Deathclaws[4];
	std::string modules[4];

	//Rectangle Selection needed
	iPoint mouse_pos;
	iPoint rectangle_origin;
	int rectangle_width;
	int rectangle_height;
	bool win;
	bool lose;
	bool deathclaw1;
	bool deathclaw2;
	bool deathclaw3;
	bool deathclaw4;
	
private:
	int SongPlaying;
	int beaten_enemies;
};

#endif // __j1SCENE_H__
