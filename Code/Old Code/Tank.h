#ifndef __Tank__ 
#define __Tank__
#include "Shooter.h"
#include "Game.h"

class Game;

class Tank : public Shooter
{
private:


public:

	Tank * nextTank;

	Tank(int X, int Y, int Type, int Who, int theDir);
	~Tank();

	


};
#endif


