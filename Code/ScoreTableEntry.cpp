#include "Game.h"
#include "StrConversions.h"
#include "ScoreTableEntry.h"
#include "StrConversions.h"
#include "Player.h"
#include <stdio.h>

const int kstats[5]={M_PL,M_DR,M_TK,M_CA,M_MA};

ScoreTableEntry::ScoreTableEntry(int iRound,  ScoreTableEntry * inextTable){
	SaveableClass::Initialize("ScoreTableEntry");
	RegisterItem("Kills",&Kills[0][0],sizeof(Kills));
	RegisterItem("TimePlayed",&TimePlayed,sizeof(TimePlayed));
	RegisterItem("Round",&Round,sizeof(Round));
	RegisterItem("Score",&Score[0],sizeof(Score));
	RegisterItem("LeftPlayer",&LeftPlayer[0],sizeof(LeftPlayer));
	RegisterItem("RightPlayer",&RightPlayer[0],sizeof(RightPlayer));
	RegisterItem("WinnerName",&WinnerName[0],sizeof(WinnerName));

	Round=iRound;
	nextTable=inextTable;
	UZ2();

	Zero2d(Kills,2,NumberOfMortals);
}
ScoreTableEntry::~ScoreTableEntry(){
	SAFE_DELETE(nextTable);
}



void ScoreTableEntry::ReportStats(int Winner, int lScore, int rScore, int RoundTime){
	Score[0]=lScore;
	Score[1]=rScore;
	TimePlayed=RoundTime;
	if(Winner==2)
		strcpy(WinnerName,"Draw");
	else
		strcpy(WinnerName,ThePlayers[Winner]->GetName());

	strcpy(LeftPlayer,ThePlayers[0]->GetName());
	strcpy(RightPlayer,ThePlayers[1]->GetName());

}

void ScoreTableEntry::ReportKill(int pl, MortalType MType){Kills[pl][MType]++;}

void ScoreTableEntry::Blit(int y, int Highlighted){
	if(y> 800 -24)
		return;

	if(Highlighted)
		SetTextColor(TheGame->hdc, cgs->ContrastAdjust(144, 0, 255));
	else
		SetTextColor(TheGame->hdc, cgs->ContrastAdjust(0, 0, 189));



	TheGame->IntText(41,y, Round );

	TheGame->Text(120,y, LeftPlayer );
	TheGame->Text(181,y, "vs." );
	TheGame->Text(241,y, RightPlayer );
	TheGame->Text(356,y, WinnerName );

	char sstr[20];
	sprintf(sstr,"(%i-%i)",Score[0],Score[1]);
	TheGame->Text(425,y, sstr );

	TheGame->Text(489,y, Dword2TimeStr((DWORD) 1000*TimePlayed));
	
	int x=543;
	for(int pl=0;pl<2;pl++){
		for(int i=0; i<5;i++){
			TheGame->IntText(x,y, Kills[pl][kstats[i]] );
			x+=22;
		}
		x+=23;
	}

	if(nextTable && y+12< 800-24)
		nextTable->Blit(y+12);
}
int ScoreTableEntry::GetRound(){return Round;}