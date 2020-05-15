#ifndef _EMITTER_H__
#define _EMITTER_H__

#include "p2Point.h"
#include "j1Timer.h"
#include "j1Entity.h"
#include <math.h>

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
	Emitter(iPoint spawn_point, int lifetime, int max_particles);
	~Emitter();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool Active();
	bool DeActive();


private:

	iPoint spawn_point;
	int lifetime;
	int max_particles;
	bool finished;

};


#endif // !_EMITER_H__