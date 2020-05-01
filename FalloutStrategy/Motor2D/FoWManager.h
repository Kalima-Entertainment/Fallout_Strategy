#ifndef __FOW_MANAGER_H__
#define __FOW_MANAGER_H__
#include "SDL_image/include/SDL_image.h"
#include <vector>
#include <list>
#include <map>
#include "p2Point.h"
#include "FoWBitDefs.h"
#include "j1Module.h"
#include "FoWEntity.h"


//this struct holds the information of fog and shroud for every tile
struct FoWDataStruct
{
	unsigned short tileFogBits; //saves information about which type of fog are we in (useful for smooth edges)
	unsigned short tileShroudBits; //same as above but for shroud
};


class FoWManager :public j1Module
{
	//FUNCTIONS
public:
	FoWManager();
	~FoWManager();


	bool Awake(pugi::xml_node&);
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

	FoWEntity* CreateFoWEntity(iPoint pos, bool providesVisibility);

	//Resets the map to its shrouded state
	void ResetFoWMap();
	void CreateFoWMap(uint width, uint height);
	void DeleteFoWMap();
	//Updates the data on the FoWMap based on the FoWEntities position and mask shape
	void UpdateFoWMap();
	void DrawFoWMap();
	//Tell the map that it needs to be updated the next frame
	void MapNeedsUpdate();

	//Returns the visibility state of the chosen tile (given its map coordinates)
	FoWDataStruct* GetFoWTileState(iPoint mapPos)const;
	//Returns true if the tile is inside the map boundaries, otherwise returns false
	bool CheckFoWTileBoundaries(iPoint mapPos)const;
	//Returns true if the tile is visible (there's no FOG in it) otherwise returns false
	bool CheckTileVisibility(iPoint mapPos)const;

	//VARIABLES
public:
	//A number of precomputed circle masks for you to use ranging between a radius of 2 to a radius of 5
	unsigned short circleMasks[4][fow_MAX_CIRCLE_LENGTH * fow_MAX_CIRCLE_LENGTH] =
	{
		{//R2
		fow_ALL, fow_CNW, fow_NNN, fow_CNE, fow_ALL,
		fow_CNW, fow_JNW, fow_NON, fow_JNE, fow_CNE,
		fow_WWW, fow_NON, fow_NON, fow_NON, fow_EEE,
		fow_CSW, fow_JSW, fow_NON, fow_JSE, fow_CSE,
		fow_ALL, fow_CSW, fow_SSS, fow_CSE, fow_ALL,
		},
		{//R3
		fow_ALL, fow_ALL, fow_CNW, fow_NNN, fow_CNE, fow_ALL, fow_ALL,
		fow_ALL, fow_CNW, fow_JNW, fow_NON, fow_JNE, fow_CNE, fow_ALL,
		fow_CNW, fow_JNW, fow_NON, fow_NON, fow_NON, fow_JNE, fow_CNE,
		fow_WWW, fow_NON, fow_NON, fow_NON, fow_NON, fow_NON, fow_EEE,
		fow_CSW, fow_JSW, fow_NON, fow_NON, fow_NON, fow_JSE, fow_CSE,
		fow_ALL, fow_CSW, fow_JSW, fow_NON, fow_JSE, fow_CSE, fow_ALL,
		fow_ALL, fow_ALL, fow_CSW, fow_SSS, fow_CSE, fow_ALL, fow_ALL,
		},
		{//R4
		fow_ALL, fow_ALL, fow_CNW, fow_NNN, fow_NNN, fow_NNN, fow_CNE, fow_ALL, fow_ALL,
		fow_ALL, fow_CNW, fow_JNW, fow_NON, fow_NON, fow_NON, fow_JNE, fow_CNE, fow_ALL,
		fow_CNW, fow_JNW, fow_NON, fow_NON, fow_NON, fow_NON, fow_NON, fow_JNE, fow_CNE,
		fow_WWW, fow_NON, fow_NON, fow_NON, fow_NON, fow_NON, fow_NON, fow_NON, fow_EEE,
		fow_WWW, fow_NON, fow_NON, fow_NON, fow_NON, fow_NON, fow_NON, fow_NON, fow_EEE,
		fow_WWW, fow_NON, fow_NON, fow_NON, fow_NON, fow_NON, fow_NON, fow_NON, fow_EEE,
		fow_CSW, fow_JSW, fow_NON, fow_NON, fow_NON, fow_NON, fow_NON, fow_JSE, fow_CSE,
		fow_ALL, fow_CSW, fow_JSW, fow_NON, fow_NON, fow_NON, fow_JSE, fow_CSE, fow_ALL,
		fow_ALL, fow_ALL, fow_CSW, fow_SSS, fow_SSS, fow_SSS, fow_CSE, fow_ALL, fow_ALL,
		},
		{//R5
		fow_ALL, fow_ALL, fow_ALL, fow_ALL, fow_CNW, fow_NNN, fow_CNE, fow_ALL, fow_ALL, fow_ALL, fow_ALL,
		fow_ALL, fow_ALL, fow_CNW, fow_NNN, fow_JNW, fow_NON, fow_JNE, fow_NNN, fow_CNE, fow_ALL, fow_ALL,
		fow_ALL, fow_CNW, fow_JNW, fow_NON, fow_NON, fow_NON, fow_NON, fow_NON, fow_JNE, fow_CNE, fow_ALL,
		fow_ALL, fow_WWW, fow_NON, fow_NON, fow_NON, fow_NON, fow_NON, fow_NON, fow_NON, fow_EEE, fow_ALL,
		fow_CNW, fow_JNW, fow_NON, fow_NON, fow_NON, fow_NON, fow_NON, fow_NON, fow_NON, fow_JNE, fow_CNE,
		fow_WWW, fow_NON, fow_NON, fow_NON, fow_NON, fow_NON, fow_NON, fow_NON, fow_NON, fow_NON, fow_EEE,
		fow_CSW, fow_JSW, fow_NON, fow_NON, fow_NON, fow_NON, fow_NON, fow_NON, fow_NON, fow_JSE, fow_CSE,
		fow_ALL, fow_WWW, fow_NON, fow_NON, fow_NON, fow_NON, fow_NON, fow_NON, fow_NON, fow_EEE, fow_ALL,
		fow_ALL, fow_CSW, fow_JSW, fow_NON, fow_NON, fow_NON, fow_NON, fow_NON, fow_JSE, fow_CSE, fow_ALL,
		fow_ALL, fow_ALL, fow_CSW, fow_SSS, fow_JSW, fow_NON, fow_JSE, fow_SSS, fow_CSE, fow_ALL, fow_ALL,
		fow_ALL, fow_ALL, fow_ALL, fow_ALL, fow_CSW, fow_SSS, fow_CSE, fow_ALL, fow_ALL, fow_ALL, fow_ALL,
		},
	};


private:
	//This is where the FoWEntites are stored
	std::vector<FoWEntity*> fowEntities;
	//This is where we store our FoW information
	FoWDataStruct* fowMap = nullptr;

	SDL_Texture* smoothFoWtexture = nullptr;
	SDL_Texture* debugFoWtexture = nullptr;

	//Map that we use to translate bits to Texture Id's
	std::map<unsigned short, uint> bitToTextureTable;

	uint width;
	uint height;
	bool debugMode = false;
	bool foWMapNeedsRefresh = false;
};

#endif // !__FOW_MANAGER_H__