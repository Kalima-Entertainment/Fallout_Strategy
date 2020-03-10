#ifndef _j1ENTITY_H
#define _j1ENTITY_H
#include <iostream>
#include <vector>
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "j1EntityManager.h"
#include "p2DynArray.h"
#include "p2Log.h"

//struct SDL_Texture;
//struct Collider;

struct Resource {
	uint money, water, food;
};

enum State {
	IDLE,
	WALK,
	ATTACK,
	GATHER,
	HIT,
	DIE,
	MAX_ANIMATIONS
};

enum Faction {
	VAULT,
	GHOUL,
	BROTHERHOOD,
	MUTANT
};

enum Direction {
	TOP_LEFT,
	TOP_RIGHT,
	RIGHT,
	BOTTOM_RIGHT,
	BOTTOM_LEFT,
	LEFT,
	NONE
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

	bool LoadAnimations(const char* animation_file);
	virtual bool LoadReferenceData() { return true; };

	iPoint MapPosition();

	//Check if mouse pointer its inside the same spot than the entity and if pressed returns any advice

public:
	fPoint position;
	fPoint lastPosition;
	fPoint initialPosition;
	iPoint current_tile;
	iPoint target_tile;
	p2DynArray<iPoint>* path_to_target = nullptr;

	int health;

	j1Entity* reference_entity;

	Collider* collider;
	Collider* last_collider;
	
	Animation animations[MAX_ANIMATIONS][6];
	Animation idle[6];
	Animation* current_animation;
	Direction direction;

	State state;
	Faction faction;

	SDL_Texture* texture;
	SDL_RendererFlip flip;

	bool to_destroy;
	bool particles_created;
	bool playing_fx;
};
#endif // !_j1ENTITY_H

