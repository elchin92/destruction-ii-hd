#ifndef __Shooter__
#define __Shooter__

#include "Mortal.h"
#include "TazeringActor.h"
#define SPINUPTIME 800
#define SPINDOWNTIME 2000
#define SPINSTEPS 10

#pragma warning ( disable : 4250 )


enum BulletType;
class Shooter : public Mortal, public TazeringActor
{
protected:

	BulletType bType, AbType;
	int ShotSpd, ShotAcc, ShotDmg;
	int AShotSpd, AShotAcc, AShotDmg;
	int Prob,lldir, Spin;
	double ShotToShotTime;
	double LastShotFrame;
	double CurrentShotExtraTimeFrame;
	__int64 LastTazer, nextSpin, LastShotTime;
	int LeftRightShot, PipeDistance;
	int PipeLength[2];

	// Fire related

	int GetShots(int MaxShots=40000000);

public:
	
	Shooter * nextShooter;
	Shooter();
	Shooter(int X, int Y, int Type, int Who);
	virtual ~Shooter();
	void Fire();
	void Initialize(int ADDMORTAL=1, int ADDSHOOTER=1);
	double CalculateAngle(int Speed);
	int CalculateDistance(double Angle, int Speed);


	virtual int GetPipeLength(int iLeftRightShot);
	virtual int GetPipeSideAdjust();
	virtual int GetPipeDistance(int Rotated);
	int GetLeftRightShot();
	void StartShooting(int ExtraDelay=0);
	double GetCurrentShotExtraTimeFrame();

	Shooter * IsShooter();

	
};

extern	Shooter *					TheShooters;
#endif

