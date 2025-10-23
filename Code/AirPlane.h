#include "Actor.h"

#ifndef __AirPlane__ 
#define __AirPlane__


class Player;

class AirPlane :public Actor
{
private:
	Player * Owner;
	__int64 AirStart;
	int StopDropPos[2], AirFreePos[2], AirBombs;
	
public:
	int KILLME;

	AirPlane(Player * iOwner);
	~AirPlane();
	void Fire();
	void Move();

	int OwnerSafe();

};
#endif

