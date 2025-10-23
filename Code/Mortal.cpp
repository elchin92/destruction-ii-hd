#include "Mortal.h"
#include "Game.h"
#include "Player.h"
#include "ScoreTableEntry.h"
#include "AmmoAndArmorName.h"
#include "StopReason.h"
#include "LastRoundsInfo.h"
#include "Debugger.h"

Mortal * TheMortals;
Mortal * TheDeadMortals;
Mortal *** TheMortalQuadrants;

Mortal::Mortal(int X, int Y, int iTeam){
	// Dead player
	Pos[0]=X;
	Pos[1]=Y;
	Initialize(FALSE);
	Team=iTeam;
	Type=0;
	MType=M_PL;
	ExplodeType=EXP_NONE;
	Health=0;
	Tick=0;

	SetBlit(224,32*Team , 32, 32, DIR_0, TT_NOTICK, 0, (&TheGame->g_pDDSSprites));
	AddThisLastToLinkedList(Mortal, TheDeadMortals, nextDeadMortal);
}

Mortal::Mortal(int X, int Y, int theType, int Who){

	Pos[0]=X;
	Pos[1]=Y;
	Initialize();
	Team=Who;
	Type=theType;
	Thickness=TheGame->GetThick(theType);

	
	if(Type==T_PACMAN){
		SetImmunity(T_APMINE);
		SetStrength(T_ATMINE);
		SetStrength(T_NRJ);
		Type=0;
		Size=1;
		MType=M_PM;
		SEEKER=TRUE;
		BreaksWalls=FALSE;

		ExplodeType=EXP_NONE;
		MaxHealth=20000;
		Price=250000;
		BulletPrice=200;
		RegularSpeed=2;
		LockSpeed=2.1;
		
		SetBlit(384, 0 , 32, 32, DIR_4, TT_PINGPONG, 6, (&TheGame->g_pDDSSprites));

	}else if(Type<T_CAR+Units[M_CA]){
		
		SetStrength(T_APMINE);
		SetWeakness(T_BULLET);
		Type-=T_CAR;
		Size=1;
		MType=M_CA;
		ExplodeType=EXP_NORMAL;
		SEEKER=TRUE;
		SetBlit(128*Type, 0 , 32, 32, DIR_4, TT_NOTICK, 1, (&TheGame->g_pDDSCar));
		if(Type==0){
			Price=7000;
			MaxHealth=1500;
			BulletPrice=10;
		}if(Type==1){
			BulletPrice=15;
			Price=20000;
			MaxHealth=3000;
			LockSpeed=2.8;
		}
	}else if(Type<T_MACHINE+Units[M_MA]){
		SetImmunity(T_APMINE);
		SetStrength(T_APEXP);
		SetStrength(T_ATEXP);
		SetStrength(T_BULLET);
		SetWeakness(T_NRJ);
		Type-=T_MACHINE;
		Size=1;
		MType=M_MA;
		SEEKER=TRUE;
		BreaksWalls=TRUE;
		if(Type==0){
			ExplodeType=EXP_NORMAL;
			MaxHealth=4000;
			Price=23000;
			BulletPrice=40;
			RegularSpeed=1;
		}else if(Type==1){
			ExplodeType=EXP_BIG;
			MaxHealth=7500;
			Price=59000;
			BulletPrice=75;
			LockSpeed=3.9;
		}

		SetBlit(128*Type, 0 , 32, 32, DIR_4, TT_ROLLER, 2+2*(Type==1), (&TheGame->g_pDDSMachine));
	}
	Health=MaxHealth;
	Dir=rand()%4;
	ChangeDir(FALSE);

}
void Mortal::Initialize(int ADDMORTAL){

	nextQuadMortal=NULL;
	ActualCenterPos=16;
	Actor::Initialize();
	// Don't forget to set MType
	nextMortal=NULL;
	nextDeadMortal=NULL; 
	HitStop=-10000;
	LockedDir=-1;
	LockSpeed=2;
	RegularSpeed=2;
	Speed=1; 
	HitByBullet=0;
	ExplodeType=EXP_NONE;
	Weaknesses=0;
	Strengths=0;
	Immunities=0;
	SEEKER=TRUE;
	ReasonToStop=HaventStopped;
	Moving=1;
	Team=2;
	Thickness=16;
	TRAPPED=FALSE;
	Explosions=0;
	Price=0;
	BulletPrice=0;
	Frame=0;
	ExpStart=0;
	BLITDEAD=TRUE;
	StopDistance=0;
	Size=0;
	INVISIBLE=FALSE;


	if(ADDMORTAL){
		AddThisToLinkedList(TheMortals, nextMortal);
		AddMeToCurrentQuadrant();	
	}
}
Mortal::Mortal(){

}
Mortal::~Mortal(){

}

void Mortal::MoveMeToTheDeadMortals(){
Beacon(2341243);
	RemoveThisFromLinkedList(Mortal, TheMortals, nextMortal);
Beacon(3532453);
	AddThisLastToLinkedList(Mortal, TheDeadMortals, nextDeadMortal);
Beacon(3532452);
}
void Mortal::MoveMeToTheLivingMortals(){
Beacon(26353533);
	RemoveThisFromLinkedList(Mortal, TheDeadMortals, nextDeadMortal);
Beacon(234535477);
	AddThisToLinkedList(TheMortals, nextMortal);
}
void Mortal::Kill(Mortal * Killer, DamageType dType){

	// Killer can be NULL!!!
	Beacon(1234234);

	RemoveMeFromLastQuadrant();
	MoveMeToTheDeadMortals();

	Beacon(234234);
	if(IsShooter()){
		Beacon(12634);
		IsShooter()->StopTazerSound();

	}
	Beacon(71534);
	if(GetMType()==M_PL){
		Beacon(715340);
		// Player BEING killed
		((Player *)this)->PlayerKill(Killer, dType);
		
	}else{
		Beacon(715341);
		// Mortal Kill
		Health=0;
		LockedDir=-1;
		if(ExplodeType!=EXP_NONE){
				Beacon(715342);
			ExpStart=TheGame->thisTickCount;
			if(ExplodeType==EXP_SINGLE)
				Explosions=1;
			else if(ExplodeType==EXP_SMALL)
				Explosions = 4+rand()%8;
			else if(ExplodeType==EXP_NORMAL)
				Explosions = 10+rand()%15;
			else if(ExplodeType==EXP_BIG)
				Explosions = 25+rand()%40;
		}
		Beacon(715343);
	}

	// Statistics report, Killer is a player
	if(Killer && Killer->GetMType() == M_PL){

		((Player *)Killer)->ReportKill(this);
		if(Killer!=this)
			TheLastRounds->CurrentGame->ReportKill(Killer->GetTeam(),(MortalType)GetMType());
		
	}

	if(MType==M_PM){
		CreateAndPlaySound(sPacmanDies,dFar,spHigh);
	}

}

int Mortal::GetPrice(int kTeam){
	if(Team!=kTeam)
		return Price;
	
	return 0;
}
int Mortal::GetBulletPrice(int kTeam){
	if(Team!=kTeam)
		return BulletPrice;
	
	return 0;
}

void Mortal::SetImmunity(int Type){
	Immunities=Immunities|Type;
}
void Mortal::SetStrength(int Type){
	Strengths=Strengths|Type;
}
void Mortal::SetWeakness(int Type){
	Weaknesses=Weaknesses|Type;
}

int Mortal::IsImmuneAgainst(DamageType dType){
	return DamageTypeToStatusBit[(int)dType]&Immunities;
}
int Mortal::IsStrongAgainst(DamageType dType){
	return DamageTypeToStatusBit[(int)dType]&Strengths;
}
int Mortal::IsWeakAgainst(DamageType dType){
	return DamageTypeToStatusBit[(int)dType]&Weaknesses;
}
void Mortal::Damage(int Dam, DamageType dType, BulletType bType, Mortal * Damager){

		Beacon(777124);
	// Damager can be NULL!!!

	DP2("Damage()", Dam);
	DP2("MType",MType);
	if(bType==B_NOBULLET){
		DP("Nob");
	}else{
		DP(AmmoName[bType]);
	}
	DP2("DamageType", dType);
	if(!IsAlive() || IsImmuneAgainst(dType)){
		DP("Immune!");
		return;
	}


	if(IsStrongAgainst(dType))
		Dam/=2;

	if(IsWeakAgainst(dType))
		Dam*=2;
		
	Beacon(777125);

	if(GetMType()==M_PL){
		DP(" Player");
		ArmorType at;
		if(bType==B_NOBULLET){
			at=DamageTypeToArmorType[dType];
		}else
			at=BulletTypeToArmorType[bType];
		((Player *)this)->PlayerDamage(Dam, dType, at, Damager);
		return;
	}
	Beacon(777126);
	GotHit();
	DP2("Applied", Dam);
	Health-=Dam;
	DP2("Health after", Health);
	DP2("MaxHealth", MaxHealth);
	if(Health<=0){
		Kill(Damager, dType);
		Beacon(777127);
	}
	
		Beacon(777128);
}

void Mortal::GotHit(){
	HitStop=TheGame->thisTickCount+50;

}

int Mortal::IsAlive(){
	return Health>0;
}


int Mortal::GetType(){
	return Type;
}

BYTE Mortal::GetBlitColor(){
	return HitStop>TheGame->thisTickCount ? 15 : 0; 
}

int Mortal::GetSize(){
	return Size;
}

int Mortal::GetMType(){
	return MType;
}
void Mortal::BlitDead(int where){
	if(BLITDEAD)
		Actor::Blit(where+2);
}
void Mortal::Blit(int who){
	if(ThePlayers[who]->BlitLife()&&!IsPlayer()&&!IsClone() ){
		SetBlitRect();
		TheGame->BGOSL(Pos[0],Pos[1],(14*Health)/MaxHealth,g_pDDSBlitSource);
	}
	else
		Actor::Blit(who);
}

void Mortal::SetTeam(int iTeam){
	Team=iTeam;
}


void Mortal::SetPos(int x, int y){
	// Inv: Exists in quadrants
	Pos[0]=x;
	Pos[1]=y;
	if(GetXs()!=GetNewXs()||GetYs()!=GetNewYs())
		FixQuadrantShit();
}

void Mortal::MaybeExplode(){
	if(Explosions && ExpStart < TheGame->thisTickCount){

		int Delay=800 - min(500,Explosions*Explosions*10);
		int Damage=80;
		RegExpType rType=RET_MORTALEXPLOSIONSMALL;

		int b =rand()%Delay;
		ExpStart+=b;

		if(ExplodeType==EXP_NORMAL&&!(rand()%3)){
			rType=RET_MORTALEXPLOSIONNORMAL;
			Damage=150;
		}
		if(ExplodeType==EXP_BIG&&rand()%2){
			rType=RET_MORTALEXPLOSIONNORMAL;
			Damage=150;	
			if(!(rand()%3)){
				rType=RET_MORTALEXPLOSIONBIG;
				Damage=245;
			}
		}
		TheGame->RegExp(GetActualXPos()-16-Thickness+rand()%(Thickness*2+1),
						GetActualYPos()-16-Thickness+rand()%(Thickness*2+1),
						Damage, rType, DT_MORTALEXPLOSION,this);
		
		Explosions--;
		
	}

}

Mortal * Mortal::IsMortal(){return this;}