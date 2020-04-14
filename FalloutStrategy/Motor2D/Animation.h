#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#include "j1App.h"
#include <math.h>
#define MAX_FRAMES 200

struct Collider;

struct Frame {

	SDL_Rect rect;
	float speed;
};

class Animation
{
public:
	bool loop = true;
	Frame frames[MAX_FRAMES];
	float current_frame = 0;
private:
	int loops;
	int num_frames = 0;
	int last_frame = 0;
	int cont = -1;

public:

	void PushBack(const SDL_Rect& rect, float speed = 1.0f)
	{
		num_frames++;
		++last_frame;
		int i = ++cont;
		frames[i].rect = rect;
		frames[i].speed = speed;
	}

	void SetWidth(const int & width) {
		for (int i = 0; i < num_frames; ++i) {
			frames[i].rect.w = width;
		}
	}

	SDL_Rect& GetCurrentFrame(float dt)
	{
		current_frame += frames[(int)current_frame].speed * dt;
		if (current_frame >= last_frame) {
			current_frame = (loop) ? 0.0f : last_frame - 1;
			loops++;
		}
		
		return frames[(int)current_frame].rect;
	}
	bool Finished() const
	{
		return loops == num_frames;
	}

	void Reset()
	{
		current_frame = 0;
		loops = 0;
	}
	int SeeCurrentFrame() {
		return (int)current_frame;
	}
	SDL_Rect GetCurrentRect(){
		return frames[(int)current_frame].rect;
	}
};

#endif