#include "j1App.h"
#include "FoWManager.h"
#include "FoWEntity.h"
#include "FoWBitDefs.h"
#include "j1Map.h"

FoWEntity::FoWEntity(iPoint WorldPos, bool providesVisibility) :boundingBoxRadius(3), deleteEntity(false), providesVisibility(providesVisibility), posInMap({ 0,0 }), isVisible(false)
{
	SetNewPosition(WorldPos);
}


FoWEntity::~FoWEntity()
{
	CleanUp();
}


bool FoWEntity::CleanUp()
{
	bool ret = true;

	return ret;
}


void FoWEntity::SetNewPosition(iPoint pos)
{
	posInMap = App->map->WorldToMap(pos.x, pos.y);
	App->fowManager->MapNeedsUpdate();
}


void FoWEntity::SetNewVisionRadius(uint radius)
{
	//Changes the vision radius of the entity if there's a precomputed shape with that radius
	if (radius >= fow_MIN_CIRCLE_RADIUS && radius <= fow_MAX_CIRCLE_RADIUS)
	{
		boundingBoxRadius = radius;
		App->fowManager->MapNeedsUpdate();
	}
}

std::vector<iPoint> FoWEntity::GetTilesInsideRadius()const
{
	std::vector<iPoint> ret;
	int length = (boundingBoxRadius * 2) + 1;
	iPoint startingPos;
	startingPos.x = posInMap.x - boundingBoxRadius;
	startingPos.y = posInMap.y - boundingBoxRadius;
	iPoint finishingPos;
	finishingPos.x = startingPos.x + length;
	finishingPos.y = startingPos.y + length;

	//Creates a vector with all the tiles inside a bounding box delimited by the radius
	for (int i = startingPos.y; i < finishingPos.y; i++)
	{
		for (int j = startingPos.x; j < finishingPos.x; j++)
		{
			ret.push_back({ j,i });
		}
	}

	return ret;
}

//TODO 3: Comprehend and complete this function: (this is the function that does the magic for us)
void FoWEntity::ApplyMaskToTiles(std::vector<iPoint>tilesAffected)
{

	//We first take the correct precomputed mask and store it in the precMask variable (it is recommended to see what they are made of. You can find the masks at the FoWManager.h module)
	//Note that it is an array
	unsigned short* precMask = &App->fowManager->circleMasks[boundingBoxRadius - fow_MIN_CIRCLE_RADIUS][0];

	//You have to complete the code inside this for
	for (int i = 0; i < tilesAffected.size(); i++)
	{
		//You have to request the fog & shroud values of each affected tile. Hint:(You can take both with a single function call)
		FoWDataStruct* tileValue = App->fowManager->GetFoWTileState(tilesAffected[i]);

		//And (bitwise AND) them with the mask if the tile FoW values are not nullptr
		//To bitwise AND values you just simply do this: value1 &= value2 
		//the operation result will be stored in the variable on the left side. 
		//In this case you want to modify the fog and shroud values that you have requested above

		if (tileValue != nullptr) {
			tileValue->tileShroudBits &= *precMask;
			tileValue->tileFogBits &= *precMask;
		}
		precMask++;
	}
}

//Applies the Mask to all tiles inside the radius
void FoWEntity::Update()
{
	if (providesVisibility)
		ApplyMaskToTiles(GetTilesInsideRadius());
}


iPoint FoWEntity::GetPos()const
{
	return posInMap;
}
