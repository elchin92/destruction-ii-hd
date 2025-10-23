#include "PlayerSelecter.h"
#include "Game.h"
#include "Player.h"
#include "MarkableMenuItem.h"
#include "ButtonMenuItem.h"
#include "Menu.h"
#include "InfoBox.h"
#include "Settings.h"
#include "InputEngine.h"
#include "GameKeeper.h"
#include "PlayerSetup.h"
#include "Debugger.h"
#include <stdio.h>

PlayerSelecter * ThePlayerSelecter;

void OnStart(){ThePlayerSelecter->StartNewGame();}



PlayerSelecter::PlayerSelecter(){
	Beacon(2462458);
	Initialize();
	Beacon(2462459);
	PlayerMenu=NULL;

	ButtonMenu = new Menu();
	b1 = ButtonMenu->AddMenuItem(new ButtonMenuItem(490, 538, 251, 49, 15, &TheGame->g_pDDSLife, OnStart));

	Beacon(2462460);
       
	//ButtonMenu->SetNeighbours(2);
	Beacon(2462461);
	BackgroundSurface=&TheGame->g_pDDSBackground;

	CreateStandardUserMsgBox();

	UZ();Zero(PreviousMarked,16);
	PreviousMarked[0]=TRUE;
	PreviousMarked[1]=TRUE;

	tFont=CreateFont(30,
		0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, VARIABLE_PITCH,"Arial");

	SelectedInfoBox = new InfoBox(59,244, 321, 99, &tFont, mcWhite,TA_CENTER);
	SelectedPlayersBox = new InfoBox(347,402, 61, 30, &tFont, mcWhite,TA_CENTER);

	strcpy(PSString[0][0],"1 on 1, single match");
	strcpy(PSString[1][0],"Cup mode");
	strcpy(PSString[2][0],"League mode");
	strcpy(PSString[0][1],"Select 2 players");
	strcpy(PSString[1][1],"Select 2, 4, 8 or 16 players");
	strcpy(PSString[2][1],"Select 2-16 players");

}
PlayerSelecter::~PlayerSelecter(){
	SAFE_DELETE(PlayerMenu);
	delete SelectedPlayersBox;
	delete SelectedInfoBox;
}

void PlayerSelecter::Setup(){
	RebuildPlayerMenu();

	TheSettings->SettingsMenu->ValidateData(LOCK_TAKEEFFECTNEXTGAME);
	TheSettings->EnemySetupMenu->ValidateData(LOCK_TAKEEFFECTNEXTGAME);
	TheSettings->SettingsMenu->ValidateData(LOCK_TAKEEFFECTNEXTROUND);
	TheSettings->EnemySetupMenu->ValidateData(LOCK_TAKEEFFECTNEXTROUND);


}


void PlayerSelecter::InternalBlit(){
	if(SelectedInfoBox->BlankAndPrepareBox(0) ){


		SelectedInfoBox->Print(0,0,PSString[AL_GT][0]);
		SelectedInfoBox->Print(0,33,"-----");
		SelectedInfoBox->Print(0,66,PSString[AL_GT][1]);


	}SelectedInfoBox->CloseBox();

	if(SelectedPlayersBox->BlankAndPrepareBox(0) ){

		char spstr[10], dchar='?';
		if(AL_GT==0)
			dchar='2';
		sprintf(spstr,"%i/%c",GetSelectedPlayers(),dchar);
		SelectedPlayersBox->Print(0,0,spstr);
	}SelectedPlayersBox->CloseBox();

	PlayerMenu->BlitMenu();

}
void PlayerSelecter::InternalUpdateInput(){
	if(TrueDown(DIK_ESCAPE)){

		for(int t=0;t<16;t++){
			PreviousMarked[t]=0;		
		}
		for(MarkableMenuItem * mmi = PlayerMenu->GetFirstMarked();mmi;mmi=mmi->nextMarked){
			PreviousMarked[mmi->GetID()]=mmi->IsMarked();
		}
		ReturnToMotherState();
		return;
	}

	DP("Calling ButtonMenu->UpdateInput()");
	ButtonMenu->UpdateInput();


}


void PlayerSelecter::SetSurfaces(){
	AddBitmapAndSurface("PlayerSelecter", &TheGame->g_pDDSBackground);
	AddBitmapAndSurface("PlayerSelecterS", &TheGame->g_pDDSLife);
	AddBitmapAndSurface("PlayerSelecterMarker", &TheGame->g_pDDSCar);
}


void PlayerSelecter::StartNewGame()
{


	if(SelectedPlayersOK()){
		TheGameKeeper->StartAddingPlayers();

		for(MarkableMenuItem * mmi = PlayerMenu->GetFirstMarked();mmi;mmi=mmi->nextMarked){
			if(mmi->IsMarked())
				TheGameKeeper->AddPlayerForNextGame(ThePlayerSetup->GetPlayer(mmi->GetID()));
		}
		

			TheGameKeeper->BeginNewGame();

			
/*				DP("lala");
			for(int i=0;i<15*8;i++){

				DP(ThePlayers[0]->GetName());
				DP(ThePlayers[1]->GetName());
				ThePlayers[0]->GivePoints(0);
				ThePlayers[1]->GivePoints(1);
				DP2("Matches Played",i+1);
				TheGameKeeper->EndRound(1);
			}

  */
		// Game must begin in order to shop, etc, etc
		TheGame->BeginNewGame();
		// The Game must have begun to interpret settings
		TheGame->InterpretSettings();	

		SwitchState(NEXTROUND);
	}
}

int PlayerSelecter::GetSelectedPlayers(){
	int SelectedPlayers=0;
	MarkableMenuItem * mmi = PlayerMenu->GetFirstMarked();
	while(mmi){
		mmi=mmi->nextMarked;
		SelectedPlayers++;
	}
	return SelectedPlayers;
}

int PlayerSelecter::SelectedPlayersOK(){
	int SelectedPlayers=GetSelectedPlayers();

	if((GameType)AL_GT==gtOneOnOne){
		if(SelectedPlayers!=2){
			Message("You must select two players!",mcRed);
			return FALSE;
		}
	}else if((GameType)AL_GT==gtLeague){
		if(SelectedPlayers<2){
			Message("You must select at least two players!",mcRed);
			return FALSE;
		}
	}else if((GameType)AL_GT==gtCup){
		if(SelectedPlayers!=2&&SelectedPlayers!=4&&SelectedPlayers!=8&&SelectedPlayers!=16){
			Message("You must select 2, 4, 8 or 16 players!",mcRed);
			return FALSE;
		}
	}
	return TRUE;
}
void PlayerSelecter::RebuildPlayerMenu(){

	SAFE_DELETE(PlayerMenu);
	PlayerMenu = new Menu();
	PlayerMenu->SetMultipleMarks();
	ActiveMenuItem * ami=NULL;
	Beacon(3432);
	Beacon(54640000+ThePlayerSetup->GetNumberOfPlayers());

	for(int t=0;t<ThePlayerSetup->GetNumberOfPlayers();t++){
		Beacon(3433);
		ami = PlayerMenu->AddMarkableItem(ThePlayerSetup->GetPlayer(t)->GetName(),0,-2,t,&TheGame->g_pDDSCar);
		if(PreviousMarked[t]){
			((MarkableMenuItem *)ami)->Mark();
		}
		Beacon(3433);
	}

	PlayerMenu->CreatePredefinedMenu(462,36,769,520,pPlayerSetup);

	PlayerMenu->SetMotherInput(ButtonMenu->GetMotherInput());
	
	ami->SetDownNeighbour(b1);
	b1->SetUpNeighbour(ami);

	(*ButtonMenu->pSelectedItem)=b1;
}

