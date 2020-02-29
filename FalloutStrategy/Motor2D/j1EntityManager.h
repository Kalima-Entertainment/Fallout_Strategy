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
//	void RellocateEntities();

public:

	std::vector<j1Entity*> entities;
	pugi::xml_node config_data;

public:

	//reference characters

	bool blocked_movement = false;

};

#endif // !_ENTITY_MANAGER_H_

