#ifndef _ENTITY_MANAGER_H_
#define _ENTITY_MANAGER_H_

#include <vector>
#include "j1Module.h"
#include "p2Point.h"
#include "Animation.h"
#include "j1Timer.h"
#include "SDL_image/include/SDL_image.h"
#include <string>

class j1Entity;
struct SDL_Texture;
enum Faction;
enum class BuildingType;
class DynamicEntity;
class StaticEntity;
enum EntityType;
class GenericPlayer;

class ParticleSystem;
class Emiter;

#define REFERENCE_ENTITIES 28

enum class Resource {
	CAPS,
	WATER,
	FOOD,
	NO_TYPE
};

struct ResourceBuilding {
	Resource resource_type = Resource::NO_TYPE;
	int quantity = 0;
	std::vector<iPoint> tiles;
	iPoint no_resources_blit_position;
	~ResourceBuilding() {tiles.clear();}
};

struct Unit_Data {
	int cost_water;
	int cost_meat;
	int spawn_seconds;
};

enum Upgrades {
	RESOURCES_LIMIT,
	GATHERER_CAPACITY,
	UNITS_DAMAGE,
	UNITS_SPEED,
	UNITS_HEALTH,
	CREATION_TIME,
	NO_UPGRADE
};

struct Upgrades_Data{
	Faction faction;
	Upgrades upgrade;
	int upgrade_num;
	int first_price;
	int price_increment;
	int seconds;
	float value_increment;
};

class j1EntityManager : public j1Module
{
public:

	j1EntityManager();
	~j1EntityManager();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	void OnCommand(std::vector<std::string> command_parts);

	// -----------------------------------------------------------------------------------------------------------------------------

	j1Entity* CreateEntity(Faction faction, EntityType type, int position_x, int position_y, GenericPlayer* owner = nullptr);
	void DestroyEntity(j1Entity* delete_entity);
	void DestroyDynamicEntities();
	void DestroyAllEntities();
	void DestroyAllEntitiesNow();
	iPoint FindSpawnPoint(int position_x, int position_y);
	void SpawnAnimals();
	ResourceBuilding* CreateResourceSpot(int position_x, int position_y, Resource resource_type, int resource_quantity);
	void DestroyResourceSpot(ResourceBuilding* resource_spot);

	//Find entities
	j1Entity*		  FindEntityByTile(iPoint tile, j1Entity* entity_to_skip = nullptr);
	ResourceBuilding* FindResourceBuildingByTile(iPoint tile);
	ResourceBuilding* GetClosestResourceBuilding(iPoint current_position, Resource resource_type);
	bool IsTileOccupied(iPoint position);
	bool IsTileInPositionOccupied(fPoint position);

	iPoint ClosestTile(iPoint position, std::vector<iPoint> entity_tiles);
	iPoint FindClosestFreeTile(iPoint origin, iPoint destination);
	iPoint FindClosestFreeTileFromVector(iPoint origin, std::vector<iPoint> tiles);
	iPoint FindFreeAdjacentTile(iPoint origin, iPoint destination);

	bool LoadReferenceEntityAnimations();
	bool LoadReferenceEntityData();

	void BubbleSortEntities();

	void RandomFactions();
	Faction FactionByIndex(int i) { return static_cast<Faction>(i); }

	void LoadUpgradeCosts(pugi::xml_node& config);
	void LoadUnitCosts();
	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	int GetReferenceEntityID(Faction faction, EntityType type);

	ParticleSystem* CreateParticle(fPoint);
	void DeleteParticles();
	void ReleaseParticle(ParticleSystem*);
	void RestartOccupiedTiles();

public:
	std::vector<j1Entity*> entities;
	std::vector<j1Entity*> particles;
	std::vector<ResourceBuilding*> resource_buildings;
	pugi::xml_node config_data;

	int count = 0;
	bool showing_building_menu;
	bool blocked_movement;

	int randomFaction[4];
	Unit_Data unit_data[4][3];
	j1Entity* reference_entities[REFERENCE_ENTITIES];
	bool occupied_tiles[150][150];
	j1Timer sort_timer;

	bool loading_reference_entities;

	Upgrades_Data base_resource_limit[4];
	Upgrades_Data gatherer_resource_limit[4];
	Upgrades_Data units_damage[4];
	Upgrades_Data units_speed[4];
	Upgrades_Data units_health[4];
	Upgrades_Data units_creation_time[4];
	int radar_cost;
	int mr_handy_cost;
	int mr_handy_time;

	// -- Particle textures
	SDL_Texture* blood;
	SDL_Texture* smoke;
	SDL_Texture* hit;

	//HealthBar
	SDL_Texture* life_bars;
	SDL_Rect background_health_bar;
	SDL_Rect foreground_health_bar;
	SDL_Rect gathering_health_bar;
	SDL_Rect frame_quad;
	//No Resourecs
	SDL_Rect no_reso;

	SDL_Texture* debug_tex;

private:
	int loading_faction;
	int loading_entity;
	int entities_loaded;
	std::string texture_folders[REFERENCE_ENTITIES];
	std::string tmx_files[REFERENCE_ENTITIES];
	j1Timer load_timer;
};

#endif // !_ENTITY_MANAGER_H_
