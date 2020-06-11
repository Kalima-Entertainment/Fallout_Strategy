#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Cursor.h"
#include "j1Gui.h"
#include "j1Window.h"

j1Cursor::j1Cursor() : j1Module()
{
	name = ("cursor");

	cursorRect = { 0,0, 18,25 };
	cursor_position = { 0,0 };
	cursor_offset = { 0, 0 };
}

j1Cursor::~j1Cursor() {

}

bool j1Cursor::Awake(pugi::xml_node& config)
{


	return true;
}

bool j1Cursor::Start()
{
	

	return true;
}

bool j1Cursor::Update(float dt)
{

	return true;
}

bool j1Cursor::PostUpdate()
{
	//CURSOR
	App->input->GetMousePosition(cursor_position.x, cursor_position.y);
	App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), cursor_position.x * App->win->GetScale() + cursor_offset.x, cursor_position.y * App->win->GetScale() + cursor_offset.y, &cursorRect, 1.0f, 0.0f);

	return true;
}

bool j1Cursor::CleanUp()
{
	
	return true;
}