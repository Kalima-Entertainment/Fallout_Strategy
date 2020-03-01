#include "j1Entity.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1EntityManager.h"
#include "j1Pathfinding.h"
#include "j1Map.h"
#include "p2Log.h"
#include "j1Collision.h"


j1Entity::~j1Entity() {}

void j1Entity::PathfindToPosition(iPoint destination) {

	//if the player is close we create a path to him
	iPoint origin = App->map->WorldToMap(position.x, position.y);
	App->pathfinding->CreatePath(current_tile, destination);

	//pathfinding debug
	int x, y;
	SDL_Rect Debug_rect = { 0,0,32,32 };

	path_to_target = App->pathfinding->GetLastPath();

	for (uint i = 0; i < path_to_target->Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path_to_target->At(i)->x, path_to_target->At(i)->y);
		Debug_rect.x = pos.x;
		Debug_rect.y = pos.y;
		if (App->collision->debug)App->render->DrawQuad(Debug_rect, 90, 850, 230, 40);
	}
}

// to be updated
bool j1Entity::LoadAnimations(const char* path) {
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
		p2SString animation_name(animation.child("properties").child("property").attribute("name").as_string());
			if (animation_name == "idle")
				animations.push_back(&idle[0]);
			else if (animation_name == "walk")
				animations.push_back(&walk[0]);
			else if (animation_name == "attack")
				animations.push_back(&attack[0]);
			else if (animation_name == "gather")
				animations.push_back(&gather[0]);
			else if (animation_name == "hit")
				animations.push_back(&hit[0]);
			else if (animation_name == "die")
				animations.push_back(&die[0]);
			else goto CHANGE_ANIMATION;

			id = animation.attribute("id").as_int();

			while (frame != nullptr) {
				tile_id = frame.attribute("tileid").as_int();
				speed = frame.attribute("duration").as_int() * 0.001f;
				rect.x = rect.w * ((tile_id) % columns);
				rect.y = rect.h * ((tile_id) / columns);
				animations[i]->PushBack(rect, speed);
				frame = frame.next_sibling();
			}

	CHANGE_ANIMATION: animation = animation.next_sibling();
		frame = animation.child("animation").child("frame");
	}

	return ret;
}

bool j1Entity::PostUpdate() {
	App->render->Blit(reference_entity->texture, position.x, position.y, &current_animation->GetCurrentFrame());
	//App->render->DrawQuad({ position.x, position.y, current_animation->GetCurrentFrame().w, current_animation->GetCurrentFrame().h }, 10, 150, 10, 80);
	
	return true;
}

iPoint j1Entity::MapPosition() {
	iPoint spot = App->render->ScreenToWorld(position.x, position.y);
	spot = App->map->WorldToMap(spot.x, spot.y);
	return spot;
}
