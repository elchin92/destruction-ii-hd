#include "Actor.h"

#ifndef __shot__ 
#define __shot__


class Mortal;
class Shooter;
class Game;
enum BulletType;
enum RegExpType;

class shot: public Actor
{
private:
	int Dmg,Spr, MOVEDASKEW;
	Shooter * Owner;
	Mortal * dhto;
	double Askew;
	double AskewCounter;
	BulletType bType;
	RegExpType rType;
public:

	shot(int StartX, int StartY, double Speed, double Ask,BulletType ibType, int Damage, Shooter * Owner,  RegExpType irType, int DontHitOwner=0);
	~shot();
	void Security();
	BulletType GetBulletType();
	int DidntExplode();
	void KillAll();

	// ShotMovement.cpp

	void Move();
	void MoveAskewOnce();
	void AddMeToCurrentQuadrant();
	void RemoveMeFromLastQuadrant();
	int MoveThroughWall(int XS, int YS);
	int HitMortal(Mortal * Hit);
	shot * IsShot();

	void SetDontHitThisOne(Mortal * idhto);
	Mortal * DontHitThisOne();

	shot * nextshot;
	shot * nextQuadShot;
	int KILLME;
};

extern	shot *						TheShots;
extern shot  *** TheShotQuadrants;
#endif

