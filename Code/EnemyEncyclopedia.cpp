// EnemyEncyclopedia.cpp: implementation of the EnemyEncyclopedia class.
//
//////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "InputEngine.h"
#include "EnemyEncyclopedia.h"
#include "staticchar.h"

EnemyEncyclopedia * TheEnemyEncyclopedia;

const int etp = 12;

const int sc[etp][2]={
	{0,0},
	{128,0},
	{0,64},
	{128,64},
	{0,0},
	{0,0},
	{128,0},
	{256,0},
	{0,0},
	{128,0},
	{0,0},
	{128,0}
};
const int td=36;
const int dc[etp][2]={
	{478,431},
	{478,431+td},
	{478,431+2*td},
	{643,457},
	{643,457+td},
	{118,248},
	{118,248+td},
	{118,248+2*td},
	{465,244},
	{465,244+td},
	{134,508},
	{134,508+td}
};
const char * eb[etp]={
	TankBitmap,
	TankBitmap,
	TankBitmap,
	TankBitmap,
	RocketTankBitmap,
	DroidBitmap,
	DroidBitmap,
	DroidBitmap,
	CarBitmap,
	CarBitmap,
	MachineBitmap,
	MachineBitmap
};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

EnemyEncyclopedia::EnemyEncyclopedia(){
	Initialize();
}

EnemyEncyclopedia::~EnemyEncyclopedia(){

}



void EnemyEncyclopedia::Blit(){

}

void EnemyEncyclopedia::Setup(){
}

void EnemyEncyclopedia::SetupSurfaces(){

	SetPaletteAndGamma(100);
	for(int q=0;q<etp;q++){
 		TheGame->LoadBitmapToSurface(&TheGame->g_pDDSTemporary,eb[q]);

		TheGame->rcRect.left  = sc[q][0];
		TheGame->rcRect.top   = sc[q][1];  
		TheGame->rcRect.right =	TheGame->rcRect.left+32;
		TheGame->rcRect.bottom=	TheGame->rcRect.top +32;

		TheGame->CopyRoutineNoCC(dc[q][0],dc[q][1],&TheGame->g_pDDSBackground, &TheGame->g_pDDSTemporary,0);
	
		TheGame->g_pDDSTemporary->Release();
		TheGame->g_pDDSTemporary=NULL;

	}

	SetPaletteAndGamma(0);
	TheGame->FixImage(&TheGame->g_pDDSBackground);
}
void EnemyEncyclopedia::SetSurfaces(){
	AddBitmapAndSurface("EnemyEncyclopedia", &TheGame->g_pDDSBackground);
}

void EnemyEncyclopedia::InternalUpdateInput(){

	if(TrueDown(DIK_ESCAPE)||TrueDown(DIK_SPACE)){
		ReturnToMotherState();
			return;
	}
}