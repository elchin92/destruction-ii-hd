#include "Menu.h"
#include "ddraw.h"
#ifndef __GraphicalButton__
#define __GraphicalButton__

class Game;

class GraphicalButton
{
private:

	Function

	ActiveMenuItem * LeftNeighbour;
	ActiveMenuItem * RightNeighbour;
	ActiveMenuItem * UpNeighbour;
	ActiveMenuItem * DownNeighbour;


public:

	GraphicalButton(int x, int y, int w, int h);
	~GraphicalButton();


	void BlitMenu();

};

#endif 