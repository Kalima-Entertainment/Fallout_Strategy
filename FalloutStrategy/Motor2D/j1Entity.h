#ifndef _j1ENTITY_H
#define _j1ENTITY_H
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "j1EntityManager.h"
#include "p2DynArray.h"
#include "p2Log.h"

struct SDL_Texture;
struct Collider;

#define COLLIDER_MARGIN 20

enum EntityState {
	IDLE,
	ATTACK,
	WALK,
	DIE
};

class j1Entity : public j1EntityManager
{
public:

	j1Entity(EntityType type);
	virtual ~j1Entity();

	virtual bool Awake(pugi::xml_node&) { return true; }
	virtual bool Start() { return true; }
	virtual bool PreUpdate() { return true; }
	virtual bool Update(float dt) { return true; }
	virtual bool PostUpdate() { return true; }
	virtual bool CleanUp() { return true; }
	virtual void OnCollision(Collider* c1, Collider* c2) {};
	void PathfindtoPlayer(int detection_range, j1Entity* player);
	bool LoadAnimations(const char* animation_file);

public:
	iPoint position = { 0, 0 };
	iPoint lastPosition = { 0, 0 };
	iPoint current_speed = {0, 0};
	iPoint speed = { 0, 0 };
	iPoint initialPosition = { 0, 0 };

	int health = 0;
	int damage = 0;
	int detection_range = 0;

	bool grounded = false;
	int	 gravity = 0;
	int	 max_falling_speed = 0;

	Collider* collider = nullptr;
	Collider* raycast = nullptr;
	Collider* last_collider = nullptr;
	Collider* attack_collider = nullptr;
	
	p2List<Animation*> animations;
	Animation idle;
	Animation walk;
	Animation slide;
	Animation crouch_down;
	Animation crouch_up;
	Animation jump;
	Animation run;
	Animation fall;
	Animation attack;
	Animation die;
	Animation rest;
	Animation* current_animation = nullptr;
	Animation* last_animation = nullptr;

	EntityType type = EntityType::UNKNOWN;
	EntityState state = EntityState::IDLE;
	SDL_Texture* texture = nullptr;
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	bool particles_created = false;

	bool going_after_player = false;

	uint		die_fx = 0;
	p2SString	die_fx_path;

	uint		attack_fx = 2;
	p2SString	attack_fx_path;

	bool playing_fx = false;

	const p2DynArray<iPoint>* path_to_player = nullptr;

};
#endif // !_j1ENTITY_H

