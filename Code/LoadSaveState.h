#ifndef __LoadSaveState__ 
#define __LoadSaveState__

#define MAXSAVEGAMES 90

#include "GameState.h"

class Menu;
class LoadSaveState : public GameState
{
protected:
	Menu * LoadSaveMenu;
	void Initialize();
public:

	virtual ~LoadSaveState();
	void BuildMenu(int ExtraEmpty);
};
#endif

