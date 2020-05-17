#include "DialogManager.h"
#include "p2Log.h"
#include "j1Render.h"
#include "j1App.h"
#include "j1Gui.h"
#include "UI_element.h"

DialogManager::DialogManager() : j1Module() {
	background_box = { 60, 355, 1170, 340};
	borders[0] =	 { 58, 355, 1172, 344 };

	statement_box =  { 80, 370, 1135, 85};
	borders[1] =     { 78, 368, 1139, 89 };

	option_box[0] =  { 80, 470, 1135, 65};
	borders[2] =     { 78, 468, 1139, 69 };
	option_box[1] =  { 80, 545, 1135, 65 };
	borders[3] =     { 78, 543, 1139, 69 };
	option_box[2] =  { 80, 620, 1135, 65 };
	borders[4] =     { 78, 618, 1139, 69 };
}

DialogManager::~DialogManager() {

}

bool DialogManager::Awake(pugi::xml_node&) {
	bool ret = true;

	LoadDialogs();

	return ret;
}

bool DialogManager::Start() {
	bool ret = true;
	


	return ret;
}

bool DialogManager::Update(float dt) {
	bool ret = true;
	return ret;
}

bool DialogManager::PostUpdate() {
	bool ret = true;

	App->render->DrawQuad(background_box, 0, 0, 00, 150, true, false);
	App->render->DrawQuad(statement_box, 0, 60, 30, 200, true, false);
	App->render->DrawQuad(option_box[0], 0, 60, 30, 200, true, false);
	App->render->DrawQuad(option_box[1], 0, 60, 30, 200, true, false);
	App->render->DrawQuad(option_box[2], 0, 60, 30, 200, true, false);
	App->render->DrawQuad(borders[0], 100, 255, 100, 220, false, false);
	App->render->DrawQuad(borders[1], 100, 255, 100, 220, false, false);
	App->render->DrawQuad(borders[2], 100, 255, 100, 220, false, false);
	App->render->DrawQuad(borders[3], 100, 255, 100, 220, false, false);
	App->render->DrawQuad(borders[4], 100, 255, 100, 220, false, false);

	return ret;
}

bool DialogManager::LoadDialogs() {
	bool ret = true;

	pugi::xml_document dialogs_file;
	pugi::xml_node dialogs_node;
	pugi::xml_parse_result result = dialogs_file.load_file("dialogs.xml");

	if (result == NULL)
		LOG("Could not load xml file dialogs.xml pugi error: %s", result.description());
	else
		dialogs_node = dialogs_file.child("dialogs");

	pugi::xml_node dialog_node = dialogs_node.child("dialog");
	pugi::xml_node option_node;

	while (dialog_node)
	{
		Dialog* dialog = new Dialog();
		dialog->statement = dialog_node.attribute("statement").as_string();

		option_node = dialog_node.child("option");
		int i = 0;

		while (option_node) {
			dialog->options[i] = option_node.attribute("text").as_string();
			dialog->response[i] = option_node.attribute("response").as_string();

			option_node = option_node.next_sibling();
			i++;
		}
		dialogs.push_back(dialog);
		dialog_node = dialog_node.next_sibling();
	}

	return ret;
}