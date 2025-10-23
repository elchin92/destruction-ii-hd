#include "MovementEvent.h"

#ifndef __CenterSquareEvent__ 
#define __CenterSquareEvent__

class Mortal;

class CenterSquareEvent : public MovementEvent
{
private:
	Mortal * Mover;
public:

	CenterSquareEvent(Mortal * iMover,int iMP);
	~CenterSquareEvent();

	void Act();

};
#endif