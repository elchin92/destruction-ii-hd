#include "Menu.h"
#include "SDL2_DirectDrawCompat.h"
#ifndef __GraphicMenu__
#define __GraphicMenu__

class Game;

class GraphicMenu 
{
private:

	int Choice,  MenuPos[2], NumberOfMenuItems, Cols;

	LPDIRECTDRAWSURFACE7 * SelectedSurface;
	LPDIRECTDRAWSURFACE7 * BackgroundSurface;
	int BlitDim[2], BackDim[2], BackPos[2];

public:

	GraphicMenu(int Columns, int Items,
		LPDIRECTDRAWSURFACE7 * BlitSource, int X, int Y, int Width, int Height,
		LPDIRECTDRAWSURFACE7 * Background, int bX, int bY, int bWidth, int bHeight);
	~GraphicMenu();


	void BlitMenu();
	void UpdateInput();

	int GetChoice();


};

#endif 