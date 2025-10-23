#include "Actor.h"

#ifndef __LaserBeam__ 
#define __LaserBeam__

#define LASER_VISIBLE_TIME 50

class Mortal;
class Game;

class LaserBeam: public Actor
{
private:
	int EndPos[2], BeamColor;
	__int64 StopTime;

public:
	int KILLME;
	LaserBeam(int StartX, int StartY,  int Askew, int Damage, Mortal * Owner, int Color);
	~LaserBeam();
	void Blit(int where);
	void Move();

	void KillAll();
	LaserBeam * nextLaserBeam;

};
extern	LaserBeam *					TheLaserBeams;
#endif

