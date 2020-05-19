#ifndef _PARTICLE_MANAGER_H__
#define _PARTICLE_MANAGER_H__

#include "j1Module.h"
#include "Emitter.h"

class ParticleManager : public j1Module {

	ParticleManager();
	~ParticleManager();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();


	bool CreateEmitter();
	void DestroyEmitter();
	void DestroyAllEmitters();

public:

	std::vector<Emitter*> Emitters;

};


#endif // _PARTICLE_MANAGER_H__