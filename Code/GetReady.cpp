#include "GetReady.h"
#include "Game.h"
#include "Start.h"
#include "Player.h"
#include "DebugWindow.h"
#include "InputEngine.h"
#include "staticchar.h"
GetReady::GetReady(){
	Initialize();
}
GetReady::~GetReady(){}

void GetReady::Blit(){
	TheGame->BlitOntoBack(&TheGame->g_pDDSBackground);
	TheGame->BlankArea(0,0,500,100);
	for(int pl=0;pl<2;pl++)
		if(READY[pl])
			TheGame->BlitCoords(302, 0, 480, 65, 107+400*pl, 500, &TheGame->g_pDDSBackground);
		else
			TheGame->BlitCoords(0, 0, 300, 52, 51+400*pl, 500, &TheGame->g_pDDSBackground);
}

void GetReady::InternalUpdateInput(){


	if(TrueDown(DIK_F10))
		SwitchState(SAVE,TRUE);
	else if(TrueDown(DIK_ESCAPE)){
		SwitchState(START,TRUE);
	}else if(TrueDown(ThePlayers[0]->GetFireKey()) ){
		if(TOGGLEALLOWED)
			READY[0]=!READY[0];
	}else if(TrueDown(ThePlayers[1]->GetFireKey()) ){
		if(TOGGLEALLOWED)
			READY[1]=!READY[1];
	}

	if(READY[0]&&READY[1]&&TOGGLEALLOWED){
		SetFadeOutTime(1500);
		SwitchState(PLAY);
		TheGame->STARTNEWROUND=TRUE;
		TOGGLEALLOWED=FALSE;
	}
}
void GetReady::Setup(){

	
	SetFadeOutTime(300);

	READY[0]=FALSE;
	READY[1]=FALSE;
	TOGGLEALLOWED=TRUE;

}
void GetReady::SetSurfaces(){
	AddBitmapAndSurface(GetReadyBitmap, &TheGame->g_pDDSBackground);

}