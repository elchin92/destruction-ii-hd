#include "LookAway.h"
#include "Start.h"
#include "Game.h"
#include "InputEngine.h"
#include "staticchar.h"
LookAway::LookAway(){
	KEEPSETTINGS=FALSE;
	State=1;
	LABitmaps[0]=RightLookAwayBitmap;
	LABitmaps[1]=LeftLookAwayBitmap;
	Initialize();
}
LookAway::~LookAway(){}

void LookAway::Blit(){

}
void LookAway::InternalUpdateInput(){
	if(TrueDown(DIK_ESCAPE)){
		SwitchState(START, TRUE);
	}else if(TrueDown(DIK_SPACE)){
		SwitchState(NEWSTORE);
		KEEPSETTINGS=FALSE;
	}
}
void LookAway::Setup(){
	if(!KEEPSETTINGS){
		StateBitmaps[0]=LABitmaps[State];
		State=!State;
		KEEPSETTINGS=TRUE;
	}
	SetPaletteAndGamma(0);   

	TheGame->FixImage(&TheGame->g_pDDSTemporary);

}
void LookAway::SetSurfaces(){
	AddBitmapAndSurface(LABitmaps[0], &TheGame->g_pDDSTemporary);
}
