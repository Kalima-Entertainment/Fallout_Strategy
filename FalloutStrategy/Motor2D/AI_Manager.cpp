#include "AI_Manager.h"
#include "j1Player.h"
#include "AI_Player.h"
#include "j1Entity.h"
#include "j1Scene.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Input.h"
#include "brofiler/Brofiler/Brofiler.h"

AI_Manager::AI_Manager() : j1Module() {
	name = ("AI");
	players_created = false;
	ai_player[0] = ai_player[1] = ai_player[2] = ai_player[3] = nullptr;
	beaten_ai_players = 0;
	node_map_divisions = 15;
}

AI_Manager::~AI_Manager() {

}

bool AI_Manager::Awake(pugi::xml_node& config) {
	bool ret = true;
	pugi::xml_node faction_node = config.first_child();
	Faction faction = NO_FACTION;
	std::string faction_name;

	for (int i = 0; i < 4; i++)
	{
		faction_name = faction_node.name();
		if (faction_name == "vault")
			faction = VAULT;
		else if (faction_name == "brotherhood")
			faction = BROTHERHOOD;
		else if (faction_name == "mutant")
			faction = MUTANT;
		else if (faction_name == "ghoul")
			faction = GHOUL; 

		ai_info[faction].initial_caps = faction_node.attribute("caps").as_int();
		ai_info[faction].initial_water = faction_node.attribute("water").as_int();
		ai_info[faction].initial_food = faction_node.attribute("food").as_int();
		ai_info[faction].minimum_melees = faction_node.attribute("minimum_melees").as_int();
		ai_info[faction].minimum_rangeds = faction_node.attribute("minimum_rangeds").as_int();

		faction_node = faction_node.next_sibling();
	}

	return ret;
}

bool AI_Manager::Start() {
	bool ret = true;	
	node_map = CreateNodeMap();
	show_nodes = App->render->debug;

	for (int i = 0; i < 4; i++)
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

	if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN) {
		show_nodes = !show_nodes;
	}

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

bool AI_Manager::PostUpdate() {
	bool ret = true;
	iPoint node_world_position;
	SDL_Rect node;
	/*
	if ((App->render->debug) && (show_nodes))
	{
		for (int i = 0; i < node_map.size(); i++)
		{
			node_world_position = App->map->MapToWorld(node_map[i].x, node_map[i].y);
			node = { node_world_position.x, node_world_position.y, 6, 6 };
			App->render->DrawQuad(node, 0, 0, 255, 255);
		}
	}
	*/
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

	node_map.clear();
	return ret;
}

std::vector<iPoint> AI_Manager::CreateNodeMap(){
	std::vector<iPoint> map;
	int distance = 150 / node_map_divisions;

	for (int y = distance; y < 150; y += distance)
	{
		for (int x = distance; x < 150; x += distance)
		{
			map.push_back(iPoint(x,y));
		}
	}
	return map;
}

std::vector<iPoint> AI_Manager::GetNodeMap() { return node_map;}

AI_Info AI_Manager::GetAI_PlayerInfo(Faction faction) { return ai_info[faction]; }