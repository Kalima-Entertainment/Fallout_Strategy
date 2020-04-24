#ifndef _j1ENTITY_H
#define _j1ENTITY_H
#include <iostream>
#include <vector>
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "j1EntityManager.h"
#include "p2DynArray.h"
#include "p2Log.h"
#include "j1MovementManager.h"
#include "j1Group.h"
#include "j1Timer.h"

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

enum Faction { //Don't change faction order. Just don't do it
	VAULT,	
	BROTHERHOOD,
	MUTANT,
	GHOUL,
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
	virtual bool LoadFx() { return true; };
	virtual bool LoadReferenceData() { return true; };	

	int GetPositionScore() const;

	iPoint MapPosition();

	void SpatialAudio(int fx, int channel, int positionx, int positiony);

	//Check if mouse pointer its inside the same spot than the entity and if pressed returns any advice

public:
	iPoint current_tile;
	fPoint position;
	iPoint render_position;
	iPoint target_tile;
	std::vector<iPoint> path_to_target;
	fPoint speed;
	iPoint spawnPosition;
	int sprite_size;

	int current_health;
	int max_health;
	int damage;
	int storage_capacity;
	int max_capacity;
	int volume;

	EntityType type;
	Faction faction;
	j1Entity* reference_entity;
	j1Entity* target_entity;
	DynamicEntity* attacking_entity;
	GenericPlayer* owner;

	Animation* current_animation;
	SDL_Texture* texture;
	float last_dt;

	bool is_dynamic;
	bool to_delete;
	bool particles_created;
	bool playing_fx;

	Group_Unit info;
	j1Timer delete_timer;
};

#endif // !_j1ENTITY_H
