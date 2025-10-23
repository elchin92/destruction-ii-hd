#ifndef __Player__
#define __Player__

#include "Shooter.h"
#include "SaveableClass.h"
#include <windows.h>
#include "Definitions.h"
#include "Controls.h"
#include "GeneralConstants.h"
class Game;
class Base;
class Weapon;
class Clone;
class Tank;
class shot;
class SpecialClusterBomb;
class SpecialClone;
class Animation;
class Missile;
class AirPlane;
enum AimType;

enum RoundStats{rsPlayed,rsWon,rsLost,rsDraw,NumberOfRoundsStats};


enum PlayerStatistic{ST_EARNED, ST_BULLETS, NumberOfPlayerStats};

const int ArmorInfo[NumberOfArmorTypes][3][2]=
// HP, %prot
// Str1     Str2     Str3
{50, 15, 100, 30, 150, 50, //  West
 25 , 10, 45 , 20, 70, 35, //  Helmet
 40 , 5 , 75, 10, 120, 15, //  Pants
 25 , 30, 45 , 40, 70, 50, //  Boots
 50, 10, 75, 30, 100, 50, //  Aura
   0,  0,   0,  0,   0,  0};//  None

#define GetStartY GetStartX
#define DROP_ARM_LENGTH 10
class Player : public Shooter, public SaveableClass
{
private:

// Player Info

	int Lives, Cash, Special[3][2], CoverItem[2][3], Bullets[PlayerBulletTypes], Status[MISCITEMS];
	int Armor[NumberOfArmorTypes];
	int Frame;
	int MoneySpent[5],Statistics[NumberOfPlayerStats], Items[30];
	int Deaths[NumberOfDamageTypes][3], Kills[NumberOfMortals], Score[5];
	int theKeys[INGAMEKEYS], BodyColor, HairColor;
	int isFrozen,TheFrame, GamePoints;
	int ActiveGun, ItemCreate[2], Donation[2];
	int CashBefore, ChargesB4, LivesB4, ID;
	int RoundStats[NumberOfRoundsStats];
	int MatchStats[NumberOfRoundsStats];
	char  Donor[2][MNC+1];

	int moneymult, dFrame;
	int ACTIVATEGHOST, BASEKEY, TIMEOUT, BLITLIFE;
	int UP,DOWN,LEFT,RIGHT,FIRE,ALTFIRE, LOCKEDSHOT[2];
	int InputAction[INGAMEKEYS];
	int Camera[2];
	int OWNEDWEAPONS[30],DUAL, DBLCOLLECTED, KAMIKAZE;
	int MinePos[2],MINESAFE,ChargePos[3], MISSILEACTIVE, DetPack[3][3];

	__int64 SpeedStop, ShieldStop, InvisibilityStop,DoubleStop, ShootInvisibleStop,DarkStop;
	__int64 BulletReflection, PainReflection, StopStop, DoubleDamageStop, CameraLock, GrenadeStop, BlitArmStop;
	__int64 FreezeStart, LastDeath,ChargeStart, RapidFireStop, SupermanStop, SuperBulletStop;
	__int64 NextStopSpin;


// Game Copies

	Animation * DetPackAnims[3];
	Player * Opponent;
	Base * OpBase;
	SpecialClusterBomb * Cluster;
	SpecialClone * sClone;

public:
	AirPlane * ThePlane;
	Missile * TheMissile;

	char Name[30];

	// Player.cpp

	Player(int num, int ADDMORTAL=1);
	~Player();
	void Reset();
	void SetVariables();
	void Freeze(__int64 thisTickCount);
	void UnFreeze();
	void GoToMenu(int RESET);
	void LoadCharacterSprites();
	void LoadWeaponSprites();
	void LoadCorpseSprites();
	void PostLoad(int PerformLoad);
	__int64 FrStart();
	int Frozen();



	// Life.cpp

	void FullHealth();
	void Heal(int ItemType);
	void PlayerKill(Mortal * Killer, DamageType dType);
	void PlayerDamage(int Damage, DamageType dType, ArmorType aType, Mortal * Damager);
	void SetArmor(ArmorType aType,int Strength);
	void CalculateMaxHealth();
	void SetMaxHealth(int Max);
	void DoRevive();
	void Revive();
	int GetHealth();
	int GetMaxHealth();
	int HaveArmor(ArmorType aType,int Strength);
	int GetArmorPercentage(ArmorType aType);
	int GetArmorStrength(ArmorType aType);
	int GetAP();

	// PlayerStatus.cpp

	int GotItem(int Item);
	int IsShielded();
	int IsFast();
	int IsShooting();
	int GetAimType();
	int	BlitWhite();
	int UsingSingleHandedWeapon();
/*	__int64 ShStart();
	__int64 SpStart();
	__int64 IvStart();*/
	int Iv60();
	int Sp60();
	int SpL();
	int SpR();
	int Sh60();
	int ShL();
	int SSB();
	int TWN();
	int MWN();
	int RDR();
	int URDR();
	int PRDR();
	int SNP();
	int BBoost();
	int CUpgrade();
	int Duality();
	int XtraTime();
	int ChargeActive();
	int Ghost();
	int GhostActivated();
	int NoLivesLeft();
	int GotDual();
	int QueryCoverItem(int X, int Y);
	int PainReflect();
	int BulletReflect();
	int BaseKey(int door);
	int GotGun(int Gun);
	int Doubling();
	int FiringFast();
	int Stopped();
	int DoubleDamage();
	int	Superman();
	int BlitLife();
	int ShoulderFire();
	int DualFire();
	int ShootsInvisible();
	int ShootingSuper();
	int IsDark();
	int OKToCreateItemAtDesignatedSpot(int BulletSensitive=FALSE);
	int PressingMovementKeys();

	// PlayerUpdate.cpp
	
	void UpdatePowers();
	void ReportKill(Mortal * Dead);
	void SetGun(int Gun);
	void SubBullets(BulletType bType);
	void SetKey(int Key, int Value);
	void ReportStat(int Stat);
	void GiveReward(int Reward);
	void SetID(int iID);
	void GoGhost();
	void GoNormal();
	void SetOpBase(Base * base);
	void SetTimeOut();
	void SetTeam(int iTeam);
	void GivePoints(int pts, int DOSTATS=TRUE);
	void InactivateCharge();
	void GoMineSafe();
	void Stop();
	void SetStartPos(int EXISTSINQUADRANTS=TRUE);
	void StillMineSafe();
	void LooseStates(BOOL BETWEENROUNDS=TRUE);
	void GiveBullets(BulletType bType, int Amount);
	void LockCamera(int ms);
	void UpdateCamera();
	void SetHairColor(int iHC);
	void SetBodyColor(int iBC);
	void SwitchHairColor();
	void SwitchBodyColor();
	void SetOpponent(Player * iOp);
	void SetName(char * iName);
	void BecomeShielded();
	void BecomeUnshielded();
	void TurnInvisible();
	void TurnVisible();
	void GoDark();
	void GoFast();
	void GoSlow();
	void BecomeSuperman();
	void StartDoubling();
	void DoDoubleDamage();
	void FireRapidly();
	void DestroyItems(int Xs,int Ys);
	void Detonate();
	void UpdateStates();
	void GetDonation(int Donation, Player * Giver);
	void ReceiveDonation();
	void GiveATShot();
	void SetShotToShotTime();
	void MightAndMagic();
	void GoSuperFast();

	// PlayerRequest.cpp

	int GetExtraThrowingSpeed();
	int GetSpeed();
	int GetWorldSize();
	int GetKey(int Key);
	int GetStat(int Stat);
	int GetScore(int i);
	int GetSpecial(int i);
	int GetLives();
	int GetCash();
	int GetMines();
	int GetStartX();
	int GetTankMines();
	int GetCharges();
	int GetShells();
	int GetID();
	int GetSpecialAmount(int SpecialType);
	int GetBullets(BulletType bType);
	int GetGun();
	int GetItemCount(int IType);
	int GetMoneySpent(int Store);
	int GetMoneySpentPercentage(int Store);
	int GetDeath(DamageType dType, int iTeam);
	int GetChargeTicks();
	int GetCameraX();
	int GetCameraY();
	int GetHairColor();
	int GetBodyColor();
	int GetFrames();
	int GetSpecialSlotAmount(int slot);
	int PowerTime(int OriginalTime=60000);
	int IsKamikaze();
	int GetOwnedWeapons(int iWeapon);
	int GetPipeLength(int iLeftRightShot);
	int GetPipeSideAdjust();
	int GetPipeDistance(int Rotated);
	int GetMatchesPlayed();
	int GetMatchesWon();
	int GetMatchesLost();
	int GetMatchesDraw();
	int GetRoundsPlayed();
	int GetRoundsWon();
	int GetRoundsLost();
	int GetRoundsDraw();
	int GetPoints();
	int GetKills(MortalType mType);
	int * GetBlitDimension();
	int * GetChargePos();
	char * GetName();
	char * GetAmmoString();
	BYTE GetBlitColor();
	Player * IsPlayer();
	Weapon *  GetWeapon();
	SpecialClusterBomb * GetCluster();
	Player * GetOpponent();
	__int64 GetLastDeath();
	__int64 GetChargeStart();
	__int64 GetStop(int Power);
	AimType GetFireType();

	// Shop.cpp

	void Buy(int Store, int Item);
	int BuyWeapon(int Item);
	int BuyAmmo(int Item);
	int BuyArmor(int Item);
	int BuyMisc(int Item);
	int BuySpecial(int Item);
	void Sell(int Store, int Item);
	int SellWeapon(int Item);
	int SellAmmo(int Item);
	int SellArmor(int Item);
	int SellMisc(int Item);
	int SellSpecial(int Item);
	int GetPrice(int Store, int Item, int Buy);

	// PlayerInput.cpp

	void Fire();
	void SwitchWeapon();
	void UpdateInput();
	void ResetInput();
	int TryToStep(int xy, int way);
	int xyStep(int x, int y);
	void TakeInputAction();
	int GetFireKey();


	// PlayerEvent.cpp

	void EndRound(int Winner);
	void BeginRound();
	void CollectItem(int Item);

	// Specials.cpp

	void ActivateSpecial(int which);
	void InactivateMissile();
	void SubtractSpecial(int which);

	// PlayerMovement.cpp

	void Move();
	void EndMovement();
	int MoveThroughWall(int XS, int YS);
	int CalculatePlayerDir();
};

extern	Player *					ThePlayers[2];
#endif

