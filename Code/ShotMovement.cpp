#include "shot.h"
#include "Game.h"
#include "Player.h"
#include "MovementObject.h"
#include "Functions.h"
#include "Debugger.h"

void shot::Move(){

	DP2("ActualCenterPos",ActualCenterPos);
	DP2("bType",bType);
	if(mo->CalculateNewMP()){

		mo->SetMover(this);
		mo->SetXYWay8();
		CalculateMaxMPDir8();
		DP2("Shot Moving First Time", mo->GetMP());
	}else{
		DP2("Shot moving more time", mo->GetMP());
	}
	DP2("Shot Move Speed", (int)Speed);

	if(!mo->GetMP()||KILLME){
		mo->Reset();
		return;
	}

	mo->DetectCollisions();
	mo->MoveAndAct();
	

	if(mo->GetMP()){
		Move();
	}else{
		mo->Reset();
		MoveAskewOnce();
	}

}

void shot::MoveAskewOnce(){
	
	
	// Inv: xy & way is calculated
	MOVEDASKEW=!MOVEDASKEW;
	if(MOVEDASKEW){
		MOVEDASKEW=TRUE;
		return;
	}

	double aMP    = ( Speed * Askew * ((double)TheGame->TheFrame))/((double)TheGame->Dlay);


	if(Dir%2){
		aMP*=isq2;
	}

	aMP+=AskewCounter;
	AskewCounter  = aMP - (int)aMP;

	if(!((int)aMP))
		return;

	mo->MoveAskew((int)aMP);
	Move();

}

int shot::MoveThroughWall(int XS, int YS){

	// Inv : TheGame->WorldMap[ XS ][ YS ][1]>0)

	KILLME=TRUE;
	if(DidntExplode())
		TheGame->WorldMap[ XS ][ YS ][1]-=Dmg;

	if( TheGame->WorldMap[ XS ][ YS ][1]<=0 ){
		WallSound();
	}
		
	return 0;
	
}

int shot::HitMortal(Mortal * Hit){
	if(!Hit)
		return 0;

	if(Hit->GetMType()==M_PL && ((Player *)Hit)->BulletReflect()){
			Dir=(Dir+4)%8;
			dhto=Hit;
			return 1;
		}
		
	// Extra damage in case of directhit
	Hit->Damage(Dmg, BulletTypeToDamageType[(int)bType], bType, Owner);

	if(DidntExplode()){
		if(Hit->GetMType()==M_TK){
			CreateAndPlaySound(sTankHit);
		}
	}
				
	KILLME=TRUE;

	return 0;
}

void shot::AddMeToCurrentQuadrant(){

	Beacon(777634);
	UpdateS();
	DP2("SX",SX);
	DP2("SY",SY);

	if(this==TheShotQuadrants[SX][SY]){
		Beacon(7776666);
		return;
	}

	nextQuadShot=TheShotQuadrants[SX][SY];
	TheShotQuadrants[SX][SY]=this;


	Beacon(77761234);

}
void shot::RemoveMeFromLastQuadrant(){
		Beacon(7771614);
	if(TheShotQuadrants[SX][SY]==this){
		TheShotQuadrants[SX][SY]=nextQuadShot;
	}else{

		shot * c=TheShotQuadrants[SX][SY];
		for(;c->nextQuadShot!=this;){
			c=c->nextQuadShot;
		}
		c->nextQuadShot=nextQuadShot;

	}
	Beacon(77716147);
}

