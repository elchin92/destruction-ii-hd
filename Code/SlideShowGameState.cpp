// SlideShowGameState.cpp: implementation of the SlideShowGameState class.
//
//////////////////////////////////////////////////////////////////////
#include "Debugger.h"
#include "SlideShowGameState.h"
#include "Game.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


void SlideShowGameState::Initialize(int iSlides){
	Beacon(909090);
	FadingGameState::Initialize();
	Slides=iSlides;
	for(int i=0; i<MAXSLIDES;i++){
		SlideShowFadeTime[i][0]=FadeTime[0];
		SlideShowFadeTime[i][1]=FadeTime[1];
		AutoFade[i]=0;
	}
}



void SlideShowGameState::InitializeFadeIn(){
	SlidesLeft=Slides;
	BitmapToUsePaletteFrom=0;
	FadingGameState::InitializeFadeIn();
	FadeTime[0]=SlideShowFadeTime[0][0];
	FadeTime[1]=SlideShowFadeTime[0][1];
	SetPaletteAndGamma(0);
	FixCorrectImage();
}

void SlideShowGameState::SubtractSlide(){

	SlidesLeft--;

	BitmapToUsePaletteFrom++;
	BitmapToUsePaletteFrom%=Slides;

	FadeTime[0]=SlideShowFadeTime[BitmapToUsePaletteFrom][0];
	FadeTime[1]=SlideShowFadeTime[BitmapToUsePaletteFrom][1];
}
void SlideShowGameState::FixCorrectImage(){
	if(SlidesLeft){
  		TheGame->FixImage(StateSurfaces[BitmapToUsePaletteFrom]);	
	}
}
int SlideShowGameState::IsSlideShow(){return 1;}
int SlideShowGameState::PerformSwitch(){Beacon(999000+SlidesLeft);return SlidesLeft==0;}
void SlideShowGameState::SetSSFadeTime(int Slide, int iFadeInTime, int iFadeOutTime, int iAutoFadeTime){
	SlideShowFadeTime[Slide][0]=iFadeInTime;
	SlideShowFadeTime[Slide][1]=iFadeOutTime;
	AutoFade[Slide]=iAutoFadeTime;
}
int SlideShowGameState::AutomaticFadeDown(){
	return AutoFade[BitmapToUsePaletteFrom]&&
		NextFade+AutoFade[BitmapToUsePaletteFrom]<TheGame->thisTickCount;
}