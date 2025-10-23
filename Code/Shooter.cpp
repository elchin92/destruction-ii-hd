#include "Shooter.h"
#include "Shot.h"
#include "Game.h"
#include "Functions.h"
#include "TazerBeam.h"
#include "Debugger.h"
#include "StopReason.h"
#include <Math.h>
Shooter * TheShooters;


Shooter::Shooter(int X, int Y, int theType, int Who){

	Pos[0]=X;
	Pos[1]=Y;
	Initialize();

	if(theType<T_SMALLESTSHOOTER)
		TheGame->ErrorReport(104);


	Type=theType;
	Team=Who;
	Thickness=TheGame->GetThick(theType);

	SetImmunity(T_APMINE);

	if(Type<T_DROID+Units[M_DR]){

		SetWeakness(T_REGEXP);
		SetStrength(T_ATMINE);
		SetStrength(T_NRJ);

		MType=M_DR;
		Size=0;

		bType = B_NRJ;
		Prob=0;

		LastTazer=TheGame->thisTickCount;

		if(Type==T_INFANTRYDROID){
			Price=12000;
			StopDistance=100;
			BulletPrice=10;
			MaxHealth=950;
			ExplodeType=EXP_SMALL;
			ShotSpd=C_RED;
			ShotAcc=7;
			ShotDmg=30;
			ShotToShotTime=300;
			PipeLength[0]=10;
			RegularSpeed=1.8;			
		}
		else if(Type==T_BATTLEDROID){
			Price=38000;
			StopDistance=150;
			BulletPrice=30;
			MaxHealth=3500;
			ExplodeType=EXP_NORMAL;
			ShotSpd=C_RED;
			ShotAcc=12;
			ShotDmg=70;
			ShotToShotTime=200;
			LeftRightShot = TRUE;
			PipeDistance=8;
			RegularSpeed=1.7;
			LockSpeed=2.2;
			PipeLength[0]=9;
			PipeLength[1]=9;
		}
		else if(Type==T_WALKER){
			Size=1;
			Price=120000;
			BulletPrice=60;
			MaxHealth=10000;
			ExplodeType=EXP_BIG;
			ShotSpd=C_GREEN;
			ShotAcc=8;
			ShotDmg=50;
			ShotToShotTime=60;
			RegularSpeed=1.5;
			LockSpeed=2;
			LeftRightShot = TRUE;
			PipeDistance=6;
			PipeLength[0]=9;
			PipeLength[1]=9;
		}

		Type-=T_DROID;

		SetBlit(32*4*Type , 0, 32, 32, DIR_4, TT_PINGPONG, 4+(Type==2), &(TheGame->g_pDDSDroid));
	}else if(Type<T_TANK+Units[M_TK]){

		MType=M_TK;

		SetStrength(T_APEXP);
		
		Size=1;



		if(Type==T_MINITANK){
			Price=24000;
			BulletPrice=15;
			MaxHealth=3000;
			ExplodeType=EXP_NORMAL;
			bType = B_556;
			ShotSpd=12;
			ShotAcc=10;
			ShotDmg=40;
			Prob = 0;
			ShotToShotTime=200;
			PipeLength[0]=12;
			RegularSpeed=1.3;
			LockSpeed=1.8;
		}
		else if(Type==T_SILVERTANK){
			Price=40000;
			BulletPrice=30;
			MaxHealth=4500;
			ExplodeType=EXP_NORMAL;
			bType = B_762;
			ShotSpd=18;
			ShotAcc=13;
			ShotDmg=80;
			Prob = 0;
			ShotToShotTime=180;
			PipeLength[0]=15;
			RegularSpeed=1.7;
			LockSpeed=2.0;
		}
		else if(Type==T_DOUBLEBARRELEDTANK){
			Price=70000;
			BulletPrice=50;
			MaxHealth=6250;
			ExplodeType=EXP_NORMAL;
			bType = B_762;
			ShotSpd=15;
			ShotAcc=14;
			ShotDmg=80;
			Prob = 0;
			ShotToShotTime=90;
			LeftRightShot = TRUE;
			PipeDistance=4;
			RegularSpeed=1.5;
			LockSpeed=2.0;
			PipeLength[0]=10;
			PipeLength[1]=10;
		}
		else if(Type==T_BIGTANK){
			Price=180000;
			BulletPrice=80;
			MaxHealth=18000;
			ExplodeType=EXP_BIG;
			bType = B_12;
			ShotSpd=22;
			ShotAcc=15;
			ShotDmg=120;
			AbType = B_APRKT;
			AShotSpd = 7;
			AShotAcc = 40;
			AShotDmg = 600;
			Prob = 11;
			ShotToShotTime=80;
			RegularSpeed=2.5;
			LockSpeed=4;			
			PipeLength[0]=15;
		}else if(Type==T_ROCKETTANK){

			Price=96000;
			StopDistance=0;
			BulletPrice=10;
			MaxHealth=7000;
			ExplodeType=EXP_NORMAL;
			bType = GR_ROCKET;
			ShotSpd=11;
			ShotAcc=45; // Angle
			ShotDmg=480;
			Prob = 0;
			ShotToShotTime=700;
			LeftRightShot=TRUE;
			PipeDistance=2;
			RegularSpeed=2.5;
			LockSpeed=1.5;
			PipeLength[0]=9;
		}
		Type-=T_TANK;
		if(Type==4)
			SetBlit(0 , 0, 32, 32, DIR_4, TT_ROLLER, 3,&(TheGame->g_pDDSRTank));
		else
			SetBlit(32*4*((Type)%2) , 32*2*((Type)/2), 32, 32, DIR_4, TT_NOTICK, 1, &(TheGame->g_pDDSTank));
	}else if(Type==T_SENTRYGUN){

		Type-=T_SENTRYGUN;

		MType=M_SG;
		Size=1;

		bType = B_762;
		Prob=0;

		if(Type==0){
			Price=0;
			BulletPrice=0;
			MaxHealth=700;
			ExplodeType=EXP_SMALL;
			ShotSpd=14;
			ShotAcc=6;
			ShotDmg=50;
			ShotToShotTime=80;
			PipeLength[0]=15;
		}

		SetBlit(0 , 0, 32, 32, DIR_8, TT_ROLLER, 2, &(TheGame->g_pDDSMachineGun));
	}

	Health=MaxHealth;
	Dir=rand()%4;
	ChangeDir(FALSE);
}

Shooter::~Shooter(){

}


int Shooter::GetShots(int MaxShots){
	// Relies on that a double subtraction is >= 0 and == 0 when a-b & a<b
	int TimePassed=(int)(TheGame->thisTickCount - LastShotTime);
	if(TimePassed>=ShotToShotTime){
		int Shots = (int)(TimePassed/ShotToShotTime);

		if(MaxShots<Shots){
			Shots=MaxShots;
		}

		double dAddition=ShotToShotTime*(double)Shots+LastShotFrame;
		LastShotTime+=(int)dAddition;
		LastShotFrame= dAddition - (int)dAddition;
		// Adds the last shot first, so that they will move in the right order
		CurrentShotExtraTimeFrame=TheGame->thisTickCount-LastShotTime+LastShotFrame;

		return Shots;
	}

	return 0;
}
void Shooter::StartShooting(int ExtraDelay){
	__int64 pLastShotTime=TheGame->thisTickCount+ExtraDelay;
	if(pLastShotTime>LastShotTime){
		LastShotTime=pLastShotTime;
	}
}
double Shooter::GetCurrentShotExtraTimeFrame(){
	return CurrentShotExtraTimeFrame;
}
void Shooter::Fire(){

	if(!IsAlive())
		return;

	if(MType==M_DR){
		while(LastTazer<TheGame->thisTickCount){
			LastTazer+=100;
			TazerBeam * newTazerBeam;
			newTazerBeam = new TazerBeam(this, 10, C_BLUE, 48);
			AddToLinkedList(TheTazerBeams, nextTazerBeam, newTazerBeam);
			UpdateTazerSound(newTazerBeam->HitAnyone());
		}

	}else if(MType==M_SG){
		// Sentry Gun spin code
		if(LockedDir==-1){
			// Spin Down
			if(lldir!=-1){
				nextSpin=TheGame->thisTickCount;
			}

			while(Spin&&nextSpin<TheGame->thisTickCount){
				Spin--;
				nextSpin+=SPINDOWNTIME/SPINSTEPS;
			}

		}else{
			if(lldir==-1){
				nextSpin=TheGame->thisTickCount;
				LastTazer=TheGame->thisTickCount;
			}
			// Spin Up
			while(Spin<SPINSTEPS&&nextSpin<TheGame->thisTickCount){
				Spin++;
				nextSpin+=SPINUPTIME/SPINSTEPS;
				if(Spin==SPINSTEPS)
					StartShooting();
			}
		
		}

		// Spin
		while(Spin&&TheGame->thisTickCount>LastTazer){
			TickUp(1);
			LastTazer+=((int)(ShotToShotTime*Spin))/SPINSTEPS;
		}
		lldir=LockedDir;
		if(Spin!=SPINSTEPS)
			return;
	}

	if(LockedDir!=Dir){
		if(!TRAPPED || bType==GR_ROCKET ){
			StartShooting();
			return;
		}
		if(TRAPPED&&LockedDir!=-1){
			// A hack to make sure firers doesn't shoot themselves
			// If a tank is cornered by walls and another unit, this 
			// other unit can try to turn the tank which fails and
			// the tanks thinks he is cornered (TRAPPED), but he still
			// needs a lockeddir to fire
			// When the shooters are spawned, they might be trapped
			// but they are never locked so this doesn't matter!
			StartShooting();
			return;
		}
	}

	DP2("TRAPPED",TRAPPED);
	DP2("Dir",Dir);

	DP2("lDir",LockedDir);
	DP("New Fire Turn");
	for(int Shots=GetShots();Shots;Shots--,CurrentShotExtraTimeFrame+=ShotToShotTime){
		DP2("Shots",Shots);
		LeftRightShot=-LeftRightShot;

		if(!Prob||(rand()%Prob)){
			if(bType!=GR_ROCKET)
				TheGame->AddShot(LeftRightShot,ShotSpd,ShotAcc,bType,ShotDmg,0, this);
			else
				TheGame->AddShot(LeftRightShot,ShotSpd,CalculateAngle(ShotSpd),bType,ShotDmg,0, this);
		}
		else
			TheGame->AddShot(LeftRightShot,AShotSpd,AShotAcc,AbType,AShotDmg,0, this);

		

	}

}

void Shooter::Initialize(int ADDMORTAL, int ADDSHOOTER){
	
	LastShotTime=0;
	LastShotFrame=0;

	Mortal::Initialize(ADDMORTAL);

	SEEKER=TRUE;
	TazerSound=NULL;

	bType = B_9;
	ShotSpd=10;
	ShotAcc=10;
	ShotDmg=10;
	AbType = B_9;
	AShotSpd = 10;
	AShotAcc = 20;
	AShotDmg = 10;
	Prob = 10;
	ShotToShotTime=100;
	LeftRightShot = FALSE;
	PipeDistance=0;
	nextSpin=0;
	Spin=0;
	lldir=-1;
	nextShooter=NULL;

	if(ADDSHOOTER)
		AddThisLastToLinkedList(Shooter, TheShooters, nextShooter);

}

double Shooter::CalculateAngle(int iSpeed){

	double Temp = 0.1;
	

	Speed=LockSpeed;
	if(LockedEnemyDistance<CalculateDistance(Temp,iSpeed )){

	}
	else if(LockedEnemyDistance>CalculateDistance(pi/4.0,iSpeed)){
		Temp=pi/4.0;
		
	}
	else{ 
		Moving=0;
		ReasonToStop=StopDistanceReached;
		while(LockedEnemyDistance >= CalculateDistance(Temp, iSpeed) ){
			Temp+=1.0/128.0;
		}
	}

/*	TheGame->Beacon(LockSpeed+999998);
	TheGame->Beacon(Distance);
	TheGame->Beacon(CalculateDistance(Temp, Speed));
	TheGame->Beacon(CalculateDistance(0.1, Speed));
	TheGame->Beacon(CalculateDistance(0.11, Speed));
	TheGame->Beacon(CalculateDistance(pi/4.0, Speed));*/
	return Temp;
		
}

int Shooter::CalculateDistance(double Angle, int Speed){

	return (int)((2.0*invg[0]*(double)( (double)Speed*sin(Angle) )+1.0)*(double)( (double)Speed*cos(Angle) ));

}



Shooter::Shooter(){
	Beacon(9999);
}

int Shooter::GetPipeDistance(int Rotated){
	return PipeDistance;
}


int Shooter::GetPipeLength(int iLeftRightShot){
	int Adjust = 0;
	if(MType==M_DR){
		if(Type==0&&Tick==0){
			Adjust=1;
		}else if(Type==1){
			if(Tick==0){
				Adjust=iLeftRightShot;
			}else if(Tick==3){
				Adjust=-iLeftRightShot;
			}
		
		}
	}

	return PipeLength[iLeftRightShot==1]+Adjust;
}

int Shooter::GetPipeSideAdjust(){
	return 0;
}

Shooter * Shooter::IsShooter(){return this;}