// Spawner.cpp: implementation of the Spawner class.
//
//////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "Spawner.h"
#include "Shooter.h"
#include "Settings.h"
#include "Player.h"
#include "Functions.h"
#include "shot.h"
Spawner * TheSpawnPit;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Spawner::Spawner()
{
	SumOfCashEarned = 0;	
}

Spawner::~Spawner()
{

}
void Spawner::SpawnEnemiesForNewRound(){

	UpdateSum();

	for(EnemyType et=(EnemyType)0;et<eTypes;et = (EnemyType)(et + 1)){
		int Amount = TheSettings->GetEnemyBaseCount(et);
		while(rand()%100<TheSettings->GetExtraEnemyPercentage(et))
			Amount++;
	
		while(Amount){
			SpawnParticularMortal(eType2MType[et]);
			Amount--;
		}
	}

}

void Spawner::SpawnParticularMortal(int MType){
	int ep=GetEarnedPoints();
	for(int uType=0; ep&&uType<Units[MType]-1; ep--){
		if(rand()%100<TheSettings->GetUpgradePercentage(MType2eType[MType]))
			uType++;
		if((rand()%(5+3*Units[MType]))==0)
			break;
	}
	SpawnMortal(MType, uType);
	
}


void Spawner::SpawnMortal(int MType, int uType){

	ThicknessType Type=(ThicknessType)(uType+SmallestUnitThicknessType[MType]);

	int r=rand()%(TheGame->WorldSize*11-11),
		t=rand()%(TheGame->WorldSize*11-11);
	
	r-=r%2;
	t-=t%2;
	r+=5;
	t+=5;
	int sr=r,st=t;
	do{
	// Find free spot to spawn

		r+=2;
		if(r>=TheGame->WorldSize*11-6){
			r=5;
			t+=2;
			if(t>=TheGame->WorldSize*11-6){
				t=5;
			}
		}
		if(r == sr && t == st){
			//NoSpace!
			return;
		}

	} while(!CreateMortalOK(Type,r*32,t*32));

	CreateMortal(r*32,t*32,Type,2);

}
void Spawner::SpawnRandomEnemy(){

	SpawnParticularMortal(eType2MType[rand()%eTypes]);
	
}


void Spawner::UpdateSum(){
	SumOfCashEarned = ThePlayers[0]->GetStat(ST_EARNED)+ThePlayers[1]->GetStat(ST_EARNED);
}
int Spawner::GetEarnedPoints(){
	
	return SumOfCashEarned/27000+1;
}

int Spawner::CreateMortalOK(ThicknessType tType, int X, int Y){
	return CreateGeneralObjectOK(TheGame->GetThick(tType), X, Y, TRUE);
}
int Spawner::CreateItemOK(int X, int Y, int BulletSensitive){
	return CreateGeneralObjectOK(16, X, Y, BulletSensitive);
}
int Spawner::CreateGeneralObjectOK(int Thick, int X, int Y, int BulletSensitive){

	if(BulletSensitive && TheShotQuadrants[X/32][Y/32])
		return FALSE;

	for(Mortal * Iterator = TheMortals;Iterator;Iterator=Iterator->nextMortal){
		if( Cross(X,Y,Thick, Iterator->GetX(), Iterator->GetY(), Iterator->GetThickness()))
				return FALSE;
	}

	return TRUE;

}

void Spawner::CreateMortal(int x, int y, int Type, int Team){
	// Check with CreateMortalOK() first!!!;


	if(Type>=T_SMALLESTSHOOTER){
		Shooter * temp = new Shooter(x,y,Type,Team);
	}else{
		Mortal * temp =new Mortal(x,y,Type,Team);
	}

}