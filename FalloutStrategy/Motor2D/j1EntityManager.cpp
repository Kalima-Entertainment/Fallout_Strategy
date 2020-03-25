#include "j1EntityManager.h"
#include "j1App.h"
#include "p2Log.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Collision.h"
#include "j1Window.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Audio.h"
#include "DynamicEntity.h"
#include "StaticEntity.h"
#include "Player.h"
#include "brofiler/Brofiler/Brofiler.h"


j1EntityManager::j1EntityManager(){
	name.create("entities");

	selected_unit_tex = nullptr;
	blocked_movement = false;
	total_entities = 0;
}

j1EntityManager::~j1EntityManager(){}

j1Entity* j1EntityManager::CreateEntity(Faction faction, EntityType type, int position_x, int position_y){
	//BROFILER_CATEGORY("EntityCreation", Profiler::Color::Linen)
	//static_assert(EntityType::UNKNOWN == 4, "code needs update");

	j1Entity* entity = nullptr;

	if ((type == MELEE) || (type == RANGED) || (type == GATHERER)) {
		entity = new DynamicEntity(faction, type);
		entity->is_dynamic = true;

		entity->reference_entity = reference_entities[faction][type];

		if (entity != NULL)
		{
			entity->faction = faction;
			entity->current_tile.x = position_x;
			entity->current_tile.y = position_y;

			entity->position = App->map->fMapToWorld(entity->current_tile.x, entity->current_tile.y);
			entity->position.x += 32;
			entity->position.y += 32;

			if (entity->reference_entity != nullptr){
				entities.push_back(entity);
				total_entities++;
				entity->LoadReferenceData();
			}
		}
	}
	else if ((type == BASE) || (type == LABORATORY) || (type == BARRACK))
	{
		entity = new StaticEntity(faction, type);
		entity->is_dynamic = false;
		entity->reference_entity = reference_entities[faction][type];

		if (entity != NULL)
		{
			entity->faction = faction;
			entity->current_tile.x = position_x;
			entity->current_tile.y = position_y;

			entity->position = App->map->fMapToWorld(entity->current_tile.x, entity->current_tile.y);

			if (entity->reference_entity != nullptr) {
				entities.push_back(entity);
				total_entities++;
				entity->LoadReferenceData();
			}
		}
	}

	return entity;
}

bool j1EntityManager::Awake(pugi::xml_node& config){
	bool ret = true;

	config_data = config;

	//automatic entities loading
	for (int faction = VAULT; faction < NO_FACTION; faction++)
	{
		for (int type = MELEE; type < NO_TYPE; type++)
		{
			reference_entities[faction][type] = CreateEntity((Faction)faction, (EntityType)type, faction, type);
		}
	}

	ret = LoadReferenceEntityData();

	return ret;
}

bool j1EntityManager::Start()
{
	bool ret = true;
	//create reference entities

	//load all textures

	for (int faction = VAULT; faction < NO_FACTION; faction++)
	{
		for (int type = MELEE; type < BASE; type++)
		{
			LoadAnimations(reference_entities[faction][type]);
		}
	}

	//Vault Dwellers
	reference_entities[VAULT][BASE]->LoadAnimations("VaultDwellers/Vault_Dweller_Base");
	reference_entities[VAULT][BARRACK]->texture = reference_entities[VAULT][BASE]->texture;
	reference_entities[VAULT][LABORATORY]->texture = reference_entities[VAULT][BASE]->texture;

	//Brotherhood

	reference_entities[BROTHERHOOD][BASE]->LoadAnimations("Brotherhood/Brotherhood_Buildings");
	reference_entities[BROTHERHOOD][BARRACK]->texture = reference_entities[BROTHERHOOD][BASE]->texture;
	reference_entities[BROTHERHOOD][LABORATORY]->texture = reference_entities[BROTHERHOOD][BASE]->texture;

	//Super Mutants);
	reference_entities[MUTANT][BASE]->LoadAnimations("SuperMutant/SuperMutant_Buildings");
	reference_entities[MUTANT][BARRACK]->texture = reference_entities[MUTANT][BASE]->texture;
	reference_entities[MUTANT][LABORATORY]->texture = reference_entities[MUTANT][BASE]->texture;

	//Ghouls
	//reference_entities[GHOUL][BASE]->LoadAnimations("Ghouls/Ghouls_Base");
	reference_entities[GHOUL][BASE]->LoadAnimations("Ghouls/Ghouls_Buildings");
	reference_entities[GHOUL][BARRACK]->LoadAnimations("Ghouls/Ghouls_Buildings");
	reference_entities[GHOUL][BARRACK]->texture = reference_entities[GHOUL][BASE]->texture;

	return ret;
}

bool j1EntityManager::CleanUp()
{
	bool ret = true;
	/*
	for (int i = 0; i < REFERENCE_ENTITIES; i++)
	{
		if (i < REFERENCE_ENTITIES)
		{
			App->tex->UnLoad(reference_entities[i]->texture);
		}
		delete entities[i];
	}
	*/
	entities.clear();
	return ret;
}

bool j1EntityManager::PreUpdate()
{
	//BROFILER_CATEGORY("EntitiesPreUpdate", Profiler::Color::Bisque)

	bool ret = true;
	return ret;
}

bool j1EntityManager::Update(float dt)
{
	BROFILER_CATEGORY("EntitiesUpdate", Profiler::Color::GreenYellow)
	bool ret = true;

	for (int i = 0; i < entities.size(); i++)
	{
		entities[i]->Update(dt);
	}
	return ret;
}

bool j1EntityManager::PostUpdate()
{
	BROFILER_CATEGORY("EntitiesPostUpdate", Profiler::Color::CadetBlue)
	bool ret = true;
	SDL_Rect tex_rect = {64,0,64,64 };
	iPoint tex_position;

	if (App->render->debug) {
		for (int i = 0; i < entities.size(); i++)
		{
			if (entities[i]->is_dynamic)
			{
				SDL_Rect rect = { 0,0,64,64 };
				tex_position = App->map->MapToWorld(entities[i]->current_tile.x, entities[i]->current_tile.y);
				App->render->Blit(App->render->debug_tex, tex_position.x, tex_position.y, &rect);
			}

		}
	}

	if (App->player->selected_entity != nullptr)
	{
		//Selected entity is a unit
		if (App->player->selected_entity->is_dynamic == true) {
			tex_position = App->map->MapToWorld(App->player->selected_entity->current_tile.x, App->player->selected_entity->current_tile.y);
			App->render->Blit(App->render->debug_tex, tex_position.x, tex_position.y, &tex_rect);
		}
		//Selected entity is a building
		else { 
			for (int i = 0; i < 9; i++) {
				tex_position = App->map->MapToWorld(App->player->selected_entity->positions[i].x, App->player->selected_entity->positions[i].y);
				App->render->Blit(App->render->debug_tex, App->player->selected_entity->positions[i].x, App->player->selected_entity->positions[i].y, &tex_rect);
			}
		}
	}

	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i]->to_destroy)
		{
			entities.erase(entities.begin() + i);
		}
		else
		{
			SortEntities();
			entities[i]->PostUpdate();
		}
	}
	return ret;
}

bool j1EntityManager::LoadReferenceEntityData() {
	bool ret = true;
	pugi::xml_document entities_file;
	pugi::xml_node entities_node;
	pugi::xml_parse_result result = entities_file.load_file("entities.xml");

	if (result == NULL)
		LOG("Could not load map xml file entities.xml. pugi error: %s", result.description());
	else
		 entities_node = entities_file.child("entities");

	pugi::xml_node dynamic_node = entities_node.child("dynamic");
	pugi::xml_node faction_node;
	Faction faction = NO_FACTION;
	EntityType type = NO_TYPE;

	//load dynamic entities
	faction_node = dynamic_node.first_child();
	while (faction_node != nullptr)
	{
		p2SString faction_string(faction_node.name());

		//check faction
		if (faction_string == "vault")
			faction = VAULT;
		else if (faction_string == "brotherhood")
			faction = BROTHERHOOD;
		else if (faction_string == "mutants")
			faction = MUTANT;
		else if (faction_string == "ghouls")
			faction = GHOUL;

		pugi::xml_node entity_node = faction_node.first_child();
		while (entity_node != nullptr)
		{
			p2SString type_string(entity_node.name());

			//check type
			if (type_string == "melee")
				type = MELEE;
			else if (type_string == "ranged")
				type = RANGED;
			else if (type_string == "gatherer")
				type = GATHERER;

			//load attributes
			int health = entity_node.attribute("health").as_int();
			int damage = entity_node.attribute("damage").as_int();
			int speed = entity_node.attribute("speed").as_int();

			//load into reference entities
			reference_entities[faction][type]->max_health = health;
			reference_entities[faction][type]->damage = damage;
			reference_entities[faction][type]->speed.x = speed * 0.01;
			reference_entities[faction][type]->speed.y = speed * 0.005f;

			entity_node = entity_node.next_sibling();
		}
		faction_node = faction_node.next_sibling();
	}

	return ret;
}

void j1EntityManager::SortEntities() {
	int i, j;
	int n = entities.size();

	for (i = 0; i < n - 1; i++)
		for (j = 0; j < n - i - 1; j++)
			if (entities[j]->GetPositionScore() > entities[j + 1]->GetPositionScore())
				Swap(j, j + 1);
}

void j1EntityManager::Swap(int i, int j)
{
	int temp = i;
	j1Entity* aux = entities[i];
	entities[i] = entities[j];
	entities[j] = aux;
}

j1Entity* j1EntityManager::FindEntityByTile(iPoint tile) {
	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i]->current_tile == tile)
		{
			return entities[i];
		}
	}
	return nullptr;
}

j1Entity* j1EntityManager::FindBuildingByTile(iPoint tile) {
	for (int i = 0; i < entities.size(); i++)
	{
		if ((entities[i]->positions[i].x == tile.x) && (entities[i]->positions[i].y == tile.y))
		{
			return entities[i];
		}
	}
	return nullptr;
}

void j1EntityManager::DestroyEntity(j1Entity* entity) {
	delete entity;
	total_entities--;
}

void j1EntityManager::DestroyAllEntities() {
	for (int i = REFERENCE_ENTITIES; i < total_entities; i++)
	{
		entities[i]->to_destroy = true;
	}
}

bool j1EntityManager::LoadAnimations(j1Entity* entity) {
	bool ret = true;
	char* faction = {"NoFaction"};
	char* type = {"NoType"};

	//entity faction
	if (entity->faction == VAULT)
		faction = "VaultDwellers";
	else if (entity->faction == BROTHERHOOD)
		faction = "Brotherhood";
	else if (entity->faction == MUTANT)
		faction = "SuperMutant";
	else if (entity->faction == GHOUL)
		faction = "Ghouls";

	//entity type
	if (entity->type == MELEE)
		type = "Melee";
	else if (entity->type == RANGED)
		type = "Ranged";
	if (entity->type == GATHERER)
		type = "Gatherer";

	p2SString file("textures/characters/%s/%s_%s.tmx", faction,faction,type);
	p2SString texture_path("textures/characters/%s/%s_%s.png", faction, faction, type);

	pugi::xml_document animation_file;
	pugi::xml_parse_result result = animation_file.load_file(file.GetString());
	p2SString image(animation_file.child("tileset").child("image").attribute("source").as_string());
	entity->texture = App->tex->Load(texture_path.GetString());

	if (result == NULL)
	{
		LOG("Could not load animation tmx file %s. pugi error: %s", file, result.description());
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
		p2SString animation_direction(animation.child("properties").child("property").attribute("value").as_string());
		p2SString animation_name(animation.child("properties").child("property").attribute("name").as_string());
		int direction = TOP_RIGHT;
		State state = IDLE;
		bool loop = true;

		//animation
		if (animation_name == "idle") {
			state = IDLE;
		}
		else if (animation_name == "walk") {
			state = WALK;
		}
		else if (animation_name == "attack") {
			state = ATTACK;
		}
		else if (animation_name == "gather") {
			state = GATHER;
		}
		else if (animation_name == "hit") {
			state = HIT;
			loop = false;
		}
		else if (animation_name == "die") {
			state = DIE;
			loop = false;
		}

		//animation direction
		if (animation_direction == "top_left")
			direction = TOP_LEFT;
		else if (animation_direction == "top_right")
			direction = TOP_RIGHT;
		else if (animation_direction == "left")
			direction = LEFT;
		else if (animation_direction == "right")
			direction = RIGHT;
		else if (animation_direction == "bottom_left")
			direction = BOTTOM_LEFT;
		else if (animation_direction == "bottom_right")
			direction = BOTTOM_RIGHT;

		id = animation.attribute("id").as_int();

		while (frame != nullptr) {
			tile_id = frame.attribute("tileid").as_int();
			speed = frame.attribute("duration").as_int() * 0.001f;
			rect.x = rect.w * ((tile_id) % columns);
			rect.y = rect.h * ((tile_id) / columns);
			entity->animations[state][direction].PushBack(rect, speed);
			frame = frame.next_sibling();
		}
		entity->animations[state][direction].loop = loop;

	    animation = animation.next_sibling();
		frame = animation.child("animation").child("frame");
	}

	return ret;
}
