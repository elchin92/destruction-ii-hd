#ifndef __RocketTank__ 
#define __RocketTank__
#include "Shooter.h"

class Game;

class RocketTank : public Shooter
{
private:


public:

	RocketTank * nextRocketTank;

	RocketTank(int X, int Y, int Type, int Who, int theDir);
	~RocketTank();
	

};
#endif


