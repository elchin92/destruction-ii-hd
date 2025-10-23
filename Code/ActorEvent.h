#include "MovementEvent.h"

#ifndef __ActorEvent__ 
#define __ActorEvent__

class Game;
class Actor;
class ActorEvent : public MovementEvent
{
private:

	Actor * Collided;
	Actor * Mover;

public:

	ActorEvent(int iMP, Actor * iMover, Actor * iCollided, int Secondary=0);
	~ActorEvent();

	KamikazeEvent();
	void Act();

};
#endif