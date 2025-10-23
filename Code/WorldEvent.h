#include "MovementEvent.h"

#ifndef __WorldEvent__ 
#define __WorldEvent__

class Actor;
class shot;

class WorldEvent : public MovementEvent
{
private:
	shot * TheShot;
public:

	WorldEvent(Actor* iMover,int iMP, int Secondary=0);
	~WorldEvent();

	int KamikazeEvent();
	void Act();

};
#endif