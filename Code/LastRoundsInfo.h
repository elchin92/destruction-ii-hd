#ifndef __LastRoundsInfo__ 
#define __LastRoundsInfo__

#include "GameState.h"

class ScoreTableEntry;

class LastRoundsInfo : public FadingGameState, public SaveableClass
{
private:
	int Entries;
	HFONT daFont;
protected:
	void InternalUpdateInput();
	void EraseAllSTEAndCreateNew();

public:
	ScoreTableEntry * CurrentGame;
	LastRoundsInfo();
	~LastRoundsInfo();

	void Blit();
	void Setup();
	void PostLoad(int PerformLoad);
	void PostSave();

	void SetSurfaces();
	void SetupSurfaces();

	void EndRound(int lScore, int rScore, int Winner, int RoundTime);

};

extern LastRoundsInfo * TheLastRounds;

#endif
