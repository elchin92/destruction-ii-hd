// Animation.cpp: implementation of the Animation class.
//
//////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "Animation.h"
#include "Debugger.h"

int ancr=1,andl=1;

Animation * TheAnimations;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void Animation::KillDead(){
		// Kill Anims
	KillLinkedList(Animation, TheAnimations, KILLME, nextAnimation);
/*
	Animation * PreviousTemp2=NULL;
	Animation * AnimationTemp=this;

	while(AnimationTemp){
		Animation * kill=NULL;
		if(AnimationTemp->KILLME){

			if(PreviousTemp2){
				PreviousTemp2->nextAnimation=AnimationTemp->nextAnimation;
			}else{
				TheAnimations=AnimationTemp->nextAnimation;
			}
			kill=AnimationTemp;
		}

		if(!kill)
			PreviousTemp2=AnimationTemp;
		AnimationTemp=AnimationTemp->nextAnimation;
		if(kill)
			delete kill;
	}*/
}


Animation::Animation(int iFrames, int iPlayTime, int iAnimType,int x, int y, int BlitX, int BlitY, int iReverse)
{

	ActualCenterPos=16;
	Pos[0]=x;
	Pos[1]=y;
	DP("Anim init");
	Initialize();
	DP("anim init end");

	FrameTime=iPlayTime/iFrames;
	AnimType=iAnimType;
	StartTime=TheGame->thisTickCount;
	TICKHORIZONTAL=TRUE;
	nextAnimation=NULL;
	KILLME=FALSE;
	Reverse=iReverse;
	if(FrameTime){
		if(Reverse)
			Tick=TickFrames-1;
	}else Tick=0;
	SetBlit(BlitX,BlitY,32,32,DIR_0,TT_NOTICK,iFrames,&TheGame->g_pDDSSprites);
	DP2("Creating an animation",ancr++);
}
void Animation::Move(){}

Animation::~Animation()
{
	DP2("deleting an animation",andl++);
}
void Animation::ShiftLeft(){Tick--;}
void Animation::ShiftRight(){Tick++;}
void Animation::TickUp(){
	if(nextAnimation)
		nextAnimation->TickUp();
	if(FrameTime==0)
		return;

	Tick=((int)(TheGame->thisTickCount-StartTime))/FrameTime;

	if(Reverse){
		Tick=TickFrames-1-Tick;
		if(Tick<0){
			Tick=0;
			KILLME=TRUE;
		}
	}else{

		if(Tick>=TickFrames){
			Tick=TickFrames-1;
			KILLME=TRUE;
		}
	}



}

void Animation::Blit(int iAnimType){
	if(nextAnimation)
		nextAnimation->Blit(iAnimType);
	
	if(KILLME)
		return;

	if(AnimType==iAnimType){
		Actor::Blit(0);
		Actor::Blit(1);
	}

}

void Animation::KillAll(){
	if(nextAnimation){
		nextAnimation->KillAll();
		delete nextAnimation;
	}

}	