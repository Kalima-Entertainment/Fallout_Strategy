#include "Emiter.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "Particle.h"
#include "math.h"
#include "p2Log.h"

Emiter::Emiter(std::vector<float>& position, std::vector<float>& particleSpeed, std::vector<int>& particleVariationSpeed,
	std::vector<float>& particleAcceleration, std::vector<int>& particleVariationAcceleration, float particleAngularSpeed,
	int particleVariableAngularSpeed, float particlesRate, float particlesLifeTime, SDL_Rect* areaOfSpawn, SDL_Texture* texture, Animation particleAnimation, bool fade) :

	position(position),
	particleSpeed(particleSpeed),
	particleVariationSpeed(particleVariationSpeed),
	particleAcceleration(particleAcceleration),
	particleVariationAcceleration(particleVariationAcceleration),
	particleAngularSpeed(particleAngularSpeed),
	particleVariationAngularSpeed(particleVariableAngularSpeed),

	particlesRate(particlesRate),
	particlesLifeTime(particlesLifeTime),

	areaOfSpawn(areaOfSpawn),
	particleTexture(texture),
	particleAnimation(particleAnimation),

	randomizePosX(true),
	randomizePosY(true),

	randomizeSpeedX(true),
	randomizeSpeedY(true),

	randomizeAccelerationX(true),
	randomizeAccelerationY(true),

	randomizeAngularSpeed(true),

	active(true),

	fadeParticles(fade)

{
	Start();
}


Emiter::Emiter(float positionX, float positionY, float particleSpeedX, float particleSpeedY, int particleVariationSpeedX, int particleVariationSpeedY,
	float particleAccelerationX, float particleAccelerationY, int particleVariationAccelerationX, int particleVariationAccelerationY, float particleAngularSpeed,
	int particleVariableAngularSpeed, float particlesRate, float particlesLifeTime, SDL_Rect* areaOfSpawn, SDL_Texture* texture, Animation particleAnimation, bool fade) :

	position{ positionX, positionY },
	particleSpeed{ particleSpeedX, particleSpeedY },
	particleVariationSpeed{ particleVariationSpeedX, particleVariationSpeedY },
	particleAcceleration{ particleAccelerationX, particleAccelerationY },
	particleVariationAcceleration{ particleVariationAccelerationX, particleVariationAccelerationY },
	particleAngularSpeed(particleAngularSpeed),
	particleVariationAngularSpeed(particleVariableAngularSpeed),

	particlesRate(particlesRate),
	particlesLifeTime(particlesLifeTime),

	areaOfSpawn(areaOfSpawn),
	particleTexture(texture),
	particleAnimation(particleAnimation),

	randomizePosX(true),
	randomizePosY(true),

	randomizeSpeedX(true),
	randomizeSpeedY(true),

	randomizeAccelerationX(true),
	randomizeAccelerationY(true),

	randomizeAngularSpeed(true),

	active(true),

	fadeParticles(fade)
{
	Start();
}


void Emiter::Start()
{
	//TODO 3: Just calculate the max number of particles you will have in screen
	//particles rate * particles life time
	int maxParticles = particlesRate * particlesLifeTime + 1;

	//We assume that the game will allways go at 60 FPS
	particlesPerFrame = particlesRate * 16 / 1000;

	particleVector.reserve(maxParticles);

	for(size_t i = 0; i < maxParticles; i++)
	{
		CreateParticle();
	}

	//Set all the bools to check what variables will be randomized in the Generate() functions
	if (areaOfSpawn == nullptr)
	{
		randomizePosX = false;
		randomizePosY = false;
	}

	else
	{
		if (areaOfSpawn->w == 0)
		{
			randomizePosX = false;
		}

		if (areaOfSpawn->h == 0)
		{
			randomizePosY = false;
		}
	}


	if (particleVariationSpeed[0] == NULL)
	{
		randomizeSpeedX = false;
	}

	if (particleVariationSpeed[1] == NULL)
	{
		randomizeSpeedY = false;
	}


	if (particleVariationAcceleration[0] == NULL)
	{
		randomizeAccelerationX = false;
	}

	if (particleVariationAcceleration[1] == NULL)
	{
		randomizeAccelerationY = false;
	}

	if (particleVariationAngularSpeed == 0)
	{
		randomizeAngularSpeed = false;
	}
}


void Emiter::CreateParticle()
{
	particleVector.push_back(Particle(particlesLifeTime, particleTexture, particleAnimation, fadeParticles));
}


Emiter::~Emiter() {

	particleVector.clear();

	position.clear();
	particleSpeed.clear();
	particleAcceleration.clear();

	particleVariationSpeed.clear();
	particleVariationAcceleration.clear();

	areaOfSpawn = nullptr;
	particleTexture = nullptr;
}


void Emiter::Update(float dt) {

	if (active)
	{
		ThrowParticles();
	}
	int numParticles = particleVector.size();


	for(size_t i = 0; i < numParticles; i++)
	{
		particleVector[i].Update(dt);
	}
}


void Emiter::PostUpdate(float dt) {

	int numParticles = particleVector.size();

	for(size_t i = 0; i < numParticles; i++)
	{
		particleVector[i].PostUpdate(dt);
	}

}


void Emiter::Desactivate()
{
	active = false;
}

void Emiter::Activate()
{
	active = true;
}


//This funtions activates the necesary particles and gives them the initial values
void Emiter::ThrowParticles() {

	//You could use delta time instead of particlesPerFrame, but i dont recommend it
	particlesEmited += particlesPerFrame;

	if (particlesEmited >= 1)
	{
		int emited = 0;

		for (size_t i = 0; i < particleVector.size(); i++)
		{
			//TODO 2: Call Activate(), use Generate...() functions to get the parameters Activate() needs.
			//Activate returns false if the particle is already active, and true if we activate it.

			if (particleVector[i].Activate())
			{
				particleVector[i].SetLife(particlesLifeTime);
				particleVector[i].Reset(GeneratePosX(), GeneratePosY(), GenerateSpeedX(), GenerateSpeedY(), GenerateAccelerationX(), GenerateAccelerationY(), GenerateAngularSpeed());
				emited++;
			}

			//If we activated the necesary particles this frame, break
			if ((int)particlesEmited == emited)
				break;

		}

		particlesEmited -= emited;
	}
}


float Emiter::GeneratePosX()
{
	if (randomizePosX == true)
	{
		float x = (rand() % areaOfSpawn->w) + position[0];
		return x;
	}

	else
		return position[0];
}


float Emiter::GeneratePosY()
{
	if (randomizePosY == true)
	{
		float y = (rand() % areaOfSpawn->h) + position[1];
		return y;
	}

	else
		return position[1];
}

void Emiter::SetSpeed(float x, float y)
{
	particleSpeed[0] = x;
	particleSpeed[1] = y;
}

void Emiter::SetMaxTime(float NewTime)
{
	particlesLifeTime = NewTime;
	/*particleVector.clear();

	particlesPerFrame = particlesRate * 16 / 1000;

	int maxParticles = particlesRate * particlesLifeTime + 1;

	particleVector.reserve(maxParticles);

	for(size_t i = 0; i < maxParticles; i++)
	{
		CreateParticle();
	}*/

}

float Emiter::GenerateSpeedX()
{
	if (randomizeSpeedX == true)
	{
		float speedX = (rand() % particleVariationSpeed[0]) + particleSpeed[0];
		return speedX;
	}

	else
		return particleSpeed[0];
}


float Emiter::GenerateSpeedY()
{
	if (randomizeSpeedY == true)
	{
		float speedY = (rand() % particleVariationSpeed[1]) + particleSpeed[1];
		return speedY;
	}

	else
		return particleSpeed[1];

}


float Emiter::GenerateAccelerationX()
{
	if (randomizeAccelerationX == true)
	{
		float accX = (rand() % particleVariationAcceleration[0]) + particleAcceleration[0];
		return accX;
	}

	else
		return particleAcceleration[0];
}


float Emiter::GenerateAccelerationY()
{
	if (randomizeAccelerationY == true)
	{
		float accY = (rand() % particleVariationAcceleration[1]) + particleAcceleration[1];
		return accY;
	}

	else
		return particleAcceleration[1];
}


float Emiter::GenerateAngularSpeed()
{
	if (randomizeAngularSpeed == true)
	{
		float angularSpeed = (rand() % particleVariationAngularSpeed) + particleAngularSpeed;
		return angularSpeed;
	}

	else
		return particleAngularSpeed;
}


void Emiter::GetPosition(int& x, int& y)
{
	x = position[0];
	y = position[1];
}

void Emiter::SetPosition(int x, int y)
{
	position[0] = x;
	position[1] = y;
}