#include "Controls.h"
#include "Game.h"
#include "Player.h"
#include "Menu.h"
#include "InfoBox.h"
#include "MenuStyle.h"
#include "Settings.h"
#include "Staticchar.h"
#include "DIK_CODES.h"
#include "InputEngine.h"
#include "Functions.h"
#include "SelectKeyMenuItem.h"
#include "ButtonMenuItem.h"
#include "Debugger.h"
#include <stdio.h>
#include <fstream>

using namespace std;

Controls * TheControls;


void OnSwitch(){TheControls->SwitchKeys();}
void OnPrint(){TheControls->PrintKeys();}
Controls::Controls(){
	ControlsMenu[0] = NULL;
	ControlsMenu[1] = NULL;
	Initialize();

	NextJoyCheck=0;
	lJoysUsed=-1;

	ButtonMenu = new Menu();
	Beacon(2412457);
	
	ActiveMenuItem * b1 = ButtonMenu->AddMenuItem(new ButtonMenuItem(386, 251, 29, 180, 15, &TheGame->g_pDDSLife, OnSwitch));
	ActiveMenuItem * b2 = ButtonMenu->AddMenuItem(new ButtonMenuItem(445, 538, 333, 49, 15,  &TheGame->g_pDDSLife, OnPrint));


	b1->SetDownNeighbour(b2);
	b2->SetLeftNeighbour(b1);
	Beacon(2412458);      

	BackgroundSurface=&TheGame->g_pDDSBackground;

	CreateStandardUserMsgBox();
}
Controls::~Controls(){
	SAFE_DELETE(ControlsMenu[0]);
	SAFE_DELETE(ControlsMenu[1]);
}

void Controls::InternalBlit(){
	UpdateJoyInfo();
	for(int i=0;i<2;i++){
		Side=i;
		ControlsMenu[i]->BlitMenu();
	}

}
void Controls::InternalUpdateInput(){

	if(TrueDown(DIK_ESCAPE)&&!( * (ButtonMenu->pSelectedItem) )->CatchesEscape()){
		SaveControls();
		TheInputEngine->SetDigital();
//		SwitchState(SETTINGS);
		SwitchState(ReturnState);
		return;
	}
	
	ButtonMenu->UpdateInput();
}

int Controls::GetCurrentSideKey(int id){

	DP2("Getting id",id);
	DP2("Getting Key",Keys[Side][id]);
	DP2("Side",Side);
	return Keys[Side][id];
}
int Controls::GetKey(int id, int iSide){
	return Keys[iSide][id];
}

void Controls::SetKey(int id, int Key,int iSide){

	Keys[iSide][id]=Key;
}


const char * Controls::GetCurrentSideKeyName(int id){
	return DIK2STR(GetCurrentSideKey(id));

}
void Controls::SetupSurfaces(){
	TheGame->FixImage(&TheGame->g_pDDSBackground);
}
void Controls::Setup(){

	Beacon(52934);

	LoadControls();
	Beacon(52935);
	for(int i=0;i<2;i++){
		Side=i;
		SAFE_DELETE(ControlsMenu[i]);
		Beacon(52936);
		ControlsMenu[i] = new Menu();
		Beacon(52937);
		for(int a=0;a<INGAMEKEYS;a++){
			ActiveMenuItem * added = ControlsMenu[i]->AddSelectKey(KeyNames[a], a,i);
			ControlsMenu[i]->LISetSelection(Keys[Side][a]);
			if(i){
				added->SetLeftNeighbour(ButtonMenu->GetActiveMenuItem(0));
			}else{
				added->SetRightNeighbour(ButtonMenu->GetActiveMenuItem(0));	
			}

		}
		Beacon(52938);
		ControlsMenu[i]->CreatePredefinedMenu(73+376*i,181,375+376*i,506,pControls);
		Beacon(52939);
		ControlsMenu[i]->GetMenuStyle()->SetSelectKeyAdjust(177);
		ControlsMenu[i]->SetMotherInput(ButtonMenu->GetMotherInput());
	}

	ButtonMenu->GetActiveMenuItem(0)->SetLeftNeighbour(ControlsMenu[0]->GetActiveMenuItem((INGAMEKEYS-1)/2));
	ButtonMenu->GetActiveMenuItem(0)->SetRightNeighbour(ControlsMenu[1]->GetActiveMenuItem((INGAMEKEYS-1)/2));
	ButtonMenu->GetActiveMenuItem(1)->SetUpNeighbour(ControlsMenu[1]->GetActiveMenuItem(INGAMEKEYS-1));
	ControlsMenu[0]->GetActiveMenuItem(INGAMEKEYS-1)->SetDownNeighbour(ButtonMenu->GetActiveMenuItem(1));
	ControlsMenu[1]->GetActiveMenuItem(INGAMEKEYS-1)->SetDownNeighbour(ButtonMenu->GetActiveMenuItem(1));

	ButtonMenu->SetChoice(ControlsMenu[0]->GetActiveMenuItem(0));
	Beacon(52940);

	Beacon(52941);

	UpdateJoyInfo();
}
void Controls::UpdateJoyInfo(){

	if(TheGame->thisTickCount<NextJoyCheck)
		return;

	NextJoyCheck=TheGame->thisTickCount+1000;
	TheInputEngine->LookForJoysticks();
	int ju=TheInputEngine->GetJoysUsed();
	if(ju!=lJoysUsed){
		if(ju==0)
			Message("No joysticks found",mcBlue);
		else if(ju==1)
			Message("One joystick detected!",mcBlue);
		else if(ju==2)
			Message("Two joysticks detected!",mcBlue); 
	}
	lJoysUsed=ju;

}

void Controls::LoadControls(){

	for(int rad=0;rad<INGAMEKEYS;rad++){
		Keys[0][rad]=0;
		Keys[1][rad]=0;
	}

	char Key[12];
	ifstream File("Data\\Keys.config");
	
	File.getline(Key,10,'\n');
	if(!strcmp(Key,"KBD")){	
		for(int ra=0;ra<INGAMEKEYS;ra++){
			if(!File.getline(Key,10,'\n'))
				break;
			Keys[0][ra]=atoi(Key);
			if(!File.getline(Key,10,'\n'))
				break;
			Keys[1][ra]=atoi(Key);
		}	
	}

	File.close();
}

void Controls::SetControls(Player * LeftPlayer, Player * RightPlayer){
	DP("Controls set");
	//TheGame->PlaySound(sCash);
	for(int rad=0;rad<INGAMEKEYS;rad++){
		 LeftPlayer->SetKey(rad, Keys[0][rad]);
		RightPlayer->SetKey(rad, Keys[1][rad]);
	}	

}

void Controls::SaveControls(){
	ofstream File("Data\\Keys.config");
			
	File << "KBD" << endl;
			
	for(int ra=0;ra<14;ra++){
		File << Keys[0][ra] << endl;
		File << Keys[1][ra] << endl;
	}
	File.close();
}

int Controls::KeyFree(int Key, int id, int side){

	for(int a=0;a<ReservedKeys;a++){
		if(Reserved[a]==Key){
			
			sprintf(Msg,"%s is reserved for functionality",DIK2STR(Key));
			iMessage(mcRed);
			return 0;
		}
	}

	for(int b=0;b<INGAMEKEYS;b++){
		for(int aSide=0;aSide<2;aSide++){
			if(Keys[aSide][b]==Key){
				if(!(id==b && aSide == side )){ // Set the same key to the same value is supposed to be allowed
					Message("This key is already in use", mcRed);
					return 0;
				}
			}
		}
	}
	return 1;
}

void Controls::SetSurfaces(){
	AddBitmapAndSurface("Controls", &TheGame->g_pDDSBackground);	
	AddBitmapAndSurface("ControlsS", &TheGame->g_pDDSLife);
}

void Controls::SwitchKeys(){
	SwitchArray((int *)Keys[0],(int *)Keys[1],INGAMEKEYS);
}

const int np=4;

const char * Ports[np]={"LPT1:","LPT2:","LPT3:","LPT4:"};

void Controls::PrintKeys(){

	Message("Printed to Layout.txt", mcBlue);
	ofstream print("Layout.txt"); // stream variable declaration

	DP("About to print");
	// Print Text (the character �\f� will produce a form feed)
	print << "Destruction II controls layout\n------------------------------\n";
	DP("printed on..");
	print << " Left Player\n\n";
	DP("printed left player");
	int x; // C++20: declare before loops so variable is accessible in both
	for(x=0;x<INGAMEKEYS;x++){
		print << KeyNames[x] << ": " << DIK2STR(Keys[0][x]) << endl;
	}
	print << "\n Right Player\n\n";

	for(x=0;x<INGAMEKEYS;x++){
		print << KeyNames[x] << ": " << DIK2STR(Keys[1][x]) << endl;
	}

	print.close(); // close stream

/*	
	ofstream print; // stream variable declaration

	int i=0;
	int PrintLineFeed=TRUE;
	for(;i<np;i++){
		print.clear();
		print.open(Ports[i],ios::out); // open stream
		if(!print.fail())
			break;
	}


	if(print.fail()){
		DP("No printer found");
		Message("Printed to Layout.txt", mcBlue);
		print.clear();
		print.open("Layout.txt");
		PrintLineFeed=FALSE;
	}else{
	   	DP("Port opened");
		DP( Ports[i]);
		sprintf(Msg,"Control layout printed on port %s %i!",Ports[i], i);
		iMessage(mcGreen);
	}


	DP("About to print");
	// Print Text (the character �\f� will produce a form feed)
	print << "Destruction II controls layout\n------------------------------\n";
	DP("printed on..");
	print << " Left Player\n\n";
	DP("printed left player");
	for(int x=0;x<INGAMEKEYS;x++){
//		print << "Button 10 buba" << "nananannanannananna" << '\n'; // buba
		print << KeyNames[x] << ": " << DIK2STR(Keys[0][x]) << '\n';
		DP2("x",x);
	}
	print << "\n Right Player\n\n";
	DP("faen");
	for(x=0;x<INGAMEKEYS;x++){
		DP(DIK2STR(Keys[1][x]));
		print << KeyNames[x] << ": " << DIK2STR(Keys[1][x]) << '\n';
				DP2("y",x);
	}

	DP("one");
	if(PrintLineFeed){
		DP("lf");
		BYTE lf=12;
		print.write(&lf,1);
	}
	DP("closing stream");
	print.close(); // close stream
	DP("Printed and closed stream");
*/

}