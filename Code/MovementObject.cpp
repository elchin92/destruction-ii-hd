
#include "shot.h"
#include "Mortal.h"
#include "MovementObject.h"
#include "Game.h"
#include "WallEvent.h"
#include "WorldEvent.h"
#include "OutOfMPEvent.h"
#include "ActorEvent.h"
#include "NewSquareEvent.h"
#include "CenterSquareEvent.h"
#include "Quadrants.h"
#include "Debugger.h"
#include <algorithm> // C++20: for std::min



MovementObject * mo;

MovementObject::MovementObject(){nextStop=NULL;MP=0;}
MovementObject::~MovementObject(){	SAFE_DELETE(nextStop);}



int MovementObject::CalculateNewMP(){
	Beacon(77789246);
	if(!MP){
		Beacon(77789247);
		return TRUE;
	}
	SAFE_DELETE(nextStop);
	Beacon(7778928);
	nextStop = new OutOfMPEvent(MP);	
	Beacon(7778927);

	return FALSE;
}

void MovementObject::SetMover(Actor * iMover){
	Mover=iMover;
}
void MovementObject::SetXYWay4(){
	int Dir=Mover->GetRealDir();
	Way[0]=(Dir==1 || Dir==2) - (Dir==0 || Dir ==3);
	xy =(Dir==0 || Dir==2);
	MovingTwice=FALSE;
}

void MovementObject::SetXYWay8(){
	int Dir=Mover->GetRealDir();
	Way[1]=( Dir==1 ||  Dir==3) - ( Dir==5 ||  Dir ==7);
	MovingTwice=Dir%2;
	Dir-=MovingTwice;
	Way[0]=(Dir==2 || Dir==4) - (Dir==0 || Dir ==6);
	xy =(Dir==0 || Dir==4);


}

void MovementObject::MoveAskew(int aMP){
	// Inv :: xy & Way is set

	xy=!xy;

	int samedir = Way[0]==Way[1];
	Way[0]=(aMP>=0)-(aMP<0);

	if(MovingTwice){
		Way[1]=Way[0];
		if(samedir)
			Way[1]=-Way[0];

	}

	MP=abs(aMP);

}


void MovementObject::SetMP(int iMP){
	MP=iMP;
	SAFE_DELETE(nextStop);
	nextStop = new OutOfMPEvent(iMP);
}


void MovementObject::AddEvent(MovementEvent * me){
	if(me->GetMP()>MP){
		DP("  MovementEvent discarded");
		delete me;
		return;
	}
		Beacon(7774566);
	if(nextStop->GetMP()>=me->GetMP()){
		DP("  MovementEvent inserted first");
		me->nextEvent=nextStop;
		nextStop=me;
		Beacon(7774567);
	}else{
		Beacon(7774568);
		DP("  MovementEvent going deeper...");
		nextStop->insertEvent(me);
	}
}
MovementEvent * MovementObject::MoveAndAct(){
	
	Beacon(77745662);

	MovementEvent  * lastStop = nextStop->ActLoop();

	Beacon(77745672);


	DP2(" Moved: ",lastStop->GetMP());
	TwoWayLoop{
		if(Mover->IsPlayer()){
			DP2(" >Pos ",twxy);
			DP2(" >+= ",Way[i]*lastStop->GetMP());
			DP2(" >Dir ",Mover->GetRealDir());
		}
		Mover->MoveXY( twxy, Way[i]*lastStop->GetMP());
	}

	Mover->TickUp(lastStop->GetMP());
	Beacon(77745673);
	if(lastStop->GetStopType()==ST_TOTALSTOP ){
		Reset();
		return lastStop;
	}
	if(lastStop->GetStopType()==ST_SAVEMP){
		// Don't reset, MP might be saved!
		lastStop->Act();
		SaveMP(lastStop->GetMP());
		return lastStop;
	}
	Beacon(7772713);
	// Inv: Sliding
	if(Mover->IsPlayer() && MovingTwice){
	Beacon(7772714);
		MovingTwice=FALSE;

		if(lastStop->GetStopType() == ST_SLIDESECONDARY){
			xy=!xy;
			Way[0]=Way[1];	
				Beacon(7772715);

		}
			// Inv: lastStop->GetStopType() == ST_SLIDEPRIMARY
		if(Way[0])
		SaveMP(lastStop->GetMP());


	}else{ 
		Reset();
	}
	return lastStop;
}


void MovementObject::DetectCollisions(int CollideWithWalls){

	Beacon(77780);
	SetMoverCollisionPos();

	Beacon(77781);		 // In case of Equal MP
	DetectOutOfMapCollisions();  // First go in first

	Beacon(77781);
	if(CollideWithWalls){
		DetectWallCollisions();      // Comes in last
		Beacon(77783);
	}

	DetectMortalCollisions();
	if(Mover->IsMortal())
		DetectShotCollisions();
	Beacon(77782);

	Beacon(77784);
	SaveSquareMP();	             // Comes in second
	Beacon(77785);

}

void MovementObject::SaveSquareMP(){


	//int tMaxMP, MaxMP=32;
	int MaxMP[2]={32,32};
	//int TargetSquare[2];

	TwoWayLoop{
		MaxMP[i]=(Mover->GetActualPos(twxy))%32;


		if(Way[i]==1)
			MaxMP[i]=32-MaxMP[i];
		else MaxMP[i]++;
	}

	int tMaxMP=std::min(MaxMP[0],MaxMP[1]);

	if(tMaxMP>=1&&tMaxMP<=GetMP())
		AddEvent(new NewSquareEvent(tMaxMP, Mover ));

}

int MovementObject::GetMP(){
	return MP;
}

void MovementObject::Reset(){
	Beacon(77723452);
	MP=0;
	Beacon(77723453);
}
void MovementObject::SaveMP(int MovedMP){
	MP-=MovedMP;
	SAFE_DELETE(nextStop);
}



// Collision related



#define Forward 0
#define Lower	1
#define Upper	2


#define CollisionMP (CollisionDist[i]*Way[i])
#define Collide() (CollisionMP >= 0 && CollisionMP <= GetMP() && MoverPos[Lower][i] + (CollisionMP+1)*Way[!i] <= CollidePos[Upper][i] && MoverPos[Upper][i] + (CollisionMP+1)*Way[!i] >= CollidePos[Lower][i] )

void MovementObject::SetColliderCollisionPos(shot * Collider){
	TwoWayLoop{
		CollidePos[Forward][i] = Collider->GetActualPos( twxy); // xy Pos
		CollidePos[Lower  ][i] = Collider->GetActualPos(!twxy);	// !xy Pos top
		CollidePos[Upper  ][i] = Collider->GetActualPos(!twxy);	// !xy Pos bottom
	}
}

void MovementObject::SetColliderCollisionPos(Mortal * Collider){
	TwoWayLoop{
		CollidePos[Forward][i] = Collider->GetActualPos( twxy) - Way[i] * Collider->GetThickness(); // xy Pos
		CollidePos[Lower  ][i] = Collider->GetActualPos(!twxy) - Collider->GetThickness();			// !xy Pos top
		CollidePos[Upper  ][i] = Collider->GetActualPos(!twxy) + Collider->GetThickness();			// !xy Pos bottom
	}
}


void MovementObject::SetMoverCollisionPos(){


	TwoWayLoop{
		MoverPos[Forward][i] = Mover->GetActualPos( twxy) + Way[i] * Mover->GetThickness();	 // xy Pos
		MoverPos[Lower  ][i] = Mover->GetActualPos(!twxy) - Mover->GetThickness();				// !xy Pos top
		MoverPos[Upper  ][i] = Mover->GetActualPos(!twxy) + Mover->GetThickness();			// !xy Pos bottom
	}
}
void MovementObject::CalculateCollisionDistances(){
	CollisionDist[0] = CollidePos[Forward][0]-MoverPos[Forward][0]-Way[0];
	CollisionDist[1] = MovingTwice*(CollidePos[Forward][1] - MoverPos[Forward][1] - Way[1]);
}

int MovementObject::WallBit(int wxy, int wWay){ 

	if(wxy==0&&wWay==-1)
		return 1;
	if(wxy==0&&wWay==1)
		return 2;	
	if(wxy==1&&wWay==-1)
		return 4;
	if(wxy==1&&wWay==1)
		return 8 ;
	
	return 0;
}
#define IsBasePiece(i) (i>16)
#define DoWallExtension(dxy, dWay, dext) (  IsBasePiece(TheGame->WorldMap[NineX][NineY][0]) ||  ((dxy)==(dext)  &&  ((TheGame->WorldMap[NineX][NineY][0]-1) & WallBit((dxy),(dWay)))!=0) )
//#define DoWallAdjust 0
void MovementObject::DetectShotCollisions(){

	Beacon(7778545);

	if(Mover->GoesThroughActors())
		return;
	Beacon(7778546);

	ShotQuadLoop(){
			Beacon(7778547);

			Beacon(7778548);
		SetColliderCollisionPos(Iterator);
			Beacon(7778549);
		CalculateCollisionDistances();
			Beacon(77785410);
		TwoWayLoop
			if(Collide()){
				DP2(" ActorCollision added ", CollisionMP);
				AddEvent(new ActorEvent(CollisionMP, Mover, Iterator, !i));
			}
		
	}
				
	Beacon(77722345);

}
void MovementObject::DetectMortalCollisions(){

	Beacon(7778545);

	if(Mover->GoesThroughActors())
		return;
	Beacon(7778546);

	MortalQuadLoop(){
			Beacon(7778547);
		if(Mover->CouldCollideWith(Iterator)) {
				Beacon(7778548);
			SetColliderCollisionPos(Iterator);
				Beacon(7778549);
			CalculateCollisionDistances();
				Beacon(77785410);
			TwoWayLoop
				if(Collide()){
					DP2(" ActorCollision added ", CollisionMP);
					AddEvent(new ActorEvent(CollisionMP, Mover, Iterator, !i));
				}
		}
	}
				
	Beacon(77722345);

}

void MovementObject::DetectWallCollisions(){
	Beacon(7773001);
	if(Mover->GoesThroughWalls()){
		DP("Mover Goes through walls");
		return;
	}
	Beacon(7773002);
	NineLoop()
		if(TheGame->WorldMap[ NineX  ][ NineY ][1]>0){
			Beacon(7773003);
			
			for(int wext=0;wext<2;wext++){

				int Valid[2]={0,0};

				TwoWayLoop{
					int CheckBlownUpX=NineX-Way[i]*(twxy==0), 
						CheckBlownUpY=NineY-Way[i]*twxy;

					int BlownUpOrBaseOK=( CheckSquare(CheckBlownUpX,CheckBlownUpY) &&
							   ( TheGame->WorldMap[CheckBlownUpX][CheckBlownUpY][1]<=0 ||
							       (TheGame->WorldMap[NineX][NineY][0]>16&& 
								    TheGame->WorldMap[CheckBlownUpX][CheckBlownUpY][0]<=16)    ));

					int csWay= (i==0)*Way[!i]-(i==1)*Way[i];

					int CheckSideWextX=NineX-Way[1]*(twxy),
						CheckSideWextY=NineY-Way[0]*(twxy==0);


					int CheckSideOK= CheckSquare(CheckSideWextX, CheckSideWextY) &&
									 TheGame->WorldMap[CheckSideWextX][CheckSideWextY][0]>0;


					Valid[i]= BlownUpOrBaseOK || CheckSideOK;
					CollidePos[Forward][i] = Nine( twxy)*32 + 16 - Way[i]*(16 - WallAdjust + WallAdjust*DoWallExtension( twxy,  Way[ i],  wext));
					CollidePos[Lower  ][i] = Nine(!twxy)*32 +  0 +              WallAdjust - WallAdjust*DoWallExtension(!twxy,  1,  wext);
					CollidePos[Upper  ][i] = Nine(!twxy)*32 + 32 -              WallAdjust + WallAdjust*DoWallExtension(!twxy,  -1,  wext);	
					}
				CalculateCollisionDistances();
				/*
				Beacon(9 87654);
				Beacon(10000+Mover->GetPos(xy));
				Beacon(60000+NineX);
				Beacon(60000+NineY);
				Beacon(20000+xy);
				Beacon(20000+Way[0]);
				Beacon(40000+MoverPos[Forward][0]);
				Beacon(40000+MoverPos[Lower][0]);
				Beacon(40000+MoverPos[Upper][0]);
				Beacon(30000+CollidePos[Forward][0]);
				Beacon(30000+CollidePos[Lower][0]);
				Beacon(30000+CollidePos[Upper][0]);
				Beacon(20000+CollisionDist[0]);
				Beacon(20000+CollisionDist[1]);

				Beacon(987654);
*/


				twoWayLoop
					if(Valid[i]&&Collide()){
						AddEvent(new WallEvent(CollisionMP,NineX,NineY,Mover, !i));				
						DP2(" WallCollision added ", CollisionMP);
					}
			}
		}
}


void MovementObject::DetectOutOfMapCollisions(){
	
	TwoWayLoop{
		int wMP= (Way[i]==1)*(TheGame->WorldSize*11*32-1) - Way[i]*MoverPos[Forward][i];
		if(wMP>=0 && wMP <= GetMP()){
			AddEvent(new WorldEvent(Mover, wMP,!i));
			DP2(" WorldEvent added ", wMP);
		}
	}
}

void MovementObject::DetectNewCenterSquareDistance(){
	int ncsMP=(Way[0]==1)*32-Way[0]*(Mover->GetPos(xy)%32);
	if(ncsMP>=1&&ncsMP<=GetMP()){
		AddEvent(new CenterSquareEvent(Mover->IsMortal(), ncsMP));
		DP2(" CenterSquare added ", ncsMP);
	}

}