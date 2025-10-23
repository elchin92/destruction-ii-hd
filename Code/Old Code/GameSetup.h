#ifndef __GameSetup__ 
#define __GameSetup__

#include ".h"

enum GameType{gtOneOnOne, gtCup, gtLeague, NoOfGameTypes};



//													    BC	FQ	UL
const int EnemyDefaultSettings[eTypes][eSettings]={	{	1,	3,	2	}, // Droids
													{	1,	2,	3	}, // Tanks
													{	0,	3,	1	}, // Cars
													{	0,	2,	1	}};// Machines

class Game;
class ActiveMenuItem;
class Menu;

#include "GameState.h"

class GameSetup : public GameState
{
private:

	GameType cgt;

	Menu * GameSetupMenus[(int)NoOfGameTypes];
	ActiveMenuItem * EnemySettings[eTypes][eSettings];
	ActiveMenuItem * SwitchMenuItems[NoOfGameTypes];

	int * PlayersPointer;
	int * GroupsPointer;

	void UpdateGroups();
	void SwitchGameType();
	void SetNeighbours();

public:

	Menu * EnemySetupMenu;

	GameSetup();
	~GameSetup();
	void Blit();
	void Setup();
	void UpdateInput();
	void SetSurfaces();



};

extern GameSetup * TheGameSetup;

#endif

