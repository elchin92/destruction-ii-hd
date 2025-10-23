#ifndef __PlayerSelecter__ 
#define __PlayerSelecter__

class Game;
class Menu;
class InfoBox;
class ActiveMenuItem;

#include "ButtonGameState.h"

class PlayerSelecter : public ButtonGameState
{
protected:

	int PreviousMarked[16];

	int SelectedPlayersOK();
	void RebuildPlayerMenu();

	void InternalUpdateInput();
	void InternalBlit();

	int GetSelectedPlayers();

	Menu * PlayerMenu;
	InfoBox * SelectedPlayersBox;
	InfoBox * SelectedInfoBox;

	HFONT tFont;

	ActiveMenuItem * b1;

	char PSString[3][2][35];
public:

	PlayerSelecter();
	~PlayerSelecter();
	void Setup();

	void SetSurfaces();

	void StartNewGame();


};

extern PlayerSelecter * ThePlayerSelecter;

#endif
