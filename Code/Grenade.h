#include "Actor.h"
#include "definitions.h"

#ifndef __Grenade__
#define __Grenade__

class Grenade : public Actor  
{
private:
	double Angle;
	int Dmg;
	Mortal * Owner;
	double PFrame,ZFrame;
	double PSpeed, ZSpeed;
	double Height;
	int TURNED;
	BulletType bType;
	void MoveZ();
	void MoveP();
	void Explode();
	void ChangeHeight();


public:

	Grenade * nextGrenade;
	Grenade(int X, int Y, int Spd, double angle, int Damage, BulletType bType, Mortal * iOwner);
	~Grenade();

	void Move();
	void KillAll();

	int KILLME;
	int GetZSpeed();
	int GetHeight();

};
extern	Grenade *					TheGrenades;
#endif