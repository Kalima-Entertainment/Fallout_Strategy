#include "Particle.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1Render.h"

Particle::Particle(std::vector<float>& position, std::vector<float>& speed, std::vector<float>& acceleration,
	float angle, float angularSpeed, float life, SDL_Texture* texture, const Animation &animation, bool fade) :

	position(position),
	speed(speed),
	acceleration(acceleration),
	angle(angle),
	angularSpeed(angularSpeed),

	life(life),
	originalLife(life),

	texture(texture),
	animation(animation),

	fade(fade),
	active(false)

{}


Particle::Particle(float life, SDL_Texture* texture, const Animation &animation, bool fade) :

	position{ NULL, NULL },
	speed{ NULL, NULL },
	acceleration{ NULL, NULL },
	angle(0),
	angularSpeed(0),

	life(life),
	originalLife(life),

	texture(texture),
	animation(animation),

	fade(fade),
	active(false)
{}



Particle::Particle(float positionX, float positionY, float speedX, float speedY, float accelerationX, float accelerationY,
	float angle, float angularSpeed, float life, SDL_Texture* texture, const Animation &animation, bool fade) :

	position{ positionX, positionY },
	speed{ speedX, speedY },
	acceleration{ accelerationX, accelerationY },
	angle(angle),
	angularSpeed(angularSpeed),

	life(life),
	originalLife(life),

	texture(texture),
	animation(animation),

	fade(fade),
	active(false)
{}


Particle::~Particle()
{
	position.clear();
	speed.clear();
	acceleration.clear();

	texture = nullptr;
}


std::vector<float> Particle::GetPosition() {
	return position;
}


std::vector<float> Particle::GetSpeed() {
	return speed;
}


std::vector<float> Particle::GetAcceleration() {
	return acceleration;
}


float Particle::GetAngle() {
	return angle;
}


float Particle::GetAngularSpeed() {
	return angularSpeed;
}


float Particle::GetLife() {
	return life;
}

void Particle::SetLife(float newLife) {
	originalLife = newLife;
}
SDL_Texture* Particle::GetTexture() {
	return texture;
}


void Particle::SetPosition(std::vector<float>& pos) {
	position = pos;
}


void Particle::SetSpeed(std::vector<float>& spd) {
	speed = spd;
}


void Particle::SetAcceleration(std::vector<float>& acc) {
	acceleration = acc;
}


void Particle::SetAngle(float ang) {
	angle = ang;
}


void Particle::SetAngularSpeed(float aspd) {
	angularSpeed = aspd;
}


void Particle::Update(float dt)
{
	if (active)
	{
		Move(dt);
		CheckLife(dt);
		Draw(dt);
	}
}


void Particle::PostUpdate(float dt)
{

}


void Particle::Draw(float dt)
{
	if (fade == true)
	{
		Uint8 transparency = life / originalLife * 255;
		App->render->Blit(texture, position[0], position[1], &animation.GetCurrentFrame(dt), 1, 1, false, NONE, angle, 0, 0, transparency);
	}

	else
		App->render->Blit(texture, position[0], position[1], &animation.GetCurrentFrame(dt), 1, 1, angle);

}

void Particle::Move(float dt)
{
	speed[0] += acceleration[0] * dt * 60;
	speed[1] += acceleration[1] * dt * 60;

	// Do the same with position and angles
	position[0] += speed[0] * dt * 60;
	position[1] += speed[1] * dt * 60;

	angle += angularSpeed * dt * 60;
}


void Particle::CheckLife(float dt)
{
	life -= dt;

	if (life < 0)
	{
		Desactivate();
	}
}


void Particle::Desactivate()
{
	active = false;
}


bool Particle::Activate()
{
	return !active;
}


void Particle::Reset(float x, float y, float speedX, float speedY, float accX, float accY, float angSpeed)
{
	life = originalLife;

	position[0] = x;
	position[1] = y;

	speed[0] = speedX;
	speed[1] = speedY;

	acceleration[0] = accX;
	acceleration[1] = accY;

	angularSpeed = angSpeed;

	active = true;
}