// LastRoundsInfo.cpp: implementation of the LastRoundsInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "ScoreTableEntry.h"
#include "Game.h"
#include "Player.h"
#include "strconversions.h"
#include "InputEngine.h"
#include "Settings.h"
#include "LastRoundsInfo.h"
#include "GameKeeper.h"
#include "Debugger.h"


LastRoundsInfo * TheLastRounds;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LastRoundsInfo::LastRoundsInfo(){

	SaveableClass::Initialize("LastRoundsInfo");
	RegisterItem("Entries",&Entries,sizeof(Entries));

	FadingGameState::Initialize();
	CurrentGame= new ScoreTableEntry(1,NULL);
	Entries=1;
	daFont=CreateFont(14,
		0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, VARIABLE_PITCH,"Arial");
}

LastRoundsInfo::~LastRoundsInfo(){
	SAFE_DELETE(CurrentGame);
	DeleteObject(daFont);
}


void LastRoundsInfo::PostLoad(int PerformLoad){
	// Make sure we call this one!!
	DP("LRI::PostLoad() called");

	if(PerformLoad){
		EraseAllSTEAndCreateNew();

		nextSaveableClass=CurrentGame;

		for(ScoreTableEntry * ste = CurrentGame;ste;ste=ste->nextTable){
			ste->nextSaveableClass=ste->nextTable;
		}
	}else{
		DP("Impossible: Lastrounds is only loaded in real load");
	}
}

void LastRoundsInfo::PostSave(){
	// Make sure we call this one!!
	DP("LRI::PostSave() called");


	nextSaveableClass=CurrentGame;

	for(ScoreTableEntry * ste = CurrentGame;ste;ste=ste->nextTable){
		ste->nextSaveableClass=ste->nextTable;
	}
}


void LastRoundsInfo::EraseAllSTEAndCreateNew(){
	SAFE_DELETE(CurrentGame);
	for(int i=0;i<Entries;i++){
		// få rätt ordning på dem, current game ska va  en maed
		CurrentGame = new ScoreTableEntry(20,  CurrentGame);
	}

}

void LastRoundsInfo::SetupSurfaces(){
}
void LastRoundsInfo::Blit(){
	TheGame->BlitOntoBack(&TheGame->g_pDDSBackground);

	if (TheGame->g_pDDSBack->GetDC(&TheGame->hdc) == DD_OK)
	{
			// Current Game hasn't begun yet!
		SetBkMode(TheGame->hdc, TRANSPARENT);
		SelectObject(TheGame->hdc,daFont);
		SetTextAlign(TheGame->hdc, TA_CENTER );	
		if(CurrentGame->nextTable)
			CurrentGame->nextTable->Blit(yLabel,TRUE);			
	}
	TheGame->g_pDDSBack->ReleaseDC(TheGame->hdc);	
}

void LastRoundsInfo::Setup(){

}

void LastRoundsInfo::SetSurfaces(){
	AddBitmapAndSurface("LastRounds", &TheGame->g_pDDSBackground);
}

void LastRoundsInfo::InternalUpdateInput(){

	if(TrueDown(DIK_ESCAPE)){
		SwitchState(START,TRUE);
		return;
	}
	if(TrueDown(DIK_SPACE)||TrueDown(DIK_RETURN)){

		if(TheGameKeeper->GetGameType()==gtLeague){
			SwitchState(LEAGUE);
		}else if(TheGameKeeper->GetGameType()==gtCup){
			SwitchState(CUP);
		}else{
			if(TheGameKeeper->GameOver()){
				DP("Game over...");
				SwitchState(STATISTICS);
			}else
				SwitchState(NEXTROUND);
		}
			return;
	}
}

void LastRoundsInfo::EndRound(int lScore, int rScore, int Winner, int RoundTime){

	CurrentGame->ReportStats(Winner, lScore, rScore, RoundTime); //
	CurrentGame = new ScoreTableEntry(CurrentGame->GetRound()+1,  CurrentGame);
	Entries++;
	
}