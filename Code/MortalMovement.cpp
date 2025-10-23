#include "Mortal.h"
#include "Game.h"
#include "Player.h"
#include "MovementObject.h"
#include "OutOfMPEvent.h"
#include "Functions.h"
#include "DamageConstants.h"
#include "StopReason.h"
#include "Debugger.h"

void Mortal::Move(){

	DP2("Initiating Mortal Movement. MType: ",MType);
	DP2("Direction",Dir);
	DP2("lDirection",LockedDir);
	DP2("locked",Dir==LockedDir);
	DP2              ("Moving",Moving);
	if(MType==M_SG){
		Lock();
		if(LockedDir!=-1)
			Dir=LockedDir;
		return;
	}

	if(mo->CalculateNewMP()){
		DP(" Calculating New MP");

		mo->SetMover(this);
		mo->SetXYWay4();

		if(Moving*Speed==0){
			DP("tada");
			if(LockedDir==Dir){
				DP("tudelu");
				if(ConfirmLock()){
					DP("dada");
					Speed=LockSpeed;
					if(LockedEnemyDistance>StopDistance)
						Moving=1;
				}else{
					DP("nana");
					Moving=1;
					Speed=RegularSpeed;
				}
			}else if(TRAPPED&&NoLongerTrapped()){
				Moving=1;
			}
		}else 
			CalculateMaxMPDir4();


		
	}else{
		DP2("Keeping saved MP: ", mo->GetMP());
	}
	

	if(!IsAlive()||!mo->GetMP()){
		DP("Exiting Move() 1");
		mo->Reset();
		return;
	}
	Beacon(77712341);

	CalculateMortalMaxDistanceMP();
	mo->DetectCollisions(LockedDir!=-1&&BreaksWalls);
	Beacon(77712343);
	mo->DetectNewCenterSquareDistance();
		Beacon(77712344);


	Beacon(77712342);


	mo->MoveAndAct();

		Beacon(77712345);

	if(mo->GetMP()){
		DP(" Moving Again");
		Move();
		Beacon(77712347);
	}else{
		DP(" Exiting Move() 2");
		mo->Reset();
	Beacon(77712348);
	}

}

int  Mortal::ItemIsAMine(int Item){
	return Item == IT_MINE+1  || Item==IT_MINE1+1 || Item==IT_MINE2+1; 
}


Player *  Mortal::GetMineDamager(int Item){
	if(Item==IT_MINE+1)
		return NULL;
	return ThePlayers[Item- (IT_MINE1+1)];
}

void Mortal::HitMines(){
	// Hit any mines?
	Beacon(77798124);
	Beacon(80000000+Team);

	int Item=TheGame->WorldMap[GetXs()][GetYs()][2];

	if(ItemIsAMine(Item)){
		TheGame->WorldMap[GetXs()][GetYs()][2] = 0;
		Beacon(777252314);
		Mortal * Damager=GetMineDamager(Item);
		Damage( APDamage/2, DT_PMINE, B_MINE, Damager);
			Beacon(777250297);
		TheGame->RegExp(GetXs()*32,GetYs()*32,APDamage/2,RET_PMINE, DT_PMINE, Damager);
			Beacon(777250773 );

	}
	if( Size &&
		(Item == IT_TMINE1+1 || Item == IT_TMINE2+1) ){	
		int xWho = (TheGame->WorldMap[GetXs()][GetYs()][2]==IT_TMINE2+1);
		Damage( ATDamage/2, DT_TMINE, B_MINE, (Mortal *)ThePlayers[xWho]);
		TheGame->RegExp(Pos[0],Pos[1],ATDamage/2,RET_TMINE,DT_TMINE,(Mortal *)ThePlayers[xWho]);
	}
	Beacon(77725364);
	Beacon(80000000+Team);
}

int Mortal::ChangeDir(int InsideMovement){
	int nTRAPPED=!InternalChangeDir(InsideMovement);
	if(!TRAPPED&&nTRAPPED){
		DP("StartShooting???");
		Moving=0;
		if(IsShooter())
			((Shooter *)this)->StartShooting();
	}
	DP2("OR.TRAP",nTRAPPED);
	TRAPPED=nTRAPPED;
	return !TRAPPED;
}
int Mortal::NoLongerTrapped(){

	for(int d=0;d<4;d++){
		int x=GetXs(),y=GetYs();
		LengthAdjust(&x,&y,Get8Dir(),1);
		if(	(CheckSquare(x,y)&&TheGame->WorldMap[x][y][1]<=0) )
			return TRUE;
		Dir++;
		Dir%=4;
	}

	return FALSE;
}
int Mortal::InternalChangeDir(int InsideMovement){

	int goDir=Dir;

	Speed=RegularSpeed;

	if(LockedDir!=-1 && Go(LockedDir, TRUE)){
		// Locked Movement
		goDir=LockedDir;
		Speed=LockSpeed;
			
	}else{
		LockedDir=-1;

		// Regular Movement
		int Dirs[4]={goDir,(goDir+1)%4,(goDir+3)%4,(goDir+2)%4};

		if(rand()%2)
			// Turn left and right as many times
			IntSwitch(&Dirs[1],&Dirs[2]);

		if(rand()%2)
			// Try to turn
			IntSwitch(&Dirs[0],&Dirs[1]);
		
		int i;

		for(i=0; i<4;i++){
			if(Go(Dirs[i], FALSE)){
				goDir=Dirs[i];
				break;
			}
		}

		if(i==4){
			for(i=0; i<4;i++){
				if(Go(Dirs[i], TRUE)){
					goDir=Dirs[i];
					break;
				}
			}	
		
			if(i==4)
				return 0;
		}
	}
	Dir=goDir;
	if(InsideMovement)
		mo->SetXYWay4();

	return 1;

}


int Mortal::MoveThroughWall(int XS, int YS){
	if(BreaksWalls && LockedDir!=-1 && TheGame->WorldMap[ XS ][ YS ][0]<=16){
		TheGame->WorldMap[ XS ][ YS ][1]=0;
		WallSound();
		return 1;
	}
	return GoThroughWalls;
}

void Mortal::CalculateMortalMaxDistanceMP(){


	if(LockedDir==-1||!StopDistance)
		return;
	

	int MP2Move=-1;
	if(LockedEnemyDistance <= StopDistance ){
		MP2Move=0;
		ReasonToStop=StopDistanceReached;
		Moving=0;
		//mo->AddEvent(new OutOfMPEvent(0));
	}
	else if(mo->GetMP()>LockedEnemyDistance-StopDistance)
		MP2Move=LockedEnemyDistance-StopDistance;


	if(MP2Move>=0){
		Beacon(77722100+LockedEnemyDistance);
		Beacon(77722200+StopDistance);
		Beacon(77722300+MP2Move);
		mo->AddEvent(new OutOfMPEvent(MP2Move));
		if(MP2Move==0){
			// Only so that we shall confirm the lock
		}
		DP2(" StopDistance only allows: ", MP2Move);
	}

}

void Mortal::AddMeToCurrentQuadrant(){

	Beacon(777634);

	UpdateS();

	nextQuadMortal=TheMortalQuadrants[SX][SY];
	TheMortalQuadrants[SX][SY]=this;

	Beacon(77761234);

}

void Mortal::RemoveMeFromLastQuadrant(){
		Beacon(7772614);
	if(TheMortalQuadrants[SX][SY]==this){
		Beacon(7772615);
		TheMortalQuadrants[SX][SY]=nextQuadMortal;
	}else{
		Beacon(7772616);
		Mortal * c=TheMortalQuadrants[SX][SY];

		for(;c->nextQuadMortal!=this;c=c->nextQuadMortal);

		c->nextQuadMortal=nextQuadMortal;
	}
	
}
int Mortal::Go(int lDir, int BREAKEMDOWN){

	int xs=GetXs()+XChangeDir4(lDir);
	int ys=GetYs()+YChangeDir4(lDir);
	return CheckSquare(xs,ys) && 
		(TheGame->WorldMap[xs][ys][1]<=0 || 
		(BreaksWalls&&BREAKEMDOWN&&TheGame->WorldMap[xs][ys][0]<=16)  );
}



