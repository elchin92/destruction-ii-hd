#include "MovementEvent.h"
#ifndef __NewSquareEvent__ 
#define __NewSquareEvent__

class Actor;
class Game;

class NewSquareEvent : public MovementEvent
{
private:

	Actor * Mover;

public:

	NewSquareEvent(int iMP, Actor * iMover);
	~NewSquareEvent();

	void Act();

};
#endif