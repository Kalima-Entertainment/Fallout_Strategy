#ifndef _EMITTER_H__
#define _EMITTER_H__

#include "p2Point.h"
#include "j1Timer.h"
#include "j1Entity.h"

struct Particle {

	iPoint position;
	fPoint speed;
	
	// -- Looking to add forward
	/*int rotation;
	float rotation_speed*/

	bool Update(float dt);
};

class Emitter {
	
public:

	Emitter();
	~Emitter();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool Active();
	bool DeActive();


private:

	iPoint spawn_point;
	int lifetime;
	int min_particles;
	int max_particles;

};


#endif // !_EMITER_H__