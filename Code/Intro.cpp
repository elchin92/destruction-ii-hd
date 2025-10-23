#include "Intro.h"
#include "Game.h"
#include "Graphicseffects.h"
#include "InputEngine.h"
#include "staticchar.h"
#include "Debugger.h"


Intro * TheIntro;

Intro::Intro(){
	Initialize(3);
	SetSSFadeTime(0, 5000, 3000,1);
	SetSSFadeTime(1, 1500, 1500,1);
	SetSSFadeTime(2, 3000, 1500,1);
	NextState=START;
}
Intro::~Intro(){}

void Intro::Blit(){

}
void Intro::InternalUpdateInput(){

	if(TrueUp(DIK_ESCAPE)||TrueUp(DIK_SPACE)){
		SwitchState(START);
			return;
	}
}
void Intro::Setup(){
	Beacon(20);

#ifdef __D2PROTECTED__	
	if(TheGame->ISREGISTERED){
		unsigned short a=TheGame->RegCode[0],b=TheGame->RegCode[1],c=TheGame->RegCode[2],d=TheGame->RegCode[3],e;
		long E= a*b + a*c + a*d + b*c + b*d + c*d;
		E*=E;
		e=(unsigned short)E;

		if(TheGame->RegCode[4] != e){
			DP("Intro::Setup quit");
			TheGame->Quit();
		}
	}
#endif
//	TheGame->FixImage(StateSurfaces[BitmapToUsePaletteFrom]);

	Beacon(21);
	Beacon(22);
	TheGame->StartTime=TheGame->thisTickCount;
}
void Intro::SetSurfaces(){
	AddBitmapAndSurface(NocDevBitmap, &TheGame->g_pDDSGrenade);
	AddBitmapAndSurface(PresentsBitmap, &TheGame->g_pDDSBackground);
	AddBitmapAndSurface(Destruction2Bitmap, &TheGame->g_pDDSLevels);

}