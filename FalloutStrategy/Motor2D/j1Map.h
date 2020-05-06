#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include <vector>
#include <string>

#define TILE_SIZE 64
#define HALF_TILE 32
#define MODULE_LENGTH 75
#define MAP_LENGTH 150
#define MAX_LAYERS 8
#define MAX_TILESETS 5

// ----------------------------------------------------
struct Properties
{
	struct Property
	{
		std::string name;
		int value;
	};

	~Properties()
	{
		p2List_item<Property*>* item;
		item = list.start;

		while(item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		list.clear();
	}

	int Get(const char* name, int default_value = 0) const;

	p2List<Property*>	list;
};

// ----------------------------------------------------
struct MapLayer
{
	std::string	name;
	int			width;
	int			height;
	uint		data[MAP_LENGTH * MAP_LENGTH];
	Properties	properties;

	MapLayer() {}

	~MapLayer() {}

	inline uint Get(int x, int y) const
	{
		int position = (y * width) + x;
		return data[position];
	}
};

// ----------------------------------------------------
struct TileSet
{
	SDL_Rect GetTileRect(int id) const;

	std::string			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------

struct Object {
	//ResourceBuilding
};


struct ObjectGroup {
	std::string name = ("No name");
	Object* object;
	uint size = 0u;
};

// ----------------------------------------------------

struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	TileSet				tilesets[MAX_TILESETS];
	MapLayer			layers[MAX_LAYERS];
	ObjectGroup			objectgroup;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(std::string modules[4]);

	iPoint MapToWorld(int x, int y) const;
	fPoint fMapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;
	iPoint fWorldToMap(float x, float y) const;
	iPoint IsometricWorldToMap(int x, int y) const;
	bool CreateWalkabilityMap() const;
	std::vector<iPoint> CalculateArea(iPoint first_tile_position, int width, int height);
	void SetBuildingTilesUnwalkable(std::vector<iPoint> tiles);
	int GetWidth() const { return data.width * data.tile_width; };
	TileSet* GetTilesetFromTileId(int id) const;

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer, int module_number);
	bool LoadObjectGroup(pugi::xml_node& node, ObjectGroup objectgroup, int module_number);
	bool LoadProperties(pugi::xml_node& node, Properties& properties);

public:

	MapData data;

private:

	pugi::xml_document	map_file;
	std::string			folder;
	bool				map_loaded;
	uchar*				walkability_map;
};

#endif // __j1MAP_H__