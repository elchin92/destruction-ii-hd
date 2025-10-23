#ifndef __GameKeeper__ 
#define __GameKeeper__

#include "Settings.h"
#include "SaveableClass.h"
#include "GeneralConstants.h"

enum GameType;
class Player;
class GameKeeper : public SaveableClass
{
private:

	GameType gt;
	int MatchCounter,RoundsCounter, FirstTo, RoundsPerMatch; // MatchCounter reset after each internal meeting in League mode
	int CupCounter, CupLevel, PlayBFinal, StartedBFinal;
	int LeagueMatches, InternalMeetingCounter, InternalMeetings, Met[16][16];
	int NumberOfPlayers;
	int GameFinished, WasShared;
	int Victories[3], lp[2];

	char CupNames[15][2][MNC+2];
	int CupLevelWinner[15];

	Player * PlayerArray[16];
	Player * PlayArray[16];
	Player * MatchWinner, * MatchLoser;

	void spOneOnOne();
	void spCup();
	void spLeague();

	void initiateOneOnOne();
	void initiateCup();
	void initiateLeague();

	char * gsOneOnOne();
	char * gsCup();
	char * gsLeague();

	void (GameKeeper::*(SetPlayers[NoOfGameTypes]))();
	void (GameKeeper::*(Initiate[NoOfGameTypes]))();
	char * (GameKeeper::*(GetString[NoOfGameTypes]))();

	void InternalSetPlayers(int one, int two);

	void EndGame();

	void DecideWinner();
	void Reset();
	void SetCupInfoNames();
protected:

public:

	GameKeeper();
	~GameKeeper();

	void StartAddingPlayers();
	void AddPlayerForNextGame(Player * pl);
	void BeginNewGame();

	void BeginNewMatch();
	void BeginNewRound();
	void EndRound(int Winner);

	void SortPlayerArray();

	char * GetCurrentMatchString();

	int GetNumberOfPlayers();
	int GetRoundVictories(int side);
	int GetRoundsCounter();
	int GetRoundsPerMatch();


	GameType GetGameType();

	int AllowDrawInCurrentRound();
	int GameOver();

	// For Cup Info
	char * GetBoxName(int bnr, int pl);
	int GetBoxWinner(int bnr); // 0 - Match not played, 1 - Winner is pl1, 2 - guess =)

	// For League Info
	int GetTotalLeagueMatches();
	int GetMatchesPlayed();

	// For Stats
	int GetRank(int pl);
	int WasSharedPlacement();

	Player * GetPlayer(int rank);

	void GiveAllReward(int Cash);
	void ResetAll();
	void ResetAllInput();

	void PostLoad(int PerformLoad);
	void PostSave();

};

extern GameKeeper * TheGameKeeper;
#endif
