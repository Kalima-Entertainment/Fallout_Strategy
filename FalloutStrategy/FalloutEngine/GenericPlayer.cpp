#include "GenericPlayer.h"
#include "j1Entity.h"
#include <vector>

GenericPlayer::GenericPlayer() : j1Module() {


	name.assign("GenericPlayer");


}

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
			if (troops[m] == (Troop*)entity) {
				troops.erase(troops.begin() + m);
			}
		}
		melees--;
		break;
	case RANGED:
		for (int r = 0; r < troops.size(); r++)
		{
			if (troops[r] == (Troop*)entity) {
				troops.erase(troops.begin() + r);
			}
		}
		rangeds--;
		break;
	case GATHERER:
		for (int g = 0; g < gatherers_vector.size(); g++) {
			if (gatherers_vector[g] == (Gatherer*)entity) {
				gatherers_vector.erase(gatherers_vector.begin() + g);
			}
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
	return true;
}

// Load Game State
bool GenericPlayer::Load(pugi::xml_node& data)
{
	//camera.x = data.child("camera").attribute("x").as_int();
	//camera.y = data.child("camera").attribute("y").as_int();

	return true;
}

// Save Game State
bool GenericPlayer::Save(pugi::xml_node& data) const
{
	//pugi::xml_node cam = data.append_child("camera");

	//cam.append_attribute("x") = camera.x;
	//cam.append_attribute("y") = camera.y;

	return true;
}