#include "Emitter.h"
#include "j1App.h"
#include "j1Render.h"

Emitter::Emitter() {
	
	spawn_point = { 0,0 };
	lifetime = 0;
	max_particles = 0;
	finished = true;
	current_particle = nullptr;
}

// -- Overload
Emitter::Emitter(iPoint spawn, int particle_life, int particle_number) {

	spawn_point = spawn;
	lifetime = particle_life;
	max_particles = particle_number;
	current_particle = new Particle[max_particles]; //Check this later
}

Emitter::~Emitter() {

}

bool Emitter::PreUpdate() {

	// -- Change relative position in world

	return true;
}

bool Emitter::Update(float dt) {

	// -- Main Particle Logic
	while (max_particles != 0) {

		current_particle->Update(dt);
		max_particles--;
	}


	return true;
}

bool Emitter::PostUpdate() {

	// -- Blit and Other stuff

	return true;
}

bool Particle::PreUpdate() {

}

bool Particle::Update(float dt) {
	
}

bool Particle::PostUpdate() {
	
	//App->render->Blit();

}