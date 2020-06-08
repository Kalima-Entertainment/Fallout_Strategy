#ifndef _j1MINIMAP_H
#define _j1MINIMAP_H
#include "j1Module.h"
#include "p2Point.h"
#include "SDL/include/SDL.h"
#include "j1Timer.h"
#include "j1PerfTimer.h"

#define Pi 3.1415

struct Line {
	int x1;
	int x2;
	int y1;
	int y2;
};

enum class Corner {
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_RIGHT
};

class j1Minimap : public j1Module {
public:
	j1Minimap();
	~j1Minimap();

	bool Start();
	bool Awake(pugi::xml_node& config);
	bool PostUpdate();
	bool CleanUp();
	bool Reset();

	bool CreateMinimap();
	iPoint WorldToMinimap(int x, int y);
	iPoint ScreenToMinimapToWorld(int x, int y);
	fPoint MapToMinimap(int x, int y);
	iPoint MinimapToMap(int x, int y);
	void EnableRadar();
	void DrawFogOfWar();

	void Show();
	void Hide();
	bool IsVisible();

public:
	iPoint position;
	int width;
	int height;

	unsigned short int grid[150][150];

private:
	float scale;
	int map_width;
	int map_height;
	int margin;
	Corner corner;
	SDL_Texture* texture;
	j1PerfTimer	ptimer;

	//radar
	bool radar;
	j1Timer radar_timer;
	int radar_time;
	Line radar_line;
	float minimap_x_center;
	bool visible;
};

#endif // !_j1MINIMAP_H


