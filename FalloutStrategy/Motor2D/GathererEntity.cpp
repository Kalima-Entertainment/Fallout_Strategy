#include "GathererEntity.h"

GathererEntity::GathererEntity(Faction type) : DynamicEntity(Troop::GATHERER) {

	//TO DO: Should be rendering each sprite depending faction type ¿?
	switch (type) {
	case Faction::VAULT:
		break;
	case Faction::GHOUL:
		break;
	case Faction::MUTANT:
		break;
	case Faction::BROTHERHOOD:
		break;
	}
}

GathererEntity::~GathererEntity() {}

bool GathererEntity::Update(float dt) {
	return true;
}