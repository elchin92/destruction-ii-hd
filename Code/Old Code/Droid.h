#ifndef __Droid__ 
#define __Droid__
#include "Shooter.h"

class Game;

class Droid : public Shooter
{
private:


public:

	Droid * nextDroid;

	Droid(int X, int Y, int Type, int Who, int theDir);
	~Droid();


};
#endif


