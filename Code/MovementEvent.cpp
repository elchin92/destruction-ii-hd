#include "MovementEvent.h"
#include "Game.h"
#include "Debugger.h"

int medl=1; int  mecr=1;

MovementEvent::~MovementEvent(){
	DP2("Deleting a movementevent",medl++);
	if(nextEvent)
		delete nextEvent;
}

void MovementEvent::Initialize(){
	DP2("Creating a movement event",mecr++);
	nextEvent=0;
	Beacon(7771);


}

void MovementEvent::insertEvent(MovementEvent * newEvent){
	// Inv: MP checked and larger than ours
	if(!nextEvent){
		DP("   MovementEvent inserted last");
		nextEvent=newEvent;	
		return;
	}

	Beacon(777523);
	if(newEvent->GetMP() > nextEvent->GetMP()){
		// Keep pushing
		DP("  MovementEvent going deeper...");
		nextEvent->insertEvent(newEvent);
	}else{
		DP("  MovementEvent inserted after current one");
		// Insert after this one
		newEvent->nextEvent=nextEvent;
		nextEvent=newEvent;
	}


}
int MovementEvent::GetMP(){return MP;}
int MovementEvent::KamikazeEvent()
{
	return 0;
}
MovementEvent * MovementEvent::ActLoop(){
	
	DP2(" MP: ", GetMP());
	if(st!=ST_SAVEMP){

		// NewSquareEvent must act AFTER moving
		Act();
		DP2(" Acted... ", GetMP());
	}

	if(st==ST_CONTINUE&&nextEvent){
		// Inv: nextEvent exists because last event must be outofmp
		DP(" Another one...");
		return nextEvent->ActLoop();	
	}

	return this;
}
StopType MovementEvent::GetStopType(){
	return st;
}