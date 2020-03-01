#ifndef _ENTITY_MANAGER_H_
#define _ENTITY_MANAGER_H_

#include <vector>
#include "j1Module.h"
#include "p2Point.h"
#include "Animation.h"
#include "SDL_image/include/SDL_image.h"

class j1Entity;
struct SDL_Texture;
enum Faction;
enum class Troop;
enum class BuildingType;

#define REFERENCE_ENTITIES 12

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

	//bool Load(pugi::xml_node& data);
	//bool Save(pugi::xml_node& data) const;
	//bool CheckpointSave();
	//bool CheckpointLoad();

	j1Entity* CreateDynamicEntity(Faction faction, Troop troop_type, int position_x, int position_y);
	j1Entity* CreateStaticEntity(Faction faction, BuildingType building_type , int position_x, int position_y);
	void DestroyEntity(j1Entity* delete_entity);
	void DestroyAllEntities();
    //void RellocateEntities();

public:

	std::vector<j1Entity*> entities;
	pugi::xml_node config_data;

public:

	//reference characters
	j1Entity* reference_vault_melee;
	j1Entity* reference_vault_range;
	j1Entity* reference_vault_gatherer;
	j1Entity* reference_vault_base;

	j1Entity* reference_brotherhood_melee;
	j1Entity* reference_brotherhood_range;
	j1Entity* reference_brotherhood_gatherer;
	j1Entity* reference_brotherhood_base;

	j1Entity* reference_mutant_melee;
	j1Entity* reference_mutant_range;
	j1Entity* reference_mutant_gatherer;
	j1Entity* reference_mutant_base;

	j1Entity* reference_ghoul_melee;
	j1Entity* reference_ghoul_range;
	j1Entity* reference_ghoul_gatherer;
	j1Entity* reference_ghoul_base;

	bool blocked_movement = false;

};

#endif // !_ENTITY_MANAGER_H_

