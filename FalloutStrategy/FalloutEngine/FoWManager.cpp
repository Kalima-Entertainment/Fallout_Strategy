#include "FoWManager.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Console.h"
#include "p2Log.h"
#include "j1Minimap.h"
#include "brofiler/Brofiler/Brofiler.h"
#include "j1EntityManager.h"
#include "j1Player.h"

FoWManager::FoWManager()
{

}


FoWManager::~FoWManager()
{
	CleanUp();
}

bool FoWManager::Awake(pugi::xml_node&)
{
	bool ret = true;
	return ret;
}

bool FoWManager::Start()
{
	bool ret = true;

	smoothFoWtexture = App->tex->Load("Assets/maps/fogTiles.png");
	debugFoWtexture = App->tex->Load("Assets/maps/fogTilesDebug.png");
	update_timer.Start();

	if (smoothFoWtexture == nullptr || debugFoWtexture == nullptr)
	ret = false;

	//---------Initialize the map being used to translate bits to texture ID---------//
	//Straight-forward cases
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_ALL, 0));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_NNN, 1));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_WWW, 2));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_EEE, 3));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_SSS, 4));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_CNW, 5));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_CSE, 6));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_CNE, 7));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_CSW, 8));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_JNE, 9));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_JSW, 10));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_JNW, 11));
	bitToTextureTable.insert(std::pair<unsigned short, int>(fow_JSE, 12));

	//more complicated cases (combinations)
	//diagonals
	bitToTextureTable.insert(std::pair<unsigned short, int>(20, 9));
	bitToTextureTable.insert(std::pair<unsigned short, int>(80, 10));
	bitToTextureTable.insert(std::pair<unsigned short, int>(17, 11));
	bitToTextureTable.insert(std::pair<unsigned short, int>(272, 12));
	bitToTextureTable.insert(std::pair<unsigned short, int>(273, 13));
	bitToTextureTable.insert(std::pair<unsigned short, int>(84, 14));
	//lines
	bitToTextureTable.insert(std::pair<unsigned short, int>(23, 1));
	bitToTextureTable.insert(std::pair<unsigned short, int>(308, 3));
	bitToTextureTable.insert(std::pair<unsigned short, int>(89, 2));
	bitToTextureTable.insert(std::pair<unsigned short, int>(464, 4));
	//joints
	bitToTextureTable.insert(std::pair<unsigned short, int>(6, 9));
	bitToTextureTable.insert(std::pair<unsigned short, int>(36, 9));
	bitToTextureTable.insert(std::pair<unsigned short, int>(72, 10));
	bitToTextureTable.insert(std::pair<unsigned short, int>(192, 10));
	bitToTextureTable.insert(std::pair<unsigned short, int>(3, 11));
	bitToTextureTable.insert(std::pair<unsigned short, int>(9, 11));
	bitToTextureTable.insert(std::pair<unsigned short, int>(384, 12));
	bitToTextureTable.insert(std::pair<unsigned short, int>(288, 12));
	//corners
	bitToTextureTable.insert(std::pair<unsigned short, int>(4, 9));
	bitToTextureTable.insert(std::pair<unsigned short, int>(64, 10));
	bitToTextureTable.insert(std::pair<unsigned short, int>(1, 11));
	bitToTextureTable.insert(std::pair<unsigned short, int>(256, 12));
	//------------------------end of map initialization------------------------//

	App->console->CreateCommand("fog_of_war", "(De)Activates fog of war", this);

	return ret;
}

bool FoWManager::PreUpdate()
{
	bool ret = true;

	//deletes all the entities that request to do so
	for(size_t i = 0; i < fowEntities.size(); i++)
	{
		if (fowEntities[i]->deleteEntity)
		{
			delete fowEntities[i];
			fowEntities[i] = nullptr;
			fowEntities.erase(fowEntities.begin() + i);
			i--;
		}
	}

	//debug input handling
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		debugMode = !debugMode;
		MapNeedsUpdate();
	}
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		ResetFoWMap();
		MapNeedsUpdate();
	}
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		App->render->fog_of_war = !App->render->fog_of_war;
	}

	return ret;
}

bool FoWManager::Update(float dt) {
	BROFILER_CATEGORY("Fog of War Update", Profiler::Color::Gray)
	bool ret = true;

	//We update the fowMap only when its needed
	if (foWMapNeedsRefresh)
	{
		UpdateFoWMap();
		foWMapNeedsRefresh = false;
	}
	return ret;
}

bool FoWManager::PostUpdate() {
BROFILER_CATEGORY("Fog of War PostUpdate", Profiler::Color::Black)
	bool ret = true;
	if(App->render->fog_of_war)
		DrawFoWMap();
	return ret;
}

bool FoWManager::CleanUp()
{
	bool ret = true;
	DeleteFoWMap();

	int i = 0;
	while (fowEntities.size() > 0)
	{


		if (fowEntities[i] != nullptr)
		{
			delete fowEntities[i];
			fowEntities[i] = nullptr;
			fowEntities.erase(fowEntities.begin() + i);
			i--;
		}
		i++;
	}
	fowEntities.clear();


	if (debugFoWtexture != nullptr)
	{
		App->tex->UnLoad(debugFoWtexture);
		debugFoWtexture = nullptr;
	}
	if (smoothFoWtexture != nullptr)
	{
		App->tex->UnLoad(smoothFoWtexture);
		smoothFoWtexture = nullptr;
	}

	return ret;
}

void FoWManager::ResetFoWMap()
{
	if (fowMap != nullptr)
	{
		memset(fowMap, NULL, width * height);

		for(int i = 0; i < width * height; i++)
		{
			fowMap[i].tileShroudBits = fow_ALL;
			fowMap[i].tileFogBits = fow_ALL;
		}
	}
}

FoWDataStruct* FoWManager::GetFoWTileState(iPoint mapPos)const
{
	FoWDataStruct* ret = nullptr;
	if (CheckFoWTileBoundaries(mapPos) && fowMap != nullptr)
	{
		ret = &fowMap[(mapPos.y * width) + mapPos.x];
	}

	return ret;
}


bool FoWManager::CheckFoWTileBoundaries(iPoint mapPos)const
{
	bool ret = false;
	if (mapPos.x >= 0 && mapPos.x < width && mapPos.y >= 0 && mapPos.y < height)
		ret = true;

	return ret;
}

void FoWManager::CreateFoWMap(uint w, uint h)
{
	width = w;
	height = h;

	// Complete this function to create a FoWMap. EASY!
	//If a map has already been created you will need to delete it first, hint: there's a function for that :) 
	//Note that the map will be a 1 dimensional array and you might need the 2 variables above to set it up. The map should be stored in the variable "fowMap"
	//Don't forget to reset it once is creeated, hint: there's another function for that :)
	DeleteFoWMap();
	fowMap = new FoWDataStruct[width * height];
	ResetFoWMap();

	MapNeedsUpdate();
}


void FoWManager::DeleteFoWMap()
{
	if (fowMap != nullptr)
	{
		RELEASE_ARRAY(fowMap);
		fowMap = nullptr;
	}
}


void FoWManager::UpdateFoWMap()
{
	if (fowMap != nullptr)
	{
		for(int i = 0; i < width * height; i++)
		{
			fowMap[i].tileFogBits = fow_ALL;
		}

		for(int i = 0; i < fowEntities.size(); i++)
		{
			fowEntities[i]->Update();
		}

		if (!debugMode)
		{
			for(int i = 0; i < fowEntities.size(); i++)
			{
				if (CheckTileVisibility(fowEntities[i]->GetPos()))
				{
					fowEntities[i]->isVisible = true;
				}
				else fowEntities[i]->isVisible = false;
			}
		}
		else
		{
			for(int i = 0; i < fowEntities.size(); i++)
			{
				fowEntities[i]->isVisible = true;
			}
		}
	}
}

void FoWManager::DrawFoWMap()
{
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			FoWDataStruct* tileInfo = GetFoWTileState({ x, y });
			int fogId = -1;
			int shroudId = -1;

			if (tileInfo != nullptr)
			{
				iPoint worldDrawPos;
				worldDrawPos = App->map->MapToWorld(worldDrawPos.x, worldDrawPos.y);
				worldDrawPos = App->map->MapToWorld(x, y);

				//Offset for fow camera culling
				iPoint offset;
				offset = App->map->MapToWorld(4, 2);

				//Camera Culling -> Draw only what is on screen
				if (((worldDrawPos.x + offset.x > -(App->render->camera.x))
					&& (worldDrawPos.x < -App->render->camera.x + App->render->camera.w)
					&& (worldDrawPos.y + offset.y > -(App->render->camera.y))
					&& (worldDrawPos.y < (-App->render->camera.y + App->render->camera.h))) || (update_timer.Read() > 2500))
				{

					if (bitToTextureTable.find(tileInfo->tileFogBits) != bitToTextureTable.end())
					{
						fogId = bitToTextureTable[tileInfo->tileFogBits];
					}

					if (bitToTextureTable.find(tileInfo->tileShroudBits) != bitToTextureTable.end())
					{
						shroudId = bitToTextureTable[tileInfo->tileShroudBits];
					}

					SDL_Texture* displayFogTexture = nullptr;
					if (debugMode)
					{
						displayFogTexture = debugFoWtexture;
					}
					else displayFogTexture = smoothFoWtexture;

					App->minimap->grid[x][y] = tileInfo->tileShroudBits;

					//draw fog
					if (fogId != -1)
					{
						SDL_SetTextureAlphaMod(displayFogTexture, 128);//set the alpha of the texture to half to reproduce fog
						SDL_Rect r = { fogId * 64,0,64,64 }; //this rect crops the desired fog Id texture from the fogTiles spritesheet
						App->render->Blit(displayFogTexture, worldDrawPos.x, worldDrawPos.y, &r);
					}
					if (shroudId != -1)
					{
						SDL_SetTextureAlphaMod(displayFogTexture, 255);//set the alpha to white again
						SDL_Rect r = { shroudId * 64,0,64,64 }; //this rect crops the desired fog Id texture from the fogTiles spritesheet
						App->render->Blit(displayFogTexture, worldDrawPos.x, worldDrawPos.y, &r);
					}
				}	
			}
		}
	}
	if (update_timer.Read() > 2500)
		update_timer.Start();
}

//Given a position and a flag, create a new entity and return a pointer to it (or nullptr if something has gone wrong)
//Note that the FoWManager needs to know about the entity we are creating, try to find where the FoWManager module stores all the FoWEntities and add it there
FoWEntity* FoWManager::CreateFoWEntity(iPoint pos, bool providesVisibility)
{
	FoWEntity* entity = nullptr;

	pos = App->map->MapToWorld(pos.x, pos.y);

	entity = new FoWEntity(pos, providesVisibility);

	if (entity != nullptr) {
		fowEntities.push_back(entity);
	}

	return entity;
}


// Complete the following function: it shoud return the tile visibility (true if visible, otherwise false)
//This function will be used to check if we need to draw a certain entity
bool FoWManager::CheckTileVisibility(iPoint mapPos)const
{
	bool ret = false;
	//First check if the entity is inside the map
	//& get the tile fog information,its state, to check if is visible. 
	//Note that the function that you need does both things for you, it is recommended to check and understand what the needed function does

	FoWDataStruct* tileState = GetFoWTileState(mapPos);

	if (tileState != nullptr)
	{
		//Entity will only be visible in visible areas (no fog nor shroud)
		//Think about what happens with the smooth borders, are the considered visble or fogged?
		//Also, do you need to check both the fog and shroud states?
		if (tileState->tileFogBits != fow_ALL)
			ret = true;
	}

	return ret;
}

void FoWManager::AddFowToResourceBuildings(iPoint basePos)
{
	//App->entities->GetClosestResourceBuilding(App->player->base);
	ResourceBuilding* resource_building[2];

	//We get the two buildings
	resource_building[0] = App->entities->GetClosestResourceBuilding(basePos, Resource::WATER);
	resource_building[1] = App->entities->GetClosestResourceBuilding(basePos, Resource::CAPS);

	//We get resource_building sizes
	size_t building_size[2];
	building_size[0] = resource_building[0]->tiles.size();
	building_size[1] = resource_building[1]->tiles.size();

	//We add the vision entity to them
	//Water Resource Building
	{
		visionEntity = App->fowManager->CreateFoWEntity({ resource_building[0]->tiles[(int)(building_size[0] * 0.9)] }, true);
		visionEntity->SetNewVisionRadius(7);
	}

	//Caps Resource Building
	{
		visionEntity = App->fowManager->CreateFoWEntity({ resource_building[1]->tiles[(int)(building_size[1] * 0.9)] }, true);
		visionEntity->SetNewVisionRadius(7);
	}
}

void FoWManager::MapNeedsUpdate()
{
	if (foWMapNeedsRefresh == false)
		foWMapNeedsRefresh = true;
}

void FoWManager::OnCommand(std::vector<std::string> command_parts)
{
	std::string command_beginning = command_parts[0];

	//Increase all resources
	if (command_beginning == "fog_of_war") {
		App->render->fog_of_war = !App->render->fog_of_war;
		if (App->render->fog_of_war)	LOG("Fog Of War Activated");
		else LOG("Fog Of War Deactivated");
	}
}