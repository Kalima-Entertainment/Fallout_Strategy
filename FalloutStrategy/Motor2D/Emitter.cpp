#include "Emitter.h"


Emitter::Emitter() {

}

// -- Overload
Emitter::Emitter(iPoint spawn, int particle_life, int particle_number) {

	spawn_point = spawn;
	lifetime = particle_life;
	max_particles = particle_number;
}

Emitter::~Emitter() {

}

bool Emitter::PreUpdate() {

	// -- Change relative position in world

	return true;
}

bool Emitter::Update(float dt) {

	// -- Main Particle Logic

	return true;
}

bool Emitter::PostUpdate() {

	// -- Blit and Other stuff

	return true;
}

bool Particle::Update(float dt) {

}