// TazeringActor.cpp: implementation of the TazeringActor class.
//
//////////////////////////////////////////////////////////////////////
#include "Debugger.h"
#include "TazeringActor.h"


void TazeringActor::PlayTazerSound(){
	Beacon(1811);
	if(TazerSound){
			Beacon(1812);
		TazerSound->SetPosNonActor(GetActualXPos(),GetActualYPos());
	}
	else{
		TazerSound=CreateAndPlaySound(sElectric,dClose,spLow,TRUE);
	}
		Beacon(1813);
}
void TazeringActor::UpdateTazerSound(int Hit){
	Beacon(18100);
	if(Hit)
		PlayTazerSound();
	else 
		StopTazerSound();
}
void TazeringActor::StopTazerSound(){
	Beacon(1814);
	if(TazerSound){
		TazerSound->StopPlaying();
		TazerSound=NULL;
	}
}
TazeringActor::TazeringActor(){
	TazerSound=NULL;
	DP("TazeringActor() run");
}
TazeringActor::~TazeringActor(){
	StopTazerSound();
}