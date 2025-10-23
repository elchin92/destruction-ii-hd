// FadingGameState.cpp: implementation of the FadingGameState class.
//
//////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "SlideShowGameState.h"
#include "Debugger.h"

#define FADESTEP	FadeTime[FADEDOWN]/100
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FadingGameState::~FadingGameState(){

}

void FadingGameState::Initialize(){
	GameState::Initialize();
	NextFade=0;
	FADEDOWN=FALSE;
	Brightness=lBrightness=0;
	FadeTime[0]=500;
	FadeTime[1]=500;

}

void FadingGameState::UpdateFade(){
	if(!NextFade)
		NextFade=TheGame->thisTickCount;

	if(FADEDOWN){
		// Fade out
		while(Brightness && 
			  TheGame->thisTickCount > NextFade){

			  Brightness--;
			  NextFade+=FADESTEP;
			  //Beep(Brightness*20,100);
		}			
		if(!Brightness){
			if(IsSlideShow()){
				((SlideShowGameState *)(this))->SubtractSlide();
				((SlideShowGameState *)(this))->FixCorrectImage();
				FADEDOWN=FALSE;
				SwitchingState=FALSE;
			}
			if(PerformSwitch()){
				Beacon(1234444);
				InternalSwitchState(NextState);	
			}
			return;
		}
	}else if(FadeTime[0]){
		// Fade in
		while(Brightness<100 && 
			  TheGame->thisTickCount > NextFade){
			  Brightness++;
			  NextFade+=FADESTEP;
			  //Beep(Brightness*20,100);
		}
	
	}else 
		Brightness=100;

	if(Brightness!=lBrightness){
		SetCorrectPaletteAndGamma();
		lBrightness=Brightness;
	}
	
	if(Brightness==100&&IsSlideShow() && ((SlideShowGameState *)(this))->AutomaticFadeDown()){
		FADEDOWN=TRUE;
		SwitchingState=TRUE;		
	}
}
COLORREF FadingGameState::ContrastAdjust(int r, int g, int b){

	return RGB((r*Brightness)/100,(g*Brightness)/100,(b*Brightness)/100);
}

COLORREF FadingGameState::GetWhite(){
	return RGB((255*Brightness)/100,(255*Brightness)/100,(255*Brightness)/100);
}
COLORREF FadingGameState::GetRed(){
	return RGB((255*Brightness)/100,0,0);
}
COLORREF FadingGameState::GetGreen(){
	return RGB(0,(255*Brightness)/100,0);
}
COLORREF FadingGameState::GetBlue(){
	return RGB(0,0,(255*Brightness)/100);
}
COLORREF FadingGameState::ContrastAdjust(COLORREF rgb){
	return RGB((GetRValue(rgb)*Brightness)/100,(GetGValue(rgb)*Brightness)/100,(GetBValue(rgb)*Brightness)/100);	
}

void FadingGameState::SetFadeInTime(int iFadeTime){FadeTime[0]=iFadeTime;}
void FadingGameState::SetFadeOutTime(int iFadeTime){FadeTime[1]=iFadeTime;}
void FadingGameState::InitializeFadeIn(){
	NextFade=0;
	FADEDOWN=FALSE;
	Brightness=lBrightness=0;
}


void FadingGameState::SwitchState(gState iNextState, int SubMenu){

	if(FADEDOWN)
		return;

	FADEDOWN=TRUE;
	SwitchingState=TRUE;
	if(SubMenu)
		TheGame->GameStates[iNextState]->SetReturnState(GameStatus);

	NextFade=0;
	NextState=iNextState;
	

}
int FadingGameState::IsFading(){return 1;}
int FadingGameState::IsSlideShow(){return 0;}
int FadingGameState::PerformSwitch(){return 1;}