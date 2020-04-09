#include <math.h>
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Transition.h"
#include "j1Render.h"
#include "SDL/include/SDL_render.h"
#include "j1Window.h"
#include "SDL/include/SDL_timer.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1EntityManager.h"


#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1Transition.h"
#include "j1Window.h"

j1Transition::j1Transition() : j1Module()
{

	name = ("Transition");
}

// Destructor
j1Transition::~j1Transition()
{
}
bool j1Transition::LoadAnimations() {
	bool ret = true;

	return ret;
}
bool j1Transition::CleanUp()
{
	return true;
}

bool j1Transition::Awake(pugi::xml_node& config)
{
	return true;
}

bool j1Transition::Start()
{
	fade = false;
	alpha = 0;
	return true;
}

bool j1Transition::Update(float dt)
{
	deltatime = dt;
	//if fade is active, then call fadetoblack function
	if (fade) {
		Transition();
	}
	return true;
}
bool j1Transition::PostUpdate(float dt)
{
	return true;
}


void j1Transition::Transition()
{
	if (!fade)fadetimer.Start();
	fade = true;
	if (alpha < 255) {
		alpha = alpha + 240 * deltatime;
	}
	if (alpha > 255) {
		alpha = 255;
	}
	SDL_Rect screen;
	screen.x = -1 * App->render->camera.x;
	screen.y = -1 * App->render->camera.y;
	screen.w = App->win->width * App->win->GetScale();
	screen.h = App->win->height * App->win->GetScale();
	App->render->DrawQuad(screen, 0, 0, 0, alpha);
	if (fadetimer.Read() > 1000) {
		fade = false;
		alpha = 0;
	}

}
