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
#include "FoWEntity.h"

#define TILE_SIZE 64
#define HALF_TILE 32

enum EntityType {
	MELEE,
	RANGED,
	GATHERER,
	BASE,
	LABORATORY,
	BARRACK,
	BIGHORNER,
	BRAHAM,
	DEATHCLAW,
	MR_HANDY,
	NO_TYPE
};

enum Faction { //Don't change faction order. Just don't do it
	VAULT,	
	BROTHERHOOD,
	MUTANT,
	GHOUL,
	NO_FACTION
};

enum State {
	IDLE,
	DIE,
	WALK,
	ATTACK,
	GATHER,
	HIT,
	NO_STATE
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

	virtual bool LoadAnimations(const char* folder, const char* file_name) { return true; };
	virtual bool LoadFx() { return true; };
	virtual bool LoadReferenceData(pugi::xml_node& ) { return true; };	
	virtual bool LoadDataFromReference() { return true; }

	int GetPositionScore() const;

	iPoint MapPosition();

	void SpatialAudio(int positionx, int positiony, Faction faction, State state, EntityType type);

	//Check if mouse pointer its inside the same spot than the entity and if pressed returns any advice

public:
	//position
	iPoint current_tile;
	fPoint position;
	iPoint render_position;
	iPoint spawnPosition;
	fPoint speed;
	int sprite_size;

	//stats
	float current_health;
	float max_health;
	int storage_capacity;
	int damage;

	//audio
	int volume;
	bool playing_fx;

	EntityType type;
	Faction faction;
	j1Entity* reference_entity;
	j1Entity* target_entity;
	DynamicEntity* attacking_entity;
	GenericPlayer* owner;
	State state;

	//graphics
	Animation* current_animation;
	SDL_Texture* texture;
	float last_dt;

	bool is_dynamic;
	bool to_delete;
	bool particles_created;

	Group_Unit info;
	j1Timer delete_timer;

	FoWEntity* visionEntity;

protected:
	SDL_Rect background_health_bar;
	SDL_Rect foreground_health_bar;
	SDL_Rect frame_quad;
};

#endif // !_j1ENTITY_H
