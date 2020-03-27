#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;
class DynamicEntity;

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

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	StatesMenu GetMenuState();
	void SetMenuState(const StatesMenu& menu);

public:

	bool create = false;
	StatesMenu menu_state = StatesMenu::NONE;


private:

	int SongPlaying = 0;

};

#endif // __j1SCENE_H__
