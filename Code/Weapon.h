#ifndef __Weapon__ 
#define __Weapon__
#include <windows.h>
#include "definitions.h"
class Game;


enum AimType{aNoAim, aRifle, aGun, aDual, aThrow,AimTypes};


enum wType						{wGun,wSMG,wRRf,wSRf,wARf,wMGn,wRSg,wASg,wMor,wShd,wTzr,wNum};
const AimType wType2AimType[(int)wNum]=	{	aGun,   aGun,   aRifle,   aRifle,   aRifle,   aRifle,   aRifle,   aRifle,   aRifle,   aRifle,   aGun};


class Weapon
{

private:
	BulletType AmmoType, AltAmmo;
	int Damage,Speed, Accuracy, ClipSize,ClipDelay,Price;
	int LengthAdj[2], SideAdj[2];
	BOOL ALTERNATIVEATTACK;
	int AltDamage,AltSpeed, AltAccuracy;
	double FireRate, AltFireRate;
	int Dual,SniperBonus[2];
	const char * Name;  // C++20: stores string literals (const char*)
	wType wpnType;


	// Player clip related

	int BulletsLeftInClip[2][2];

public:

	Weapon(BulletType Type, int Dmg, int Spd, int Acc, int Clip, double Rate,
		   int Delay, int Cost, wType iType, int iLengthUnrotated, int iLengthRotated,
		   const char * iName,  // C++20: string literals are const char*
		   BulletType AltType=B_NOBULLET, int AltDmg=0, int AltSpd=0,
		   int AltAcc=0,double AltRate=0);

	~Weapon();

	BOOL ALTERNATIVE();

	const char * GetName();  // C++20: returns const char* (string literal)

	BulletType GetAT(int Alt);
	int GetDm(int Alt);
	int GetSp(int Alt);
	int GetCS();
	int GetCD();
	int GetAc(int Alt);
	int GetPrice();
	AimType GetAimType();
	wType GetWeaponType();
	int IsDualable();
	int HasSniperBonus(int Primary);
	int ShoulderFire();
	int SingleHanded();
	int GetLengthAdjust(int Rotated);
	int GetSideAdjust(int Rotated);
	int GetBazookaSideAdjust(int Rotated);
	int GetBazookaLengthAdjust(int Rotated);

	double GetFr(int Alt);

	// Player clip related
	int GetBulletsLeftInClip(int pl, int a);
	int NeedsToReload(int pl, int a);
	void SubBullets(int pl, int a);
	void Reload(int pl, int a, int FiringDual);
};


	//-----------------------------------------------------------------------------
	// The Weapons
	//-----------------------------------------------------------------------------

extern	Weapon * Glock;
extern	Weapon * UZI;
extern	Weapon * HKMP5;
extern	Weapon * Colt;
extern	Weapon * Aksu;
extern	Weapon * M16;
extern	Weapon * M16203;
extern	Weapon * Enfield;
extern	Weapon * Sporter;
extern	Weapon * Minimi;
extern	Weapon * RKM60;
extern	Weapon * AK47;
extern	Weapon * M14;
extern	Weapon * M60;
extern	Weapon * BarretGun;
extern	Weapon * Barret;
extern	Weapon * Browning;
extern	Weapon * BenelliM3;
extern	Weapon * BenelliM1014;
extern	Weapon * KS23K;
extern	Weapon * Saiga;
extern	Weapon * LaserRifle;
extern	Weapon * AssaultLaser;
extern	Weapon * Tazer;
extern	Weapon * Mortar;
extern	Weapon * ATShot;
extern	Weapon * Bazooka;

#endif

