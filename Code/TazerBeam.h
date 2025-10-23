#include "Actor.h"

#ifndef __TazerBeam__ 
#define __TazerBeam__

#define TAZER_VISIBLE_TIME 50
#define TAZER_DISTANCE 48
class Game;
class Mortal;
class Actor;

class TazerBeam
{
private:
	Actor * Attacker;
	Mortal * Victim;
	int BeamColor, Range;
	__int64 StopTime;
	int WithinRange(Mortal * aMortal);
	int HitSomebody;
public:
	int KILLME;
	TazerBeam(Actor * mAttacker, int Damage, int Color, int Distance);
	TazerBeam(Actor * mAttacker, Mortal * mVictim, int Color);
	~TazerBeam();
	void Initialize();
	void Blit(int where);
	void KillAll();
	int HitAnyone();

	TazerBeam * nextTazerBeam;
};

extern	TazerBeam *					TheTazerBeams;
#endif

