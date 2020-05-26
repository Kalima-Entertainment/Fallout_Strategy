#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "vector"
#include "Animation.h"

struct SDL_Texture;

class Particle
{
public:
	Particle();
	Particle(float life, SDL_Texture* texture, Animation animation, bool fade = false); //The emiter uses this constructor
	Particle(std::vector<float>& position, std::vector<float>& speed, std::vector<float>& acceleration, float angle, float angularSpeed, float life, SDL_Texture* texture, Animation animation, bool fade = false);
	Particle(float positionX, float positionY, float speedX, float speedY, float accelerationX, float accelerationY, float angle, float angularSpeed, float life, SDL_Texture* texture, Animation animation, bool fade = false);
	~Particle();

	//Getters and setters
	std::vector<float> GetPosition();
	std::vector<float> GetSpeed();
	std::vector<float> GetAcceleration();

	float GetAngle();
	float GetAngularSpeed();
	void SetLife(float NewLife);
	float GetLife();
	SDL_Texture* GetTexture();

	void SetPosition(std::vector<float>&);
	void SetSpeed(std::vector<float>&);
	void SetAcceleration(std::vector<float>&);

	void SetAngle(float);
	void SetAngularSpeed(float);

	//the actual useful functions
	void Update(float dt);
	void PostUpdate(float dt);

	bool Activate();
	void Reset(float x, float y, float speedX, float speedY, float accX, float accY, float angularSpeed);
	void Desactivate();

private:
	void Move(float dt);
	void Draw(float dt);

	void CheckLife(float dt);

private:
	SDL_Texture* texture;
	Animation animation;

	std::vector<float> position; // 0 is the x axis, and 1 the y axis
	std::vector<float> speed;
	std::vector<float> acceleration;

	float angle;
	float angularSpeed;//positive = right, negative = left

	float life;
	float originalLife;

	bool fade;
	bool active;
};

#endif //__PARTICLE_H__#pragma once
