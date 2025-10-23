#include "RocketTank.h"
#include "Game.h"

RocketTank::RocketTank(int X, int Y, int theType, int Who, int theDir){

	TheGame=theGame;
	Initialize();
	BLITDEAD=TRUE;
	MType=M_TK;
	Pos[0]=X;
	Pos[1]=Y;
	Dir=theDir;
	Team=Who;
	Size=0;
	TRIGGERAPMINES=TRUE;
	Type=theType;
	Strengths = Strengths | T_APEXP;
	Immunities = Immunities | T_APMINE;
	Thickness=TheGame->GetThick(TH_ROCKET);


	Price=35000;
	StopDistance=280;
	BulletPrice=10;
	Health=4000;
	ExplodeType=EXP_NORMAL;
	ShotType = GR_ROCKETS;
	ShotSpd=7;
	ShotAcc=45; // Angle
	ShotDmg=240;
	Prob = 0;
	FireDelay=300;
	DUALFIRE=TRUE;
	PipeDistance=2;
	


	nextRocketTank=NULL;
	SetBlit(0 , 0, 32, 32, DIR_4, FALSE, &(TheGame->g_pDDSRocketTank));

}

RocketTank::~RocketTank(){
	if(nextRocketTank)
		delete nextRocketTank;
}

