// L// LeagueInfo.cpp: implementation of the LeagueInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "InfoBox.h"
#include "GameKeeper.h"
#include "InputEngine.h"
#include "LeagueInfo.h"
#include "Player.h"



LeagueInfo * TheLeagueInfo;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LeagueInfo::LeagueInfo(){
	Initialize();

	LeagueFont=CreateFont(22,
		0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, VARIABLE_PITCH,"Arial");
	LeagueBox = new InfoBox(73,173,659,362,&LeagueFont, mcWhite,TA_LEFT);

	for(int r=0;r<2;r++){
		OutOfBox[r] = new InfoBox(282+117*r,565,34,19,&LeagueFont, mcWhite,TA_CENTER);
	}

}

LeagueInfo::~LeagueInfo(){
	DeleteObject(LeagueFont);
	SAFE_DELETE(LeagueBox);
	for(int r=0;r<2;r++){
		delete OutOfBox[r];
	}
}



void LeagueInfo::Blit(){
	TheGame->BlitOntoBack(&TheGame->g_pDDSBackground);
	if(LeagueBox->BlankAndPrepareBox(0) ){
		LeagueBox->SetTemporaryInfoBoxTextAlign(TA_CENTER);
		for(int pl=0;pl<TheGameKeeper->GetNumberOfPlayers();pl++){
			Player * apl = TheGameKeeper->GetPlayer(pl);
			LeagueBox->Print(147,23*pl,apl->GetName());
			LeagueBox->Print(317,23*pl,apl->GetMatchesPlayed());
			LeagueBox->Print(383,23*pl,apl->GetMatchesWon());
			LeagueBox->Print(428,23*pl,apl->GetMatchesDraw());
			LeagueBox->Print(476,23*pl,apl->GetMatchesLost());
			LeagueBox->Print(569,23*pl,apl->GetPoints());
		}

	}LeagueBox->CloseBox();

	int ooData[2]={TheGameKeeper->GetMatchesPlayed(),TheGameKeeper->GetTotalLeagueMatches()};
	for(int r=0;r<2;r++){
		if(OutOfBox[r]->BlankAndPrepareBox(0) ){
			OutOfBox[r]->Print(0,0,ooData[r]);
		}OutOfBox[r]->CloseBox();
	}
}

void LeagueInfo::Setup(){

}

void LeagueInfo::SetSurfaces(){
	AddBitmapAndSurface("LEAGUE", &TheGame->g_pDDSBackground);
}

void LeagueInfo::InternalUpdateInput(){
	if(TrueDown(DIK_ESCAPE)){
		SwitchState(START,TRUE);
		return;
	}
	if(TrueDown(DIK_SPACE)||TrueDown(DIK_RETURN)){
			if(TheGameKeeper->GameOver()){
				//DP("Game over... League");
				SwitchState(STATISTICS);
			}else
				SwitchState(NEXTROUND);
		return;
	}
}