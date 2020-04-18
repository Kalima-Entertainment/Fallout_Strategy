#include "GenericPlayer.h"
#include "j1Entity.h"
#include <vector>

GenericPlayer::GenericPlayer() : j1Module() {}

GenericPlayer::~GenericPlayer() {}

int GenericPlayer::GetTroopsAmount() const {
	int troops = 0;
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->is_dynamic) {
			troops++;
		}
	}
	return troops;
}

bool GenericPlayer::DeleteEntity(j1Entity* entity) {
	for (int i = 0; i < entities.size(); i++)
	{
		if (entity->type == MELEE) melees--;
		else if (entity->type == RANGED) rangeds--;
		else if (entity->type == GATHERER) {
			for (int g = 0; g < gatherers; g++) {
				gatherers_vector.erase(gatherers_vector.begin() + g);
			}
			gatherers--;
		}
		else if (entity->type == BASE) base = nullptr;
		else if (entity->type == LABORATORY) laboratory = nullptr;
		else if (entity->type == BARRACK) {
			if (barrack[0] == (StaticEntity*)entity) barrack[0] = nullptr;
			else if (barrack[1] == (StaticEntity*)entity) barrack[1] = nullptr;
		}

		if (entities[i] == entity) {
			entities[i] = nullptr;
			entities.erase(entities.begin() + i);
		}
	}

	return ((base == nullptr) && (laboratory == nullptr) && (barrack[0] == nullptr) && (barrack[1] == nullptr));
}