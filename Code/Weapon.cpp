#include "Weapon.h"
#include "AimRules.h"
#include "Game.h"


	Weapon * Glock;
	Weapon * UZI;
	Weapon * HKMP5;
	Weapon * Colt;
	Weapon * Aksu;
	Weapon * M16;
	Weapon * M16203;
	Weapon * Enfield;
	Weapon * Sporter;
	Weapon * Minimi;
	Weapon * RKM60;
	Weapon * AK47;
	Weapon * M14;
	Weapon * M60;
	Weapon * BarretGun;
	Weapon * Barret;
	Weapon * Browning;
	Weapon * BenelliM3;
	Weapon * BenelliM1014;
	Weapon * KS23K;
	Weapon * Saiga;
	Weapon * LaserRifle;
	Weapon * AssaultLaser;
	Weapon * Tazer;
	Weapon * Mortar;
	Weapon * ATShot;
	Weapon * Bazooka;


Weapon::Weapon(BulletType Type, int Dmg, int Spd, int Acc, int Clip, double Rate,
		   int Delay, int Cost, wType iType, int iLengthUnrotated, int iLengthRotated,
		   const char * iName,  // C++20: string literals are const char*
		   BulletType AltType, int AltDmg, int AltSpd,
		   int AltAcc,double AltRate){

	AmmoType=Type;
	Damage=Dmg;
	Speed=Spd;
	ClipSize=Clip;
	FireRate=Rate;
	ClipDelay=Delay;
	Price=Cost;
	Accuracy=Acc;

	AltAmmo=AltType;
	AltDamage=AltDmg;
	AltSpeed=AltSpd;
	AltFireRate=AltRate;
	AltAccuracy=AltAcc;

	wpnType=iType;

	ALTERNATIVEATTACK=AltDmg!=0&&AltFireRate!=0;

	Name=iName;

	LengthAdj[0]=iLengthUnrotated;
	LengthAdj[1]=iLengthRotated;

	Dual=(wpnType==wGun||wpnType==wSMG);

	SniperBonus[0]=wpnType==wGun||wpnType==wRRf||wpnType==wSRf;
	SniperBonus[1]=SniperBonus[0]||wpnType==wARf||wpnType==wSRf||wpnType==wSMG;
}
Weapon::~Weapon(){}

BOOL Weapon::ALTERNATIVE(){
	return ALTERNATIVEATTACK;
}

BulletType Weapon::GetAT(int Alt){
	if(Alt)
		return AltAmmo;
	return AmmoType;
}
int Weapon::GetDm(int Alt){
	if(Alt)
		return AltDamage;
	return Damage;
}
int Weapon::GetSp(int Alt){
	if(Alt)
		return AltSpeed;
	return Speed;
}

double Weapon::GetFr(int Alt){
	if(Alt)
		return AltFireRate;
	return FireRate;
}
int Weapon::GetAc(int Alt){
	if(Alt)
		return AltAccuracy;
	return Accuracy;
}


int Weapon::GetCD(){
	return ClipDelay;	
}

int Weapon::GetCS(){
	return ClipSize;
}

int Weapon::GetPrice(){
	return Price;
}
const char * Weapon::GetName(){  // C++20: returns const char*
	return Name;
}
AimType Weapon::GetAimType(){
	return wType2AimType[(int)wpnType];
}
wType Weapon::GetWeaponType(){return wpnType;}
int Weapon::IsDualable(){return Dual;}
int Weapon::HasSniperBonus(int Primary){return SniperBonus[Primary];}
int Weapon::ShoulderFire(){return wpnType==wShd;}
int Weapon::SingleHanded(){return GetAimType()==aGun;}
int Weapon::GetBazookaLengthAdjust(int Rotated){return (ShoulderFire())*ShoulderLengthAdjust[Rotated];}
int Weapon::GetLengthAdjust(int Rotated){
	return LengthAdj[Rotated]+AimLengthAdjust[GetAimType()][Rotated]+
				AimLengthDifference[GetAimType()][Rotated]+
				GetBazookaLengthAdjust(Rotated);
}
int Weapon::GetSideAdjust(int Rotated){
	return AimSideAdjust[GetAimType()][Rotated]+
				AimSideDifference[GetAimType()][Rotated]+
				GetBazookaSideAdjust(Rotated);
}
int Weapon::GetBazookaSideAdjust(int Rotated){return (ShoulderFire())*ShoulderSideAdjust[Rotated];}


int Weapon::GetBulletsLeftInClip(int pl, int a){
	return BulletsLeftInClip[pl][a];
}
int Weapon::NeedsToReload(int pl, int a){
	return GetBulletsLeftInClip(pl, a)==0;
}
void Weapon::SubBullets(int pl, int a){
	BulletsLeftInClip[pl][a]--;
}
void Weapon::Reload(int pl, int a, int FiringDual){
	if(a&&AltAmmo!=AmmoType)
		BulletsLeftInClip[pl][a]=1;
	else
		BulletsLeftInClip[pl][a]=GetCS();

	if(FiringDual)
		BulletsLeftInClip[pl][a]*=2;
}