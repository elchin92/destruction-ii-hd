// Play.cpp: implementation of the Play class.
//
//////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "Play.h"
#include "InputEngine.h"
#include "staticchar.h"

#include "Debugger.h"
#include <stdio.h>
#include "TimeKeeper.h"

Play * ThePlay;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Play::Play()
{
	Initialize();
	// So that we will begin a new round when we set a new game up
	PlayStatus=ROUNDOVER;
	MAGICDONE=FALSE;
}

Play::~Play()
{

}


void Play::InternalUpdateInput(){

	TheGame->UpdateGameInput();		

}




void Play::Blit(){

	if(ThePlay->GetPlayStatus()==PLAYING){
		if(TheGame->IsPaused()){
			DP("Blitting back and returning");
			TheGame->BlitBack();
			return;
		}
				
		if(!MAGICDONE)
			TheGame->DoMagic();

		MAGICDONE=FALSE;

		TheGame->BlitIt();

				
#ifdef DEBUGGING
dw->Blit();
#endif
					
		if(PlayStatus==PLAYING){
			while(PlayStatus==PLAYING && TheGame->g_pDDSBack->GetFlipStatus( DDGFS_ISFLIPDONE ) == DDERR_WASSTILLDRAWING ){
				while(TheInputEngine->UpdateInputState( TheGame->hWnd )!=S_OK);
				TheGame->UpdateTime();	
				TheGame->DoMagic();
				MAGICDONE=TRUE;
			}	
			
			while( TheGame->g_pDDSPrimary->Flip(0, 0) != DD_OK );				
		}

	}else{
		DP("Freezing Screen");
		TheGame->FreezeScreen();
	}

}
void Play::SetupSurfaces(){
	TheGame->InternalSetupSurfaces();
	if(TheGame->IsPaused()){
		TheGame->BlitIt();
		TheGame->COPYBACK=TRUE;
		TheGame->BlitBack();
	}
}
void Play::Setup(){
	TheGame->ESCSUPRESSED=Down(DIK_ESCAPE);
	TheTimeKeeper->SetLastCallToNow();
	TheGame->SetGameUp();
}
void Play::SetSurfaces(){
	Beacon(5334);

	AddBitmapAndSurface(TempPlayersBitmap, &TheGame->g_pDDSPlayer);
	AddBitmapAndSurface("PlayerAssembly", &TheGame->g_pDDSPlayerAssembly);
	AddBitmapAndSurface(FlamesBitmap, &TheGame->g_pDDSFlames);
	AddBitmapAndSurface("TempLevel", &TheGame->g_pDDSLevels);
	AddBitmapAndSurface(ItemBitmap, &TheGame->g_pDDSItem);
	AddBitmapAndSurface(SpritesBitmap, &TheGame->g_pDDSSprites);
	AddBitmapAndSurface(BulletBitmap, &TheGame->g_pDDSBullets);
	AddBitmapAndSurface(GrenadeBitmap, &TheGame->g_pDDSGrenade);
	AddBitmapAndSurface(DroidBitmap, &TheGame->g_pDDSDroid);
	AddBitmapAndSurface(CarBitmap, &TheGame->g_pDDSCar);
	AddBitmapAndSurface(MachineBitmap, &TheGame->g_pDDSMachine);
	AddBitmapAndSurface(TankBitmap, &TheGame->g_pDDSTank);
	AddBitmapAndSurface(RocketTankBitmap, &TheGame->g_pDDSRTank);
	AddBitmapAndSurface(MachineGunBitmap, &TheGame->g_pDDSMachineGun);

	AddBitmapAndSurface(LifeBitmap, &TheGame->g_pDDSLife);
	AddBitmapAndSurface(ELifeBitmap, &TheGame->g_pDDSEmptyLife);
	AddBitmapAndSurface(TankWarnerBitmap, &TheGame->g_pDDSTankWarner);
	AddBitmapAndSurface(BarsBitmap, &TheGame->g_pDDSBars);
	AddBitmapAndSurface(EBarsBitmap, &TheGame->g_pDDSEmptyBar);
	AddBitmapAndSurface(GUIPowerUpsBitmap, &TheGame->g_pDDSGUIPowerUps);
	AddBitmapAndSurface(BackgroundBitmap, &TheGame->g_pDDSBackground);

	if(NumberOfStateSurfaces!=MaximumNumberOfSurfaces){
		Beacon(NumberOfStateSurfaces);
		TheGame->ErrorReport(110);
	}
	Beacon(5334);
}

int Play::GetPlayStatus(){return PlayStatus;}
void Play::SetPlayStatus(int iPlayStatus){PlayStatus=iPlayStatus;DP2("PlayStatus set",PlayStatus);}