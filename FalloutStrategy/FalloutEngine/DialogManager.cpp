#include "DialogManager.h"
#include "p2Log.h"
#include "j1Render.h"
#include "j1App.h"
#include "j1Gui.h"
#include "UI_element.h"
#include "MenuManager.h"
#include "j1Audio.h"
#include "UI_Label.h"
#include "j1Hud.h"

DialogManager::DialogManager() : j1Module() {


	borders[0] = { 58, 355, 1172, 344 };
	borders[1] = { 78, 368, 1139, 89 };
	borders[2] = { 78, 468, 1139, 69 };
	borders[3] = { 78, 543, 1139, 69 };
	borders[4] = { 78, 618, 1139, 69 };

	option_box[0] = { 80, 470, 1135, 65 };
	option_box[1] = { 80, 545, 1135, 65 };
	option_box[2] = { 80, 620, 1135, 65 };

	dialog_level = 0;

	background_box.x = 60;
	background_box.y = 355;
	background_box.w = 1170;
	background_box.h = 340;

	statement_box.x = 80;
	statement_box.y = 370;
	statement_box.w = 1135;
	statement_box.h = 85;
};

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

bool DialogManager::CleanUp() {
	bool ret = true;
	if (App->quitGame) {
		for(int i = 0; i < dialogs.size(); i++)
		{
			delete dialogs[i];
			dialogs[i] = nullptr;
		}

		dialogs.clear();
	}
	App->menu_manager->DestroyMenu(Menu::QUEST);
	App->menu_manager->DestroyMenu(Menu::DIALOG);
	App->menu_manager->CreateMenu(Menu::GUI);
	App->menu_manager->CreateMenu(Menu::RESOURCES);
	App->isPaused = false;
	App->hud->activateTimer = true;
	App->hud->minutes = 14;
	App->hud->timer = 59;
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

void DialogManager::Callback(UI_element* button) {
	UI_Label* label = (UI_Label*)App->menu_manager->dialogs[0];
	UI_Label* option = (UI_Label*)App->menu_manager->dialogs[1];

	if (dialog_level < dialogs.size())
	{
		switch (button->GetType())
		{
		case UI_Type::OPTION_A:
			App->audio->PlayFx(1, App->audio->character_fx, 0);
			if (option->text != " ") {
				//LOG("Option A");
				label->SetLabelText(dialogs[dialog_level]->response[0].c_str(), "StackedPixelSmall");
				for(int i = 1; i < 4; i++)
				{
					label = (UI_Label*)App->menu_manager->dialogs[i];
					label->SetLabelText(" ", "StackedPixelSmall");
				}
				dialog_level++;
			}
			break;
		case UI_Type::OPTION_B:
			App->audio->PlayFx(1, App->audio->character_fx, 0);
			if (option->text != " ") {
				//LOG("Option B");
				label->SetLabelText(dialogs[dialog_level]->response[1].c_str(), "StackedPixelSmall");
				for(int i = 1; i < 4; i++)
				{
					label = (UI_Label*)App->menu_manager->dialogs[i];
					label->SetLabelText(" ", "StackedPixelSmall");
				}
				dialog_level++;
			}
			break;
		case UI_Type::OPTION_C:
			App->audio->PlayFx(1, App->audio->character_fx, 0);
			if (option->text != " ") {
				//LOG("Option C");
				label->SetLabelText(dialogs[dialog_level]->response[2].c_str(), "StackedPixelSmall");
				for(int i = 1; i < 4; i++)
				{
					label = (UI_Label*)App->menu_manager->dialogs[i];
					label->SetLabelText(" ", "StackedPixelSmall");
				}
				dialog_level++;
			}
			break;
		case UI_Type::ADVANCE_DIALOGS:
			App->audio->PlayFx(1, App->audio->click_fx, 0);
			label = (UI_Label*)App->menu_manager->dialogs[0];
			label->SetLabelText(dialogs[dialog_level]->statement.c_str(), "StackedPixelSmall");

			for(int i = 1; i < 4; i++)
			{
				label = (UI_Label*)App->menu_manager->dialogs[i];
				label->SetLabelText(dialogs[dialog_level]->options[i-1].c_str(), "StackedPixelSmall");
			}

			if (dialog_level == dialogs.size() -1) {
				dialog_level++;
			}
			break;

		case UI_Type::FINISH_DIALOGS:
			App->audio->PlayFx(1, App->audio->back_fx, 0);
			Disable();
			App->menu_manager->CreateMenu(Menu::QUEST);
			break;
		default:
			break;
		}
	}
	else
	{
		Disable();
		App->menu_manager->CreateMenu(Menu::QUEST);
	}
	
}