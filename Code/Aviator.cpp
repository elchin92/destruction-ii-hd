// Aviator.cpp: implementation of the Aviator class.
//
//////////////////////////////////////////////////////////////////////

#include "Player.h"
#include "Game.h"
#include "TazerBeam.h"
#include "Aviator.h"
#include "Functions.h"
#include "Debugger.h"
#include <math.h>


Aviator * TheAviators;

const int aBlitX[NumberOfAviators]  ={256,160};
const int aBlitY[NumberOfAviators]  ={224,0};
const int aBlitW[NumberOfAviators]  ={32,64};
const int aBlitH[NumberOfAviators]  ={32,64};
const int aDirType[NumberOfAviators]={DIR_8,DIR_0}; // DIR_8 or DIR_0 accepted, see CalculateNewDir()
const int aacp[NumberOfAviators]    ={16,32};
const int  BeamDistance[NumberOfAviators] = {0,135};
const int  SeekDistance[NumberOfAviators] = {150,0};
const int  KillDistance[NumberOfAviators] = {20,0};
const int  LockAcc[NumberOfAviators]      = {2200, 0}; // mPixels/Frame^2
const int  MaxAcc[NumberOfAviators]       = {500, 1200}; // mPixels/Frame^2
const int  MinAcc[NumberOfAviators]	      = {100, 300};  // mPixels/Frame^2
const int  InvTurnProb[NumberOfAviators]  = {200, 30};
const double  TopSpeed[NumberOfAviators]  =	{1.75,5.4};  //  Pixels/Frame
const Sound   StartSound[NumberOfAviators]={sHeartBeat,sSignal};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Aviator::Aviator(int x, int y, AviatorType iaType){
	DP2("Aviator spawned!",iaType);
	aType=iaType;
	Pos[0]=x;
	Pos[1]=y;
	ActualCenterPos=aacp[iaType];
	nextAviator=NULL;
	Initialize();
	Beacon(2342228);

	KILLME=FALSE;
	Team=2;

	plDist[0]=plDist[1]=0;
	xPos[0]=xPos[1]=0;
	cSpeed[0]=cSpeed[1];
	LastTazer[0]=LastTazer[1]=0;
	Beacon(2342229);
	NewAcc();
	Beacon(2342240);
	SetBlit(aBlitX[aType] , aBlitY[aType], aBlitW[aType], aBlitH[aType], aDirType[aType], TT_NOTICK, 1,&(TheGame->g_pDDSSprites));
	Beacon(2342248);
	AddThisToLinkedList(TheAviators, nextAviator);
	
	LastTazer[0]=TheGame->thisTickCount;
	LastTazer[1]=TheGame->thisTickCount;
	Beacon(2342230);
	CreateAndPlaySound(StartSound[aType],dFar,spHigh);
	Beacon(2342231);
	
}

Aviator::~Aviator()
{	

}

void Aviator::Move(){
	// How many Frames?

	double Points = TheGame->TheFrame;

	if(Points>0){
		Points/=TheGame->Dlay;
		for(int xy=0;xy<2;xy++){
			//Travel
			Pos[xy]+=(int)(Points*cSpeed[xy]+xPos[xy]);
			xPos[xy]=(Points*cSpeed[xy]+xPos[xy]) - (int)(Points*cSpeed[xy]+xPos[xy]);
			// Accelerate

			cSpeed[xy]+= Points * Acc[xy];

			if(Pos[xy]>=TheGame->WorldSize*11*32||Pos[xy]<=-BlitDimension[xy])
				KILLME=TRUE;
		}

		double absSpeed=dist(cSpeed[0],cSpeed[1]);
		if(absSpeed>TopSpeed[aType]){
			cSpeed[0]=(cSpeed[0]*TopSpeed[aType])/absSpeed;
			cSpeed[1]=(cSpeed[1]*TopSpeed[aType])/absSpeed;
		}
		
		CalculateNewDir();

		CalculatePlayerDist();

		BeamPlayers();
		KillPlayers();

		if(!SetSeekAcc())
			while(Points>0){
				if (!(rand()%InvTurnProb[aType])){
					NewAcc();
					break;
				}
				Points--;
			}


		
		
	}
	
}
#define dabs(x) (x>=0?x:-x)
void Aviator::CalculateNewDir(){
	if(DirType==DIR_0)
		return;

	// Inv: DirType==DIR_8

	double x=cSpeed[0],y=cSpeed[1];
	if(dabs(x)>=2*dabs(y)){
		if(x>0)
			Dir=2;
		else
			Dir=6;
	
	}else if(dabs(y)>=2*dabs(x)){
		if(y>0)
			Dir=4;
		else
			Dir=0;	
	}else{
		if(x>0){
			if(y>0)
				Dir=3;
			else
				Dir=1;
		}else{
			if(y>0)
				Dir=5;
			else
				Dir=7;
		}
	}
}

void Aviator::NewAcc(){
	
	for(int xy=0;xy<2;xy++){	
		Acc[xy]=rand()%(MaxAcc[aType]-MinAcc[aType]);
		Acc[xy]+=MinAcc[aType];
		if(rand()%2)
			Acc[xy]=-Acc[xy];

		Acc[xy]/=1000.0;
		Acc[xy]/=TheGame->Dlay;
	}
	
}

void Aviator::CalculatePlayerDist(){
	for(int pl=0;pl<2;pl++){
		plDist[pl]=(int)sqrt(square(GetActualXPos()-ThePlayers[pl]->GetActualXPos())+
					   	     square(GetActualYPos()-ThePlayers[pl]->GetActualYPos()));
	}

}
void Aviator::BeamPlayers(){

	if(!BeamDistance[aType])
		return;

	BOOL HIT[2]={FALSE,FALSE};

	for(int pl=0;pl<2;pl++){
		if(plDist[pl]<=BeamDistance[aType]&&ThePlayers[pl]->IsAlive()){
			HIT[pl]=TRUE;
			while(LastTazer[pl]<TheGame->thisTickCount){
				LastTazer[pl]+=100;
				TazerBeam * newTazerBeam = new TazerBeam(this, 17, C_BLUE, BeamDistance[aType]);
				AddToLinkedList(TheTazerBeams, nextTazerBeam, newTazerBeam);
			}
		}
	}

	UpdateTazerSound(HIT[0]||HIT[1]);
	if(!HIT[0])
		LastTazer[0]=TheGame->thisTickCount;
	if(!HIT[1])
		LastTazer[1]=TheGame->thisTickCount;


}
void Aviator::KillPlayers(){
	if(!KillDistance[aType])
		return;

	for(int pl=0;pl<2;pl++){
		if(plDist[pl]<=KillDistance[aType]&&ThePlayers[pl]->IsAlive()){
			ThePlayers[pl]->Kill(NULL, DT_SPECIALUNIT);
		}
	}

}

int Aviator::SetSeekAcc(){
	if(!SeekDistance[aType])
		return FALSE;

	for(int pl=0;pl<2;pl++){
		if(plDist[pl]<=SeekDistance[aType] && plDist[pl]<plDist[!pl]&&ThePlayers[pl]->IsAlive()){
			Acc[0]=ThePlayers[pl]->GetActualXPos()-GetActualXPos();
			Acc[1]=ThePlayers[pl]->GetActualYPos()-GetActualYPos();
			// normalize V' vector
			double vectorLength=dist(Acc[0],Acc[1]);
			Acc[0]*=LockAcc[aType];
			Acc[1]*=LockAcc[aType];
			Acc[0]/=vectorLength*1000*TheGame->Dlay;
			Acc[1]/=vectorLength*1000*TheGame->Dlay;
			return TRUE;
		}
		
	}
	return FALSE;
}