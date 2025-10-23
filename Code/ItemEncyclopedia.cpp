// ItemEncyclopedia.cpp: implementation of the ItemEncyclopedia class.
//
//////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "staticchar.h"
#include "InputEngine.h"
#include "ItemEncyclopedia.h"


ItemEncyclopedia * TheItemEncyclopedia;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ItemEncyclopedia::ItemEncyclopedia(){
	Initialize();
}

ItemEncyclopedia::~ItemEncyclopedia(){

}



void ItemEncyclopedia::Blit(){

}

void ItemEncyclopedia::Setup(){
}

void ItemEncyclopedia::SetupSurfaces(){
	SetPaletteAndGamma(100);
 	TheGame->LoadBitmapToSurface(&TheGame->g_pDDSTemporary,ItemBitmap);
	SetPaletteAndGamma(0);
	for(int i=0,total=0;i<3;i++){
		for(int a=0;a<11&&total<32;a++,total++){
			TheGame->rcRect.left  = (total%8)*32;
			TheGame->rcRect.top   = (total/8)*32;  
			TheGame->rcRect.right =	TheGame->rcRect.left+32;
			TheGame->rcRect.bottom=	TheGame->rcRect.top +32;

			TheGame->CopyRoutineNoCC(84+246*i,156+34*a,&TheGame->g_pDDSBackground, &TheGame->g_pDDSTemporary,0);
	
		}
	}
	TheGame->g_pDDSTemporary->Release();
	TheGame->g_pDDSTemporary=NULL;

	TheGame->FixImage(&TheGame->g_pDDSBackground);
}

void ItemEncyclopedia::SetSurfaces(){
	AddBitmapAndSurface("ItemEncyclopedia", &TheGame->g_pDDSBackground);
}

void ItemEncyclopedia::InternalUpdateInput(){
	if(TrueDown(DIK_ESCAPE)||TrueDown(DIK_SPACE)){
		ReturnToMotherState();
			return;
	}
}