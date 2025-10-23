#ifndef __ScoreTable__ 
#define __ScoreTable__

class Game;
class ScoreTableEntry;
class Player;

const int					sx=30,sy=3,sz=6;

#include "GameState.h"

class ScoreTable : public GameState
{
private:


protected:
	void InternalUpdateInput();

public:
	ScoreTableEntry * CurrentGame;

	ScoreTable();
	~ScoreTable();
	void Blit();
	void Setup();
	void SetSurfaces();
	void EndRound(int lScore, int rScore, int Winner, int RoundTime);
};

extern ScoreTable * TheScoreTable;

#endif

