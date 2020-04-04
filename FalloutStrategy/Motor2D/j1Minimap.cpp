#include "j1Minimap.h"
#include "j1Map.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "j1Window.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1EntityManager.h"
#include "j1Entity.h"
#include "MenuManager.h"

j1Minimap::j1Minimap() : j1Module() {
	name = ("minimap");

	texture = nullptr;
	map_height = 200;
	scale = 1;
	width = 100;
	margin = 0;
	corner = Corner::TOP_LEFT;
}

j1Minimap::~j1Minimap() {
}

bool j1Minimap::Awake(pugi::xml_node& config) {
	uint window_width, window_height;

	width = config.attribute("width").as_int();

	//corner
	std::string corner_string = std::string(config.attribute("corner").as_string());
	margin = config.attribute("margin").as_int();

	if (corner_string == "top_left") {
		corner = Corner::TOP_LEFT;
	}
	else if (corner_string == "top_right") {
		corner = Corner::TOP_RIGHT;
	}
	else if (corner_string == "bottom_left") {
		corner = Corner::BOTTOM_LEFT;
	}
	else if (corner_string == "bottom_right") {
		corner = Corner::BOTTOM_RIGHT;
	}

	return true;
}

bool j1Minimap::Start() {
	bool ret = true;
	uint window_width, window_height;
	App->win->GetWindowSize(window_width, window_height);

	map_width = App->map->data.tile_width * App->map->data.width;
	map_height = App->map->data.tile_height * App->map->data.height;
	scale = ((width) / ((float)map_width));
	height = (map_height) * scale;

	texture = SDL_CreateTexture(App->render->renderer, SDL_GetWindowPixelFormat(App->win->window), SDL_TEXTUREACCESS_TARGET, width, height);

	SDL_SetRenderTarget(App->render->renderer, texture);
	CreateMinimap();
	SDL_SetRenderTarget(App->render->renderer, NULL);

	switch (corner)
	{
	case Corner::TOP_LEFT:
		position.x = margin;
		position.y = margin;
		break;
	case Corner::TOP_RIGHT:
		position.x = window_width - width - margin;
		position.y = margin;
		break;
	case Corner::BOTTOM_LEFT:
		position.x = margin;
		position.y = window_height - height - margin;
		break;
	case Corner::BOTTOM_RIGHT:
		position.x = window_width - width - margin;
		position.y = window_height - height - margin;
		break;
	}

	return ret;
}

bool j1Minimap::PostUpdate() {

	if ((App->menu_manager->current_menu == Menu::NO_MENU)||(App->menu_manager->current_menu == Menu::PAUSE_MENU)) {

		App->render->Blit(texture, position.x, position.y, NULL, 1.0, 0);

		for (int i = 0; i < App->entities->entities.size(); i++)
		{
			SDL_Rect entity_rect = {0,0,3,3};
			iPoint entity_position = App->minimap->WorldToMinimap(App->entities->entities[i]->position.x, App->entities->entities[i]->position.y);
			entity_rect.x = entity_position.x;
			entity_rect.y = entity_position.y;
			App->render->DrawQuad(entity_rect, 0, 255, 0, 255, true, false);
		}

		SDL_Rect rect = { 0,0,0,0 };
		iPoint rect_position = WorldToMinimap(-App->render->camera.x, -App->render->camera.y);
		App->render->DrawQuad({ rect_position.x, rect_position.y, (int)(App->render->camera.w * scale),(int)(App->render->camera.h * scale) }, 255, 255, 255, 255, false, false);

	}

	return true;
}

bool j1Minimap::CreateMinimap() {

	PERF_START(ptimer);
	p2List_item<MapLayer*>* item = App->map->data.layers.start;

	for (; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		if (layer->properties.Get("Nodraw") != 0)
			continue;

		int half_width = map_width * 0.5f;

		for (int y = 0; y < App->map->data.height; ++y)
		{
			for (int x = 0; x < App->map->data.width; ++x)
			{
				int tile_id = layer->Get(x, y);
				if (tile_id > 0)
				{
					TileSet* tileset = App->map->GetTilesetFromTileId(tile_id);

					SDL_Rect r = tileset->GetTileRect(tile_id);
					iPoint pos = App->map->MapToWorld(x, y);
					pos = App->render->WorldToScreen(pos.x, pos.y);

					App->render->Blit(tileset->texture, pos.x + half_width + tileset->offset_x, pos.y + tileset->offset_y, &r, scale);
				}
			}
		}
	}
	PERF_PEEK(ptimer);

	return true;
}

iPoint j1Minimap::WorldToMinimap(int x, int y) {
	iPoint minimap_position;
	minimap_position.x = position.x + width * 0.5f + x * scale;
	minimap_position.y = position.y + y * scale;

	return minimap_position;
}

iPoint j1Minimap::ScreenToMinimapToWorld(int x, int y) {
	iPoint minimap_position;
	minimap_position.x = (x - position.x - width * 0.5f)/scale;
	minimap_position.y = (y - position.y)/scale;
	return minimap_position;
}
