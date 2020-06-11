#include "vector"
#include "Animation.h"

#define FPS 60
#define MSPF 16

class Particle;
struct SDL_Texture;

class Emiter
{
public:
	Emiter(float posX, float posY, float SpeedX, float SpeedY, int VariationSpeedX, int VariationSpeedY, float AccelerationX,
		float AccelerationY, int VariationAccelerationX, int VariationAccelerationY, float AngularSpeed, int VariableAngularSpeed,
		float Rate, float LifeTime, SDL_Rect* spawn, SDL_Texture* texture,const Animation &particleAnimation, bool fade);

	~Emiter();

	void Update(float dt);
	void PostUpdate(float dt);

	void GetPosition(int& x, int& y);
	void SetPosition(int x, int y);

	void Activate();
	void Desactivate();
	void SetSpeed(float x, float y);
	void SetMaxTime(float NewTime);

private:
	void Start();
	void CreateParticle();
	void ThrowParticles();

	//Generate functions, used in ThrowParticles
	float GeneratePosX();
	float GeneratePosY();

	float GenerateSpeedX();
	float GenerateSpeedY();

	float GenerateAccelerationX();
	float GenerateAccelerationY();

	float GenerateAngularSpeed();


private:
	std::vector<float> position;
	std::vector<float> particleSpeed;
	std::vector<float> particleAcceleration;

	float particleAngularSpeed;

	std::vector<int> particleVariationSpeed;
	std::vector<int> particleVariationAcceleration;
	int particleVariationAngularSpeed;


	std::vector<Particle> particleVector;

	SDL_Rect* areaOfSpawn; //Optional
	SDL_Texture* particleTexture;
	Animation particleAnimation;

	float particlesRate; //per second
	float particlesLifeTime;
	float particlesPerFrame;

	float particlesEmited;

	bool randomizePosX;
	bool randomizePosY;

	bool randomizeSpeedX;
	bool randomizeSpeedY;

	bool randomizeAccelerationX;
	bool randomizeAccelerationY;

	bool randomizeAngularSpeed;

	bool fadeParticles;

	bool active;
};
