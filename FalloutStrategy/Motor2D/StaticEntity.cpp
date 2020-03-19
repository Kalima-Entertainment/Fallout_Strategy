#include "StaticEntity.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Textures.h"

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

	target_entity = nullptr;
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

bool StaticEntity::LoadAnimations(const char* path) {
	bool ret = true;

	p2SString file("textures/characters/%s.tmx", path);

	pugi::xml_document animation_file;
	pugi::xml_parse_result result = animation_file.load_file(file.GetString());
	p2SString image(animation_file.child("tileset").child("image").attribute("source").as_string());
	p2SString texture_path("textures/characters/%s.png", path);

	if (type == BASE)
	{
		this->texture = App->tex->Load(texture_path.GetString());
	}
	
	if (result == NULL)
	{
		LOG("Could not load animation tmx file %s. pugi error: %s", path, result.description());
		ret = false;
	}

	int tile_width = animation_file.child("map").child("tileset").attribute("tilewidth").as_int();
	int tile_height = animation_file.child("map").child("tileset").attribute("tileheight").as_int();
	int columns = animation_file.child("map").child("tileset").attribute("columns").as_int();
	int firstgid = animation_file.child("map").child("tileset").attribute("firstgid").as_int();
	int id, tile_id;
	float speed;

	pugi::xml_node animation = animation_file.child("map").child("tileset").child("tile");
	pugi::xml_node frame = animation.child("animation").child("frame");

	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	int i = 0;

	while (animation != nullptr)
	{
		p2SString building_type(animation.child("properties").child("property").attribute("name").as_string());
		p2SString animation_name(animation.child("properties").child("property").attribute("value").as_string());
		int direction = TOP_RIGHT;
		State state = IDLE;
		EntityType entity_type;
		bool loop = true;

		//building type
		if (building_type == "base")
			entity_type = BASE;
		else if (building_type == "barrack")
			entity_type = BARRACK;
		else if (building_type == "laboratory")
			entity_type = LABORATORY;

		//animation
		if (animation_name == "idle") {
			state = IDLE;
		}
		else if (animation_name == "die") {
			state = DIE;
			loop = false;
		}
		else goto CHANGE_ANIMATION;

		id = animation.attribute("id").as_int();

		if (type == entity_type)
		{
			while (frame != nullptr) {
				tile_id = frame.attribute("tileid").as_int();
				speed = frame.attribute("duration").as_int() * 0.001f;
				rect.x = rect.w * ((tile_id) % columns);
				rect.y = rect.h * ((tile_id) / columns);
				animations[state][TOP_RIGHT].PushBack(rect, speed);
				frame = frame.next_sibling();
			}
			animations[state][TOP_RIGHT].loop = loop;
		}

	CHANGE_ANIMATION: animation = animation.next_sibling();
		frame = animation.child("animation").child("frame");
	}

	return ret;
}