#include "Game.h"
#include "Grenade.h"
#include "Mortal.h"
#include "Player.h"
#include "Functions.h"
#include "Quadrants.h"
#include <math.h>
Grenade * TheGrenades;

Grenade::Grenade(int X, int Y, int Spd, double angle, int Damage, BulletType ibType, Mortal * iOwner){

	Pos[0]=X-8;
	Pos[1]=Y-8;
	ActualCenterPos=8;
	Actor::Initialize();
	TURNED=FALSE;
	DirType=DIR_8;

	Angle=angle;
	Team=iOwner->GetTeam();
	Owner=iOwner;


	bType=ibType;
	Dmg=Damage;
	Height=0;
	KILLME=FALSE;

	Dir=Owner->Get8Dir();
	if(Owner->GetMType()==M_PL){
		GoThroughWalls=((Player *)Owner)->ShootingSuper();
		INVISIBLE=((Player *)Owner)->ShootsInvisible();
	}

	double Modifier=1+0.4142*(double)(Dir%2);
	PSpeed = ( (double)Spd*cos(Angle) )/Modifier;
	ZSpeed = ( (double)Spd*sin(Angle) )/Modifier;
	PFrame=ZFrame=0;

	SetBlit(0, 6*16*(int)(bType-GR_LOWEST), 16, 16, DIR_8, TT_NOTICK, 5+(bType!=GR_GRENADE), &(TheGame->g_pDDSGrenade));
	nextGrenade=NULL;
}
Grenade::~Grenade(){}
#define HeightQuote 6
void Grenade::ChangeHeight(){
	if(Height<TickFrames*HeightQuote)
		Tick=(int)Height/HeightQuote;
	else
		Tick=TickFrames-1;	
}

void Grenade::Move(){

	int PSmooth;
	PSmooth=(int)((( PSpeed * (double)TheGame->TheFrame ) / (double)TheGame->Dlay) + PFrame);
	PFrame =     ((( PSpeed * (double)TheGame->TheFrame ) / (double)TheGame->Dlay) + PFrame) - PSmooth;

	int ZSmooth;
	ZSmooth=(int)((( (double)TheGame->TheFrame ) / ((double)TheGame->Dlay*invg[bType==GR_GRENADE])) + ZFrame);
	ZFrame =     ((( (double)TheGame->TheFrame ) / ((double)TheGame->Dlay*invg[bType==GR_GRENADE]))) + ZFrame - ZSmooth;

	double Quote=0;
	if(ZSmooth)
		Quote=(double)PSmooth/(double)ZSmooth;
	int PrimaryMove;
	while(ZSmooth||PSmooth&&!KILLME){
		if(ZSmooth){
			if((double)PSmooth/(double)ZSmooth>Quote)
				PrimaryMove=1;
			else
				PrimaryMove=0;
		}
		else{
			PrimaryMove=1;
		}
		if(PrimaryMove==1){
			MoveP();
			PSmooth--;
		}else{
			MoveZ();
			ZSmooth--;
		}
	}


	
}
void Grenade::MoveP(){
	if(Dir==5||Dir==6||Dir==7)
		Pos[0]--;	
	else if(Dir==1||Dir==2||Dir==3)
		Pos[0]++;
	if(Dir==3||Dir==4||Dir==5)
		Pos[1]++;
	else if(Dir==7||Dir==0||Dir==1)
		Pos[1]--;	
	UpdateS();
	if(OutOfMap())
		KILLME=TRUE;
	if((Height<2*HeightQuote&&!GoThroughWalls || Height<=0)&&TURNED){
		if(HitWall())
			Explode();

		int Marginal;
		// Hit anyone?
		MortalQuadLoopDM(this){
		//for(Mortal * PresentMortal=TheMortals;PresentMortal;PresentMortal=(*PresentMortal).nextMortal){
			Marginal = Iterator->GetThickness();
			if(	GetActualXPos() >(*Iterator).GetX()+16-Marginal && Pos[0]+8<(*Iterator).GetX()+16+Marginal &&
				GetActualYPos() >(*Iterator).GetY()+16-Marginal && Pos[1]+8<(*Iterator).GetY()+16+Marginal &&
				!( Iterator->GetMType()==M_PL && ((Player *)Iterator)->Frozen() ) ){
				
					// Extra damage for direct hit
					Iterator->Damage(Dmg, BulletTypeToDamageType[(int)bType], bType, Owner);
					Explode();
					return;

			}
		}

	}		
	

}
void Grenade::MoveZ(){

	if(TURNED){

		if(Height<ZSpeed){
			Height=0;

		}
		else
			Height-=ZSpeed;
		ZSpeed++;

	}
	else{
		Height+=ZSpeed;
		if(ZSpeed<1){

			TURNED=TRUE;
			ZSpeed=0;
		}
		else
			ZSpeed--;
	}

	if(Height==0)
		Explode();
	else{
		ChangeHeight();
	}

	
}
void Grenade::Explode(){

	KILLME=TRUE;


	RegExpType rType;


	if(bType==GR_ROCKET){
		if(Dir%2)
			rType=RET_MX;
		else
			rType=RET_MPLUS;
	}if(bType==GR_SHELL){
		if(Dir%2)
			rType=RET_SX;
		else
			rType=RET_SPLUS;
	}else if(bType==GR_GRENADE)
		rType=RET_GRENADE;
	TheGame->RegExp(GetX()+BlitDimension[0]-16,GetY()+BlitDimension[1]-16, Dmg, rType, DT_GRENADE,Owner);

}
void Grenade::KillAll(){
	if(nextGrenade){
		nextGrenade->KillAll();
		delete nextGrenade;
	}
}

int Grenade::GetZSpeed(){
	return (int)ZSpeed;
}
int Grenade::GetHeight(){
	return (int)Height;
}

