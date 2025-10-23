#ifndef __FireFlame__ 
#define __FireFlame__

class Shooter;
enum Flame;

class FireFlame
{
private:

	Shooter * Owner;

	__int64 FlameStop;
	int Frames;
	int LeftRightShot;
	Flame FlameType;
	

public:
	int KILLME;
	FireFlame * nextFireFlame;

	FireFlame(Shooter * iOwner, int iSide, BulletType ibType, int LaserColor);
	~FireFlame();

	void Blit();
};

extern FireFlame * TheFireFlames;

#endif