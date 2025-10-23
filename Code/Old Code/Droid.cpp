#include "Droid.h"
#include "Game.h"

Droid::Droid(int X, int Y, int theType, int Who, int theDir){

	TheGame=theGame;
	Initialize();
	MType=M_DR;
	Pos[0]=X;
	Pos[1]=Y;
	Dir=theDir;
	Team=Who;
	Size=0;
	Type=theType;
	Strengths = Strengths | T_APEXP;
	Immunities = Immunities | T_APMINE;
	Thickness=TheGame->GetThick(TH_DROID+Type);
	ShotType = B_NRJ;
	Prob=0;

	LastTazer=TheGame->thisTickCount;

	if(Type==0){
		Price=7000;
		StopDistance=80;
		BulletPrice=10;
		MaxHealth=1200;
		ExplodeType=EXP_SMALL;
		ShotSpd=C_RED;
		ShotAcc=7;
		ShotDmg=40;
		FireDelay=300;
	}
	else if(Type==1){
		Price=25000;
		StopDistance=120;
		BulletPrice=30;
		MaxHealth=3000;
		ExplodeType=EXP_NORMAL;
		ShotSpd=C_RED;
		ShotAcc=16;
		ShotDmg=105;
		FireDelay=500;
		DualFire = TRUE;
		PipeDistance=8;
	}
	else if(Type==2){
		Size=1;
		Price=100000;
		BulletPrice=60;
		MaxHealth=10000;
		ExplodeType=EXP_BIG;
		ShotSpd=C_GREEN;
		ShotAcc=10;
		ShotDmg=100;
		FireDelay=60;
		DualFire = TRUE;
		PipeDistance=6;
	}
	Health=MaxHealth;

	nextDroid=NULL;
	SetBlit(32*4*Type , 32*2*Type, 32, 32, DIR_4, TT_PINGPONG, 4+(Type==2), &(TheGame->g_pDDSDroid));

}

Droid::~Droid(){
	if(nextDroid)
		delete nextDroid;
}


