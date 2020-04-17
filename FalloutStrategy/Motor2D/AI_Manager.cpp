#include "AI_Manager.h"
#include "j1Player.h"
#include "AI_Player.h"
#include "j1Entity.h"

AI_Manager::AI_Manager() : j1Module() {
	name = ("AI");
	players_created = false;
}

AI_Manager::~AI_Manager() {}

void AI_Manager::CreateAI_PLayers() {

	for (int i = 0; i < 4; i++)
	{
		if (App->player->faction != (Faction) i) {
			ai_player[i] = new AI_Player((Faction) i);
		}
		else
		{
			ai_player[i] = NULL;
		}
	}
	players_created = true;
}

bool AI_Manager::Update(float dt) {
	bool ret = true;
	if (players_created) {
		for (int i = 0; i < 4; i++)
		{
			if (ai_player[i] != nullptr) {
				ai_player[i]->Update(dt);
			}
		}
	}
	return ret;
}