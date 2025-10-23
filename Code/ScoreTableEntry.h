#ifndef __ScoreTableEntry__ 
#define __ScoreTableEntry__

#include "definitions.h"
#include "SaveableClass.h"
#include "GeneralConstants.h"

//enum IntStatistic{isRound,isLives,isTime,isEvents,isOpponentKill1,isCarKill1,isDroidKill1,isMachineKill1,isTankKill1,isOpponentKill2,isCarKill2,isDroidKill2,isMachineKill2,isTankKill2, isNoKill, NoOfIntStats};

const int yLabel=157;

class Player;
class Game;

class ScoreTableEntry : public SaveableClass
{
private:

	int TimePlayed, Round;
	int Score[2];
	int Kills[2][NumberOfMortals];
	char LeftPlayer[MNC+2], RightPlayer[MNC+2];
	char WinnerName[MNC+2];
	
public:

	ScoreTableEntry * nextTable;
	ScoreTableEntry(int iRound,ScoreTableEntry * inextTable);
	~ScoreTableEntry();

	int GetRound();
	void ReportKill(int pl, MortalType MType);
	void ReportStats(int Winner, int lScore, int rScore, int RoundTime);

	void Blit(int y, int Highlighted = 0);
};
#endif