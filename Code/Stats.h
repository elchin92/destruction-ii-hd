#ifndef __Stats__ 
#define __Stats__

class Game;

#include "ButtonGameState.h"

class Stats : public ButtonGameState
{
private:
	InfoBox * NameBox[2];
	InfoBox * StatsBox[2];
	InfoBox * MoneyBox[2];
	InfoBox * ItemBox[2];
	InfoBox * KillsBox[2];
	InfoBox * DeathBox[2];

	HFONT NameFont;
	HFONT StatsFont;

	int sp[2];
protected:
	void InternalUpdateInput();
	void InternalBlit();
public:
	Stats();
	~Stats();
	void Setup();
	void SetSurfaces();

	void SwitchLeft();
	void SwitchRight();
	void EndGame();

};

extern Stats * TheStats;
#endif

