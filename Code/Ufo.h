#ifndef __Ufo__ 
#define __Ufo__
#include <windows.h>
#include "actor.h"
class Game;

class Ufo: public Actor
{
private:

	int TopSpeed; // 100s of Pixles / Frame
	int MaxAcc,MinAcc; // 100s of Pixles / (Frame sq)
	int vWS;
	int BeamDist;
	BOOL BEAM[2];
	BOOL ACTIVE;
	int msPF;
	double cSpeed[2];
	//double dSpeed[2]; // Pixels / Frame
	__int64 Frame;
	double Acc[2]; // Pixels / (Frame sq)
	double xPos[2]; // These are
	double xPoints; // softeners!


public:

	Ufo();
	~Ufo();
	void Move();
	void Kill();
	void ActivateIt();
	int GetX();
	int GetY();
	int MoveThroughWall(int XS, int YS);
	void NewAcc();
	void BeamEm(double Points);
	void BlitBeam(int where);

	BOOL BeamActive(int Player);
	BOOL IsActive();

};

extern	Ufo	*						TheUfo;

#endif

