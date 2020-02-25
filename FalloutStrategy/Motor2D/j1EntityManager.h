#ifndef _ENTITY_MANAGER_H_
#define _ENTITY_MANAGER_H_

#include "j1Module.h"
#include "p2Point.h"
#include "Animation.h"
#include "SDL_image/include/SDL_image.h"

class j1Entity;
struct SDL_Texture;
class j1Player;
class j1WalkingEnemy;
class j1FlyingEnemy;

enum class EntityType
{
	PLAYER,
	WALKING_ENEMY,
	WALKING_ENEMY2,
	FLYING_ENEMY,
	TRAP,
	PARTICLES,
	UNKNOWN
};


class j1EntityManager : public j1Module
{
public:

	j1EntityManager();
	~j1EntityManager();

	virtual bool Awake(pugi::xml_node&);
	virtual bool Start();

	virtual bool PreUpdate();
	virtual bool Update(float dt);
	virtual bool PostUpdate();

	virtual bool CleanUp();

	bool Load(pugi::xml_node& data);
	bool Save(pugi::xml_node& data) const;
	bool CheckpointSave();
	bool CheckpointLoad();

	j1Entity*	 getPlayer();
	j1Entity*	 CreateEntity(EntityType type, int position_x, int position_y);
	virtual void DestroyEntity(j1Entity* delete_entity);
	void		 DestroyAllEntities();
	void		 RellocateEntities();

public:

	p2List<j1Entity*> entities;
	pugi::xml_node config_data;

	j1Player* player;

public:
	int gravity = 0;
	int max_falling_speed = 0;

	j1WalkingEnemy* reference_walking_enemy = nullptr;
	j1FlyingEnemy* reference_flying_enemy = nullptr;

	//sfx
	uint walking_enemy_attack_fx;
	uint flying_enemy_attack_fx;

	uint walking_enemy_die_fx;
	uint flying_enemy_die_fx;
	uint walking_enemy2_die_fx;

	float time_between_updates = 0.01f;
	float accumulated_time = 0;

	bool blocked_movement = false;

};

#endif // !_ENTITY_MANAGER_H_

