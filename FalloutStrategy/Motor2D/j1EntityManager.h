#ifndef _ENTITY_MANAGER_H_
#define _ENTITY_MANAGER_H_

#include <vector>
#include "j1Module.h"
#include "p2Point.h"
#include "Animation.h"
#include "j1Timer.h"
#include "SDL_image/include/SDL_image.h"

class j1Entity;
struct SDL_Texture;
enum Faction;
enum class BuildingType;
class DynamicEntity;
class StaticEntity;
enum EntityType;
class GenericPlayer;

#define REFERENCE_ENTITIES 24

enum class Resource {
	CAPS,
	WATER,
	FOOD
};

struct ResourceBuilding {
	Resource resource_type;
	int quantity;
	std::vector<iPoint> tiles;
};

struct Unit_Data {
	int cost_water;
	int cost_meat;
	int spawn_seconds;

};

class j1EntityManager : public j1Module
{
public:

	j1EntityManager();
	~j1EntityManager();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	void OnCommand(std::vector<std::string> command_parts);

	// -----------------------------------------------------------------------------------------------------------------------------

	j1Entity*		CreateEntity(Faction faction, EntityType type, int position_x, int position_y, GenericPlayer* owner = nullptr);
	virtual void	SpawnUnit(int buildingID, EntityType type);

	//Find entities
	j1Entity*		FindEntityByTile(iPoint tile);
	j1Entity*		FindEntityByType(Faction faction, EntityType type);
	ResourceBuilding* FindResourceBuildingByTile(iPoint tile);
	ResourceBuilding* GetClosestResourceBuilding(iPoint current_position);
	iPoint ClosestTile(iPoint position, std::vector<iPoint> entity_tiles) const;

	bool LoadReferenceEntityData();
	void DestroyEntity(j1Entity* delete_entity);
	void DestroyAllEntities();

	void BubbleSortEntities();
	//void QuickSortEntities(std::vector<j1Entity*> entities, int low, int high);
	//int Partition(std::vector<j1Entity*> entities, int low, int high);

	void RandomFactions();
	Faction FactionByIndex(int i) { return static_cast<Faction>(i); }

public:
	std::vector<j1Entity*> entities;
	std::vector<ResourceBuilding*> resource_buildings;
	pugi::xml_node config_data;

	int count = 0;
	bool blocked_movement;

	int randomFaction[4];
	Unit_Data unit_data[4][3];
	j1Entity* reference_entities[4][6];
	j1Timer sort_timer;
	SDL_Texture* selected_unit_tex;

};

#endif // !_ENTITY_MANAGER_H_

