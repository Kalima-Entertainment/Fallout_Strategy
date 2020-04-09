#include "AI_Manager.h"
#include "j1Player.h"
#include "AI_Player.h"
#include "j1Entity.h"

AI_Manager::AI_Manager() : j1Module() {
	name = ("AI");
}

AI_Manager::~AI_Manager() {}

bool AI_Manager::Start() {
	bool ret = true;

	for (int i = 0; i < 4; i++)
	{
		if (!App->player->factions[i]) {
			ai_player[i] = new AI_Player((Faction)i);
		}
	}

	return ret;
}

bool AI_Manager::Update(float dt) {
	bool ret = true;
	for (int i = 0; i < 4; i++)
	{
		if (ai_player[i] != nullptr) {}
	}
	return ret;
}