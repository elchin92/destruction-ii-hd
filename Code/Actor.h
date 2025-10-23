#include "Definitions.h"

#ifndef __Actor__ 
#define __Actor__

#define SPos(xy) (((Pos[xy]+ActualCenterPos)/32)*32)


#include "SDL2_DirectDrawCompat.h"
#include "SoundSource.h"

class Game;
class Player;
class Shooter;
class Clone;
class shot;
class Mortal;

class Actor : public virtual SoundSource
{
protected:

int Health;
int Team;
int BlitDimension[2];
int BlitStart[2];
int GoThroughWalls, GoThroughActors, BreaksWalls;
int Dir, DirType;
double MPRest;
int Thickness;  // 1 - 16 (16=32*32)

int Tick, TickType, TickFrames, TickFrame, PingPong, TICKHORIZONTAL;
int INVISIBLE,SHIELD;

//int BREAKWALL[3][3];
double Speed;

LPDIRECTDRAWSURFACE7 * g_pDDSBlitSource;

public:

	Actor();
	virtual ~Actor();
	virtual shot * IsShot();
	virtual Player * IsPlayer();
	virtual Mortal * IsMortal();
	virtual Shooter * IsShooter();
	virtual Clone * IsClone();
	int Get4Dir();
	int Get8Dir();
	int GetDirType();
	int GetRealDir();
	int GetTicks();
	int OutOfMap();
	int HasLeftScreen();
	int HitWall();
	int GetTeam();
	int IsInvisible();
	int BlitXAdjust();
	int BlitYAdjust();
	int GetThickness();
	int GoesThroughActors();
	int GoesThroughWalls();
	virtual BYTE GetBlitColor();
	int * GetBlitDimension();
	double GetMPRest();
	void BlitPlayer();
	void SetBlitRect(int Mult=1);
	void Blit(int where);
	void Initialize();
	void SetBlit(int BlitX, int BlitY, int BlitWidth, int BlitHeight, int dirType, int ticktype, int tickframes, LPDIRECTDRAWSURFACE7 * BlitSource);

	// ActorMovement.cpp

	int CouldCollideWith(Actor * Iterator);
	void MoveXY(int XY, int MP);
	void TickUp(int MP);
	void CalculateMaxMPDir4();
	void CalculateMaxMPDir8();
	void TakeAction(Actor * Collider);
	void FixQuadrantShit();
	virtual void AddMeToCurrentQuadrant();
	virtual void RemoveMeFromLastQuadrant();
	virtual int MoveThroughWall(int XS, int YS);

	Actor * CalculateActorMP();

};

#endif

