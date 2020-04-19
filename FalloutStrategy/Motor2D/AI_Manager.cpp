#include "AI_Manager.h"
#include "j1Player.h"
#include "AI_Player.h"
#include "j1Entity.h"
#include "j1Scene.h"

AI_Manager::AI_Manager() : j1Module() {
	name = ("AI");
	players_created = false;
	ai_player[0] = ai_player[1] = ai_player[2] = ai_player[3] = nullptr;
	beaten_ai_players = 0;
}

AI_Manager::~AI_Manager() {}

bool AI_Manager::Start() {
	bool ret = true;
	for (int i = 0; i < 4; i++)
	{
		if (App->player->faction != (Faction) i) {
			ai_player[i] = new AI_Player((Faction) i);
			App->scene->players[i] = ai_player[i];
		}
		else
		{
			ai_player[i] = NULL;
			App->scene->players[i] = App->player;
		}
	}
	players_created = true;
	return ret;
}

bool AI_Manager::Update(float dt) {
	bool ret = true;
	if (players_created) {
		for (int i = 0; i < 4; i++)
		{
			if ((ai_player[i] != nullptr)&&(!ai_player[i]->defeated)) {
				ai_player[i]->Update(dt);
			}
		}
	}
	return ret;
}

bool AI_Manager::CleanUp() {
	bool ret = true;

	for (int i = 0; i < 4; i++)
	{
		if (ai_player[i] != nullptr) {
			delete ai_player[i];
			ai_player[i] = nullptr;
		}
	}
	players_created = false;

	return ret;
}