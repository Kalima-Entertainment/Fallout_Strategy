#include "DynamicEntity.h"
#include "j1Map.h"
#include "j1App.h"
#include "j1Scene.h"
#include "j1Render.h"
#include "j1Pathfinding.h"
#include "j1Textures.h"
#include "j1EntityManager.h"
#include "Player.h"

DynamicEntity::DynamicEntity(Faction g_faction, EntityType g_type) {

	switch (g_type)
	{
	case MELEE:
		range = 1;
		break;
	case RANGED:
		range = 3;
		break;
	case GATHERER:
		range = 1;
		break;
	default:
		break;
	}

	type = g_type;
	faction = g_faction;
	state = IDLE;
	direction = TOP_RIGHT;

	current_speed = { 0, 0 };
	target_entity = nullptr;
	resource_building = nullptr;
	action_time = 3.0f;
	resource_collected = 0;
}

DynamicEntity::~DynamicEntity() {}

bool DynamicEntity::Update(float dt) {

	switch (state)
	{
	case IDLE:
		break;
	case WALK:
		Move();
		break;
	case ATTACK:
		if (timer.ReadSec() > action_time)
		{
			if (current_tile.DistanceManhattan(target_entity->current_tile) <= range)
			{
				Attack();
			}
		}
		break;
	case GATHER:
		if (timer.ReadSec() > action_time)
		{
			Gather();
			state = IDLE;
		}
		break;
	case HIT:
		if (current_animation->Finished())
		{
			state = IDLE;
			current_animation->Reset();
		}
		break;
	case DIE:
		if (current_animation->Finished())
		{
			to_destroy = true;
		}
		break;
	default:
		break;
	}
	return true;
}

bool DynamicEntity::PostUpdate() {

	current_animation = &animations[state][direction];

	//render path
	if (App->render->debug)
	{
		if (path_to_target != NULL)
		{
			for (uint i = 0; i < path_to_target->Count(); ++i)
			{
				iPoint pos = App->map->MapToWorld(path_to_target->At(i)->x, path_to_target->At(i)->y);
				SDL_Rect debug_rect = { 192, 0, 64,64 };
				App->render->Blit(App->render->debug_tex, pos.x, pos.y, &debug_rect);
			}
		}
	}

	//render character
	iPoint render_position;
	render_position = App->map->MapToWorld(current_tile.x, current_tile.y);
	App->render->Blit(reference_entity->texture, position.x - TILE_SIZE, position.y - 1.82f * TILE_SIZE, &current_animation->GetCurrentFrame());

	//health bar
	SDL_Rect background_bar = { position.x - HALF_TILE * 0.75f, position.y - TILE_SIZE * 1.5f, 50, 4 };
	SDL_Rect foreground_bar = { position.x - HALF_TILE * 0.75f, position.y - TILE_SIZE * 1.5f, (float)current_health/max_health * 50, 4 };
	if (foreground_bar.w < 0)
		foreground_bar.w = 0;

	App->render->DrawQuad(background_bar, 255, 255, 255, 255);
	App->render->DrawQuad(foreground_bar, 0, 255, 0, 255);

	//render position
	if (App->render->debug)
	{
		App->render->DrawQuad({ (int)position.x - 2, (int)position.y - 2, 4,4 }, 255, 0, 0, 255);
		App->render->DrawQuad(next_tile_rect_center, 0, 255, 0, 255);
	}

	return true;
}

bool DynamicEntity::LoadReferenceData() {
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
	damage = reference_entity->damage;
	speed = reference_entity->speed;

	return ret;
}

void DynamicEntity::Move() {
	if (path_to_target != NULL)
	{
		if (path_to_target->Count() != 0)
		{
			//get next tile center
			next_tile_position = App->map->MapToWorld(next_tile.x, next_tile.y);
			next_tile_rect_center = { next_tile_position.x + HALF_TILE - 2, next_tile_position.y + HALF_TILE,4,4 };

			//check what's on next tile and act
			if (current_tile.LinealDistance(target_tile) <= range) {
				if (target_entity != nullptr) {
				//we reach the destination and there is an entity in it
					if ((faction != target_entity->faction) && (type != GATHERER))
					{
						state = ATTACK;
						Attack();
					}
					
					else if (next_tile == target_tile)
					{
						iPoint current_tile_center = App->map->MapToWorld(current_tile.x, current_tile.y);
						position.x = current_tile_center.x + HALF_TILE;
						position.y = current_tile_center.y + HALF_TILE;
						state = IDLE;
					}
					
				}
				else
				{
					if (type == GATHERER) {
						if ((resource_building != nullptr) && (resource_collected < storage_capacity)) {
							state = GATHER;
							timer.Start();
						}
					}
					else
					{
						if (current_tile == target_tile)
						{
						iPoint current_tile_center = App->map->MapToWorld(current_tile.x, current_tile.y);
						position.x = current_tile_center.x + HALF_TILE;
						position.y = current_tile_center.y + HALF_TILE;
						state = IDLE;
						}
					}
				}
			}

			//move to next tile
			if ((position.x > next_tile_rect_center.x + next_tile_rect_center.w) && (position.x > next_tile_rect_center.x) && (position.y > next_tile_rect_center.y) && (position.y > next_tile_rect_center.y + next_tile_rect_center.h)) {
				direction = TOP_LEFT;
				position.x -= speed.x;
				position.y -= speed.y;
			}
			else if ((position.x < next_tile_rect_center.x) && (position.x < next_tile_rect_center.x + next_tile_rect_center.w) && (position.y > next_tile_rect_center.y) && (position.y > next_tile_rect_center.y + next_tile_rect_center.h)) {
				direction = TOP_RIGHT;
				position.x += speed.x;
				position.y -= speed.y;
			}
			else if ((position.x > next_tile_rect_center.x) && (position.x > next_tile_rect_center.x + next_tile_rect_center.w) && (position.y < next_tile_rect_center.y) && (position.y < next_tile_rect_center.y + next_tile_rect_center.h)) {
				direction = BOTTOM_LEFT;
				position.x -= speed.x;
				position.y += speed.y;
			}
			else if ((position.x < next_tile_rect_center.x) && (position.x < next_tile_rect_center.x + next_tile_rect_center.w) && (position.y < next_tile_rect_center.y) && (position.y < next_tile_rect_center.y + next_tile_rect_center.h)) {
				direction = BOTTOM_RIGHT;
				position.x += speed.x;
				position.y += speed.y;
			}
			else
			{
				if (*path_to_target->At(0) != target_tile)
				{
					current_tile = *path_to_target->At(0);
					if (path_to_target->Count() > 1)
					{
						next_tile = *path_to_target->At(1);
					}
					path_to_target->Advance();
					
				}
				else
				{
					position.x = next_tile_rect_center.x + 2;
					position.y = next_tile_rect_center.y + 2;
					current_tile = target_tile;
					state = IDLE;
				}
			}
		}
		else
		{
			state = IDLE;
			target_tile = current_tile;
			current_tile = target_tile;
		}
	}
}

void DynamicEntity::Attack() {

	timer.Start();
	target_entity->current_health -= damage;
	target_entity->state = HIT;
	target_entity->current_animation->Reset();

	switch (direction)
	{
	case TOP_LEFT:
		target_entity->direction = BOTTOM_RIGHT;
		break;
	case TOP_RIGHT:
		target_entity->direction = BOTTOM_LEFT;
		break;
	case RIGHT:
		target_entity->direction = LEFT;
		break;
	case BOTTOM_RIGHT:
		target_entity->direction = TOP_LEFT;
		break;
	case BOTTOM_LEFT:
		target_entity->direction = TOP_RIGHT;
		break;
	case LEFT:
		target_entity->direction = RIGHT;
		break;
	default:
		break;
	}

	if (target_entity->current_health <= 0) { 
		target_entity->state = DIE;
		target_entity->direction = TOP_LEFT;
		target_entity = nullptr;
		state = IDLE;
	}
}

void DynamicEntity::PathfindToPosition(iPoint destination) {

	current_tile = App->map->WorldToMap(position.x, position.y);
	App->pathfinding->CreatePath(current_tile, destination);

	//pathfinding debug
	int x, y;
	SDL_Rect Debug_rect = { 0,0,32,32 };

	path_to_target = (p2DynArray<iPoint>*)App->pathfinding->GetLastPath();
	if (path_to_target->Count() != 0)
	{
		next_tile = *path_to_target->At(0);
	}

	for (uint i = 0; i < path_to_target->Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path_to_target->At(i)->x, path_to_target->At(i)->y);
		Debug_rect.x = pos.x;
		Debug_rect.y = pos.y;
		if (App->render->debug)App->render->DrawQuad(Debug_rect, 90, 850, 230, 40);
	}
}

bool DynamicEntity::LoadAnimations(const char* path) {
	bool ret = true;

	p2SString file("textures/characters/%s.tmx", path);

	pugi::xml_document animation_file;
	pugi::xml_parse_result result = animation_file.load_file(file.GetString());
	p2SString image(animation_file.child("tileset").child("image").attribute("source").as_string());
	p2SString texture_path("textures/characters/%s.png", path);
	this->texture = App->tex->Load(texture_path.GetString());

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
		else goto CHANGE_ANIMATION;

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
			animations[state][direction].PushBack(rect, speed);
			frame = frame.next_sibling();
		}
		animations[state][direction].loop = loop;

	CHANGE_ANIMATION: animation = animation.next_sibling();
		frame = animation.child("animation").child("frame");
	}

	return ret;
}

void DynamicEntity::Gather() {
	resource_building->quantity -= damage;
	resource_collected += damage;
	resource_type = resource_building->resource_type; 
}