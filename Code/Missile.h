#ifndef __Missile__ 
#define __Missile__

#include "actor.h"

class Player;

class Missile: public Actor
{
private:

	Player * Owner;

public:

	Missile(Player * iOwner);
	~Missile();
	void Move();
	void Kill();
	void Detonate();

};
#endif

