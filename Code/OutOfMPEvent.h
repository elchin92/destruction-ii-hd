#include "MovementEvent.h"

#ifndef __OutOfMPEvent__ 
#define __OutOfMPEvent__

class Game;

class OutOfMPEvent : public MovementEvent
{
private:

public:

	OutOfMPEvent(int iMP);
	~OutOfMPEvent();

	void Act();

};
#endif