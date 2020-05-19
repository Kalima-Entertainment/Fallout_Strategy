#include "ParticleManager.h"

ParticleManager::ParticleManager() {

}

ParticleManager::~ParticleManager() {

}

bool ParticleManager::PreUpdate() {

	return true;
}

bool ParticleManager::Update(float dt) {

	return true;
}

bool ParticleManager::PostUpdate() {

}

bool ParticleManager::CleanUp() {

}

bool ParticleManager::CreateEmitter() {
	
	bool ret = true;

	Emitter* particle = new Emitter();
	if (particle != nullptr) {
		Emitters.push_back(particle);
	}
	else {
		ret = false;
	}

	return ret;
}