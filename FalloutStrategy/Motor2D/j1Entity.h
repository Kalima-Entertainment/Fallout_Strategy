#ifndef _j1ENTITY_H
#define _j1ENTITY_H
#include <iostream>
#include <vector>
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "j1EntityManager.h"
#include "p2DynArray.h"
#include "p2Log.h"

class DynamicEntity;

#define TILE_SIZE 64
#define HALF_TILE 32

enum EntityType {
	MELEE,
	RANGED,
	GATHERER,
	BASE,
	LABORATORY,
	BARRACK,
	NO_TYPE
};

enum Faction {
	VAULT,
	GHOUL,
	BROTHERHOOD,
	MUTANT,
	NO_FACTION
};

class j1Entity 
{
public:
	j1Entity();
	virtual ~j1Entity();

	virtual bool Awake(pugi::xml_node&) { return true; }
	virtual bool Start() { return true; }
	virtual bool PreUpdate() { return true; }
	virtual bool Update(float dt) { return true; }
	virtual bool PostUpdate() { return true; };
	virtual bool CleanUp() { return true; }
	virtual void OnCollision(Collider* c1, Collider* c2) {};

	virtual bool LoadAnimations() { return true; };
	virtual bool LoadReferenceData() { return true; };

	int GetPositionScore() const;

	iPoint MapPosition();

	//Check if mouse pointer its inside the same spot than the entity and if pressed returns any advice

public:
	iPoint current_tile;
	fPoint position;
	iPoint target_tile;
	p2DynArray<iPoint>* path_to_target = nullptr;
	fPoint speed;
	
	int current_health;
	int max_health;
	int damage;
	int storage_capacity = 100;

	EntityType type;
	Faction faction;
	j1Entity* reference_entity;
	DynamicEntity* attacking_entity;

	Collider* collider;
	Collider* last_collider;
	
	Animation* current_animation;
	SDL_Texture* texture;

	bool is_dynamic;
	bool to_destroy;
	bool particles_created;
	bool playing_fx;
};

#endif // !_j1ENTITY_H

