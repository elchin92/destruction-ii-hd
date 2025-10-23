#include "Actor.h"
#include "Player.h"
#include "Game.h"
#include "Weapon.h"
#include "shot.h"
#include "Functions.h"
#include "NewStore.h"
#include "InputEngine.h"
#include "DamageConstants.h"
#include "Debugger.h"

#define MOVEPLAYEROK (TheGame->MoveMortal(this, Get4Dir())==WAY_CLEAR && ( !(Get8Dir()%2)) || TheGame->MoveMortal(this, (Get4Dir()+1)%4)==WAY_CLEAR)
void Player::Fire(){
	
	if(Frozen()){
		return;
	}

	Beacon(23423455);
	Weapon * ActiveWeapon=GetWeapon();

	if(FIRE || ALTFIRE || LOCKEDSHOT[0] || LOCKEDSHOT[1]){

		int Shots=GetShots(ActiveWeapon->GetBulletsLeftInClip(Team,ALTFIRE));

		if(!Shots)
			return;

		SetShotToShotTime();

		// if AltFire and Fire then Fire
		ALTFIRE=ALTFIRE&&!FIRE || LOCKEDSHOT[1]&&!LOCKEDSHOT[0];
		LOCKEDSHOT[0]=LOCKEDSHOT[1]=FALSE;

		int dFire=DualFire();
		
		BulletType bType=ActiveWeapon->GetAT(ALTFIRE);

		while(Shots&&Bullets[(int)bType]){

			LeftRightShot=-LeftRightShot;


			int SpdMult=ActiveWeapon->GetSp(ALTFIRE);
			
			SubBullets(bType);
			
			double AccMult=1;
			if(SNP()&&ActiveWeapon->HasSniperBonus(ALTFIRE))
				AccMult=2;

			if( dFire )
				AccMult/=2;

			int bSpeed=ActiveWeapon->GetSp(ALTFIRE);
			int bDamage=(1+DoubleDamage())*ActiveWeapon->GetDm(ALTFIRE);

			if(ActiveWeapon==Tazer){
				PlayTazerSound();
			}else if(ActiveWeapon==Bazooka){
				if(BBoost()){
					bDamage*=3;
					bDamage/=2;
					bSpeed*=3;
					bSpeed/=2;
				}
			}


			TheGame->AddShot(LeftRightShot*dFire,bSpeed,(*ActiveWeapon).GetAc(ALTFIRE)*AccMult,bType,bDamage, 0 , this);

			if(ActiveWeapon==ATShot&&Bullets[B_ATSHOT]==0){
				SwitchWeapon();
				OWNEDWEAPONS[25]=FALSE;
			}
			
			CurrentShotExtraTimeFrame+=ShotToShotTime;

			ActiveWeapon->SubBullets(Team,ALTFIRE);
			Shots--;

			if(ActiveWeapon->NeedsToReload(Team,ALTFIRE)){
				StopTazerSound();
				ActiveWeapon->Reload(Team,ALTFIRE,dFire);
				LastShotTime+=ActiveWeapon->GetCD();
				Shots=GetShots(ActiveWeapon->GetBulletsLeftInClip(Team,ALTFIRE));
				DP2("Reloaded shots", Shots);
			}


		}
				
	}
	Beacon(23423456);
}

void Player::TakeInputAction(){

	if(InputAction[4]){
		//LOCKEDSHOT[0]=TRUE;
		StartShooting(-(int)(ShotToShotTime+1));
		InputAction[4]=FALSE;
	}
	if(InputAction[5] && GetWeapon()->ALTERNATIVE()){
		//LOCKEDSHOT[1]=TRUE;
		StartShooting(-(int)(ShotToShotTime+1));
		InputAction[5]=FALSE;
	}
	if(InputAction[6]){
		if(!ChargePos[2]&&Bullets[B_CHARGE]){
			TheGame->NewAnimation(4,2000,AN_GROUND,GetXs()*32,GetYs()*32,0,128);
			CreateAndPlaySound(B_CHARGE);
			Bullets[B_CHARGE]--;
			ChargeStart=TheGame->thisTickCount;
			ChargePos[0]=GetXs();
			ChargePos[1]=GetYs();
			ChargePos[2]=TRUE;
		}	
		InputAction[6]=FALSE;
	}
	if(InputAction[7]){
		if(TheGame->thisTickCount>GrenadeStop && Bullets[GR_GRENADE]>0){
			TheGame->AddShot(0,9,1,GR_GRENADE,GrenadeDamage, 0 , this);
			GrenadeStop=TheGame->thisTickCount+1000;
			BlitArmStop=TheGame->thisTickCount+1000;
			Bullets[GR_GRENADE]--;
		}	
		InputAction[7]=FALSE;
	}
	if(InputAction[8]){
	
		if(OKToCreateItemAtDesignatedSpot()&&TheGame->WorldMap[GetXs()][GetYs()][2]==0)
			if(Bullets[B_MINE]){
				TheGame->WorldMap[GetXs()][GetYs()][2]=IT_MINE1+1+Team;
				CreateAndPlaySound(B_MINE);
				Bullets[B_MINE]--;
				Beacon(24234);
				GoMineSafe();
				Beacon(24235);
				if( GetXs() == Opponent->GetXs() &&
					GetYs() == Opponent->GetYs() ){
					Beacon(2423534);
					Opponent->GoMineSafe();
					Beacon(747);
				}
			}	
		InputAction[8]=FALSE;
	}
	if(InputAction[9]){
		if(TheGame->WorldMap[GetXs()][GetYs()][2]==0)
			if(Bullets[B_TMINE]){
				if(Team==0)
					TheGame->WorldMap[GetXs()][GetYs()][2]=IT_TMINE1+1;
				else
					TheGame->WorldMap[GetXs()][GetYs()][2]=IT_TMINE2+1;
				
				Bullets[B_TMINE]--;
				CreateAndPlaySound(B_TMINE);			
			}
	
		InputAction[9]=FALSE;
	}
	if(InputAction[10]){
		SwitchWeapon();	
		StartShooting(1000);	
		InputAction[10]=FALSE;
	}
	if(InputAction[11]){
		ActivateSpecial(0);
		InputAction[11]=FALSE;
	}
	if(InputAction[12]){
		ActivateSpecial(1);
		InputAction[12]=FALSE;
	}
	if(InputAction[13]){
		ActivateSpecial(2);
		InputAction[13]=FALSE;
	}

}


void Player::ResetInput(){
	UP=DOWN=LEFT=RIGHT=FALSE;	
	FIRE=FALSE;
	ALTFIRE=FALSE;
	StopTazerSound();
	for(int i=0;i<INGAMEKEYS;i++){
		InputAction[i]=0;
	}


}
void Player::UpdateInput(){

	if(!IsAlive()||KAMIKAZE)
		return;

	if(!Stopped()){
		UP=Down(theKeys[0]);
		DOWN=Down(theKeys[1]);
		LEFT=Down(theKeys[2]);
		RIGHT=Down(theKeys[3]);
	}

	FIRE=Down(theKeys[4])&&!Ghost();
	ALTFIRE=GetWeapon()->ALTERNATIVE()&&Down(theKeys[5])&&!Ghost();

	if(TrueUp(theKeys[4])){
		StopTazerSound();
	}
	
	if(!Frozen()&&!Ghost()){
		for(int i=4; i<INGAMEKEYS; i++){
			InputAction[i]=InputAction[i]||TrueDown(theKeys[i]);
		}
	}
	
}

void Player::SwitchWeapon(){

	
	int countr=GetGun()+1;
	if(countr>=StoreInfo[1][0])
		countr=0;

	while(GotGun(countr)==FALSE){
		countr++;
		if(countr>=StoreInfo[1][0])
			countr=0;
	}
	SetGun(countr);
	LoadWeaponSprites();

}

void Player::SetShotToShotTime(){
	double Fr=GetWeapon()->GetFr(ALTFIRE);
	if(FiringFast())
		Fr*=2;
	if(DualFire())
		Fr*=2;

	ShotToShotTime=1000/Fr;
}
/*
int Player::xyStep(int x, int y){
	return !(x&&!TryToStep(0,x)||y&&!TryToStep(1,y));
		
}

int Player::TryToStep(int xy, int way){
	int Thick = GetThickness()+1;
	int X1=( Pos[0]+16+(xy==0)*way*(Thick+1) );
	int Y1=( Pos[1]+16+(xy==1)*way*(Thick+1) );
	int X2=X1,Y2=Y1;
	if(xy){
		X2+=Thick;
		X1-=Thick;
	}else{
		Y2+=Thick;
		Y1-=Thick;					
	}


	if(X1<0  || Y1<0 || X1 >= TheGame->WorldSize*352 || Y1>= TheGame->WorldSize*352 )
		return 0;
	X1/=32;X2/=32;Y1/=32;Y2/=32;

	if(TheGame->WorldMap[X1][Y1][1] <= 0 &&
	   TheGame->WorldMap[X2][Y2][1] <= 0 ||
		( Ghost() || GhostActivated() ) || 
		(Superman() && !KAMIKAZE && TheGame->WorldMap[X1][Y1][0] <= 16 && TheGame->WorldMap[X2][Y2][0] <= 16)){

		if(TheGame->MoveMortal(this, XYW2DIR4(xy, way)) == WAY_CLEAR)
			Step(xy, way);

		if(! (Pos[xy]%8) )
			TickUp();
		if(Superman()&& !KAMIKAZE&&TheGame->WorldMap[X1][Y1][0]<=16 && TheGame->WorldMap[X2][Y2][0] <= 16 &&
		   (TheGame->WorldMap[X1][Y1][1]>0 || TheGame->WorldMap[X2][Y2][1]>0) ){
			TheGame->WorldMap[X1][Y1][1]=0;
			TheGame->WorldMap[X2][Y2][1]=0;
			WallSound();
		}
		return 1;
	}
	return 0;
	
}
*/

int Player::GetFireKey(){
	return theKeys[4];
}


