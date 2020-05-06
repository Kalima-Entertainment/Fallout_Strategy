#ifndef _j1MINIMAP_H
#define _j1MINIMAP_H
#include "j1Module.h"
#include "p2Point.h"
#include "SDL/include/SDL.h"
#include "j1Timer.h"
#include "j1PerfTimer.h"

#define PI 3.1415

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

	bool CreateMinimap();
	iPoint WorldToMinimap(int x, int y);
	iPoint ScreenToMinimapToWorld(int x, int y);
	void EnableRadar();

public:
	iPoint position;
	int width;
	int height;

	std::vector<iPoint> black_squares;
	std::vector<iPoint> grey_squares;

private:
	float scale;
	int map_width;
	int map_height;
	int margin;
	Corner corner;
	SDL_Texture* texture;
	j1PerfTimer	ptimer;
	std::vector<iPoint> node_map;

	//radar
	bool radar;
	j1Timer radar_timer;
	int radar_time;
	Line radar_line;
};

#endif // !_j1MINIMAP_H


