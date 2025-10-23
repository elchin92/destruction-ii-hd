#include "ScoreTable.h"
#include "ScoreTableEntry.h"
#include "Game.h"
#include "Player.h"
#include "strconversions.h"
#include "InputEngine.h"
#include "Settings.h"

ScoreTable * TheScoreTable;


ScoreTable::ScoreTable(){
	Initialize();
	CurrentGame= new ScoreTableEntry(1,NULL);

}
ScoreTable::~ScoreTable(){}

void ScoreTable::Blit(){

		TheGame->BlankScreen();
				
		// Print messages

}
void ScoreTable::InternalUpdateInput(){
	if(TrueUp(DIK_SPACE)||TrueUp(DIK_ESCAPE)){

		int QuitGame=FALSE;

		if(AL_VC){
			//First to
			QuitGame=QuitGame|| 
			(ThePlayers[0]->GetStat(ST_ROUNDSWON) >= AL_RS || 
			 ThePlayers[1]->GetStat(ST_ROUNDSWON) >= AL_RS);
		}else{
			// Best of
			DP("Best Of");
			QuitGame=QuitGame||
				(TheGame->RoundsCounter>=AL_RS);
		}

		DP2("Rounds Counter",TheGame->RoundsCounter);
		DP2("AL_RS",AL_RS);
		if(AL_RS==0){
			// Unlimited Rounds
			QuitGame=FALSE;
		}
		

		if(QuitGame){
			SwitchState(STATISTICS);
			return;
		}
		if(AL_DS)
			SwitchState(NEWSTORE);
		else
			SwitchState(LOOKAWAY);
	
	}
}
void ScoreTable::Setup(){
}
void ScoreTable::SetSurfaces(){
}
void ScoreTable::EndRound(int lScore, int rScore, int Winner, int RoundTime){

	CurrentGame->ReportWinner(ThePlayers[0]->GetName());
	CurrentGame->ReportTime(RoundTime);
	CurrentGame->ReportLivesLeft(ThePlayers[0]->GetLives());

	CurrentGame = new ScoreTableEntry(CurrentGame->GetRound()+1, CurrentGame);
	
}