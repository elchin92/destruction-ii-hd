#include "About.h"
#include "Game.h"
#include "InputEngine.h"
#include "staticchar.h"

About::About(){
	Initialize();
}
About::~About(){}

void About::Blit(){
	TheGame->BlitOntoBack(&TheGame->g_pDDSBackground);	
}
void About::InternalUpdateInput(){

	if(TrueDown(DIK_ESCAPE)||TrueDown(DIK_SPACE)){
		ReturnToMotherState();
			return;
	}
}
void About::Setup(){

}
void About::SetSurfaces(){
	AddBitmapAndSurface(AboutBitmap, &TheGame->g_pDDSBackground);

}