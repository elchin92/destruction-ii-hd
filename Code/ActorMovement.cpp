
#include "Actor.h"
#include "Game.h"
#include "Functions.h"
#include "MovementObject.h"
#include "Debugger.h"






void Actor::MoveXY(int XY, int MP){
	Pos[XY]+=MP;
}

void Actor::TickUp(int MPMoved){

	if(TickType==TT_ROLLER){
		Tick=(Tick+MPMoved)%TickFrames;
	}else if(TickType==TT_PINGPONG){
		
		for(int Ticks=(TickFrame+MPMoved)/8;Ticks;Ticks--){
			Tick+=PingPong;
			if(Tick==TickFrames-1||Tick==0)
				PingPong=-PingPong;
		}
		TickFrame=(TickFrame+MPMoved)%8;
	}			

}

void Actor::CalculateMaxMPDir8(){

		// Calculate Original MP
	double MP    = ( Speed * ((double)TheGame->TheFrame))/((double)TheGame->Dlay);

//	DP(" TheGame->TheFrame: ",TheGame->TheFrame);
//	DP(" TheGame->Dlay: ",TheGame->Dlay);
//	DP(" MPRest B4: ",MPRest*1000);
//	DP(" MP B4: ",MP);

	if(Dir%2){
		DP("  MP mulitplied by isq2 ");
		MP*=isq2;
	}

	MP+=MPRest;
	MPRest  = MP - (int)MP;

//	DP(" MPRest A: ",MPRest*1000);
//	DP(" MP A: ",MP);


	//int MP    = ( ((int)Speed) * TheGame->TheFrame + (int)MPRest)/(TheGame->Dlay);
	//MPRest    = ( ((int)Speed) * TheGame->TheFrame + (int)MPRest)%(TheGame->Dlay);
	mo->SetMP((int)MP);

}



void Actor::CalculateMaxMPDir4(){

	// Calculate Original MP
	
	double MP    = ( Speed * ((double)TheGame->TheFrame))/((double)TheGame->Dlay);

	MP+=MPRest;
	MPRest  = MP - (int)MP;

	mo->SetMP((int)MP);
	DP2("MaxMP",(int)MP);

}

