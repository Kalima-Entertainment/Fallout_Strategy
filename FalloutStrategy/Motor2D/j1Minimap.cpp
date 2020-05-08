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
#include "AI_Manager.h"
#include "j1Player.h"
#include "j1Pathfinding.h"

j1Minimap::j1Minimap() : j1Module(), texture(nullptr) {
	name = ("minimap");

	map_height = 200;
	scale = 1;
	width = 100;
	margin = 0;
	corner = Corner::TOP_LEFT;
	radar_time = 5;
	radar = false;
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

	map_width = TILE_SIZE * MAP_LENGTH;
	map_height = HALF_TILE * MAP_LENGTH;
	scale = ((width) / ((float)map_width));
	height = (map_height) * scale;

	CreateMinimap();

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

	radar_line.x1 = position.x + width * 0.5f;
	radar_line.y1 = position.y + height * 0.5f;

	node_map = App->pathfinding->GetNodeMap();

	for (int i = 0; i < node_map.size(); i++)
	{
		node_map[i] = App->map->MapToWorld(node_map[i].x, node_map[i].y);
		node_map[i] = App->minimap->WorldToMinimap(node_map[i].x, node_map[i].y);
	}

	return ret;
}

bool j1Minimap::CleanUp() {
	bool ret = true;

	App->tex->UnLoad(texture);
	texture = nullptr;

	return ret;
}

bool j1Minimap::PostUpdate() {

 	if ((App->menu_manager->current_menu == Menu::NO_MENU)||(App->menu_manager->current_menu == Menu::PAUSE_MENU) || (App->menu_manager->current_menu == Menu::GUI) || (App->menu_manager->current_menu == Menu::BUI_BASES)) {

		App->render->Blit(texture, position.x, position.y, NULL, 1.0, 0);

		/*
		if (App->render->fog_of_war) {
			SDL_Rect fog_of_war_rect = {0,0, map_width/width, map_height/height};
			iPoint rect_pos;

			fog_of_war_rect.w = width / 150;
			fog_of_war_rect.h = fog_of_war_rect.w * 0.5f;

			for (int i = 0; i < black_squares.size(); i++)
			{
				rect_pos = WorldToMinimap(black_squares[i].x, black_squares[i].y);
				fog_of_war_rect.x = rect_pos.x;
				fog_of_war_rect.y = rect_pos.y;
				App->render->DrawQuad(fog_of_war_rect, 0, 0, 0, 255, true, false);
			}

			for (int i = 0; i < grey_squares.size(); i++)
			{
				rect_pos = WorldToMinimap(black_squares[i].x, black_squares[i].y);
				fog_of_war_rect.x = rect_pos.x;
				fog_of_war_rect.y = rect_pos.y;
				App->render->DrawQuad(fog_of_war_rect, 0, 255, 0, 255, true, false);
			}

			black_squares.clear();
			grey_squares.clear();
		}
		*/

		for (int i = 0; i < App->entities->entities.size(); i++)
		{
			SDL_Rect entity_rect = { 0,0,3,3 };
			iPoint entity_position = App->minimap->WorldToMinimap(App->entities->entities[i]->position.x, App->entities->entities[i]->position.y);
			entity_rect.x = entity_position.x;
			entity_rect.y = entity_position.y;

			Faction entity_faction = App->entities->entities[i]->faction;
			if (App->player->faction == entity_faction) { App->render->DrawQuad(entity_rect, 0, 255, 0, 255, true, false); }
			else { 
				if(radar)
				App->render->DrawQuad(entity_rect, 255, 0, 0, 255, true, false);
			}
		}

		if (radar) {
			if (radar_timer.ReadSec() > radar_time) 
				radar = false;

			radar_line.x2 = radar_line.x1 + height * 0.5f * cos(radar_timer.Read() * 0.1 * PI / 180);
			radar_line.y2 = radar_line.y1 + height * 0.5f * sin(radar_timer.Read() * 0.1 * PI / 180);

			App->render->DrawLine(radar_line.x1, radar_line.y1, radar_line.x2, radar_line.y2, 0,255,0,255,false);
		}

		if (App->render->debug && App->pathfinding->show_nodes)
		{
			iPoint node_world_position;
			for (int i = 0; i < node_map.size(); i++)
			{
				App->render->DrawQuad({ node_map[i].x, node_map[i].y, 2, 2 }, 0, 0, 255, 255, true, false);
			}
		}
		SDL_Rect rect = { 0,0,0,0 };
		iPoint rect_position = WorldToMinimap(-App->render->camera.x, -App->render->camera.y);
		App->render->DrawQuad({ rect_position.x, rect_position.y, (int)(App->render->camera.w * scale),(int)(App->render->camera.h * scale) }, 255, 255, 255, 255, false, false);

	}

	return true;
}

bool j1Minimap::CreateMinimap() {

	PERF_START(ptimer);
	int half_width = map_width * 0.5f;

	texture = SDL_CreateTexture(App->render->renderer, SDL_GetWindowPixelFormat(App->win->window), SDL_TEXTUREACCESS_TARGET, width, height);
	SDL_SetRenderTarget(App->render->renderer, texture);

	for (int l = 0; l < MAX_LAYERS; l++)
	{
		MapLayer* layer = &App->map->data.layers[l];

		if (layer->properties.Get("Nodraw") != 0)
			continue;

		for (int y = 0; y < MAP_LENGTH; ++y)
		{
			for (int x = 0; x < MAP_LENGTH; ++x)
			{
				int tile_id = layer->Get(x, y);
				if (tile_id > 0)
				{
					TileSet* tileset = App->map->GetTilesetFromTileId(tile_id);

					SDL_Rect r = tileset->GetTileRect(tile_id);
					iPoint pos = App->map->MapToWorld(x, y);

					App->render->Blit(tileset->texture, pos.x + half_width + tileset->offset_x, pos.y + tileset->offset_y, &r, scale);
				}
			}
		}
	}

	SDL_SetRenderTarget(App->render->renderer, NULL);

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

void j1Minimap::EnableRadar() {
	radar = true;
	radar_timer.Start();
}