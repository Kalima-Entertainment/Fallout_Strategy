#include "StaticEntity.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "Player.h"
#include "j1Input.h"

StaticEntity::StaticEntity(Faction g_faction, EntityType g_type) {
	type = g_type;
	faction = g_faction;
	state = WAIT;
	storage_capacity = 2000;
}

StaticEntity::~StaticEntity() {}

bool StaticEntity::Update(float dt) {
	switch (state) {
	case WAIT:
		break;
	case WORK:
		break;
	case EXPLODE:
		if (current_animation->Finished())
			to_destroy = true;
		SpatialAudio(App->audio->explode, 1, position.x, position.y);
		break;
	default:
		break;
	}

	//Interact with the building to spawn units or investigate upgrades
	if (this == App->player->selected_entity) {
		if (type == BASE) {
			//Spawn GATHERER
			if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
				App->entities->CreateEntity(faction, GATHERER, spawnPosition.x, spawnPosition.y);
			else if (type == BARRACK) {
				//Spawn MELEE
				if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
					App->entities->CreateEntity(faction, MELEE, spawnPosition.x, spawnPosition.y);
				//Spawn RANGED
				if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
					App->entities->CreateEntity(faction, RANGED, spawnPosition.x, spawnPosition.y);
			}
			else if (type == LABORATORY) {
				//Upgrades
			}
		}
	}

	last_dt = dt;

	return true;
}

bool StaticEntity::PostUpdate() {
	current_animation = &animations[state];

	//Render building
	render_position = {(int)(position.x - 0.5f * sprite_size),(int)(position.y - sprite_size * 0.75)};

	App->render->Blit(reference_entity->texture, render_position.x, render_position.y, &current_animation->GetCurrentFrame(last_dt));

	if (App->render->debug) 
	{ App->render->DrawQuad({ (int)position.x -2, (int)position.y-2, 4,4 }, 255, 0, 0, 255); }
	return true;
}

bool StaticEntity::LoadReferenceData() {
	bool ret = true;
	StaticEntity* static_reference = (StaticEntity*)reference_entity;

	//load animations
	for (int i = 0; i < 3; i++)
	{
		animations[i] = static_reference->animations[i];
	}

	//load property data
	current_health = max_health = reference_entity->max_health;
	sprite_size = reference_entity->sprite_size;
	return ret;
}

bool StaticEntity::LoadAnimations() {
	
	bool ret = true;
	char* faction_char = "NoFaction";
	float speed_multiplier = 0.065f;

	if (faction == VAULT)
		faction_char = "VaultDwellers";
	else if (faction == BROTHERHOOD)
		faction_char = "Brotherhood";
	else if (faction == GHOUL)
		faction_char = "Ghouls";
	else if (faction == MUTANT)
		faction_char = "SuperMutant";

	std::string file = std::string("textures/characters/").append(faction_char).append("/").append(faction_char).append("_Buildings.tmx");

	pugi::xml_document animation_file;
	pugi::xml_parse_result result = animation_file.load_file(file.c_str());
	std::string image = std::string(animation_file.child("tileset").child("image").attribute("source").as_string());
	std::string texture_path = std::string("textures/characters/").append(faction_char).append("/").append(faction_char).append("_Buildings.png");

	if (type == BASE)
	{
		this->texture = App->tex->Load(texture_path.c_str());
	}

	if (result == NULL)
	{
		LOG("Could not load animation tmx file %s. pugi error: %s", file, result.description());
		ret = false;
	}

	int tile_width = animation_file.child("map").attribute("tilewidth").as_int();
	int tile_height = animation_file.child("map").attribute("tileheight").as_int();
	int columns = animation_file.child("map").child("tileset").attribute("columns").as_int();
	int firstgid = animation_file.child("map").child("tileset").attribute("firstgid").as_int();
	sprite_size = tile_height;
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
		std::string building_type = std::string(animation.child("properties").child("property").attribute("name").as_string());
		std::string animation_name = std::string(animation.child("properties").child("property").attribute("value").as_string());
		StaticState state = NO_STATE;
		EntityType entity_type = BASE;
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
			state = WAIT;
		}
		else if (animation_name == "work") {
			state = WORK;
			loop = false;
		}
		else if (animation_name == "die") {
			state = EXPLODE;
			loop = false;
		}
		else goto CHANGE_ANIMATION;

		id = animation.attribute("id").as_int();

		if (type == entity_type)
		{
			while (frame != nullptr) {
				tile_id = frame.attribute("tileid").as_int();
				speed = frame.attribute("duration").as_int() * speed_multiplier;
				rect.x = rect.w * ((tile_id) % columns);
				rect.y = rect.h * ((tile_id) / columns);
				animations[state].PushBack(rect, speed);
				frame = frame.next_sibling();
			}
			animations[state].loop = loop;
		}

	CHANGE_ANIMATION: animation = animation.next_sibling();
		frame = animation.child("animation").child("frame");
	}

	return ret;
}
