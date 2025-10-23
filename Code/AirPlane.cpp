// AirPlane.cpp: implementation of the AirPlane class.
//
//////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "Player.h"
#include "AirPlane.h"
#include "SoundEngine.h"
#include "MovementObject.h"
#include "DamageConstants.h"

#define StrikeDistance 11*32
#define DropDelay 7*32

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AirPlane::AirPlane(Player * iOwner)
{

	Initialize();

	Owner=iOwner;
	KILLME=FALSE;
	AirFreePos[0]=Owner->GetXs();
	AirFreePos[1]=Owner->GetYs();
	StopDropPos[0]=Owner->GetXs()-5;
	StopDropPos[1]=Owner->GetYs()-5;

	for(int two=0;two<2;two++){
		if(StopDropPos[two]<0)
			StopDropPos[two]=0;
		if(StopDropPos[two]>TheGame->WorldSize*11-11)
			StopDropPos[two]=TheGame->WorldSize*11-11;
	}
	AirBombs=11;

	SetBlit(0,0,140,93,DIR_0,TT_NOTICK,1,&(TheGame->g_pDDSSprites));

	Pos[0]=StopDropPos[0]*32+32*5+16-BlitDimension[0]/2;
	Pos[1]=StopDropPos[1]*32+32*5+16-BlitDimension[1]/2;
	Pos[0]+=StrikeDistance;

	ActualCenterPos=BlitDimension[0]/2; // (X/2)
	Dir=3;
	Speed=10;
}

AirPlane::~AirPlane()
{

}



void AirPlane::Fire(){
	while(Pos[0]+DropDelay<(StopDropPos[0]+(AirBombs-1))*32&&AirBombs){

		int Xs=StopDropPos[0]+(AirBombs-1);
		int Ys=StopDropPos[1];
		for(int Blow=0;Blow<11;Blow++){
			if(!Blow){
				TheSoundEngine->PlayLocatedSound(sBigExplosion,Xs*32,Ys*32,dFar,spNormal);
			}
			if( !( Xs == AirFreePos[0] &&
				   Ys == AirFreePos[1] )  ){
				TheGame->RegExp(32*Xs,32*Ys, AirDamage, RET_AIR, DT_SPECIAL, Owner);							
			}		
			Ys++;
		}
		AirBombs--;
	}
	
}
void AirPlane::Move(){

	if(mo->CalculateNewMP()){

		mo->SetMover(this);
		mo->SetXYWay4();
		CalculateMaxMPDir4();
		
	}

	if(!mo->GetMP()){
		mo->Reset();
		return;
	}

	mo->MoveAndAct();


	if(mo->GetMP()){
		Move();
	}else{
		mo->Reset();
		if(HasLeftScreen()&&AirBombs==0)
			KILLME=TRUE;
	}

}

int AirPlane::OwnerSafe(){
	return Owner->GetXs()==AirFreePos[0]&&Owner->GetYs()==AirFreePos[1];
}

