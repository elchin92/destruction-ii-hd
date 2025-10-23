#include "Menu.h"
#include "ddraw.h"
#ifndef __GraphicalButtonMenu__
#define __GraphicalButtonMenu__

class Game;

class GraphicalButtonMenu
{
private:

	GraphicButton * TheButtons;
	GraphicButton * TriggeredButton;
	
	InfoArea * TheInfoAreas;

public:

	GraphicalButtonMenu();
	~GraphicalButtonMenu();


	void BlitMenu();
	void UpdateInput();

};

#endif 