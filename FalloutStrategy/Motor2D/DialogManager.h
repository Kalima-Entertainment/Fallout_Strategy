#ifndef _DIALOG_MANAGER_H
#define _DIALOG_MANAGER_H
#include "j1Module.h"

class DialogManager : public j1Module
{
public:
	DialogManager();
	~DialogManager();

	bool Awake(pugi::xml_node&);
	bool Update(float dt);

private:

};


#endif // !_DIALOG_MANAGER_H
