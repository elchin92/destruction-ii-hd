#include "MovementEvent.h"

#ifndef __WallEvent__ 
#define __WallEvent__

#define WallAdjust 3

class Game;
class Actor;
class WallEvent : public MovementEvent
{
private:

	int XS, YS;
	Actor * Collider;
public:

	WallEvent(int iMP,  int XS, int YS, Actor * iCollider, int Secondary=0);
	~WallEvent();

	int KamikazeEvent();
	void Act();

};
#endif