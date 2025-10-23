#include "Actor.h"
#include "Player.h"
#include "Game.h"
#include "Base.h"
#include "Weapon.h"
#include "Functions.h"
#include "Spawner.h"

int Player::IsShielded(){
	return SHIELD;
}
int Player::IsFast(){
	return Speed!=2;
}
/*
__int64 Player::ShStart(){
	return ShieldStart;
}
__int64 Player::SpStart(){
	return SpeedStart;
}
__int64 Player::IvStart(){
	return InvisibilityStart;
}
*/
int Player::Iv60(){
	return Status[0];
}
int Player::DualFire(){
	return 
			Duality()					&&
			OWNEDWEAPONS[ActiveGun]==2	&&
			TheGame->Weapons[ActiveGun]->IsDualable();
}
	

int Player::Sp60(){
	return Status[1];
}

int Player::SpL(){
	return Status[SPL]&&!TheGame->MoneyMaze();
}
int Player::SpR(){
	return Status[SPR]&&!TheGame->MoneyMaze();
}
int Player::Sh60(){
	return Status[4];
}
int Player::ShL(){
	return Status[SHL]&&!TheGame->MoneyMaze();
}
int Player::SSB(){
	return Status[6];
}
int Player::TWN(){
	return Status[7];
}
int Player::MWN(){
	return Status[8];
}
int Player::PRDR(){
	return Status[9];
}
int Player::URDR(){
	return Status[10];
}
int Player::CUpgrade(){
	return Status[13];
}
int Player::RDR(){
	return Status[9]||Status[10];
}
int Player::SNP(){
	return Status[12];
}
int Player::BBoost(){
	return Status[11];
}
int Player::GotItem(int Item){
	return Status[Item];
}
int Player::NoLivesLeft(){
	return Lives==0;
}
int Player::GotDual(){
	return Status[MISC_DUALITY]!=0;
}
int Player::Ghost(){
	return GoThroughWalls&&!ACTIVATEGHOST;
}
int Player::ChargeActive(){
	return ChargePos[2];
}

int Player::Duality(){
	return Status[MISC_DUALITY];
}
int Player::XtraTime(){
	return Status[15];
}
int Player::GhostActivated(){
	return ACTIVATEGHOST;
}
int Player::QueryCoverItem(int X, int Y){
	for(int huba=0;huba<3;huba++)
		if(CoverItem[0][huba]==X&&CoverItem[1][huba]==Y){
			CoverItem[0][huba]=-1;
			CoverItem[1][huba]=-1;
			return TRUE;
		}
	return FALSE;
}

int Player::PainReflect(){
	return (PainReflection>TheGame->thisTickCount);
}
int Player::BulletReflect(){
	return (BulletReflection>TheGame->thisTickCount);
}
int Player::BaseKey(int door){
	if(BASEKEY==door+1){
		if(OpBase->WithinVicinity(this, door))
			return TRUE;
		BASEKEY=0;
	}
	return FALSE;
}
int Player::GotGun(int Gun){

	return OWNEDWEAPONS[Gun];
}
int Player::Stopped(){
	return StopStop&&(StopStop>TheGame->thisTickCount);
}
int Player::ShootingSuper(){
	return SuperBulletStop&&(SuperBulletStop>TheGame->thisTickCount);
}

int Player::IsDark(){
	return DarkStop&&(DarkStop>TheGame->thisTickCount);
}
int Player::OKToCreateItemAtDesignatedSpot(int BulletSensitive){
	ItemCreate[0]=GetXs(),ItemCreate[1]=GetYs();

	LengthAdjust(&ItemCreate[0], &ItemCreate[1], Dir, 1, 0);

	if( ItemCreate[0]<5&&ItemCreate[1]<5 || 
		ItemCreate[0]>TheGame->WorldSize*11-6 && ItemCreate[1]>TheGame->WorldSize*11-6)
		return 0;

	if(Cross(ItemCreate[0]*32,ItemCreate[1]*32,16, Pos[0], Pos[1], PLAYERTHICKNESS))
		LengthAdjust(&ItemCreate[0], &ItemCreate[1], Dir, 1, 0);

	return (ItemCreate[0]>=0&&ItemCreate[1]>=0&&ItemCreate[0]<TheGame->WorldSize*11&&ItemCreate[1]<TheGame->WorldSize*11 &&
		    TheGame->WorldMap[ItemCreate[0]][ItemCreate[1]][1]<=0&&TheGame->WorldMap[ItemCreate[0]][ItemCreate[1]][2]==0 && 
			TheSpawnPit->CreateItemOK(ItemCreate[0]*32,ItemCreate[1]*32, BulletSensitive));
}

int Player::BlitLife(){
	return BLITLIFE;
}
int Player::Doubling(){
	return DoubleStop&&(DoubleStop>TheGame->thisTickCount);
}
int Player::FiringFast(){
	return RapidFireStop&&(RapidFireStop>TheGame->thisTickCount);
}
int Player::DoubleDamage(){
	return DoubleDamageStop&&(DoubleDamageStop>TheGame->thisTickCount);
}
int Player::Superman(){
	return BreaksWalls;
}
int Player::IsShooting(){
	return FIRE||ALTFIRE;
}
int Player::GetAimType(){
	return IsShooting()*GetFireType();
}

int Player::ShoulderFire(){
	return GetWeapon()->ShoulderFire();
}
int Player::UsingSingleHandedWeapon(){
	return GetWeapon()->SingleHanded();
}
int Player::ShootsInvisible(){
	return ShootInvisibleStop&&ShootInvisibleStop>TheGame->thisTickCount;
}
int Player::PressingMovementKeys(){
	return (UP||DOWN||LEFT||RIGHT);
}
