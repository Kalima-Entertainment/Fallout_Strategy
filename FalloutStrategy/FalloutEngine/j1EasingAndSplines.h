#ifndef __J1EASINGSPLINES_H__
#define __J1EASINGSPLINES_H__

#include "j1Module.h"
#include <vector>
#include "SDL/include/SDL_timer.h"

#define PI 3.14159265359

struct SDL_Texture;
struct SDL_Rect;

enum class Spline_Type {

	EASE,

	EASE_IN_QUAD,
	EASE_OUT_QUAD,
	EASE_IN_OUT_QUAD,

	EASE_IN_CUBIC,
	EASE_OUT_CUBIC,
	EASE_IN_OUT_CUBIC,

	EASE_IN_QUART,
	EASE_OUT_QUART,
	EASE_IN_OUT_QUART,

	EASE_IN_QUINT,
	EASE_OUT_QUINT,
	EASE_IN_OUT_QUINT,

	EASE_IN_SINE,
	EASE_OUT_SINE,
	EASE_IN_OUT_SINE,

	EASE_IN_EXPO,
	EASE_OUT_EXPO,
	EASE_IN_OUT_EXPO,

	EASE_IN_CIRC,
	EASE_OUT_CIRC,
	EASE_IN_OUT_CIRC,

	EASE_OUT_BOUNCE,

	EASE_IN_BACK,
	EASE_OUT_BACK,
	EASE_IN_OUT_BACK,
	
	EASE_OUT_ELASTIC,
	
	NONE
};

struct EaseFunctions {
	
	int Ease(float &time_passed, int &i_pos, int &f_pos, float &duration);
	
	int EaseInQuad(float &time_passed, int &i_pos, int &f_pos, float &duration);
	int EaseOutQuad(float &time_passed, int &i_pos, int &f_pos, float &duration);
	int EaseInOutQuad(float &time_passed, int &i_pos, int &f_pos, float &duration);
	
	int EaseInCubic(float &time_passed, int &i_pos, int &f_pos, float &duration);
	int EaseOutCubic(float &time_passed, int &i_pos, int &f_pos, float &duration);
	int EaseInOutCubic(float &time_passed, int &i_pos, int &f_pos, float &duration);

	int EaseInQuart(float &time_passed, int &i_pos, int &f_pos, float &duration);
	int EaseOutQuart(float &time_passed, int &i_pos, int &f_pos, float &duration);
	int EaseInOutQuart(float &time_passed, int &i_pos, int &f_pos, float &duration);

	int EaseInQuint(float &time_passed, int &i_pos, int &f_pos, float &duration);
	int EaseOutQuint(float &time_passed, int &i_pos, int &f_pos, float &duration);
	int EaseInOutQuint(float &time_passed, int &i_pos, int &f_pos, float &duration);

	int EaseInSine(float &time_passed, int &i_pos, int &f_pos, float &duration);
	int EaseOutSine(float &time_passed, int &i_pos, int &f_pos, float &duration);
	int EaseInOutSine(float &time_passed, int &i_pos, int &f_pos, float &duration);

	int EaseInExpo(float &time_passed, int &i_pos, int &f_pos, float &duration);
	int EaseOutExpo(float &time_passed, int &i_pos, int &f_pos, float &duration);
	int EaseInOutExpo(float &time_passed, int &i_pos, int &f_pos, float &duration);

	int EaseInCirc(float &time_passed, int &i_pos, int &f_pos, float &duration);
	int EaseOutCirc(float &time_passed, int &i_pos, int &f_pos, float &duration);
	int EaseInOutCirc(float &time_passed, int &i_pos, int &f_pos, float &duration);

	int EaseOutBounce(float &time_passed, int &i_pos, int &f_pos, float &duration);
	
	int EaseInBack(float &time_passed, int &i_pos, int &f_pos, float &duration);
	int EaseOutBack(float &time_passed, int &i_pos, int &f_pos, float &duration);
	int EaseInOutBack(float &time_passed, int &i_pos, int &f_pos, float &duration);

	int EaseOutElastic(float &time_passed, int &i_pos, int &f_pos, float &duration);
	
};

struct SplineInfo {

	SplineInfo(int* position, const int &target_position, const float &duration, const Spline_Type &t);
	bool Update(float dt);

public:

	int *position = nullptr;
	int i_pos = 0, f_pos = 0;

	float time_to_travel = 0.0F, start_time = 0.0F;

	Spline_Type type = Spline_Type::NONE;
	EaseFunctions ease_function;

};

class j1EasingAndSplines : public j1Module
{
public:

	j1EasingAndSplines();

	// Destructor
	virtual ~j1EasingAndSplines();

	// Called each loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	//To create splines
	void CreateSpline(int* i_pos, const int &f_pos, const float &duration, const Spline_Type &t);


private:

	std::vector<SplineInfo*> easing_splines;

};


#endif
