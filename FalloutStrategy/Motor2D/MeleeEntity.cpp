#include "MeleeEntity.h"

MeleeEntity::MeleeEntity(Faction type) : DynamicEntity(Troop::MELEE) {
	
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

MeleeEntity::~MeleeEntity() {}

bool MeleeEntity::Update(float dt) {
	return true;
}