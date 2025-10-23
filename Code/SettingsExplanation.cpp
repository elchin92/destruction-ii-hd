// SettingsExplanation.cpp: implementation of the SettingsExplanation class.
//
//////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "SettingsExplanation.h"
#include "InputEngine.h"




SettingsExplanation * TheSettingsExplanation;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SettingsExplanation::SettingsExplanation(){
	Initialize(2);
	SetSSFadeTime(0, 1000, 1000);
	SetSSFadeTime(1, 1000, 1000);
}

SettingsExplanation::~SettingsExplanation(){

}



void SettingsExplanation::Blit(){

}

void SettingsExplanation::Setup(){

}

void SettingsExplanation::SetSurfaces(){
	AddBitmapAndSurface("SettingsExp1", &TheGame->g_pDDSBackground);
	AddBitmapAndSurface("SettingsExp2", &TheGame->g_pDDSLife);

	
}


void SettingsExplanation::InternalUpdateInput(){
	if(TrueDown(DIK_ESCAPE)||TrueDown(DIK_SPACE)){
		ReturnToMotherState();
			return;
	}
}