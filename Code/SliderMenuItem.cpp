// SliderMenuItem.cpp: implementation of the SliderMenuItem class.
//
//////////////////////////////////////////////////////////////////////

#include "SliderMenuItem.h"
#include "InputEngine.h"
#include "Game.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


SliderMenuItem::~SliderMenuItem()
{

}

void SliderMenuItem::Initialize(){
	SlideInterval=175;
	Multiplier=1;
	ActiveMenuItem::Initialize();
}

void SliderMenuItem::InternalUpdateInput(){


	if(TrueDown(DIK_RETURN)){
		MoveToNeighbours=!MoveToNeighbours;
		CatchingEscape=!CatchingEscape;
		if(!MoveToNeighbours){
			tSelection=InternalSelection;
			cgs->Message("Use the arrowkeys to change and <RETURN> to accept",mcGreen);	
		}
		return;
	}

	if(TrueDown(DIK_ESCAPE)){
		SetSelection(tSelection);
		CatchingEscape=FALSE;
		MoveToNeighbours=TRUE;
		return;
	}

	if(!MoveToNeighbours){
		if(TrueDown(DIK_UP) ||TrueDown(DIK_DOWN) ||TrueDown(DIK_LEFT) ||TrueDown(DIK_RIGHT))
			SlideStop=TheGame->thisTickCount;
		if((Down(DIK_UP)||Down(DIK_RIGHT)) && SlideStop<TheGame->thisTickCount){
			SlideStop+=SlideInterval;
			if(InternalSelection<(*MaxPointer))
				AddToSelection(Multiplier);
		}else if((Down(DIK_LEFT)||Down(DIK_DOWN)) && SlideStop<TheGame->thisTickCount){
			SlideStop+=SlideInterval;
			if(InternalSelection>Min||
				UnlimitedZero()&&InternalSelection>=Multiplier)
				AddToSelection(-Multiplier);
		}
	}

}

int SliderMenuItem::GetMax(){return (*MaxPointer);}