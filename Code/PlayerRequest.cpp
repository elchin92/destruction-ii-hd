#include "Actor.h"
#include "Player.h"
#include "Weapon.h"
#include "Game.h"
#include <stdio.h>
#include <algorithm> // C++20: for std::min

int Player::GetExtraThrowingSpeed(){return PressingMovementKeys()*GetSpeed();}

int Player::GetSpeed(){
	return 2*(IsFast()+1);
}

int Player::GetKey(int Key){
	return theKeys[Key];
}
int Player::GetStat(int Stat){
	return Statistics[Stat];
}
int Player::GetScore(int i){
	return Score[i];
}
__int64 Player::GetStop(int Power){

	if(Power==IT_SHIELD)
		return ShieldStop;
	if(Power==IT_INVIS)
		return InvisibilityStop;
	if(Power==IT_SPEED)
		return SpeedStop;

	return FALSE;
}
char * Player::GetName(){
	return &Name[0];
}
int Player::GetWorldSize(){
	return TheGame->WorldSize;
}
int Player::GetID(){
	return ID;
}
int Player::GetSpecial(int i){
	return Special[i][0];
}
AimType Player::GetFireType(){
	if(DualFire())
		return aDual;
	return GetWeapon()->GetAimType();
}
int Player::GetItemCount(int IType){
	return Items[IType];
}
int Player::GetMoneySpent(int Store){
	return MoneySpent[Store];
}
int Player::GetMoneySpentPercentage(int Store){
	CalculateZum(MoneySpent,5);
	//int Zum=0;for(int yue=0;yue<5;yue++){Zum+=MoneySpent[yue];Beacon(230+yue);	}
	if(Zum==0)
		return 0;
	int p = (1005*MoneySpent[Store])/(10*Zum);
	return std::min(100,p); // C++20: use std::min explicitly
}
int Player::GetDeath(DamageType dType, int iTeam){
	return Deaths[dType][iTeam];
}
BYTE Player::GetBlitColor(){
	if(HitStop>TheGame->thisTickCount)
		return 15;

	// Do not need to check for hits, Mortal does that
	const int ShieldColor = 113;

	int mult = 1 + SSB();
	return 	(IsShielded() && TheGame->thisTickCount%(500/mult)>=(unsigned)(400/mult)) ? ShieldColor : 0;

}

int Player::GetSpecialAmount(int SpecialType){
	SpecialType++;
	int sum=0;
	for(int i=0;i<3;i++)
		sum+=(Special[i][0]==SpecialType)*Special[i][1];
	return sum;

}
int Player::GetSpecialSlotAmount(int slot){
	return Special[slot][1];
}
int Player::PowerTime(int OriginalTime){
	if(XtraTime())
		return (3*OriginalTime)/2;
	return OriginalTime;
}
int Player::GetGun(){


	return ActiveGun;
}

char * Player::GetAmmoString(){		
	static char str[10];

	if(GetWeapon()==Bazooka)
		sprintf(str,"%i/%i",GetBullets(B_APRKT),GetBullets(B_ATRKT));
	else
		sprintf(str,"%i",GetBullets(GetWeapon()->GetAT(0)));

	return str;
}

Weapon *  Player::GetWeapon(){
	return TheGame->Weapons[ActiveGun];
}
int Player::GetLives(){
	return Lives;
}
int Player::GetCash(){
	return Cash;
}
int Player::IsKamikaze(){
	return KAMIKAZE;
}
int Player::GetMines(){
	return Bullets[B_MINE];
}
int Player::GetTankMines(){
	return Bullets[B_TMINE];
}
int Player::GetCharges(){
	return Bullets[B_CHARGE];
}
int Player::GetShells(){
	return Bullets[B_SHELL];
}
int Player::GetBullets(BulletType bType){
	return Bullets[bType];
}
int Player::GetOwnedWeapons(int iWeapon){
	return OWNEDWEAPONS[iWeapon];
}
SpecialClusterBomb * Player::GetCluster(){
	return Cluster;
}
__int64 Player::GetChargeStart(){
	return ChargeStart;
}
int Player::GetChargeTicks(){
	int cTick=(int)((TheGame->thisTickCount-ChargeStart))/500;
	if(cTick>3)
		cTick=3;
	return cTick;
}


int Player::GetCameraX(){
	return Camera[0];
}


int Player::GetCameraY(){
	return Camera[1];
}
int * Player::GetChargePos(){
	return &ChargePos[0];
}
Player * Player::GetOpponent(){
	return Opponent;
}


__int64 Player::GetLastDeath(){
	return LastDeath;
}
int Player::GetFrames(){
	return TickFrames;
}

int Player::GetHairColor(){
	return HairColor;
}
int Player::GetBodyColor(){
	return BodyColor;
}
int * Player::GetBlitDimension(){
	return &BlitDimension[0];
}
int Player::GetPipeDistance(int Rotated){
	return PipeDistance+!Rotated;
}
int Player::GetMatchesPlayed(){return MatchStats[rsPlayed];}
int Player::GetMatchesWon(){return MatchStats[rsWon];}
int Player::GetMatchesLost(){return MatchStats[rsLost];}
int Player::GetMatchesDraw(){return MatchStats[rsDraw];}
int Player::GetRoundsPlayed(){return RoundStats[rsPlayed];}
int Player::GetRoundsWon(){return RoundStats[rsWon];}
int Player::GetRoundsLost(){return RoundStats[rsLost];}
int Player::GetRoundsDraw(){return RoundStats[rsDraw];}
int Player::GetPoints(){return GamePoints;}

int Player::GetStartX(){

	if(Team)
		return (TheGame->WorldSize*11-2)*32;
	return 32;
}

int Player::GetPipeLength(int iLeftRightShot){

	return GetWeapon()->GetLengthAdjust(Dir%2);
}

int Player::GetPipeSideAdjust(){
	return GetWeapon()->GetSideAdjust(Dir%2);
}

int Player::GetKills(MortalType mType){return Kills[mType];}