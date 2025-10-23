#include "GameKeeper.h"
#include "Game.h"
#include "Player.h"
#include "Settings.h"
#include "Debugger.h"
#include "PlayerSetup.h"
#include "LastRoundsInfo.h"
#include <stdio.h>

GameKeeper * TheGameKeeper;

GameKeeper::GameKeeper(){


	SaveableClass::Initialize("GameKeeper");
	RegisterItem("gt",&gt,sizeof(gt));
	RegisterItem("MatchCounter",&MatchCounter,sizeof(MatchCounter));
	RegisterItem("RoundsCounter",&RoundsCounter,sizeof(RoundsCounter));
	RegisterItem("FirstTo",&FirstTo,sizeof(FirstTo));
	RegisterItem("RoundsPerMatch",&RoundsPerMatch,sizeof(RoundsPerMatch));
	RegisterItem("CupCounter",&CupCounter,sizeof(CupCounter));
	RegisterItem("CupLevel",&CupLevel,sizeof(CupLevel));
	RegisterItem("PlayBFinal",&PlayBFinal,sizeof(PlayBFinal));
	RegisterItem("StartedBFinal",&StartedBFinal,sizeof(StartedBFinal));
	RegisterItem("LeagueMatches",&LeagueMatches,sizeof(LeagueMatches));
	RegisterItem("InternalMeetingCounter",&InternalMeetingCounter,sizeof(InternalMeetingCounter));
	RegisterItem("InternalMeetings",&InternalMeetings,sizeof(InternalMeetings));
	RegisterItem("Met",&Met[0][0],sizeof(Met));
	RegisterItem("NumberOfPlayers",&NumberOfPlayers,sizeof(NumberOfPlayers));
	RegisterItem("GameFinished",&GameFinished,sizeof(GameFinished));
	RegisterItem("WasShared",&WasShared,sizeof(WasShared));
	RegisterItem("Victories",&Victories[0],sizeof(Victories));
	RegisterItem("lp",&lp,sizeof(lp));
	RegisterItem("CupNames",&CupNames[0][0][0],sizeof(CupNames));
	RegisterItem("CupLevelWinner",&CupLevelWinner,sizeof(CupLevelWinner));


	SetPlayers[0] = &GameKeeper::spOneOnOne;
	SetPlayers[1] = &GameKeeper::spCup;
	SetPlayers[2] = &GameKeeper::spLeague;
	Initiate[0] = &GameKeeper::initiateOneOnOne;
	Initiate[1] = &GameKeeper::initiateCup;
	Initiate[2] = &GameKeeper::initiateLeague;

	GetString[0] = &GameKeeper::gsOneOnOne;
	GetString[1] = &GameKeeper::gsCup;
	GetString[2] = &GameKeeper::gsLeague;

	LeagueMatches = 0; // required for getmatches

	UZ();Zero(PlayerArray,16);
	 Zero(PlayArray,16);
}
GameKeeper::~GameKeeper(){}

void GameKeeper::GiveAllReward(int Cash){
	for(int pl=0;pl<NumberOfPlayers;pl++){
		PlayArray[pl]->GiveReward(Cash);	
	}
}
void GameKeeper::ResetAll(){
	for(int pl=0;pl<NumberOfPlayers;pl++){
		PlayArray[pl]->Reset();	
	}
}
void GameKeeper::ResetAllInput(){
	for(int pl=0;pl<NumberOfPlayers;pl++){
		PlayArray[pl]->ResetInput();	
	}
}
void GameKeeper::StartAddingPlayers(){
	NumberOfPlayers=0;
}

void GameKeeper::AddPlayerForNextGame(Player * pl){
	PlayerArray[NumberOfPlayers]=pl;
	NumberOfPlayers++;
}

void GameKeeper::BeginNewGame(){
	// NumberOfPlayers & PlayerArray set
	Reset();
	Beacon(888000+NumberOfPlayers);
	// Randomize array
	gt=(GameType)AL_GT;
	if(gt!=gtOneOnOne){
		DP("GameKeeper ");
		for(int c=0;c<NumberOfPlayers;c++){
			int r=rand()%16;
			while(!PlayerArray[r]){r++;r%=NumberOfPlayers;}
			PlayArray[c]=PlayerArray[r];
			PlayerArray[r]=NULL;
		}
		CopyArray(PlayerArray,PlayArray,16);
		DP2("GameKeeper Players...", NumberOfPlayers);
		for(int pl=0;pl<NumberOfPlayers;pl++){
			DP(PlayArray[pl]->GetName());
		
		}
	}else{
		DP("GameKeeper Setting two players");
		PlayArray[0]=PlayerArray[1];
		PlayArray[1]=PlayerArray[0];
	}

	Beacon(7234406);

	(this->*(Initiate[gt]))();

	if(gt==gtOneOnOne){
		RoundsPerMatch=AL_RS;
		FirstTo=AL_VC;
	}else{
		RoundsPerMatch=AL_RM;
		FirstTo=FALSE;
		if(gt==gtLeague){
			InternalMeetings=AL_IM;
		}else{
			// gtCup
			PlayBFinal=AL_PB;
		}
	}
	BeginNewMatch();

}

void GameKeeper::BeginNewMatch(){
	DP("Beginning new match");
	(this->*(SetPlayers[gt]))();
	RoundsCounter=0;
	TheGame->SetActivePlayers();
}
void GameKeeper::BeginNewRound(){
	// Unlike begin new match, this is called after GETREADY!!!
	TheMortals=ThePlayers[0];
	ThePlayers[0]->nextMortal=ThePlayers[1];
	ThePlayers[1]->nextMortal=NULL;
	ThePlayers[0]->nextDeadMortal=NULL;
	ThePlayers[1]->nextDeadMortal=NULL;

}

int GameKeeper::GameOver(){
	return GameFinished;
}

void GameKeeper::EndGame(){
	GameFinished=TRUE;

}


void GameKeeper::Reset(){
	// don't reset players related stuff!
	RoundsCounter=0;MatchCounter=0;GameFinished=FALSE;
	MatchWinner=NULL;MatchLoser=NULL;
	UZ();Zero(PlayArray,16);
}

int GameKeeper::AllowDrawInCurrentRound(){
	return !(
		gt==gtCup&&Victories[0]==Victories[1]&&RoundsCounter==RoundsPerMatch-1  // Cup!
		
		);
}

void GameKeeper::DecideWinner(){
	MatchWinner=MatchLoser=NULL;
	if(FirstTo&&Victories[1]==RoundsPerMatch || !FirstTo&&Victories[1]>Victories[0]){
		MatchWinner=ThePlayers[1];
		MatchLoser=ThePlayers[0];
		MatchWinner->GivePoints(3);
		MatchLoser->GivePoints(0); // For stats reason
	}else if(FirstTo&&Victories[0]==RoundsPerMatch || !FirstTo&&Victories[0]>Victories[1]){
		MatchWinner=ThePlayers[0];
		MatchLoser=ThePlayers[1];
		MatchWinner->GivePoints(3);
		MatchLoser->GivePoints(0); // For stats reason
	}else{
		ThePlayers[0]->GivePoints(1);
		ThePlayers[1]->GivePoints(1);
	}

	if(MatchLoser&& MatchWinner &&gt==gtCup){
		if(!StartedBFinal){
			MatchWinner->GivePoints(5,FALSE);
			MatchLoser->GivePoints(5,FALSE);
		}
		MatchLoser->GivePoints(1,FALSE);

	}
	

}
void GameKeeper::EndRound(int RoundWinner){
	
	RoundsCounter++;

	ThePlayers[0]->EndRound(RoundWinner);
	ThePlayers[1]->EndRound(RoundWinner);

	Victories[RoundWinner]++;
	
	if(RoundsPerMatch && (
		!FirstTo &&  RoundsCounter==RoundsPerMatch                                  || 
		FirstTo && (Victories[0]==RoundsPerMatch || Victories[1]==RoundsPerMatch)   )){
		// EndMatch()
		MatchCounter++;
		DecideWinner();
		BeginNewMatch();
		UZ();Zero(Victories,2);
	}

	SortPlayerArray();
	// Set TheMortals Right
	BeginNewRound();

}

#define _swpl()	tPlayer=PlayerArray[i];	PlayerArray[i]=PlayerArray[j];	PlayerArray[j]=tPlayer;
void GameKeeper::SortPlayerArray(){
	// Using bubble sort
	if(gt==gtCup){

	}else{
		Player * tPlayer;
		for(int i=0;i<NumberOfPlayers;i++){
			for(int j=i+1;j<NumberOfPlayers;j++){
				if(PlayerArray[i]->GetPoints()<PlayerArray[j]->GetPoints()){
					_swpl();
				}else if(PlayerArray[i]->GetPoints()==PlayerArray[j]->GetPoints()){
					if(PlayerArray[i]->GetMatchesWon()<PlayerArray[j]->GetMatchesWon()){
						_swpl();
					}else if(gt==gtOneOnOne && PlayerArray[i]->GetRoundsWon()<PlayerArray[j]->GetRoundsWon()){
						_swpl();
					}
				}
			}
		}
	}
}

void GameKeeper::spOneOnOne(){
	if(MatchCounter){
		DP("1 on 1: MatchCounter set, ending game");
		EndGame();
		return;
	}
	Beacon(888999);
	InternalSetPlayers(0,1);
}

void GameKeeper::initiateCup(){
	CupLevel=NumberOfPlayers/2;
	CupCounter=0;
	StartedBFinal=FALSE;
	lp[0]=-2;
	lp[1]=-1;

	for(int i=0;i<15;i++){
		for(int j=0;j<2;j++){
			strcpy(CupNames[i][j],"");
		}
		CupLevelWinner[i]=0;
	}

	SetCupInfoNames();
}
void GameKeeper::initiateOneOnOne(){

}
void GameKeeper::spCup(){
	int pp[2]={lp[0]+2,lp[1]+2};

	DP2("CupCounter",CupCounter);
	DP2("CupLevel",CupLevel);

	if(StartedBFinal){
		EndGame();
		return;
	}

	if(MatchWinner){	 // Inv: !StartedBFinal
		CupLevelWinner[CupCounter+CupLevel-2]=MatchWinner->GetTeam()+1;
		if(CupLevel>1)
			strcpy(CupNames[(CupLevel+CupCounter-1)/2-1][!(CupCounter%2)],MatchWinner->GetName());
	}

	// For B-Final
	if(CupLevel==2){
		// Semi final played
		if(CupCounter==1){
			PlayArray[1]=MatchLoser;
		}else if(CupCounter==2){
			PlayArray[2]=PlayArray[1]; // Semifinal 1 Loser
			PlayArray[3]=MatchLoser;
		}

	}	



	if(CupCounter==CupLevel||CupLevel==0){
		DP("Changin cup level...");
		CupLevel/=2;
		pp[0]=0;
		pp[1]=1;
		if(CupLevel==0){
			if(PlayBFinal==1&&NumberOfPlayers>=4){	 // Inv: !StartedBFinal
				DP("Started B final");
				StartedBFinal=TRUE;
				pp[0]=2;
				pp[1]=3;
			}else{
				DP("Cup ended");
				EndGame();
				return;
			}
			
		}
		CupCounter=0;
	}

	if(MatchWinner){ // Let the previous winner proceed
		PlayArray[lp[0]/2]=MatchWinner;
		
	}

	InternalSetPlayers(pp[0],pp[1]);

	CupCounter++;

	DP2("Chose as Player#1  Player",pp[0]+1);
	DP2("Chose as Player#2, Player",pp[1]+1);
}
void GameKeeper::SetCupInfoNames(){

	for(int i=0;i<CupLevel;i++){
		strcpy(CupNames[CupLevel+i-1][0],PlayArray[0+2*i]->GetName());
		strcpy(CupNames[CupLevel+i-1][1],PlayArray[1+2*i]->GetName());
	}
}

int GameKeeper::GetTotalLeagueMatches(){
	return LeagueMatches*InternalMeetings;
}
int GameKeeper::GetMatchesPlayed(){
	return LeagueMatches*InternalMeetingCounter+MatchCounter;
}
void GameKeeper::initiateLeague(){
	InternalMeetingCounter=0;
	LeagueMatches=(NumberOfPlayers*(NumberOfPlayers-1))/2;

	UZ2();Zero2d(Met,16,16);

	for(int r=0;r<16;r++)
		Met[r][r]=TRUE;

}



void GameKeeper::spLeague(){

	DP("spLeague()");
	if(MatchCounter==LeagueMatches){
		DP("MatchCounter==LeagueMAtches");
		// New players required!
		UZ2();Zero2d(Met,16,16);
		for(int r=0;r<16;r++)
			Met[r][r]=TRUE;
		MatchCounter=0;
		InternalMeetingCounter++;
		if(InternalMeetingCounter==InternalMeetings){
			EndGame();
			return;
		}
		
	}

	int pp[2]={0,0};


	for(int pl=0;pl<2;pl++){
		//DP2("Setting Player #",pl+1);
		int op=pp[pl];
		//DP2("Initial Suggestion is Player", pp[pl]+1);
		for(int i=1;i<NumberOfPlayers;i++){
			int p = (op+i)%NumberOfPlayers;
			if( PlayArray[p]->GetMatchesPlayed()<PlayArray[pp[pl]]->GetMatchesPlayed()){
				//DP2("Player",p+1);
				//DP2("has less matches than Player",pp[pl]);
				if(!pl || !Met[pp[0]][p] ){
					//DP2("Set Player",p+1);
					pp[pl]=p;
				}
			}
		}
		//DP2("Chose Player",pp[pl]+1);
		if(!pl){
			// pp[0] decided
			pp[1]=pp[0];
			while(Met[pp[0]][pp[1]]){
				pp[1]=(pp[1]+1)%NumberOfPlayers;
			}
			
		}
	}


	InternalSetPlayers(pp[0],pp[1]);

	Met[pp[0]][pp[1]]=TRUE;
	Met[pp[1]][pp[0]]=TRUE;

}
void GameKeeper::InternalSetPlayers(int one, int two){
	ThePlayers[0]=PlayArray[one];
	ThePlayers[1]=PlayArray[two];
	lp[0]=one;
	lp[1]=two;	
}
int GameKeeper::GetNumberOfPlayers(){return NumberOfPlayers;}

int GameKeeper::GetRoundVictories(int side){return Victories[side];}

int GameKeeper::GetRoundsCounter(){return RoundsCounter;}
int GameKeeper::GetRoundsPerMatch(){return RoundsPerMatch;}

char * GameKeeper::GetCurrentMatchString(){
	return 	(this->*(GetString[gt]))();

}
char * GameKeeper::gsOneOnOne(){

	static char daMsg[100];
	sprintf(daMsg,"1 on 1, single match");

	if(!FirstTo&&RoundsCounter==RoundsPerMatch-1)
		strcat(daMsg,", final round");
	if(FirstTo&& (Victories[0]==RoundsPerMatch-1||Victories[1]==RoundsPerMatch-1 ))
		strcat(daMsg,", game round");

	return daMsg;
}
char * GameKeeper::gsCup(){

	static char daMsg[100];

	sprintf(daMsg,"Cup match, ");
	if(StartedBFinal)
		strcat(daMsg,"B final");
	else if(CupLevel==1)
		strcat(daMsg,"final");
	else if(CupLevel==2)
		strcat(daMsg,"semi final");
	else if(CupLevel==4)
		strcat(daMsg,"quarter final");
	else if(CupLevel==8)
		strcat(daMsg,"round of 16");

	return daMsg;
}
char * GameKeeper::gsLeague(){

	static char daMsg[100];
	sprintf(daMsg,"League match");

	return daMsg;
}

Player * GameKeeper::GetPlayer(int rank){
	return PlayerArray[rank];
}
GameType GameKeeper::GetGameType(){return gt;}


char * GameKeeper::GetBoxName(int bnr, int pl){return CupNames[bnr][pl];}
int GameKeeper::GetBoxWinner(int bnr){return CupLevelWinner[bnr];} // 0 - Match not played, 1 - Winner is pl1, 2 - guess =)

int GameKeeper::GetRank(int pl){

	
	WasShared=FALSE;

	if(gt==gtOneOnOne){
		if(GetPlayer(pl)->GetRoundsWon()>GetPlayer(!pl)->GetRoundsWon())
			return 1;
		if(GetPlayer(pl)->GetRoundsWon()<GetPlayer(!pl)->GetRoundsWon())
			return 2;

		WasShared=TRUE;
		return 1;
	}

	int origScore=GetPlayer(pl)->GetPoints();
	int lowPl=pl;

	if(pl+2<=NumberOfPlayers){
		if(GetPlayer(pl+1)->GetPoints()==origScore)
			WasShared=TRUE;
	}

	while(TRUE){
		pl--;
		if(pl<0)
			break;
		if(GetPlayer(pl)->GetPoints()==origScore){
			lowPl--;
			WasShared=TRUE;
		}else
			break;
	}
	return lowPl+1;
}

int GameKeeper::WasSharedPlacement(){return WasShared;}


void GameKeeper::PostLoad(int PerformLoad){
	DP("GameKeeper::PostLoad() called");

	if(PerformLoad){

		Player ** npa = ThePlayerSetup->EraseAllPlayersAndCreateNew(NumberOfPlayers);
		for(int pl=0;pl<NumberOfPlayers;pl++){
			PlayArray[pl]=PlayerArray[pl]=npa[pl];
		}

		ThePlayers[0]=PlayArray[lp[0]];
		ThePlayers[1]=PlayArray[lp[1]];

		TheGame->SetActivePlayers();
		TheGame->ZeroStats();
		TheGame->KillAll();

		nextSaveableClass=PlayArray[0];
		int pl;
		for(pl=0;pl<NumberOfPlayers-1;pl++){
			PlayArray[pl]->nextSaveableClass=PlayArray[pl+1];
		}
		PlayArray[pl]->nextSaveableClass = TheLastRounds;
	}else{
		nextSaveableClass=NULL;
	}

}

void GameKeeper::PostSave(){
	nextSaveableClass=PlayArray[0];

	int pl;
	for(pl=0;pl<NumberOfPlayers-1;pl++){
		PlayArray[pl]->nextSaveableClass=PlayArray[pl+1];
	}
	PlayArray[pl]->nextSaveableClass = TheLastRounds;
}

