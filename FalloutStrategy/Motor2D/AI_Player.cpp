#include "AI_Player.h"
#include "j1Entity.h"

AI_Player::AI_Player(Faction g_faction) : GenericPlayer() {
	faction = g_faction;
}

AI_Player::~AI_Player() {
	
}

bool AI_Player::Update(float dt) {
	bool ret = true;

	return ret;
}