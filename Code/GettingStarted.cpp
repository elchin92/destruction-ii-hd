// GettingStarted.cpp: implementation of the GettingStarted class.
//
//////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "InputEngine.h"
#include "GettingStarted.h"


GettingStarted * TheGettingStarted;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GettingStarted::GettingStarted(){
	Initialize();
}

GettingStarted::~GettingStarted(){

}



void GettingStarted::Blit(){

}

void GettingStarted::Setup(){
}

void GettingStarted::SetupSurfaces(){
	TheGame->FixImage(&TheGame->g_pDDSBackground);
}
void GettingStarted::SetSurfaces(){
	AddBitmapAndSurface("GettingStarted", &TheGame->g_pDDSBackground);
}

void GettingStarted::InternalUpdateInput(){
	if(TrueDown(DIK_ESCAPE)||TrueDown(DIK_SPACE)){
		ReturnToMotherState();
			return;
	}
}