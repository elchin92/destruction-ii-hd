// PlayerSetup.cpp: implementation of the PlayerSetup class.
//
//////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "InputEngine.h"
#include "PlayerSetup.h"
#include "ButtonMenuItem.h"
#include "Player.h"
#include "Menu.h"
#include "InfoBox.h"
#include "MarkableMenuItem.h"
#include "TextBoxMenuItem.h"
#include "staticchar.h"
#include "BodyAndHairColor.h"
#include "Debugger.h"
#include <stdio.h>
#include <fstream>

PlayerSetup * ThePlayerSetup;

const char * PlayerDir = "Players";

void OnNew(){ThePlayerSetup->CreateNewPlayer();	ThePlayerSetup->RebuildPlayerMenu();}
void OnErase(){ThePlayerSetup->ErasePlayer();ThePlayerSetup->RebuildPlayerMenu();}
void OnBody(){ThePlayerSetup->GetMarkedPlayer()->SwitchBodyColor();}
void OnHair(){ThePlayerSetup->GetMarkedPlayer()->SwitchHairColor();}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PlayerSetup::PlayerSetup(){
	Initialize();
	PlayerPointer=0;
	NumberOfPlayers=0;
	PlayersMenu = NULL;
	UZ();Zero(PlayerArray,16);

	LoadPlayers();

	ButtonMenu = new Menu();
	Beacon(2412457);
	
	b1 = ButtonMenu->AddMenuItem(new ButtonMenuItem(24, 387, 186, 56,  14, &TheGame->g_pDDSLife, OnHair));
	ButtonMenu->LILock(LOCK_NOTWHILEGAMEACTIVE);
	b2 = ButtonMenu->AddMenuItem(new ButtonMenuItem(240, 387, 186, 56, 14,  &TheGame->g_pDDSLife, OnBody));
	ButtonMenu->LILock(LOCK_NOTWHILEGAMEACTIVE);
	b3 = ButtonMenu->AddMenuItem(new ButtonMenuItem(458, 538, 139, 49, 14, &TheGame->g_pDDSLife, OnNew));
	ButtonMenu->LILock(LOCK_NOTWHILEGAMEACTIVE);
	b4 = ButtonMenu->AddMenuItem(new ButtonMenuItem(629, 538, 139, 49, 14,  &TheGame->g_pDDSLife, OnErase));
	ButtonMenu->LILock(LOCK_NOTWHILEGAMEACTIVE);
	b5 = (TextBoxMenuItem *)ButtonMenu->AddMenuItem(new TextBoxMenuItem(155, 225, 239, 32, 34, FW_BOLD, " "));
	ButtonMenu->LILock(LOCK_NOTWHILEGAMEACTIVE);

	b1->SetRightNeighbour(b2);
	b2->SetLeftNeighbour(b1);
	b1->SetDownNeighbour(b3);
	b2->SetDownNeighbour(b3);
	b3->SetRightNeighbour(b4);
	b3->SetLeftNeighbour(b2);
	b4->SetLeftNeighbour(b3);
	b1->SetUpNeighbour(b5);
	b2->SetUpNeighbour(b5);
	b5->SetDownNeighbour(b2);

	Beacon(2412458);
       
	BackgroundSurface=&TheGame->g_pDDSBackground;

	CreateStandardUserMsgBox();


}

PlayerSetup::~PlayerSetup(){
	for(int d=0;d<16;d++){
		SAFE_DELETE(PlayerArray[d]);
	}
	SAFE_DELETE(PlayersMenu);
}


Player ** PlayerSetup::EraseAllPlayersAndCreateNew(int NewPlayers){


	for(int d=0;d<16;d++){
		SAFE_DELETE(PlayerArray[d]);
	}
	Player * pPlayer=NULL;
	for(d=0;d<NewPlayers;d++){
		PlayerArray[d] = new Player(0,0);
		PlayerArray[d]->SetID(d);
		if(pPlayer)
			pPlayer->nextSaveableClass=PlayerArray[d];
		pPlayer=PlayerArray[d];
	}

	NumberOfPlayers=NewPlayers;
	PlayerPointer=0;

	return PlayerArray;
}
void PlayerSetup::InternalBlit(){
	PlayersMenu->BlitMenu();

	TheGame->LoadBitmapToSurface(&TheGame->g_pDDSTemporary, "Temp32");

	TheGame->rcRect.left = 0;
	TheGame->rcRect.top = 64;
	TheGame->rcRect.right = 32;
	TheGame->rcRect.bottom = 96;

	TheGame->BlitOntoSetRc(0,0, &TheGame->g_pDDSTemporary,&TheGame->g_pDDSPlayer);

	ScaleTemporary(GetMarkedPlayer());

	TheGame->rcRect.left = 0;
	TheGame->rcRect.top = 0;
	TheGame->rcRect.right = 32;
	TheGame->rcRect.bottom = 32;

	TheGame->BlitSetRc(215, 310, &TheGame->g_pDDSTemporary);

	TheGame->g_pDDSTemporary->Release();
	TheGame->g_pDDSTemporary=NULL;

}

void PlayerSetup::Setup(){

	RebuildPlayerMenu();
	ButtonMenu->SetChoice(b3);
}


void PlayerSetup::RebuildPlayerMenu(){

	SAFE_DELETE(PlayersMenu);
	PlayersMenu = new Menu();

	ActiveMenuItem * ami=NULL;
Beacon(3432);
PlayerArray[0]->GetName();
Beacon(2342);

	for(int t=0;t<NumberOfPlayers;t++){
		Beacon(3433);
		ami = PlayersMenu->AddMarkableItem(PlayerArray[t]->GetName(),0,-2,t,&TheGame->g_pDDSCar);
		ami->SetLeftNeighbour(b5);
		if(t==PlayerPointer){
			((MarkableMenuItem *)ami)->Mark();
		}
		Beacon(3433);
	}
	ami->SetDownNeighbour(b3);

	SetNeighboursToMarked();

	PlayersMenu->CreatePredefinedMenu(462,36,769,520,pPlayerSetup);

	PlayersMenu->SetMotherInput(ButtonMenu->GetMotherInput());

	(*ButtonMenu->pSelectedItem)=b3;

}
void PlayerSetup::SetNeighboursToMarked(){
	ActiveMenuItem * ami = PlayersMenu->GetFirstMarked();

	b2->SetRightNeighbour(ami);
	b3->SetUpNeighbour(ami);
	b4->SetUpNeighbour(ami);
	b5->SetRightNeighbour(ami);
}

void PlayerSetup::SetSurfaces(){
	AddBitmapAndSurface("PlayerSetup", &TheGame->g_pDDSBackground);
	AddBitmapAndSurface("PlayerSetupS", &TheGame->g_pDDSLife);
	AddBitmapAndSurface("PlayerSetupMarker", &TheGame->g_pDDSCar);
	AddBitmapAndSurface(PlayerBitmap, &TheGame->g_pDDSPlayer);
}

void PlayerSetup::InternalUpdateInput(){
	if(TrueDown(DIK_ESCAPE)){

		if(!TheGame->GAMEACTIVE){
			DP2("XCW Saving players...", NumberOfPlayers);
			SavePlayers();
		}else{DP("XCW Game active, not saving");}


		ReturnToMotherState();
			return;
	}
	PlayerPointer=PlayersMenu->GetFirstMarked()->GetID();
	b5->SetStr(GetMarkedPlayer()->GetName());
	ButtonMenu->UpdateInput();
}




// Player Collection related



Player * PlayerSetup::GetPlayer(int pl){return PlayerArray[pl];}

int PlayerSetup::GetNumberOfPlayers(){
	return NumberOfPlayers;
}

void PlayerSetup::LoadPlayers(){

	Beacon(8400);
	// Must only be called once!
	
	if(NumberOfPlayers)
		return;

	Player * LoadPlayer=NULL;
	Player * CreatePlayer=NULL;
	Beacon(8401);
	char FileName[60], Data[60];
	for(NumberOfPlayers=0;NumberOfPlayers<16;NumberOfPlayers++){
		Beacon(8401100+NumberOfPlayers);
		sprintf(FileName,"%s\\Player%i.DPI",PlayerDir,NumberOfPlayers);
		ifstream File(FileName, ios::nocreate, filebuf::openprot );
		File.getline(Data,60,'\n');
		if(!strcmp(Data,""))
			break;

		CreatePlayer = new Player(0,0);
		CreatePlayer->SetName(&Data[0]);

		File.getline(Data,60,'\n');
		CreatePlayer->SetBodyColor(atoi(Data));

		File.getline(Data,60,'\n');
		CreatePlayer->SetHairColor(atoi(Data));
		File.getline(Data,60,'\n');

		CreatePlayer->SetID(atoi(Data));
		
		File.close();

		PlayerArray[NumberOfPlayers]=CreatePlayer;
	}
	Beacon(8402);
	DP2("XCW Found # players", NumberOfPlayers);
	if(NumberOfPlayers<2){

		Beacon(8403);
		if(!NumberOfPlayers){
			CreateNewPlayer();
			PlayerArray[0]->SetName("Player 1");
			PlayerArray[0]->SetBodyColor(5);
			PlayerArray[0]->SetHairColor(10);
		}
		CreateNewPlayer();
		PlayerArray[1]->SetName("Player 2");
		PlayerArray[1]->SetBodyColor(10);
		PlayerArray[1]->SetHairColor(11);
		SavePlayers();
	}

	Beacon(8404);	
	Beacon(84040000+NumberOfPlayers);	
}

void PlayerSetup::SavePlayers(){
	CreateDirectory(PlayerDir,0);
	for(int pl=0;pl<NumberOfPlayers;pl++)
		SavePlayer(PlayerArray[pl], pl);
}
void PlayerSetup::SavePlayer(Player * sPlayer, int Slot){

	char FileName[60];

	sprintf(FileName,"%s\\Player%i.DPI",PlayerDir,Slot);
	ofstream File(FileName);
	char temp[30];
	strcpy(temp,sPlayer->GetName());
	File << temp << endl;
	File << sPlayer->GetBodyColor() << endl;
	File << sPlayer->GetHairColor() << endl;
	File << sPlayer->GetID() << endl;
	File.close();

}

int PlayerSetup::ErasePlayer(){
	if(NumberOfPlayers<=2)
		return 0;

	DeletePlayerFile(PlayerPointer);
	DP("Deleting player...");
	SAFE_DELETE(PlayerArray[PlayerPointer]);
	DP("Player deleted");

	NumberOfPlayers--;
	// Move down other players
	for(int pl=PlayerPointer;pl<NumberOfPlayers;pl++){
		PlayerArray[pl]=PlayerArray[pl+1];
		PlayerArray[pl]->SetID(pl);
	}

	PlayerArray[pl]=NULL;

	if(PlayerPointer==NumberOfPlayers)
		PlayerPointer--;

	SavePlayers();

	return 1;
}
void PlayerSetup::DeletePlayerFile(int Slot){
	char FileName[2][60];
	sprintf(FileName[0],"%s\\Player%i.DPI",PlayerDir,Slot);
	DeleteFile(FileName[0]);
	for(int i=1;TRUE;i++){
		strcpy(FileName[1],FileName[0]);
		sprintf(FileName[0],"%s\\Player%i.DPI",PlayerDir,Slot+i);
		if(!MoveFile( FileName[0], FileName[1] ))
			break;
	}

}


void PlayerSetup::CreateNewPlayer(){

	if(NumberOfPlayers==16)
		return;

	PlayerPointer=NumberOfPlayers;
	PlayerArray[PlayerPointer] = new Player(0,0);
	char str[20];
	sprintf(str,"Player %i",NumberOfPlayers+1);
	PlayerArray[PlayerPointer]->SetName(str);

	NumberOfPlayers++;

}

Player * PlayerSetup::GetMarkedPlayer(){return PlayerArray[PlayerPointer];}


void PlayerSetup::ScaleTemporary(Player * pl){

	TheGame->rcRect.left = 0;
	TheGame->rcRect.top = 0;
	TheGame->rcRect.right = 32;
	TheGame->rcRect.bottom = 32;
	
	// Set Hair colors
	TheGame->ScaleSurface(&TheGame->g_pDDSTemporary,76,HairScales[pl->GetHairColor()][0],HairScales[pl->GetHairColor()][1],4);
	// Set Body and Aim colors
	TheGame->ScaleSurface(&TheGame->g_pDDSTemporary,105,BodyScales[pl->GetBodyColor()][0],BodyScales[pl->GetBodyColor()][1],7);
}
