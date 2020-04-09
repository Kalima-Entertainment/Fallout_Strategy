#ifndef __j1TRANSITION_H__
#define __j1TRANSITION_H__

#include "PugiXml/src/pugixml.hpp"
#include "j1Module.h"
#include "Animation.h"
#include "p2List.h"
#include "p2Point.h"
#include "j1Timer.h"

class j1Transition : public j1Module
{
public:
	j1Transition();
	bool j1Transition::LoadAnimations();
	bool Awake(pugi::xml_node& config);
	bool Start();
	bool Update(float dt);
	bool PostUpdate(float dt);
	~j1Transition();
	bool CleanUp();
	void Transition();
private:
	int alpha;
	float deltatime;
public:
	bool fade;
	j1Timer fadetimer;

};



#endif