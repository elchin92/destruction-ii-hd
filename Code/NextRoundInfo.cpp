// NextRoundInfo.cpp: implementation of the NextRoundInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "Player.h"
#include "InfoBox.h"
#include "GameKeeper.h"
#include "InputEngine.h"
#include "NextRoundInfo.h"


NextRoundInfo * TheNextRoundInfo;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NextRoundInfo::NextRoundInfo(){
	Initialize();

	tFont=CreateFont(31,
		0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, VARIABLE_PITCH,"Arial");

	LeftPlayerBox	= new InfoBox(76,209 ,290 , 27, &tFont, mcBlue,TA_CENTER);
	RightPlayerBox	= new InfoBox(440, 209, 291, 27, &tFont, mcBlue,TA_CENTER);
	LeftPlayerSBox	= new InfoBox(194, 274, 45, 27, &tFont, mcBlue,TA_CENTER);
	RightPlayerSBox = new InfoBox(559, 274, 45, 27, &tFont, mcBlue,TA_CENTER);
	CurrentMatchBox = new InfoBox(202, 388, 405, 27, &tFont, mcBlue,TA_CENTER);
	CurrentRoundBox = new InfoBox(406, 495, 45, 27, &tFont, mcBlue,TA_CENTER);
	MatchRoundBox	= new InfoBox(545, 495, 45, 27, &tFont, mcBlue,TA_CENTER);
} 

NextRoundInfo::~NextRoundInfo(){
	DeleteObject(tFont);

	delete LeftPlayerBox;
	delete RightPlayerBox;
	delete LeftPlayerSBox;
	delete RightPlayerSBox;
	delete CurrentMatchBox;
	delete CurrentRoundBox;
	delete MatchRoundBox;
}



void NextRoundInfo::Blit(){
	TheGame->BlitOntoBack(&TheGame->g_pDDSBackground);
	if(LeftPlayerBox->BlankAndPrepareBox(0) ){

		LeftPlayerBox->Print(0,0,ThePlayers[0]->GetName());
	}LeftPlayerBox->CloseBox();

	if(RightPlayerBox->BlankAndPrepareBox(0) ){
		RightPlayerBox->Print(0,0,ThePlayers[1]->GetName());
	}RightPlayerBox->CloseBox();
	if(RightPlayerSBox->BlankAndPrepareBox(0) ){
		RightPlayerSBox->Print(0,0,TheGameKeeper->GetRoundVictories(1));
	}RightPlayerSBox->CloseBox();
	if(LeftPlayerSBox->BlankAndPrepareBox(0) ){
		LeftPlayerSBox->Print(0,0,TheGameKeeper->GetRoundVictories(0));
	}LeftPlayerSBox->CloseBox();
	if(CurrentMatchBox->BlankAndPrepareBox(0) ){
		CurrentMatchBox->Print(0,0,TheGameKeeper->GetCurrentMatchString());
	}CurrentMatchBox->CloseBox();
	if(CurrentRoundBox->BlankAndPrepareBox(0) ){

		CurrentRoundBox->Print(0,0,TheGameKeeper->GetRoundsCounter()+1);
	}CurrentRoundBox->CloseBox();
	if(MatchRoundBox->BlankAndPrepareBox(0) ){

		int rpm=TheGameKeeper->GetRoundsPerMatch();
		if(rpm)
			MatchRoundBox->Print(0,0,rpm);
		else
			MatchRoundBox->Print(0,0,"~");
	}MatchRoundBox->CloseBox();
}

void NextRoundInfo::Setup(){

}

void NextRoundInfo::SetSurfaces(){
	AddBitmapAndSurface("NextRound", &TheGame->g_pDDSBackground);
}

void NextRoundInfo::InternalUpdateInput(){

	if(TrueDown(DIK_ESCAPE)){
		SwitchState(START,TRUE);
		return;
	}

	if(TrueDown(DIK_RETURN)||TrueDown(DIK_SPACE)){
		if(ThePlayers[0]->GetCash()||ThePlayers[1]->GetCash()){
			if(AL_DS)
				SwitchState(NEWSTORE);
			else
				SwitchState(LOOKAWAY);
		}
		else
			SwitchState(GETREADY);
	}
}
