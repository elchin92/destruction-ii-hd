#include "GameState.h"

#ifndef __PCSetup__ 
#define __PCSetup__

class Game;
class Player;

class PCSetup : public GameState
{
private:

	int PlayerPointer, NumberOfPlayers, InputStatus,Choice;
	Player * InitialPlayer;
	Player * ActivePlayer;

protected:
	void InternalUpdateInput();

public:

	PCSetup();
	~PCSetup();

	void Blit();
	void Setup();
	void SetSurfaces();
	void NextPlayer();
	void PreviousPlayer();
	void SavePlayer(Player * SavePlayer, int Slot);
	void LoadPlayers();
	void CreateNewPlayer();
	void DeletePlayerFile(int Slot);
	int ErasePlayer();
	int GetPlayers();
	int GetUniqueID();
	int * GetNumberOfPlayersPointer();

	Player * GetFirstPlayer();
	Player * PlayerCollection(int pl);

};

extern PCSetup * ThePCSetup;
#endif

