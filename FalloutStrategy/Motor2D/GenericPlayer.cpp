#include "GenericPlayer.h"
#include "j1Entity.h"
#include <vector>

GenericPlayer::GenericPlayer() : j1Module() {}

GenericPlayer::~GenericPlayer() {}

int GenericPlayer::GetTroopsAmount() const {
	return troops.size();
}

bool GenericPlayer::DeleteEntity(j1Entity* entity) {

	switch (entity->type)
	{
	case MELEE:
		for (int m = 0; m < troops.size(); m++)
		{
			if (troops[m] == (DynamicEntity*)entity) {
				troops.erase(troops.begin() + m);
			}
		}
		melees--;
		break;
	case RANGED:
		for (int r = 0; r < troops.size(); r++)
		{
			if (troops[r] == (DynamicEntity*)entity) {
				troops.erase(troops.begin() + r);
			}
		}
		rangeds--;
		break;
	case GATHERER:
		for (int g = 0; g < gatherers_vector.size(); g++) {
			gatherers_vector.erase(gatherers_vector.begin() + g);
		}
		gatherers--;
		break;
	case BASE:
		base = nullptr;
		break;
	case BARRACK:
		if (barrack[0] == (StaticEntity*)entity) barrack[0] = nullptr;
		else if (barrack[1] == (StaticEntity*)entity) barrack[1] = nullptr;
		break;
	case LABORATORY:
		laboratory = nullptr;
		break;
	default:
		break;
	}

	if (base == nullptr) {
		entity->owner->target_player = nullptr;
	}

	return ((base == nullptr) && (laboratory == nullptr) && (barrack[0] == nullptr) && (barrack[1] == nullptr));
}