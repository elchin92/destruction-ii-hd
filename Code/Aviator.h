#ifndef __Aviator__
#define __Aviator__

#include "Actor.h"

enum AviatorType{aGhost, aUfo, NumberOfAviators};

class Aviator : public TazeringActor
{
protected:

	int plDist[2];
	double cSpeed[2];
	double Acc[2];
	double xPos[2];
	__int64 LastTazer[2];

	AviatorType aType;

	void NewAcc();
	void CalculatePlayerDist();
	void BeamPlayers();
	void KillPlayers();
	void CalculateNewDir();
	int SetSeekAcc(); // returns setacc or not

public:
	int KILLME;
	Aviator * nextAviator;

	Aviator(int x, int y, AviatorType iaType);
	~Aviator();


	// AviatorMovement.cpp

	void Move();


	

};
extern	Aviator *					TheAviators;

#endif
