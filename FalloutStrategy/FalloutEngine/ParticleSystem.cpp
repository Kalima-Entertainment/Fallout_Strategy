#include "ParticleSystem.h"
#include "Emiter.h"
#include "Particle.h"

ParticleSystem::ParticleSystem() : j1Entity()
{
	position.push_back(0.0f);
	position.push_back(0.0f);
	active = true;
}

ParticleSystem::ParticleSystem(float x, float y) : j1Entity()
{
	position.push_back(x);
	position.push_back(y);
	active = true;
}

ParticleSystem::~ParticleSystem()
{
	emiterVector.clear();
}

bool ParticleSystem::CleanUp()
{
	emiterVector.clear();

	return true;
}

bool ParticleSystem::Update(float dt)
{
	int numEmiters = emiterVector.size();

	for(int i = 0; i < numEmiters; i++)
	{
		emiterVector[i].Update(dt);
	}
	return true;
}


bool ParticleSystem::PostUpdate(float dt)
{
	int numEmiters = emiterVector.size();

	for(int i = 0; i < numEmiters; i++)
	{
		emiterVector[i].PostUpdate(dt);
	}
	return true;
}


void ParticleSystem::PushEmiter(Emiter& emiter)
{
	emiterVector.push_back(emiter);
}


void ParticleSystem::Desactivate()
{
	int numEmiters = emiterVector.size();

	for(int i = 0; i < numEmiters; i++)
	{
		emiterVector[i].Desactivate();
	}

	active = false;
}


void ParticleSystem::Activate()
{
	int numEmiters = emiterVector.size();

	for(int i = 0; i < numEmiters; i++)
	{
		emiterVector[i].Activate();
	}

	active = true;
}


bool ParticleSystem::IsActive()
{
	return active;
}

//Move the particle system, and its emiters in relation to the particle system
void ParticleSystem::Move(int x, int y)
{
	int previousX = position[0];
	int previousY = position[1];

	if (active)
	{
		int numEmiters = emiterVector.size();

		for(int i = 0; i < numEmiters; i++)
		{
			int xPos = 0, yPos = 0;

			emiterVector[i].GetPosition(xPos, yPos);

			xPos += x - previousX;
			yPos += y - previousY;

			emiterVector[i].SetPosition(xPos, yPos);
		}
	}

	position[0] = x;
	position[1] = y;
}