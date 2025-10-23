#include "Actor.h"
#include "Player.h"
#include "Definitions.h"
#include "Constants.h"
#include "Functions.h"
#include "Windows.h"
#include "Game.h"
#include "Debugger.h"
#include "GeneralConstants.h"
#include "PriceConstants.h"
#include "DamageConstants.h"
#include "AirPlane.h"
#include "AmmoAndArmorName.h"

void Player::PlayerDamage(int Dam, DamageType dType, ArmorType aType, Mortal * Damager){

	// Damager can be NULL!!!
	if(!Frozen()&&IsAlive()){
		DP("Player is being damaged. PlayerDamage()");
		DP2("Initial Damage", Dam);
		if(PainReflect()&&Damager==Opponent&&dType!=DT_SPECIAL){
			DP("PainReflect");
			Opponent->PlayerDamage(Dam, DT_SPECIAL, aType, this);
			return;
		}
		if(ThePlane && ThePlane->OwnerSafe() && dType==DT_SPECIAL && Dam == AirDamage/2 && Damager==this){
			// Don't get hit by AirStrike
			DP("Don't get hit by AirStrike");
			return;
		}
		int rDam=400*Dam;
		if(IsShielded()){
			DP("Shielded");
			rDam/=2;
			if(SSB()){
				DP("Static /=4");
				rDam/=2;
			}
		}

		int Percentage=GetArmorPercentage(AT_PANTS)+GetArmorPercentage(aType);
		rDam=((100-Percentage)*rDam)/100;
		int AppliedDamage=(rDam+dFrame)/400;
		dFrame= (rDam+dFrame)%400;
		
		if(AppliedDamage)
			GotHit();

		Health-=AppliedDamage;

		DP2("Applied damage", AppliedDamage);
		DP2("Percentage saved",Percentage);
		DP2("Health after", Health);
		DP2("MaxHealth", MaxHealth);
		DP2("rDam",rDam);
		DP2("dFrame",dFrame);

		if(Health<=0){
			if((aType==AT_WEST||aType==AT_AURA)&&dType!=DT_SPECIALUNIT)
				CreateAndPlaySound(sDeath,dFar,spNormal);

			Kill(Damager, dType);
		}
	}
	
}
void Player::Revive(){
	if(!Frozen()){
		Dir=4-4*Team;
		LastDeath=TheGame->thisTickCount;

		Freeze(TheGame->thisTickCount);

		MINESAFE=FALSE;

		//TheGame->CreateStaticAnimation(Pos[0],Pos[1],(Team%3)*32+224,(Team/3)*32);

		// Will create a dummy dead body
		new Mortal(Pos[0],Pos[1],Team);

	}
}

void Player::DoRevive(){

	Lives--;
	UnFreeze();
	FullHealth();

	LooseStates(FALSE);

	if(!TheGame->MoneyMaze()){
		Status[SHL]=FALSE;
		Status[SPL]=FALSE;
	}

	if(!SpR()){
		Speed=2;
	}else{
		Speed=4;
	}

	MoveMeToTheLivingMortals();
	nextDeadMortal=NULL;

	Beacon(77735);
	SetStartPos(FALSE);

}

void Player::PlayerKill(Mortal * Killer, DamageType dType){

	// Killer can be NULL!!!
	
	Health=0;
	Beacon(12443);
	StopTazerSound();
	if(Killer==this && dType==DT_SHOT)
		dType==DT_SPECIAL;

	int kId=2;
	if(Killer!=NULL){
		kId=Killer->GetTeam();
		if(kId!=2)
			kId=(kId!=Team);
	}


	Deaths[(int)dType][kId]++;

	Beacon(12442);
	if(dType==DT_RUNOVER||dType==DT_BASEDOOR)
		CreateAndPlaySound(sRanOver,dNormal,spNormal);


	if(dType==DT_SPECIALUNIT)
		CreateAndPlaySound(sDeath,dFar,spNormal);

		Beacon(12446);
	if(dType==DT_CHARGE && Killer == this){
		Cash-=ChPenalty;
		if(Cash<0)
			Cash=0;
	}
	if(KAMIKAZE){
		Detonate();
	}
	Beacon(124466);
	Revive();
}
void Player::Heal(int ItemType){
	if(ItemType==IT_MEDI)
		Health+=MaxHealth/2;
	else if(ItemType==IT_SMEDI)
		Health+=MaxHealth/4;

	if(Health>MaxHealth)
		Health=MaxHealth;
}


void Player::FullHealth(){
	Health=MaxHealth;
	dFrame=0;
}


int Player::GetMaxHealth(){
	return MaxHealth;
}

int Player::GetHealth(){
	return Health;
}

void Player::SetMaxHealth(int Max){
	MaxHealth=Max;
}
int Player::HaveArmor(ArmorType aType,int Strength){
	return Armor[(int)aType]==Strength;
}
int Player::GetArmorStrength(ArmorType aType){
	return Armor[(int)aType];
}
int Player::GetArmorPercentage(ArmorType aType){
	if(GetArmorStrength(aType))
		return ArmorInfo[(int)aType][GetArmorStrength(aType)-1][1];
	return 0;
}
int Player::GetAP(){
	int AP=0;
	for(int i=0;i<ActualArmorTypes ;i++){
		if(Armor[i])
			AP+=ArmorInfo[i][Armor[i]-1][0];
	}

	return AP;
}
void Player::SetArmor(ArmorType aType,int Strength){
	Armor[(int)aType]=Strength;
	CalculateMaxHealth();
}
void Player::CalculateMaxHealth(){
	MaxHealth=BaseHealth+GetAP();


}