#include "Player.h"
#include "Game.h"
#include "MovementObject.h"
#include "Missile.h"
#include "MovementEvent.h"
#include "AirPlane.h"
#include "Debugger.h"

#define STOPSPININTERVAL 125

void Player::Move(){

	if(!IsAlive()){
		EndMovement();
		return;
	}
		
	if(Stopped()){
		while(NextStopSpin<TheGame->thisTickCount){
			NextStopSpin+=STOPSPININTERVAL;
			Dir++;
			Dir%=8;
		}
		return;
	}


	DP("Player moving...");
		Beacon(9990000+Team);
	// Way2 must be set to zero!
 
	if(mo->CalculateNewMP()){

		DP(" Calculating new mp...");
		if(!KAMIKAZE&&CalculatePlayerDir()){
			DP(" Exiting Movement 0");
			EndMovement();
			Beacon(7771161);
			return;		
		}
		Beacon(7771162);
		mo->SetMover(this);
		mo->SetXYWay8();
		Beacon(777113);
		CalculateMaxMPDir8();
			Beacon(777114);
		
	}else{ 		DP(" Using saved mp...");}
	DP2(" Mp to move: ",mo->GetMP());
	Beacon(777117);
	if(!mo->GetMP()){
		EndMovement();
		DP(" Exiting Movement 1");
		return;
	}
	Beacon(777118);
	mo->DetectCollisions();
	Beacon(777119);
	MovementEvent * lastStop=mo->MoveAndAct();
	if(	KAMIKAZE && lastStop->KamikazeEvent()){
		if(lastStop->KamikazeEvent()==2){
			// Out of world!
			KAMIKAZE=FALSE;
		}
		Mortal::Kill(this, DT_SPECIAL);
		EndMovement();
		DP(" Exiting Movement 3");
		return;
	}
	Beacon(777120);
	if(mo->GetMP()){
		DP2(" Saving MP...:", mo->GetMP());
		Move();
	}else{
		DP(" Exiting Movement 2");
		EndMovement();
	}
	DP(" PLMove() end 2");
}

int Player::CalculatePlayerDir(){
	if(LEFT){
		if(UP)
			Dir=7;
		else if(DOWN)
			Dir=5;
		else
			Dir=6;
	}
	else if(RIGHT){
		if(UP)
			Dir=1;
		else if(DOWN)
			Dir=3;
		else
			Dir=2;							
	}
	else if(UP)
		Dir=0;
	else if(DOWN)
		Dir=4;
	else
		return TRUE;
	return FALSE;
}

int Player::MoveThroughWall(int XS, int YS){
	
	if(GoThroughWalls){
		return 1;
	}
	if(Superman()&& TheGame->WorldMap[ XS ][ YS ][0]<=16){
		TheGame->WorldMap[ XS ][ YS ][1]=0;
		WallSound();
		return 1;
	}
	return GoThroughWalls;
}
void Player::EndMovement(){
	// CAlled once per movement
	mo->Reset();
	if(TheMissile)
		TheMissile->Move();
	if(ThePlane){
		ThePlane->Move();
		if(ThePlane->KILLME){
			delete ThePlane;
			ThePlane=NULL;
		}
	}

}