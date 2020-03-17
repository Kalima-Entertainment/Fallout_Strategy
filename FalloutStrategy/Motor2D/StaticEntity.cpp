#include "StaticEntity.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Map.h"

StaticEntity::StaticEntity(Faction g_faction, EntityType g_type) {
	switch (type)
	{
	case BASE:
		break;
	case LABORATORY:
		break;
	case BARRACK:
		break;
	default:
		break;
	}

	type = g_type;
	faction = g_faction;
	state = IDLE;
}

StaticEntity::~StaticEntity() {}

bool StaticEntity::Update(float dt) {
	switch (state) {
	case IDLE:
		break;
	case DIE:
		if (current_animation->Finished())
			to_destroy = true;
		break;
	default:
		break;
	}

	return true;
}

bool StaticEntity::PostUpdate() {
	current_animation = &animations[state][direction];

	//Render building
	iPoint render_position;
	render_position = App->map->MapToWorld(current_tile.x, current_tile.y);
	App->render->Blit(reference_entity->texture, position.x - TILE_SIZE, position.y - 2 * TILE_SIZE, &current_animation->GetCurrentFrame());

	return true;
}

bool StaticEntity::LoadReferenceData() {
	bool ret = true;

	//load animations
	for (int i = 0; i < MAX_ANIMATIONS; i++)
	{
		for (int j = 0; j <= 6; j++)
		{
			animations[i][j] = reference_entity->animations[i][j];
		}
	}

	//load property data
	current_health = max_health = reference_entity->max_health;

	return ret;
}