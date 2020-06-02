#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "MenuManager.h"
#include "MainMenu.h"


MainMenu::MainMenu() : j1Module()
{
	name = ("main_menu");

	title_tex = nullptr;
	loader = nullptr;
}

bool MainMenu::LoadAnimations()
{
	bool ret = true;
	pugi::xml_document animation_file;
	pugi::xml_parse_result result = animation_file.load_file("Assets/gui/textures/LogoIntro.tmx");

	std::string image = std::string(animation_file.child("tileset").child("image").attribute("source").as_string());

	title_tex = App->tex->Load("Assets/gui/textures/LogoFallout.png");

	if (result == NULL)
	{
		LOG("Could not load animation tmx file %s. pugi error: %s", "gui/LogoIntro.tmx", result.description());
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

	id = animation.attribute("id").as_int();

	loader = &animationLogo;

	while (frame != nullptr) {
		tile_id = frame.attribute("tileid").as_int();
		speed = frame.attribute("duration").as_int();
		rect.x = rect.w * ((tile_id) % columns);
		rect.y = rect.h * ((tile_id) / columns);
		loader->PushBack(rect, speed);
		frame = frame.next_sibling();
	}
	loader->loop = true;

	animation = animation.next_sibling();
	frame = animation.child("animation").child("frame");

	return ret;
}

MainMenu::~MainMenu() {

}

bool MainMenu::Awake(pugi::xml_node& config)
{
	

	return true;
}

bool MainMenu::Start()
{
	LoadAnimations();
	App->menu_manager->CreateMenu(Menu::MAIN_MENU);

	return true;
}

bool MainMenu::Update(float dt)
{

	last_dt = dt;

	return true;
}

bool MainMenu::PostUpdate()
{
	if (App->menu_manager->current_menu == Menu::MAIN_MENU)
	{
		
		App->render->Blit(title_tex, 70, -130, &loader->GetCurrentFrame(last_dt), 1.0f, 0.0f);

	}

	return true;
}

bool MainMenu::CleanUp()
{
	App->tex->UnLoad(title_tex);
	loader = nullptr;
	title_tex = nullptr;

	return true;
}