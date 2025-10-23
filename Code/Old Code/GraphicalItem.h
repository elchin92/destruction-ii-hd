#include "Menu.h"
#include "ddraw.h"
#ifndef __GraphicalItem__
#define __GraphicalItem__

class Game;

class GraphicalItem
{
private:

	GraphicItem * TheItems;
	GraphicItem * TriggeredItem;
	
	InfoArea * TheInfoAreas;

public:

	virtual ~GraphicalItem();

	void Initialize();

};

#endif 