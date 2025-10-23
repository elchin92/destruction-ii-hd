#ifndef __Clone__ 
#define __Clone__
#include "Mortal.h"

class Game;

class Clone : public Mortal
{
private:

public:

	Clone * nextClone;

	Clone(int X, int Y, int iDir, int iTeam);
	~Clone();

	void CollectItem();
	Clone * IsClone();

};

extern	Clone *						TheClones;

#endif

