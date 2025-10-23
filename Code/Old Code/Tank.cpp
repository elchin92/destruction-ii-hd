#include "Tank.h"

Tank::Tank(int X, int Y, int theType, int Who, int theDir){

	TheGame=theGame;
	Initialize();
	BLITDEAD=TRUE;
	MType=M_TK;
	Pos[0]=X;
	Pos[1]=Y;
	Dir=theDir;
	Team=Who;
	Size=1;
	Type=theType;
	Strengths = Strengths | T_APEXP;
	Immunities = Immunities | T_APMINE;
	Thickness=TheGame->GetThick(TH_TANK+Type);

	if(Type==0){
		Price=24000;
		BulletPrice=15;
		MaxHealth=3000;
		ExplodeType=EXP_NORMAL;
		ShotType = B_556;
		ShotSpd=12;
		ShotAcc=10;
		ShotDmg=25;
		Prob = 0;
		FireDelay=125;
	}
	else if(Type==1){
		Price=40000;
		BulletPrice=30;
		MaxHealth=4500;
		ExplodeType=EXP_NORMAL;
		ShotType = B_762;
		ShotSpd=18;
		ShotAcc=13;
		ShotDmg=40;
		Prob = 0;
		FireDelay=90;
	}
	else if(Type==2){
		Price=70000;
		BulletPrice=50;
		MaxHealth=6250;
		ExplodeType=EXP_NORMAL;
		ShotType = B_762;
		ShotSpd=15;
		ShotAcc=14;
		ShotDmg=40;
		Prob = 0;
		FireDelay=45;
		DualFire = TRUE;
		PipeDistance=8;
	}
	else if(Type==3){
		Price=150000;
		BulletPrice=80;
		MaxHealth=18000;
		ExplodeType=EXP_BIG;
		ShotType = B_12;
		ShotSpd=22;
		ShotAcc=15;
		ShotDmg=60;
		AShotType = B_APRKT;
		AShotSpd = 7;
		AShotAcc = 40;
		AShotDmg = 600;
		Prob = 15;
		FireDelay=40;
		Speed=4;
		LockSpeed=4;
	}else if(Type==4){

		Price=35000;
		StopDistance=0;
		BulletPrice=10;
		MaxHealth=4000;
		ExplodeType=EXP_NORMAL;
		ShotType = GR_ROCKET;
		ShotSpd=11;
		ShotAcc=45; // Angle
		ShotDmg=240;
		Prob = 0;
		FireDelay=900;
		DualFire=TRUE;
		PipeDistance=2;	
	}
	Health=MaxHealth;

	nextTank=NULL;
	if(Type==4)
		SetBlit(0 , 0, 32, 32, DIR_4, TT_ROLLER, 3,&(TheGame->g_pDDSRTank));
	else
		SetBlit(32*4*(Type%2) , 32*2*(Type/2), 32, 32, DIR_4, TT_NOTICK, 1, &(TheGame->g_pDDSTank));
}

Tank::~Tank(){
	if(nextTank)
		delete nextTank;
}

