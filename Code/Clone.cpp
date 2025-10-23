#include "Clone.h"
#include "Game.h"
#include "Player.h"
#include "PlayerBlit.h"
#include "DamageConstants.h"
#include "Debugger.h"

Clone * TheClones;

Clone::Clone(int X, int Y, int iDir ,int iTeam){
	Pos[0]=X;
	Pos[1]=Y;
	ActualCenterPos=16;
	DP2("XS",SX);
	DP2("XY",SY);
	Mortal::Initialize(TRUE);
	DP2("XS",SX);
	DP2("CXY",SY);
	MaxHealth=250;
	Health=MaxHealth;
	MType=M_CL;
	Dir=iDir;
	Team=iTeam;
	nextClone=NULL;
	ExplodeType=EXP_SINGLE;
	BLITDEAD=FALSE;
	SEEKER=ThePlayers[iTeam]->CUpgrade();

	Thickness=TheGame->GetThick(T_PLAYER);
	Beacon(3246327);
	AddThisToLinkedList(TheClones, nextClone);

	PLSETBLIT;
	DirType=DIR_4;

	SetStrength(T_APMINE);
}

Clone::~Clone(){

}

void Clone::CollectItem(){

	Beacon(13534543);

	if(!ThePlayers[Team]->CUpgrade()){
		Beacon(13534541);
		return;
	}
	Beacon(13534545);

	int Item=TheGame->WorldMap[GetXs()][GetYs()][2];
	if(Item){

		int X=GetXs(),Y=GetYs();

		Player * CoverOwner=NULL;

		for(int pl=0;pl<2;pl++)
			if(ThePlayers[pl]->QueryCoverItem(X,Y))
				CoverOwner=ThePlayers[pl];

		if(CoverOwner){
			TheGame->RegExp(X*32,Y*32,CoverDamage, RET_COVER, DT_SPECIAL, CoverOwner);
			CreateAndPlaySound(sLaugh,dInfinite,spNormal);
			Kill(CoverOwner, DT_SPECIAL);
		}
			
		if(CoverOwner==NULL){
			ThePlayers[Team]->CollectItem(Item);
		}
		if(Item-1!=IT_TMINE1&&Item-1!=IT_TMINE2){
			TheGame->WorldMap[X][Y][2]=0;
		}
	}
	Beacon(13534542);	
}

Clone * Clone::IsClone(){return this;}