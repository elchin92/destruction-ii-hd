// InteractiveHelp.cpp: implementation of the InteractiveHelp class.
//
//////////////////////////////////////////////////////////////////////

#include "ButtonMenuItem.h"
#include "InteractiveHelp.h"
#include "InputEngine.h"
#include "Menu.h"
#include "Game.h"
#include "staticchar.h"
#include "Debugger.h"

InteractiveHelp * TheInteractiveHelp;


//////////////////////////////////////////////////////////////////////
// Button Functions
//////////////////////////////////////////////////////////////////////


void OnGettingStarted(){
	TheInteractiveHelp->SwitchState(GETTINGSTARTED,TRUE);
}
void OnItemEncyclopedia(){
	TheInteractiveHelp->SwitchState(ITEMENCYCLOPEDIA,TRUE);
}
void OnSettingsExplanation(){
	(TheInteractiveHelp->SwitchState)(SETTINGSEXPLANATION,TRUE);
}
void OnFullVersion(){
	TheInteractiveHelp->SwitchState(FULLVERSION,TRUE);
}
void OnDetailedInstructions(){
	TheInteractiveHelp->SwitchState(DETAILEDINSTRUCTIONS,TRUE);
}
void OnEnemyEncyclopedia(){
	TheInteractiveHelp->SwitchState(ENEMYENCYCLOPEDIA,TRUE);
}
void OnAbout(){
	TheInteractiveHelp->SwitchState(ABOUT,TRUE);
}
void OnVisitWebsite(){

	TheGame->GoToURL("http://www.destruction.nu");

}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

InteractiveHelp::InteractiveHelp(){

	Beacon(2412455);
	Initialize();
	Beacon(2412456);
	ButtonMenu = new Menu();
	Beacon(2412457);
	ButtonMenu->AddMenuItem(new ButtonMenuItem(23, 128, 377, 75, 15, &TheGame->g_pDDSLife, OnGettingStarted));
	ButtonMenu->AddMenuItem(new ButtonMenuItem(405, 72, 377, 75, 15,  &TheGame->g_pDDSTemporary, OnDetailedInstructions));
	ButtonMenu->AddMenuItem(new ButtonMenuItem(23, 239, 377, 75, 15, &TheGame->g_pDDSLife, OnItemEncyclopedia));
	ButtonMenu->AddMenuItem(new ButtonMenuItem(405, 183, 377, 75, 15, &TheGame->g_pDDSTemporary, OnEnemyEncyclopedia));
	ButtonMenu->AddMenuItem(new ButtonMenuItem(23, 350, 377, 75, 15, &TheGame->g_pDDSLife, OnSettingsExplanation));
	ButtonMenu->AddMenuItem(new ButtonMenuItem(405, 293, 377, 75, 15, &TheGame->g_pDDSTemporary, OnVisitWebsite));
	ButtonMenu->AddMenuItem(new ButtonMenuItem(23, 461, 377, 75, 15, &TheGame->g_pDDSLife, OnFullVersion));
	ButtonMenu->AddMenuItem(new ButtonMenuItem(405, 405, 377, 75, 15, &TheGame->g_pDDSTemporary, OnAbout));

	Beacon(2412458);
       
	ButtonMenu->SetNeighbours(2);

	BackgroundSurface=&TheGame->g_pDDSBackground;
		Beacon(2412459);
}

InteractiveHelp::~InteractiveHelp(){

}



void InteractiveHelp::InternalBlit(){

}

void InteractiveHelp::Setup(){
	Beacon(43634);
}

void InteractiveHelp::SetSurfaces(){
	AddBitmapAndSurface(InteractiveHelpBitmap, &TheGame->g_pDDSBackground);
	AddBitmapAndSurface(InteractiveHelpSLBitmap, &TheGame->g_pDDSLife);
	AddBitmapAndSurface(InteractiveHelpSRBitmap, &TheGame->g_pDDSTemporary);
}



void InteractiveHelp::InternalUpdateInput(){
	Beacon(256236593);
	if(TrueDown(DIK_ESCAPE)){
		ReturnToMotherState();
		return;
	}
	ButtonMenu->UpdateInput();
}