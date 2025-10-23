#include "Settings.h"
#include "Game.h"
#include "Menu.h"
#include "MenuStyle.h"
#include "SliderMenuItem.h"
//#include "BorderMenuItem.h"
#include "InputEngine.h"
#include "staticchar.h"
#include "Debugger.h"
#include "Debugger.h"
#include <stdio.h>
#include <fstream>

using namespace std; // For ifstream, ofstream

Settings * TheSettings;

#define Dist 35

Settings::Settings(){

	SaveableClass::Initialize("Settings");
	FadingGameState::Initialize();

	CreateWideScreenUserMsgBox();

	LastIncrTime=0;


	SettingsMenu = new Menu();

	//SettingsMenu->StartNewBorder("");

	s[WS]=SettingsMenu->AddOptionItem("World Size");
	SettingsMenu->LIAddOption("Very Small");
	SettingsMenu->LIAddOption("Small",TRUE);
	SettingsMenu->LIAddOption("Normal");
	SettingsMenu->LIAddOption("Large");
	SettingsMenu->LIAddOption("Very Large");
	SettingsMenu->LIAddOption("Huge");
	SettingsMenu->LIAddOption("Gigantic");
	SettingsMenu->LIAddOption("Exaggerated");
	SettingsMenu->LILock(LOCK_TAKEEFFECTNEXTROUND);



	s[WT]=SettingsMenu->AddOptionItem("World");
	SettingsMenu->LIAddOption("Random");
	SettingsMenu->LIAddOption("Shifting");
	SettingsMenu->LIAddOption("Concrete");
	SettingsMenu->LIAddOption("Medieval");
	SettingsMenu->LIAddOption("Stonewall");
	SettingsMenu->LIAddOption("Future");
	SettingsMenu->LIAddOption("Park");
	SettingsMenu->LIAddOption("Barbwire");
	SettingsMenu->LIAddOption("Chain");
	SettingsMenu->LIAddOption("Lolipop");
	SettingsMenu->LILock(LOCK_TAKEEFFECTNEXTROUND);


	s[SM]=SettingsMenu->AddNumericalSlider("Starting Money", FREESTARTINGMONEY,0,500000,5000,"$",TRUE);
	SettingsMenu->LILock(LOCK_TAKEEFFECTNEXTGAME);


	s[IT]=SettingsMenu->AddOptionItem("Item Amount");
	SettingsMenu->LIAddOption("Few");
	SettingsMenu->LIAddOption("Normal",TRUE);
	SettingsMenu->LIAddOption("Many");
	SettingsMenu->LILock(LOCK_TAKEEFFECTNEXTROUND);


	s[DS]=SettingsMenu->AddOptionItem("DS Mode");
	SettingsMenu->LIAddOption("Off",TRUE);
	SettingsMenu->LIAddOption("On");
	SettingsMenu->LILock(LOCK_TAKEEFFECTNEXTROUND);


	s[TO]=SettingsMenu->AddOptionItem("Time Out");
	SettingsMenu->LIAddOption("Never");
	SettingsMenu->LIAddOption("Very Slow");
	SettingsMenu->LIAddOption("Slow",TRUE);
	SettingsMenu->LIAddOption("Normal");
	SettingsMenu->LIAddOption("Quick");
	SettingsMenu->LIAddOption("Very Quick");
	


	SettingsMenu->LILock(LOCK_TAKEEFFECTNEXTROUND);

	s[TT]=SettingsMenu->AddOptionItem("Terrain Type");
	SettingsMenu->LIAddOption("Light");
	SettingsMenu->LIAddOption("Moderate",TRUE);
	SettingsMenu->LIAddOption("Rough");
	SettingsMenu->LIAddOption("Random");


	SettingsMenu->LILock(LOCK_TAKEEFFECTNEXTROUND);

	s[ST]=SettingsMenu->AddOptionItem("Wall Strength");
	SettingsMenu->LIAddOption("Weak");
	SettingsMenu->LIAddOption("Normal",TRUE);
	SettingsMenu->LIAddOption("Strong");
	SettingsMenu->LIAddOption("Massive");
	SettingsMenu->LISetSelection(1);
	SettingsMenu->LILock(LOCK_TAKEEFFECTNEXTROUND);

	s[GS]=SettingsMenu->AddOptionItem("Game Speed");
	SettingsMenu->LIAddOption("Slow");
	SettingsMenu->LIAddOption("Normal",TRUE);
	SettingsMenu->LIAddOption("Fast");
	SettingsMenu->LIAddOption("Ultra");

/*
	s[XL]=SettingsMenu->AddOptionItem("Extra Lives");
	SettingsMenu->LIAddOption("Not Sold");
	SettingsMenu->LIAddOption("Available");
*/

	s[SE]=SettingsMenu->AddOptionItem("Starting Equipment");
	SettingsMenu->LIAddOption("Advanced");
	SettingsMenu->LIAddOption("Basic");
	SettingsMenu->LILock(LOCK_TAKEEFFECTNEXTGAME);



	//SettingsMenu->EndBorder();
//	SettingsMenu->StartNewBorder("");

	s[SS]=SettingsMenu->AddOptionItem("Sound");
	SettingsMenu->LIAddOption("Stereo");
	SettingsMenu->LIAddOption("Off");
	SettingsMenu->LIAddOption("Mono");
	SettingsMenu->LIAddOption("Reversed");


	s[SS]->SetForcedYPos(213);

	s[VL]=SettingsMenu->AddGraphicalSlider("Volume",MaxVolume,MaxVolume);


	SettingsMenu->AddStaticItem("");

	s[GA]=SettingsMenu->AddGraphicalSlider("Contrast",MaxGamma,MaxGamma);

//	SettingsMenu->EndBorder();
//	SettingsMenu->StartNewBorder("");

	SettingsMenu->AddSwitch("Controls Setup",CONTROLS,TRUE)->SetForcedYPos(309);

	SettingsMenu->AddSwitch("Player Setup",PLAYERSETUP,TRUE);

	//SettingsMenu->EndBorder();

	//SettingsMenu->StartNewBorder("");

	s[GT]=SettingsMenu->AddOptionItem("Game Type");
	SettingsMenu->LIAddOption("1 on 1");
	SettingsMenu->LIAddOption("Cup");
	SettingsMenu->LIAddOption("League");
	SettingsMenu->LILock(LOCK_TAKEEFFECTNEXTGAME);
	s[GT]->SetForcedYPos(379);



	s[TL]=SettingsMenu->AddNumericalSlider("Time Limit",0,0,100,1," mins");
	SettingsMenu->LILock(LOCK_TAKEEFFECTNEXTGAME);
	SettingsMenu->LISetUnlimitedZero();

	
	s[VC]=SettingsMenu->AddOptionItem("Victory Condition");
	SettingsMenu->LIAddOption("Best of");
	SettingsMenu->LIAddOption("First to");
	SettingsMenu->LILock(LOCK_TAKEEFFECTNEXTGAME);

	
	s[RS]=SettingsMenu->AddNumericalSlider("Number Of Rounds",FREENUMBEROFROUNDS,1,100);
	SettingsMenu->LISetUnlimitedZero();
	SettingsMenu->LILock(LOCK_TAKEEFFECTNEXTGAME);	

	


	//SettingsMenu->EndBorder();

	SettingsMenu->CreatePredefinedMenu(Dist,Dist-7,800-Dist,600-Dist,pSettings);

	SettingsMenu->GetMenuStyle()->SetSlideAdjust(-31);


	cgt=gtOneOnOne;


	EnemySetupMenu = new Menu();

	//EnemySetupMenu->StartNewBorder("Enemy Setup");
	EnemySetupMenu->AddStaticItem("Type");
	EnemySetupMenu->AddStaticItem("Droids");
	EnemySetupMenu->AddStaticItem("Tanks");
	EnemySetupMenu->AddStaticItem("Cars");
	EnemySetupMenu->AddStaticItem("Machines");

	EnemySetupMenu->AddStaticItem("Base Enemies");
	for(int et=0;et<eTypes;et++){
		EnemySettings[et][esBC]=EnemySetupMenu->AddNumericalSlider("",EnemyDefaultSettings[et][esBC],0,100,1);

	}

	EnemySetupMenu->AddStaticItem("Extra Enemies");
	for(et=0;et<eTypes;et++){
		EnemySettings[et][esFQ]=EnemySetupMenu->AddOptionItem();
		EnemySetupMenu->LIAddOption("Very Few");
		EnemySetupMenu->LIAddOption("Few");
		EnemySetupMenu->LIAddOption("Rather Few");
		EnemySetupMenu->LIAddOption("Normal");
		EnemySetupMenu->LIAddOption("Rather Many");
		EnemySetupMenu->LIAddOption("Many");
		EnemySetupMenu->LIAddOption("Very Many");
		EnemySetupMenu->LISetSelection(EnemyDefaultSettings[et][esFQ]);

	}

	EnemySetupMenu->AddStaticItem("Upgrade Likeliness");
	for(et=0;et<eTypes;et++){
		EnemySettings[et][esUL]=EnemySetupMenu->AddOptionItem();
		EnemySetupMenu->LIAddOption("Very Low");
		EnemySetupMenu->LIAddOption("Low");
		EnemySetupMenu->LIAddOption("Rather Low");
		EnemySetupMenu->LIAddOption("Medium");
		EnemySetupMenu->LIAddOption("Rather High");
		EnemySetupMenu->LIAddOption("High");
		EnemySetupMenu->LIAddOption("Very High");
		EnemySetupMenu->LISetSelection(EnemyDefaultSettings[et][esUL]);

	}

	//EnemySetupMenu->EndBorder();


	for(int a=0;a<eTypes;a++){
		for(int b=0;b<eSettings;b++){
			EnemySettings[a][b]->Lock(LOCK_TAKEEFFECTNEXTROUND);
		}
	}
	EnemySetupMenu->CreatePredefinedMenu(58,474,765,565,pEnemySetup);

	CreateGameSetupItems();
	s[VC]->SetDownNeighbour(EnemySettings[0][esBC]);
	s[RS]->SetDownNeighbour(EnemySettings[2][esBC]);
	CopyGameSetupLinkage();

	EnemySetupMenu->SetMotherInput(SettingsMenu->GetMotherInput());

	RegisterItem("WS",s[WS]->GetISP(),sizeof((*s[WS]->GetISP())));
	RegisterItem("WT",s[WT]->GetISP(),sizeof((*s[WT]->GetISP())));
	RegisterItem("SM",s[SM]->GetISP(),sizeof((*s[SM]->GetISP())));
	RegisterItem("IT",s[IT]->GetISP(),sizeof((*s[IT]->GetISP())));
	RegisterItem("DS",s[DS]->GetISP(),sizeof((*s[DS]->GetISP())));
	RegisterItem("TO",s[TO]->GetISP(),sizeof((*s[TO]->GetISP())));
	RegisterItem("TT",s[TT]->GetISP(),sizeof((*s[TT]->GetISP())));
	RegisterItem("ST",s[ST]->GetISP(),sizeof((*s[ST]->GetISP())));
	RegisterItem("GS",s[GS]->GetISP(),sizeof((*s[GS]->GetISP())));
	RegisterItem("SE",s[SE]->GetISP(),sizeof((*s[SE]->GetISP())));
	RegisterItem("SS",s[SS]->GetISP(),sizeof((*s[SS]->GetISP())));
	RegisterItem("VL",s[VL]->GetISP(),sizeof((*s[VL]->GetISP())));
	RegisterItem("GA",s[GA]->GetISP(),sizeof((*s[GA]->GetISP())));
	RegisterItem("GT",s[GT]->GetISP(),sizeof((*s[GT]->GetISP())));
	RegisterItem("TL",s[TL]->GetISP(),sizeof((*s[TL]->GetISP())));
	RegisterItem("VC",s[VC]->GetISP(),sizeof((*s[VC]->GetISP())));
	RegisterItem("RS",s[RS]->GetISP(),sizeof((*s[RS]->GetISP())));
	RegisterItem("PB",s[PB]->GetISP(),sizeof((*s[PB]->GetISP())));
	RegisterItem("RM",s[RM]->GetISP(),sizeof((*s[RM]->GetISP())));
	RegisterItem("IM",s[IM]->GetISP(),sizeof((*s[IM]->GetISP())));

	for(int es=0;es<eSettings;es++){
		for(int et=0;et<eTypes;et++){
			char rstr[20];
			sprintf(rstr,"ES %i/%i",es,et);
			RegisterItem(rstr,EnemySettings[et][es]->GetISP(),sizeof((*EnemySettings[et][es]->GetISP())));	
		}	
	}

}


Settings::~Settings(){
	delete SettingsMenu;
}



void Settings::SetEnemySettingsUpNeighbours(){

	EnemySettings[0][esBC]->SetUpNeighbour(scgt[cgt][0]);
	EnemySettings[1][esBC]->SetUpNeighbour(scgt[cgt][0]);
	EnemySettings[2][esBC]->SetUpNeighbour(scgt[cgt][1]);
	EnemySettings[3][esBC]->SetUpNeighbour(scgt[cgt][1]);
}


void Settings::SwitchGameType(){
	if(!s[GT]->Unlocked())
		return;
	cgt=(GameType)(((int)cgt+1)%(int)NoOfGameTypes);


	s[TL]->nextMenuItem=scgt[cgt][0];
	s[TL]->nextActiveMenuItem=scgt[cgt][0];
	s[RM]->SetLeftNeighbour(scgt[cgt][0]);
	s[GT]->SetDownNeighbour(scgt[cgt][0]);
	s[TL]->SetDownNeighbour(scgt[cgt][1]);

	SetEnemySettingsUpNeighbours();

}
void Settings::CopyGameSetupLinkage(){
	

	s[PB]->SetUpNeighbour(s[VC]->GetUpNeighbour());
	s[PB]->SetDownNeighbour(s[VC]->GetDownNeighbour());
	s[PB]->SetRightNeighbour(s[RM]);
	s[PB]->SetPos(s[VC]->GetX(),s[VC]->GetY());
	s[PB]->nextMenuItem=s[RM];
	s[PB]->nextActiveMenuItem=s[RM];

	s[IM]->SetUpNeighbour(s[VC]->GetUpNeighbour());
	s[IM]->SetDownNeighbour(s[VC]->GetDownNeighbour());
	s[IM]->SetRightNeighbour(s[RM]);
	s[IM]->SetPos(s[VC]->GetX(),s[VC]->GetY());
	s[IM]->nextMenuItem=s[RM];
	s[IM]->nextActiveMenuItem=s[RM];

	s[RM]->SetUpNeighbour(s[RS]->GetUpNeighbour());
	s[RM]->SetDownNeighbour(s[RS]->GetDownNeighbour());
	s[RM]->SetLeftNeighbour(s[PB]);
	s[RM]->SetPos(s[RS]->GetX(),s[RS]->GetY());
	s[RM]->nextMenuItem=s[RS]->nextMenuItem;
	s[RM]->nextActiveMenuItem=s[RS]->nextActiveMenuItem;

}



void Settings::CreateGameSetupItems(){


	Menu * DummyMenu = new Menu();
	Beacon(16609);

	Beacon(16610);
	s[PB]=DummyMenu->AddOptionItem("Play B Final");
	DummyMenu->LIAddOption("No");
	DummyMenu->LIAddOption("Yes");
	DummyMenu->LILock(LOCK_TAKEEFFECTNEXTGAME);


	s[RM]=DummyMenu->AddNumericalSlider("Rounds Per Match",1,1,3);
	DummyMenu->LILock(LOCK_TAKEEFFECTNEXTGAME);


	s[IM]=DummyMenu->AddNumericalSlider("Internal Meetings",1,0,100);
	DummyMenu->LILock(LOCK_TAKEEFFECTNEXTGAME);
	DummyMenu->LISetUnlimitedZero();
	Beacon(16611);

	scgt[gtOneOnOne][0]=s[VC];
	scgt[gtOneOnOne][1]=s[RS];
	scgt[gtCup][0]=s[PB];
	scgt[gtCup][1]=s[RM];
	scgt[gtLeague][0]=s[IM];
	scgt[gtLeague][1]=s[RM];

	
	// Set Style
	DummyMenu->CreatePredefinedMenu(Dist,Dist-7,800-Dist,600-Dist,pSettings);

	Beacon(16612);
}

void Settings::Blit(){
	//TheGame->BlankScreen();
	Beacon(234934);
	SettingsMenu->BlitMenu();
	Beacon(234934);
	EnemySetupMenu->BlitMenu();
	Beacon(234937);

	TheGame->rcRect.left = 0;
	TheGame->rcRect.top = 0;
	TheGame->rcRect.right = 800;
	TheGame->rcRect.bottom = 600;

	TheGame->BlitSetRc(0,0,&TheGame->g_pDDSBackground);

}


void Settings::InternalUpdateInput(){

	if(TrueDown(DIK_RETURN)&& (*SettingsMenu->pSelectedItem)==s[GT]){
		SwitchGameType();
	}
	if(TrueDown(DIK_RETURN)&& (*SettingsMenu->pSelectedItem)==s[DS]&&!s[DS]->GetInternalSelection()){
		Message("ATTENTION: Use your <switch weapon> key instead of escape to change stores!", mcGreen);
	}
	if((TrueDown(DIK_LEFT) || TrueUp(DIK_DOWN))&& (*SettingsMenu->pSelectedItem)==s[GA]&&(*SettingsMenu->pSelectedItem)->CatchesEscape()){
		Message("For optimal image, use a low monitor brightness and keep the contrast at maximum!", mcGreen);
	}
	if(TrueDown(DIK_ESCAPE)&&!(*SettingsMenu->pSelectedItem)->CatchesEscape()){
		SaveSettings();
		SwitchState(ReturnState);
		return;
	}
	SettingsMenu->UpdateInput();

}


void Settings::Setup(){

}

void Settings::SaveSettings(){

	DP("Saving settings...");
	ofstream File(SettingsFileName, ios::binary); // C++20: use ios::binary in constructor
	PreSave();
	SaveClass(&File);
	PostSave();
	File.close();
}

void Settings::LoadSettings(){

	ifstream File(SettingsFileName, ios::binary | ios::in); // C++20: ios::nocreate removed
	BYTE KeyLength=0;
	char tTopic[100];
	File.read(reinterpret_cast<char*>(&KeyLength), 1); // C++20: explicit cast needed
	File.read(tTopic,KeyLength);
	tTopic[KeyLength]='\0';
	if(!KeyLength||!TopicCheck(tTopic)){
		DP2("LoadSettings() failed",KeyLength);
		DP2("TCheck",TopicCheck(tTopic));
		return;	
	}
	DP("Loading settings...");
	DP(tTopic);
	LoadClass(&File,TRUE);

	File.close();

	SetEnemySettingsUpNeighbours();

	while(cgt!=s[GT]->GetInternalSelection())
		SwitchGameType();

	// For OptionMenuItems
	SettingsMenu->SetSelections();
	EnemySetupMenu->SetSelections();
}
void Settings::SetSurfaces(){
	AddBitmapAndSurface("SettingsBackground", &TheGame->g_pDDSBackground);
}

int Settings::GetVolumeControl(){
	return s[VL]->GetExternalSelection();
}

int Settings::GetVolumeControlMax(){
	return ((SliderMenuItem *)s[VL])->GetMax();
}
int Settings::GetGammaControl(){
	return s[GA]->GetExternalSelection();
}

int Settings::GetGammaControlMax(){
	return ((SliderMenuItem *)s[GA])->GetMax();
}
int Settings::GetValue(Setting set){
	return s[set]->GetExternalSelection();

}
void Settings::IncreaseVolume(){
	if(LastIncrTime<TheGame->thisTickCount){
		s[VL]->AddToSelection(1);
		LastIncrTime=TheGame->thisTickCount+200;
	}
}

void Settings::DecreaseVolume(){
	if(LastIncrTime<TheGame->thisTickCount){
		s[VL]->AddToSelection(-1);
		LastIncrTime=TheGame->thisTickCount+200;
	}
}
ActiveMenuItem * Settings::GetRightSideControls(){return RightSideControls;}
ActiveMenuItem * Settings::GetItem(Setting RequestedSetting){return s[RequestedSetting];}


int Settings::GetUpgradePercentage(EnemyType et){
	if(et==etNone)
		return 0;
	
	return ULp[et][EnemySettings[et][esFQ]->GetExternalSelection()];
}

int Settings::GetExtraEnemyPercentage(EnemyType et){
	if(et==etNone)
		return 0;
	
	return FQp[et][EnemySettings[et][esUL]->GetExternalSelection()];
}
int Settings::GetEnemyBaseCount(EnemyType et){
	if(et==etNone)
		return 1;
	
	return EnemySettings[et][esBC]->GetExternalSelection();
}

ActiveMenuItem * Settings::GetEnemySetting(EnemySettingsType es,EnemyType et){
	return EnemySettings[et][es];
}