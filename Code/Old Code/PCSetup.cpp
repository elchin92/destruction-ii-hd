#include "PCSetup.h"
#include "Game.h"
#include "Player.h"
#include "StrConversions.h"
#include "InputEngine.h"
PCSetup * ThePCSetup;


PCSetup::PCSetup(){
	Initialize();
	InitialPlayer=NULL;
	NumberOfPlayers=0;

	LoadPlayers();

	PlayerPointer=-1;
	NextPlayer();
	Choice=0;


}
PCSetup::~PCSetup(){
	Player * DeletePlayer=NULL;
	for(Player * Temp = InitialPlayer;Temp;Temp=Temp->nextPlayer){
		if(DeletePlayer)
			delete DeletePlayer;
		DeletePlayer=Temp;
	}
	if(DeletePlayer)
		delete DeletePlayer;
}

void PCSetup::NextPlayer(){
	PlayerPointer++;
	if(PlayerPointer==NumberOfPlayers)
		PlayerPointer=0;
	ActivePlayer=PlayerCollection(PlayerPointer);
}

void PCSetup::PreviousPlayer(){
	PlayerPointer--;
	if(PlayerPointer==-1)
		PlayerPointer=NumberOfPlayers-1;
	ActivePlayer=PlayerCollection(PlayerPointer);
}

Player * PCSetup::PlayerCollection(int pl){
	Player * ReturnPlayer=InitialPlayer;
	while(pl){
		pl--;
		ReturnPlayer=ReturnPlayer->nextPlayer;
	}
	return ReturnPlayer;
}

int PCSetup::GetPlayers(){
	return NumberOfPlayers;
}

void PCSetup::LoadPlayers(){

	Beacon(8400);
	// Must only be called once!
	
	if(InitialPlayer)
		return;

	Player * LoadPlayer=NULL;
	Player * CreatePlayer=NULL;
	Beacon(8401);
	char FileName[60], Data[60];
	for(NumberOfPlayers=0;TRUE;NumberOfPlayers++){

		sprintf(FileName,"Player%i.DPI",NumberOfPlayers);
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
		if(!InitialPlayer){
			InitialPlayer=CreatePlayer;
			LoadPlayer=CreatePlayer;
		}else{
			LoadPlayer->nextPlayer=CreatePlayer;
			LoadPlayer=CreatePlayer;
		}
		
		File.close();
	}
	Beacon(8402);
	if(NumberOfPlayers<2){
		if(!NumberOfPlayers){
			CreateNewPlayer();
			InitialPlayer->SetName("Player 1");
			InitialPlayer->SetHairColor(5);
			InitialPlayer->SetBodyColor(7);
			SavePlayer(InitialPlayer,0);
		}
		CreateNewPlayer();
		InitialPlayer->nextPlayer->SetName("Player 2");
		InitialPlayer->nextPlayer->SetHairColor(9);
		InitialPlayer->nextPlayer->SetBodyColor(12);
		SavePlayer(InitialPlayer->nextPlayer,1);
		NumberOfPlayers=2;
	}
	Beacon(8403);	
}
void PCSetup::SavePlayer(Player * SavePlayer, int Slot){

	char FileName[60];
	sprintf(FileName,"Player%i.DPI",Slot);
	ofstream File(FileName);
	char temp[30];
	strcpy(temp,SavePlayer->GetName());
	File << temp << endl;
	File << SavePlayer->GetBodyColor() << endl;
	File << SavePlayer->GetHairColor() << endl;
	File << SavePlayer->GetID() << endl;
	File.close();

}

int PCSetup::ErasePlayer(){
	if(NumberOfPlayers<=2)
		return 0;
	Player * DeletePlayer = ActivePlayer;

	DeletePlayerFile(PlayerPointer);

	if(PlayerPointer){
		PreviousPlayer();
		ActivePlayer->nextPlayer=DeletePlayer->nextPlayer;
	}else{
		InitialPlayer=DeletePlayer->nextPlayer;
	}

	delete DeletePlayer;
	NumberOfPlayers--;
	if(PlayerPointer==NumberOfPlayers)
		PlayerPointer=0;
	return 1;
}
void PCSetup::DeletePlayerFile(int Slot){
	char FileName[2][60];
	sprintf(FileName[0],"Player%i.DPI",Slot);
	DeleteFile(FileName[0]);
	for(int i=1;TRUE;i++){
		strcpy(FileName[1],FileName[0]);
		sprintf(FileName[0],"Player%i.DPI",Slot+i);
		if(!MoveFile( FileName[0], FileName[1] ))
			break;
	}

}
void PCSetup::Blit(){
	if (TheGame->g_pDDSBack->GetDC(&TheGame->hdc) == DD_OK)
	{
		SelectObject(TheGame->hdc,TheGame->g_hFontBigPlus);
		SetTextAlign(TheGame->hdc, TA_CENTER);
		TheGame->Text(400,0, ActivePlayer->GetName());

		char Temp[60];
		SelectObject(TheGame->hdc,TheGame->g_hFontBig);
		SetTextColor(TheGame->hdc, ContrastAdjust(TheGame->UnSelectedColor));


		TheGame->MenuText(650,110, " Previous ",Choice==0);
		TheGame->MenuText(150,110, " Next ",Choice==1);
		SetTextAlign(TheGame->hdc, TA_LEFT);
		sprintf(Temp,"Player Name: %s",ActivePlayer->GetName());
		TheGame->MenuText(250,200, Temp,Choice==2 || Choice==3);
		sprintf(Temp,"Body Color: %s",BodyColor2Str(ActivePlayer->GetBodyColor()));
		TheGame->MenuText(250,250, Temp,Choice==4 || Choice==5);
		sprintf(Temp,"Hair Color: %s",HairColor2Str(ActivePlayer->GetHairColor()));
		TheGame->MenuText(250,300, Temp,Choice==6 || Choice==7);
		SetTextAlign(TheGame->hdc, TA_CENTER);
		TheGame->MenuText(650,390, "Save",Choice==8);
		TheGame->MenuText(150,390, "Erase",Choice==9);
		TheGame->MenuText(400,500, "Create New Player" ,Choice==10 || Choice==11);
		

		
	}
	TheGame->g_pDDSBack->ReleaseDC(TheGame->hdc);
}
void PCSetup::InternalUpdateInput(){



		
	if(TrueUp(DIK_ESCAPE)){

		SwitchState(SETTINGS);
		return;
	}
	
	if(TrueUp(DIK_RETURN)){
		if(Choice==0){
			PreviousPlayer();
			return;
		}else if(Choice==1){
			NextPlayer();
			return;
		}else if(Choice==2||Choice==3){

			return;						
		}else if(Choice==4||Choice==5){
			ActivePlayer->SwitchBodyColor();
			return;	
		}else if(Choice==6||Choice==7){
			ActivePlayer->SwitchHairColor();
			return;	
		}else if(Choice==8){
			SavePlayer(ActivePlayer, PlayerPointer);
			return;	
		}else if(Choice==9){
			if(!ErasePlayer())
				TheGame->RedMsg("There has to be at least two players!");
			return;	
		}else if(Choice==10||Choice==11){
			CreateNewPlayer();
			return;	
		}
		return;				
			
	}

	if(TrueDown(DIK_LEFT)){
			if(Choice%2)
				Choice--;
			return;
	}
	if(TrueDown(DIK_UP)){
			if(Choice>=2)
				Choice-=2;
			return;
	}
	if(TrueDown(DIK_RIGHT)){
			if(!(Choice%2))
				Choice++;
			return;
	}
	if(TrueDown(DIK_DOWN)){
			if(Choice<=9)
				Choice+=2;
			return;	
	}

}
void PCSetup::Setup(){

}
void PCSetup::SetSurfaces(){
}
void PCSetup::CreateNewPlayer(){
	ActivePlayer = new Player(0,0);
	ActivePlayer->SetName("New Player");
	ActivePlayer->SetID(GetUniqueID());
	if(InitialPlayer)
		PlayerCollection(NumberOfPlayers-1)->nextPlayer=ActivePlayer;
	else
		InitialPlayer=ActivePlayer;
	PlayerPointer=NumberOfPlayers;
	NumberOfPlayers++;
}
int PCSetup::GetUniqueID(){
	int ID = rand();
	for(int i = 0;i<NumberOfPlayers;i++)
		if(ID==PlayerCollection(i)->GetID())
			return GetUniqueID();

	return ID;
}

Player * PCSetup::GetFirstPlayer(){return InitialPlayer;}
int * PCSetup::GetNumberOfPlayersPointer(){return &NumberOfPlayers;}