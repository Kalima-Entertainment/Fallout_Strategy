#ifndef __j1FONTS_H__
#define __j1FONTS_H__

#include "j1Module.h"
#include <vector>
#include <string>
#include "SDL\include\SDL_pixels.h"

#define DEFAULT_FONT "fonts/stacked/StackedPixel.ttf"
#define DEFAULT_FONT_SIZE 38


struct SDL_Texture;
struct _TTF_Font;

struct Font {
	std::string name;
	_TTF_Font* font;
};

class j1Fonts : public j1Module
{
public:

	j1Fonts();

	// Destructor
	virtual ~j1Fonts();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Load Font
	_TTF_Font* const Load(const char* path, int size = 12);

	// Create a surface from text
	SDL_Texture* Print(const char* text, SDL_Color color = { 255, 255, 255, 255 }, std::string font_name = ("No_font"));

	bool CalcSize(const char* text, int& width, int& height, _TTF_Font* font = NULL) const;

public:
	std::vector<Font> fonts;
	_TTF_Font* default;
};


#endif // __j1FONTS_H__