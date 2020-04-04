#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"
#include <vector>
#include <string>

#define CURSOR_WIDTH 2

struct SDL_Texture;
struct SDL_Rect;
class UI_element;
enum UI_Type;

// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	bool Update(float dt);

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	SDL_Texture* GetAtlas() const;

	void DeleteArrayElements(UI_element* array[], int size);
	bool Delete_Element(UI_element* element);

	void DestroyVectorElements(std::vector<UI_element*> vector);

	// Gui creation functions
	UI_element* CreateButton(int x, int y, UI_Type type, SDL_Rect idle, SDL_Rect hover, SDL_Rect click, UI_element* parent, j1Module* Observer);
	UI_element* CreateImage(int x, int y, UI_Type type, SDL_Rect rect, UI_element* parent, j1Module* Observer);
	UI_element* CreateSlider(int x, int y, UI_Type type, SDL_Rect scrollbar, SDL_Rect button, float width, UI_element* parent, j1Module* Observer);
	UI_element* CreateLabel(int x, int y, UI_Type type, std::string text_input, UI_element* parent, j1Module* Observer, int* counter);

	int volume_up = 0;
	int fx_up = 0;
	int count = 0;
	/*void CreateImage(SDL_Rect& dimensions);
	void CreateText(SDL_Rect& dimensions);
	void CreateSlider(SDL_Rect& dimensions);*/


public:
	std::vector<UI_element*> ui_element;

	bool debug_UI = false;

private:
	pugi::xml_node node;
	std::string folder;
	SDL_Texture* texture;
	std::string UI_file_name;

};

#endif // __j1GUI_H__