#include "Game.h"
#include "Mortal.h"
#include "Functions.h"
#include "limits.h"
#include "Debugger.h"

void Mortal::Lock(){
	DP("Mortal::Lock()");
	if(!SEEKER)
		return;

	UnLock();
	int Temp=0;
	for(Mortal * aMortal = TheMortals;aMortal;aMortal=aMortal->nextMortal){

		int Temp = LockAble(aMortal);
	
		if(Temp>0 && (Temp < LockedEnemyDistance || LockedEnemyDistance == INT_MAX) ){
			LockedDir=GetLockDir(aMortal);
			LockedEnemyDistance=Temp;
			TheLockedEnemy=aMortal;
		}
	}

}
void Mortal::UnLock(){
	LockedEnemyDistance=INT_MAX;
	TheLockedEnemy=0;
	LockedDir=-1;
}
int Mortal::ConfirmLock(){
	

	int lDist=LockAble(TheLockedEnemy);
	DP2("ld",lDist);
	if(lDist>0 && GetLockDir(TheLockedEnemy)==Dir && TheLockedEnemy->IsAlive()){
		DP("still locked");
		LockedEnemyDistance=lDist;
		return TRUE;
	}
	DP2("pl",NULL!=TheLockedEnemy->IsPlayer());
	DP2("Alive",TheLockedEnemy->IsAlive());


	UnLock();

	return FALSE;


}


int Mortal::LockAble(Mortal * aMortal){

	if(Team==aMortal->GetTeam())
		return 0;
	if( this==aMortal )
		return 0;
	if(!TheGame->DoAttack(this, aMortal))
		return 0;
	if(aMortal->IsInvisible())
		return 0;
	if(!aMortal->IsAlive())
		return 0;


	if(GetXs() == aMortal->GetXs()){
		return abs(GetActualYPos()-aMortal->GetActualYPos());
	}
	if(GetYs() == aMortal->GetYs()){
		return abs(GetActualXPos()-aMortal->GetActualXPos());
	}
	if(MType==M_SG){
		int xDif=abs(Pos[0]-aMortal->GetX()), 
			yDif=abs(Pos[1]-aMortal->GetY());

		if(xDif+16>=yDif&&xDif-16<=yDif)
			return (int)dist(xDif,yDif);
	}
	return 0;
}

int Mortal::GetLockDir(Mortal * aMortal){

	if(aMortal==NULL)
		return -1;

	int mult=1+(MType==M_SG);

	if(GetXs() == aMortal->GetXs()){
		if(Pos[1]>aMortal->GetY())	
			return 0;
		return 2*mult;
	}
	if(GetYs() == aMortal->GetYs()){
		if(Pos[0]>aMortal->GetX())	
			return 3*mult;
		return 1*mult;
	}

	if(MType==M_SG){
		int xDif=aMortal->GetX()-Pos[0], 
			yDif=aMortal->GetY()-Pos[1];		
		if(xDif>0&&yDif<0)
			return 1;
		if(xDif>0&&yDif>0)
			return 3;
		if(xDif<0&&yDif>0)
			return 5;
		if(xDif<0&&yDif<0)
			return 7;
	}

	return -1;
}

int Mortal::TurnAround(){

	if(MType==M_PL||MType==M_SG){
		return 0;
	}
	if(!Moving)
		return 0;
	DP("Turned around");
	int wDir=Dir;
	if(DirType==DIR_4){
		wDir-=2;
		if(wDir<0)
			wDir+=4;
	}
	else{
		wDir-=4;
		if(wDir<0)
			wDir+=8;
	}
	if(!(Pos[0]%32==0&&Pos[1]%32==0)||Go(wDir,FALSE)){
		Dir=wDir;
		return 1;
	}
	return 0;
}


int Mortal::GetDistance(){
	
	int Dist=0, Temp=0;
	for(Mortal * aMortal = TheMortals;aMortal;aMortal=aMortal->nextMortal){
		int Temp = LockAble(aMortal);
		if(Temp && (Temp < Dist || !Dist)){
			Dist=Temp;
		}
	}
	
	return Dist;
}



int Mortal::MortalInteraction(Mortal * Collider){
	Beacon(23422);


	if(GetTeam()==Collider->GetTeam()){
		
		if(TurnAround()&&Get8Dir()==Collider->Get8Dir())
			Collider->TurnAround();
		return WAY_TURN;
	}

	if(TheGame->DoAttack(this, Collider)){
		if(GetSize()>Collider->GetSize()){
			Collider->Kill(this, DT_RUNOVER);
			return WAY_KILL;
		}
		if(GetMType() == M_CL && Collider->GetMType() == M_PL){
			Collider->Kill(this, DT_SPECIAL);
			Kill(this, DT_NONE);
			return WAY_DIED;
		}
		return WAY_STOP;
	}
	int CanTurn=TurnAround();
	if(Get8Dir()==Collider->Get8Dir()){
		if(TheGame->DoAttack(Collider, this)){
			if(Collider->GetMType() == M_CL && GetMType() == M_PL){
				Kill(Collider, DT_SPECIAL);
				Collider->Kill(Collider, DT_NONE);
				return WAY_DIED;
			}
			return WAY_STOP;
		}
		if(CanTurn)
			Collider->TurnAround();
	}
	return WAY_TURN;
}