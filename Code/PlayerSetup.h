#ifndef __PlayerSetup__ 
#define __PlayerSetup__

#include "ButtonGameState.h"


class Menu;
class ActiveMenuItem;
class TextBoxMenuItem;

class PlayerSetup : public ButtonGameState
{
private:

	int PlayerPointer,NumberOfPlayers;

	Player * PlayerArray[16];

	Menu * PlayersMenu;

	ActiveMenuItem * b1,*b2,*b3,*b4;
	TextBoxMenuItem * b5;


protected:
	void InternalBlit();
	void InternalUpdateInput();

public:

	PlayerSetup();
	~PlayerSetup();


	void Setup();
	void SetSurfaces();

	void RebuildPlayerMenu();
	void SetNeighboursToMarked();

	// Player Collection related

	void SavePlayer(Player * SavePlayer, int Slot);
	void SavePlayers();
	void LoadPlayers();
	void CreateNewPlayer();
	void DeletePlayerFile(int Slot);
	int ErasePlayer();
	int GetNumberOfPlayers();

	void ScaleTemporary(Player * pl);

	Player ** EraseAllPlayersAndCreateNew(int NewPlayers);

	Player * GetPlayer(int pl);
	Player * GetMarkedPlayer();
};

extern PlayerSetup * ThePlayerSetup;

#endif
