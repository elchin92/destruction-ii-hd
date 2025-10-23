#include "LaserBeam.h"
#include "Game.h"
#include "Mortal.h"
#include "Player.h"
#include "Functions.h"
#include "SoundEngine.h"
#include "Debugger.h"

LaserBeam * TheLaserBeams;

int lbcr=1, lbdl=1;

void LaserBeam::KillAll(){
	if(nextLaserBeam){
		nextLaserBeam->KillAll();
		delete nextLaserBeam;
	}

}	



LaserBeam::LaserBeam(int StartX, int StartY, int Askew, int Damage, Mortal * iOwner, int Color){


	Pos[0]=StartX; 
	Pos[1]=StartY;
	ActualCenterPos=0;

	Actor::Initialize();



	BeamColor=Color;

	KILLME=FALSE;
	nextLaserBeam=NULL;
	StopTime=TheGame->thisTickCount+LASER_VISIBLE_TIME;
	Team = iOwner->GetTeam();

	UpdateS();
	if(OutOfMap()){
		KILLME=TRUE;
		return;
	}

	//Mortal * Victim = MoveInfiniteFast();
	int AskCounter[2]={0,0};
	int cPos[2],Ask[2]={0,0},Step[2]={0,0};

	int Direction=iOwner->Get8Dir();
	if(iOwner->GetMType()==M_PL){
		GoThroughWalls=((Player *)iOwner)->ShootingSuper();
		INVISIBLE=((Player *)iOwner)->ShootsInvisible();
	}

	LengthAdjust(&Step[0],&Step[1],Direction,1);
	SideAdjust(&Ask[0],&Ask[1],Direction,Askew);

	if(Direction%2)
		Ask[rand()%2]=0;

	Pos[0]+=Step[0];
	Pos[1]+=Step[1];


	int WorldSize=TheGame->WorldSize;

	int tx=Pos[0], ty=Pos[1]; // Hack to make OutOfMap() and HitWall() work...
	int T=0;
	while(TRUE){
		
		UpdateS();

		if(OutOfMap() || ( HitWall() && !GoThroughWalls )  ){
			break;
		}

		T++;
		for(int xy=0;xy<2;xy++){
			Pos[xy]+=Step[xy];
			AskCounter[xy]+=Ask[xy];
			while(AskCounter[xy]>ACCPARTS||AskCounter[xy]<-ACCPARTS){
				int way=2*(AskCounter[xy]>0)-1;
				AskCounter[xy]-=way*ACCPARTS;
				Pos[xy]+=way;
			}
		}
	}

	cPos[0]=Pos[0];cPos[1]=Pos[1];
	Pos[0]=tx;Pos[1]=ty; // Hack end

	for(int g=0;g<2;g++){
		Step[g]*=ACCPARTS;
		Step[g]+=Ask[g];
	}



	Mortal * theLockedOne=NULL;

	int TT, mPos[2];
	int Thick, bonus, HitPosition;


	for(Mortal * aMortal = TheMortals;aMortal;aMortal=aMortal->nextMortal){
		if(aMortal!=iOwner){
			Thick=aMortal->GetThickness();
			mPos[0]=aMortal->GetActualXPos()-Thick;
			mPos[1]=aMortal->GetActualYPos()-Thick;

			for(int side=0, pointer=0,xy;side<4;side++){
				xy=side%2;
				if(Step[xy]){
					bonus=(side/2)*Thick*2;
		
					TT=(ACCPARTS*(mPos[xy]+bonus-Pos[xy]))/Step[xy];

					HitPosition=Pos[!xy]+(TT*Step[!xy]/ACCPARTS);

					if(HitPosition>=mPos[!xy]&&HitPosition<=mPos[!xy]+2*Thick){
						if(TT>=0&&TT<T){
							theLockedOne=aMortal;
							T=TT;
						}
					}
				}
			}
		}
	}

	EndPos[0]=Pos[0]+(Step[0]*T)/ACCPARTS;
	EndPos[1]=Pos[1]+(Step[1]*T)/ACCPARTS;


	if(theLockedOne){
		theLockedOne->Damage(Damage,DT_NRJ, B_NRJ,iOwner);
	}else if(!( cPos[0]<0||cPos[1]<0||cPos[0]>=WorldSize*352||cPos[1]>=WorldSize*352) ){

		TheGame->WorldMap[cPos[0]/32][cPos[1]/32][1]-=Damage;
		if(	TheGame->WorldMap[cPos[0]/32][cPos[1]/32][1]<=0 )
			TheSoundEngine->PlayLocatedSound(sWallDown,EndPos[0],EndPos[1],BREAKDIST,BREAKPRI);
	}
	if(INVISIBLE)
		StopTime=0;

	DP2("Creating a laserbeam",lbcr++);
}

/*
void MoveInfiniteFast(){

  	mo->SetMover(this);
	int afMP[2]={0,0}; 
	while(TRUE){
			
		{
		    // move forward
			mo->SetMP();
			mo->SetXYWay8();
		}
		{
			// move askew
			mo->SetMP();
			setxyway8 askew
		  }
	
	  moveonestep


	}

	mo->Reset();
	// calculate endpos
}

void ::MoveOneStep(){

	mo->DetectCollisions(DONT ADD TO SHOTQUADRANTS NOR MORTALQUADRANTS);
	mo->MoveAndAct();
	afMP[af]+=mo->GetMovedPoints();

	if(mo->GetMP())
		MoveOneStep();
}
*/

LaserBeam::~LaserBeam(){	DP2("Deleting a laserbeam",lbdl++);}
void LaserBeam::Move(){}
void LaserBeam::Blit(int where){
	Beacon(8855551);
	if(nextLaserBeam)
		nextLaserBeam->Blit(where);
	Beacon(8855552);
	if(TheGame->thisTickCount>StopTime){
		KILLME=TRUE;
		Beacon(8855553);
	}
	else{
		TheGame->BlitBeam(&Pos[0],&EndPos[0],BeamColor,where);
		Beacon(8855554);
	}
}




/*

*/