// DetailedInstructions.cpp: implementation of the DetailedInstructions class.
//
//////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "InputEngine.h"
#include "DetailedInstructions.h"


DetailedInstructions * TheDetailedInstructions;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DetailedInstructions::DetailedInstructions(){
	Initialize(2);
}

DetailedInstructions::~DetailedInstructions(){

}



void DetailedInstructions::Blit(){

}

void DetailedInstructions::Setup(){

}

void DetailedInstructions::SetSurfaces(){
	AddBitmapAndSurface("DetailedInstructions1", &TheGame->g_pDDSBackground);
	AddBitmapAndSurface("DetailedInstructions2", &TheGame->g_pDDSLife);
}

void DetailedInstructions::InternalUpdateInput(){
	if(TrueDown(DIK_ESCAPE)||TrueDown(DIK_SPACE)){
		ReturnToMotherState();
			return;
	}
}