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
	j1Entity() {}
	virtual ~j1Entity();

	virtual bool Awake(pugi::xml_node&) { return true; }
	virtual bool Start() { return true; }
	virtual bool PreUpdate() { return true; }
	virtual bool Update(float dt) { return true; }
	virtual bool PostUpdate();
	virtual bool CleanUp() { return true; }
	virtual void OnCollision(Collider* c1, Collider* c2) {};

	void PathfindToPosition(iPoint target);
	bool LoadAnimations(const char* animation_file);
	virtual bool LoadReferenceData() { return true; };

	iPoint MapPosition();

	//Check if mouse pointer its inside the same spot than the entity and if pressed returns any advice

public:
	iPoint position = { 0, 0 };
	iPoint lastPosition = { 0, 0 };
	iPoint current_speed = {0, 0};
	iPoint speed = { 0, 0 };
	iPoint initialPosition = { 0, 0 };
	iPoint current_tile = { 0,0 };
	iPoint target_tile = { 0,0 };

	int health = 0;

	j1Entity* reference_entity;

	Collider* collider = nullptr;
	Collider* last_collider = nullptr;
	Collider* attack_collider = nullptr;
	
	//std::vector<Animation*> animations;
	Animation animations[MAX_ANIMATIONS][6];
	Animation idle[6];
	Animation walk[6];
	Animation attack[6];
	Animation gather[6];
	Animation hit[6];
	Animation die[6];
	Animation* current_animation = nullptr;
	Animation* last_animation = nullptr;
	Direction direction;

	State state = State::IDLE;
	Faction faction;

	SDL_Texture* texture = nullptr;
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	bool to_destroy = false;
	bool particles_created = false;
	bool playing_fx = false;

	const p2DynArray<iPoint>* path_to_target = nullptr;

};
#endif // !_j1ENTITY_H

