#include "shot.h"
#include "Game.h"
#include "Player.h"
#include "Clone.h"
#include "Weapon.h"
#include "Functions.h"
#include "Quadrants.h"
#include "Debugger.h"

int sdl=1, scr=1;

shot * TheShots = nullptr;
shot *** TheShotQuadrants = nullptr;

shot::shot(int StartX, int StartY,  double iSpeed, double Ask,BulletType ibType, int Damage, Shooter * iOwner, RegExpType irType, int DontHitOwner){


	ActualCenterPos=8;

	Pos[0]=StartX-ActualCenterPos; 
	Pos[1]=StartY-ActualCenterPos;

	if(ibType==B_SH)
		ActualCenterPos=0;

	nextQuadShot=NULL;
	KILLME=FALSE;

	if(GetActualXPos()<0||GetActualYPos()<0||GetActualXPos()>=TheGame->WorldSize*11*32||GetActualYPos()>=TheGame->WorldSize*11*32){
		Pos[0]=0;
		Pos[1]=0;
		KILLME=TRUE;

	}
	Actor::Initialize();

	AddMeToCurrentQuadrant();
	Thickness=0;
	Dmg=Damage;

	Team=iOwner->GetTeam();
	Owner=iOwner;
	bType=ibType;
	rType=irType;
	if(rType!=RET_NONE){
		// Due to double Damage for direct hit!
		Dmg/=2;
	}
	nextshot=NULL;
	Speed=iSpeed;
	MOVEDASKEW=TRUE;
	Askew=Ask;
	AskewCounter=0;

	if(DontHitOwner)
		dhto=Owner;
	else
		dhto=NULL;

	GoThroughActors=bType==B_MISSILE;
	
	Dir=Owner->Get8Dir();

	if(Owner->GetMType()==M_PL){
		GoThroughWalls=((Player *)Owner)->ShootingSuper();
		INVISIBLE=((Player *)Owner)->ShootsInvisible();
	}

	if(bType==B_SH){
		SetBlit(262,8,1,1,DIR_0,TT_NOTICK,1,&(TheGame->g_pDDSBullets));
	}else
		SetBlit(16*8*(bType%2), 16*(bType/2), 16, 16, DIR_8, TT_NOTICK, 1, &(TheGame->g_pDDSBullets));

	DP2("HitWall",HitWall());
	if(!GoThroughWalls&&HitWall()&&!MoveThroughWall(SX,SY)){
		Speed=0;
	}
	MortalQuadLoopDM(this){
		int t=Iterator->GetThickness();
		if(Iterator != Owner && 
		   GetActualXPos()>=Iterator->GetActualXPos()-t && GetActualXPos()<=Iterator->GetActualXPos()+t-1 &&
		   GetActualYPos()>=Iterator->GetActualYPos()-t && GetActualYPos()<=Iterator->GetActualYPos()+t-1 ){
			HitMortal(Iterator);
		}
	}

	MPRest=Owner->GetMPRest()+             // Assert that we can flee our firerer (doesn't work however :) )
	Speed*Owner->GetCurrentShotExtraTimeFrame()/TheGame->TheFrame; // An extra MP which handles the problem when we were supposed to be fired a certain time ago!

	DP2("Creating a shot",scr++);
}
	
shot::~shot(){
	DP2("Deleting a shot",sdl++);
	Beacon(7777764);
RemoveMeFromLastQuadrant();
Beacon(7777765);}


void shot::KillAll(){
	if(nextshot){
		nextshot->KillAll();
		delete nextshot;
	}

}	

BulletType shot::GetBulletType(){
	return bType;
}


int shot::DidntExplode(){
	if(rType==RET_NONE)
		return 1;

	TheGame->RegExp(GetActualXPos()-16,GetActualYPos()-16,Dmg,rType,BulletTypeToDamageType[bType],Owner);	

	return 0;
}
shot * shot::IsShot(){return this;}

void shot::SetDontHitThisOne(Mortal * idhto){dhto=idhto;}

Mortal * shot::DontHitThisOne(){
	return dhto;
}