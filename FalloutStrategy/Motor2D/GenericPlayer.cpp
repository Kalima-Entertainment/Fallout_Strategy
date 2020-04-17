#include "GenericPlayer.h"
#include "j1Entity.h"

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

