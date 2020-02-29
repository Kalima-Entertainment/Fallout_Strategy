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

enum State {
	IDLE,
	ATTACK,
	WALK,
	DIE
};

enum Faction {
	VAULT,
	GHOUL,
	BROTHERHOOD,
	MUTANT
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
	bool PostUpdate();
	virtual bool CleanUp() { return true; }
	virtual void OnCollision(Collider* c1, Collider* c2) {};
	//void PathfindtoPlayer(int detection_range, j1Entity* player);
	bool LoadAnimations(const char* animation_file);

public:
	iPoint position = { 0, 0 };
	iPoint lastPosition = { 0, 0 };
	iPoint current_speed = {0, 0};
	iPoint speed = { 0, 0 };
	iPoint initialPosition = { 0, 0 };

	int health = 0;

	j1Entity* reference_entity;

	Collider* collider = nullptr;
	Collider* last_collider = nullptr;
	Collider* attack_collider = nullptr;
	
	std::vector<Animation*> animations;
	Animation idle;
	Animation walk;
	Animation attack;
	Animation hit;
	Animation die;
	Animation* current_animation = nullptr;
	Animation* last_animation = nullptr;

	State state = State::IDLE;
	Faction faction;

	SDL_Texture* texture = nullptr;
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	bool to_destroy = false;
	bool particles_created = false;
	bool playing_fx = false;

	const p2DynArray<iPoint>* path_to_player = nullptr;

};
#endif // !_j1ENTITY_H

