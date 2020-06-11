#ifndef __J1CURSOR_H__
#define __J1CURSOR_H__

#include "j1Module.h"
#include "j1App.h"

class j1Cursor : public j1Module
{
public:

	j1Cursor();
	~j1Cursor();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

private:

	SDL_Rect cursorRect;
	iPoint	cursor_position;
	iPoint	cursor_offset;
};
#endif
