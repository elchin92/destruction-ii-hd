#include "Actor.h"
#include "Game.h"
#include "shot.h"
#include "Mortal.h"
#include "Debugger.h"

int Actor::CouldCollideWith(Actor * Iterator){
	return this!=Iterator;
}

void Actor::FixQuadrantShit(){


	// Inv : We have MOVED to another quadrant!!!
	Beacon(777142);
	RemoveMeFromLastQuadrant();
	Beacon(777145);
	AddMeToCurrentQuadrant();
	Beacon(777147);
}



