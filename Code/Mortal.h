#ifndef __Mortal__
#define __Mortal__

#include "Actor.h"

enum StopReason;

class Mortal : public virtual Actor
{
protected:

	double RegularSpeed, LockSpeed;
	int LockedDir;
	int Health, LockedEnemyDistance, MaxHealth;
	int HitByBullet, Type;
	int ExplodeType;
	int Weaknesses, Strengths, Immunities;
	int SEEKER;
	int Explosions;
	int Price, BulletPrice;
	int Frame, Size;
	int MType;
	int StopDistance;
	__int64 ExpStart, HitStop;

	Mortal * TheLockedEnemy;
	StopReason ReasonToStop;
	int Moving;

	void MoveMeToTheDeadMortals();
	void MoveMeToTheLivingMortals();

	int InternalChangeDir(int InsideMovement);

public:

	int BLITDEAD,TRAPPED;
	Mortal();
	Mortal(int X, int Y, int iTeam); // Used only to create dummy dead players!
	Mortal(int X, int Y, int theType, int Who);
	virtual ~Mortal();

	void Kill(Mortal * Killer, DamageType dType);
	void Damage(int Damage, DamageType dType, BulletType bType, Mortal * Damager);
	void SetTeam(int iTeam);
	void BlitDead(int where);
	void Initialize(int ADDMORTAL=1);
	void Blit(int who);
	void GotHit();
	void SetPos(int x, int y);
	void MaybeExplode();
	int TurnAround();
	int GetSize();
	int GetPrice(int kTeam);
	int GetBulletPrice(int kTeam);
	int IsAlive();
	int GetMType();
	int GetType();
	Mortal * IsMortal();
	BYTE GetBlitColor();

	void SetImmunity(int Type);
	void SetStrength(int Type);
	void SetWeakness(int Type);
	int IsImmuneAgainst(DamageType dType);
	int IsStrongAgainst(DamageType dType);
	int IsWeakAgainst(DamageType dType);

	// MortalInteraction.cpp

	int MortalInteraction(Mortal * Collider);
	void Lock();
	void UnLock();
	int ConfirmLock();
	int GetDistance();
	int GetLockDir(Mortal * aMortal);
	int LockAble(Mortal * aMortal);

	// MortalMovement.cpp

	void Move();
	void AddMeToCurrentQuadrant();
	void RemoveMeFromLastQuadrant();
	int ChangeDir(int InsideMovement);
	int NoLongerTrapped();
	int ItemIsAMine(int Item);
	Player *  GetMineDamager(int Item);
	void HitMines();
	int MoveThroughWall(int XS, int YS);
	void CalculateMortalMaxDistanceMP();
	void CalculateMortalMaxMPDir4();
	int KeepMoving();
	int Go(int lDir, int BREAKEMDOWN);


	Mortal * nextMortal;
	Mortal * nextDeadMortal;
	Mortal * nextQuadMortal;

};
extern	Mortal *					TheMortals;
extern	Mortal *					TheDeadMortals;
extern  Mortal ***					TheMortalQuadrants;
#endif
