// SoundSource.cpp: implementation of the SoundSource class.
//
//////////////////////////////////////////////////////////////////////

#include "SoundSource.h"
#include "SoundStructure.h"
#include "SoundEngine.h"
#include "Game.h"
#include "functions.h"
#include "Player.h"
#include "DebugWindow.h"
//#include <math.h>
#include "Debugger.h"
#include <stdio.h>


#define plSound TheEngine->ThePlayingSounds[(int)Priority]
#define splLoop for(int pl=0;pl<2;pl++)
#define sPl TheGame->ThePlayers[pl] 

int sscr=1, ssdl=1;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SoundSource::SoundSource(){}


SoundSource::SoundSource(Sound iSnd, SoundPriority sPriority, int x, int y, DistanceType iDistance, int iLoop){
	
	Initialize();

	Snd=iSnd;
	Priority=sPriority;
	sDistance = iDistance;
	Pos[0]=x;
	Pos[1]=y;
	ActualCenterPos=0;
	Looping=iLoop;

	DELETEABLE=!iLoop;


}


SoundSource::~SoundSource(){
	DP2("Deleting a ss",ssdl++);
	SAFE_DELETE(TheSound);
}

void SoundSource::UpdateS(){
	SX=GetNewXs();
	SY=GetNewYs();
}
void SoundSource::SetPosNonActor(int x, int y){
	Pos[0]=x;
	Pos[1]=y;
	UpdateS();
}

int SoundSource::GetActualCenterPos(){
	return ActualCenterPos;
}
int SoundSource::GetXs(){
	return SX;
}
int SoundSource::GetYs(){
	return SY;
}
int SoundSource::GetNewXs(){
	return GetActualXPos()/32;
}
int SoundSource::GetNewYs(){
	return GetActualYPos()/32;
}

int SoundSource::GetActualXPos(){
	return (Pos[0]+ActualCenterPos);
}
int SoundSource::GetActualYPos(){
	return (Pos[1]+ActualCenterPos);
}
int SoundSource::GetActualPos(int xy){
	return (Pos[xy]+ActualCenterPos);

}

int SoundSource::IsPlaying(){
	if(TheSound)
		return TheSound->IsPlaying();
	return FALSE;
}

int SoundSource::GetX(){return Pos[0];}
int SoundSource::GetY(){return Pos[1];}
int SoundSource::GetPos(int xy){return Pos[xy];}
void SoundSource::UpdateSound(){

	DBS(39184);
	if(TheSound){

		DBS(39185);

		if(FAILED(TheSound->UpdateBuffer())){
			DBS(6660001);		
		}
	}

}

void SoundSource::Initialize(){

	DP2("Creating a ss",sscr++);
	TheSound=NULL;
	nextSoundSource=NULL;
	Snd=sVoid;
	Priority=spLowest;
	sDistance=dNormal;
	DELETEABLE=FALSE;
	DELETEME=FALSE;
	Looping=FALSE;
}
int SoundSource::StartPlaying(){
	if(!TheSoundEngine->SoundActivated()||Snd==sVoid)
		return FALSE;

	SAFE_DELETE(TheSound);

	char fullName[60];
	sprintf(fullName,"%s\\%s",WaveDir,TheSoundEngine->WaveNames[(int)Snd ]);
	Beacon(8000+Snd);
	DP(fullName);
	TheSound = new SoundStructure( this, fullName , Looping);


	if(TheSound){
		if(TheSound->PlayBuffer()==S_OK){
			DBC2("Sound started playing");
		}else{
			DBS(6660002);			
			return FALSE;
		}
	}
	else{ 
		DBCE("TheSound == NULL");	
		return FALSE;
	}

	return TRUE;
}
void SoundSource::StopPlaying(){
	// CRITICAL FIX: Delete TheSound BEFORE checking if sound is active!
	// Otherwise when sound is disabled, TheSound is NOT deleted here,
	// but WILL BE deleted in ~SoundSource() → double-free!
	SAFE_DELETE(TheSound);

	if(!TheSoundEngine->SoundActivated())
		return;

	HasStopped();

}

SoundSource * SoundSource::CreateAndPlaySound(Sound snd, DistanceType dType, SoundPriority sPri, int Loop){

	return TheSoundEngine->PlayLocatedSound(snd, GetActualXPos(), GetActualYPos(), dType, sPri, Loop);
}
SoundSource * SoundSource::CreateAndPlaySound(BulletType bType, DistanceType dType, SoundPriority sPri, int Loop){
	return CreateAndPlaySound(Ammo2Sound[bType], dType, sPri, Loop);
}

void SoundSource::HasStopped(){
	DELETEME=DELETEABLE;
	TheSoundEngine->RemoveFromThePlayingSounds(this);
}
Sound SoundSource::GetSound(){return Snd;}
void SoundSource::SetCenterPan(){
	if(TheSound)
		TheSound->SetCenterPan();
}

int SoundSource::DeleteAble(){return DELETEME;}

DistanceType SoundSource::GetDistance(){return sDistance;}
SoundPriority SoundSource::GetPriority(){return Priority;}