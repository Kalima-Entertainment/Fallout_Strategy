#include "AI_Manager.h"
#include "j1Player.h"
#include "AI_Player.h"
#include "j1Entity.h"
#include "j1Scene.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Input.h"
#include "brofiler/Brofiler/Brofiler.h"

AI_Manager::AI_Manager() : j1Module(), beaten_ai_players(0) {

	name.assign("AI");

	players_created = false;
	ai_player[0] = ai_player[1] = ai_player[2] = ai_player[3] = nullptr;
}

AI_Manager::~AI_Manager() {
	ai_player[0] = ai_player[1] = ai_player[2] = ai_player[3] = nullptr;
}

bool AI_Manager::Awake(pugi::xml_node& config) {
	bool ret = true;
	pugi::xml_node faction_node = config.first_child();
	Faction faction = NO_FACTION;
	std::string faction_name;

	for(int i = 0; i < 4; i++)
	{
		faction_name = faction_node.name();
		if (faction_name == "vault") faction = VAULT;
		else if (faction_name == "brotherhood") faction = BROTHERHOOD;
		else if (faction_name == "mutant") faction = MUTANT;
		else if (faction_name == "ghoul") faction = GHOUL; 

		ai_info[faction].initial_caps = faction_node.attribute("caps").as_int();
		ai_info[faction].initial_water = faction_node.attribute("water").as_int();
		ai_info[faction].initial_food = faction_node.attribute("food").as_int();
		ai_info[faction].minimum_melees = faction_node.attribute("minimum_melees").as_int();
		ai_info[faction].minimum_rangeds = faction_node.attribute("minimum_rangeds").as_int();
		ai_info[faction].wave_time = faction_node.attribute("wave_time").as_int();

		faction_node = faction_node.next_sibling();
	}

	return ret;
}

bool AI_Manager::Start() {
	bool ret = true;	

	for(int i = 0; i < 4; i++)
	{
		if (App->player->faction != (Faction) i) {
			ai_player[i] = new AI_Player((Faction) i);
			App->scene->players[i] = ai_player[i];
		}
		else
		{
			ai_player[i] = nullptr;
			App->scene->players[i] = App->player;
		}
	}
	players_created = true;
	return ret;
}

bool AI_Manager::Update(float dt) {
	BROFILER_CATEGORY("AI_Update", Profiler::Color::Azure)
	bool ret = true;
	
	if (players_created) {
		for(int i = 0; i < 4; i++)
		{
			if ((ai_player[i] != nullptr)&&(!ai_player[i]->defeated)) {
				ai_player[i]->Update(dt);
			}
		}
	}
	return ret;
}

bool AI_Manager::PostUpdate() {
	bool ret = true;
	iPoint node_world_position;
	SDL_Rect node;

	return ret;
}

bool AI_Manager::CleanUp() {
	bool ret = true;

	for(int i = 0; i < 4; i++)
	{
		if (ai_player[i] != nullptr) {
			delete ai_player[i];
			ai_player[i] = nullptr;
		}
	}
	players_created = false;

	return ret;
}

AI_Info AI_Manager::GetAI_PlayerInfo(Faction faction) { return ai_info[faction]; }

// Load Game State
bool AI_Manager::Load(pugi::xml_node& data)
{
	//camera.x = data.child("camera").attribute("x").as_int();
	//camera.y = data.child("camera").attribute("y").as_int();

	return true;
}

// Save Game State
bool AI_Manager::Save(pugi::xml_node& data) const
{
	//pugi::xml_node cam = data.append_child("camera");

	//cam.append_attribute("x") = camera.x;
	//cam.append_attribute("y") = camera.y;


	return true;
}