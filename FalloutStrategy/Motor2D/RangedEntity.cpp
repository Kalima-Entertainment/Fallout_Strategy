#include "RangedEntity.h"

RangedEntity::RangedEntity(Faction type) : DynamicEntity(Troop::RANGED){

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

RangedEntity::~RangedEntity() {}

bool RangedEntity::Update(float dt) {
	return true;
}