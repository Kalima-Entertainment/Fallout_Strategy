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
// TR = TOP RIGHT TL = TOP LEFT BL = BOTTOM LEFT BR = BOTTOM RIGHT
enum MapModules
{
	TL_DESERT = 0,
	TL_WASTELAND,
	TL_OLDSTONE,
	TL_GREENLAND,
	TR_DESERT,
	TR_WASTELAND,
	TR_OLDSTONE,
	TR_GREENLAND,
	BL_DESERT,
	BL_WASTELAND,
	BL_OLDSTONE,
	BL_GREENLAND,
	BR_DESERT,
	BR_WASTELAND,
	BR_OLDSTONE,
	BR_GREENLAND

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

	int topleft;
	int topright;
	int bottomleft;
	int bottomright;
	bool create = false;
	StatesMenu menu_state = StatesMenu::NONE;


private:

	int SongPlaying = 0;

};

#endif // __j1SCENE_H__
