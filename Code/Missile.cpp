#include "Missile.h"
#include "Game.h"
#include "Player.h"
#include "MovementObject.h"
#include "Functions.h"

Missile::Missile(Player * iOwner){

	// Only use iOwner here!!!!!!!!
	Pos[0]=iOwner->GetX();
	Pos[1]=iOwner->GetY();
	LengthAdjust(&Pos[0], &Pos[1], iOwner->GetRealDir(), iOwner->GetThickness()+1, 0);
	ActualCenterPos=16;

	Actor::Initialize();

	Owner=iOwner;

	Dir=Owner->GetRealDir();
	Team=Owner->GetTeam();
	Speed=8;

	SetBlit(0, 192, 32, 32, DIR_8, TT_NOTICK, 1, &(TheGame->g_pDDSSprites));

	CreateAndPlaySound(sMissile,dFar,spNormal);
}
Missile::~Missile(){}
void Missile::Move(){

	if(mo->CalculateNewMP()){

		mo->SetMover(this);
		mo->SetXYWay8();
		CalculateMaxMPDir8();
		
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
		UpdateS();
		if(OutOfMap())
			Owner->InactivateMissile();
	}

}

void Missile::Detonate(){
	TheGame->RegExp(GetActualXPos()-16,GetActualYPos()-16,200,RET_MISSILE, BulletTypeToDamageType[B_MISSILE],Owner);		
}
