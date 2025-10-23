#include "Start.h"
#include "Game.h"
#include "GraphicMenu.h"
#include "Menu.h"
#include "Player.h"
#include "Settings.h"
#include "InputEngine.h"
#include "staticchar.h"
#include "ButtonMenuItem.h"
#include "Debugger.h"
#include "FullVersionInfo.h"

void OnNewGame(){
	DP("NewGame/Resume");
	if(TheGame->GAMEACTIVE){
		DP("Resume");
		TheGame->InterpretSettings();
		TheStart->ReturnToMotherState();
	}
	else{		
		DP("Newgame....");
		TheStart->SwitchState(PLAYERSELECTER,TRUE);
	}
}
void OnLoadGame(){
	TheStart->SwitchState(LOAD,TRUE);
}
void OnSettings(){
	TheStart->SwitchState(SETTINGS,TRUE);
}
void OnInstructions(){
	TheStart->SwitchState(HELP,TRUE);
}
void OnQuit(){
	DP("Exited through start menu");

	if(TheGame->GAMEACTIVE){
		DP("Going to stats...");
		TheStart->SwitchState(STATISTICS);
		return;
	}
#ifdef __D2PROTECTED__
	if(TheGame->ISREGISTERED){

		TheGame->Quit();
	}else{
		DP("Going to fullversion");
		TheFullVersionInfo->SetCountDown();
		TheStart->SwitchState(FULLVERSION);
	}	
#else
	TheStart->SwitchState(FULLVERSION);
#endif
}

Start * TheStart;

Start::Start(){
	Initialize();
	Beacon(2412455);
	Initialize();
	Beacon(2412456);
	ButtonMenu = new Menu();
	Beacon(2412457);
	ButtonMenu->AddMenuItem(new ButtonMenuItem(122, 108, 310, 63, 8, &TheGame->g_pDDSTemporary, OnNewGame));
	ButtonMenu->AddMenuItem(new ButtonMenuItem(117, 190, 319, 62, 8,  &TheGame->g_pDDSTemporary, OnLoadGame));
	ButtonMenu->AddMenuItem(new ButtonMenuItem(161, 274, 231, 66, 8, &TheGame->g_pDDSTemporary, OnSettings));
	ButtonMenu->AddMenuItem(new ButtonMenuItem(111, 357, 333, 60, 8, &TheGame->g_pDDSTemporary, OnInstructions));
	ButtonMenu->AddMenuItem(new ButtonMenuItem(134, 438, 289, 60, 8, &TheGame->g_pDDSTemporary, OnQuit));

	Beacon(2412458);
       
	ButtonMenu->SetNeighbours(1);

	BackgroundSurface=&TheGame->g_pDDSBackground;
		Beacon(2412460);}

Start::~Start(){
}


void Start::InternalBlit(){
#ifdef __D2PROTECTED__
	if(!TheGame->ISREGISTERED){
		for(int es=0;es<eSettings;es++){
			for(int et=0;et<eTypes;et++){
				if(TheSettings->GetEnemySetting((EnemySettingsType)es, (EnemyType)et)->GetInternalSelection() != 
					EnemyDefaultSettings[et][es]){
					Beacon(72462);
					Beacon(et);
					Beacon(es);
					Beacon(TheSettings->GetEnemySetting((EnemySettingsType)es, (EnemyType)et)->GetInternalSelection());
					Beacon(EnemyDefaultSettings[et][es]);
					DP("EnemyDefaultSettingsQuit");
					TheGame->Quit();
				}
			}
		}

			     
	}
#endif
} 

void Start::InternalUpdateInput(){

	if(TrueDown(DIK_ESCAPE)){
		if(TheGame->GAMEACTIVE)
			OnNewGame();
		return;
	}
	ButtonMenu->UpdateInput();

}
void Start::Setup(){

}
void Start::SetSurfaces(){
	AddBitmapAndSurface(StartMenuBitmap, &TheGame->g_pDDSBackground);
	AddBitmapAndSurface(StartMenuSelectedBitmap, &TheGame->g_pDDSTemporary);
}
void Start::SetResumeSurfaces(){
	StateBitmaps[0]="StartMenuR";
	StateBitmaps[1]="StartMenuRS";  // Fixed: actual file is StartMenuRS.bmp (R Selected)
}