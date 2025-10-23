#include "Animation.h"
#include "Player.h"
#include "AirPlane.h"
#include "SpecialClusterBomb.h"
#include "SpecialClone.h"
#include "shot.h"
#include "Base.h"
#include "Game.h"
#include "Functions.h"
#include "Missile.h"
#include "Spawner.h"

void Player::ActivateSpecial(int which){
	
	if(!Special[which][0])
		return;

	// Do that special trick, mister
	if(Special[which][0]==1 && !Ghost()){
		if(sClone->Activate()){
			SubtractSpecial(which);
		}
	}
	else if(Special[which][0]==2 && !Ghost()){
		// Cover Item

		int huba=0;
		for(;huba<3&&!(CoverItem[0][huba]==-1&&CoverItem[1][huba]==-1);huba++);	

		if(OKToCreateItemAtDesignatedSpot()&&huba!=3){

			TheGame->WorldMap[ItemCreate[0]][ItemCreate[1]][2]=TheGame->GenerateItem(GI_NOMINE);
			
			CoverItem[0][huba]=ItemCreate[0];
			CoverItem[1][huba]=ItemCreate[1];
			CreateAndPlaySound(sCharge,dClose,spLow);
			SubtractSpecial(which);
			
		}
	}
	else if(Special[which][0]==3&& !Ghost()){
		// Cluster Bomb
		if(Cluster->Activate()){
			CreateAndPlaySound(sClusterBomb,dNormal,spNormal);
			SubtractSpecial(which);
			TheGame->NewAnimation(8,1000,AN_AIR,Pos[0],Pos[1],0,160);
		}		
	}
	else if(Special[which][0]==4){
		//Base Tele
		// Skapa 2 nya animationer
		if(TheSpawnPit->CreateItemOK(GetStartX(),GetStartY())){
			TheGame->NewAnimation(8,1000,AN_EXP,Pos[0],Pos[1],0,224,TRUE);
			TheGame->NewAnimation(8,1000,AN_EXP,GetStartX(),GetStartY(),0,224);
			SetStartPos();
			CreateAndPlaySound(sTeleport,dNormal,spNormal);
			SubtractSpecial(which);
		}
	
	}else if(Special[which][0]==5){
		// Kamikaze
		if(!Ghost()){
			ResetInput();
			KAMIKAZE=TRUE;
			Speed=8;
			CreateAndPlaySound(sKamikaze,dNormal,spNormal);
			SubtractSpecial(which);
		}
	}
	else if(Special[which][0]==6){
		// Pain Reflection
		if(!PainReflection){
			PainReflection=TheGame->thisTickCount+10000;
			SubtractSpecial(which);
		}
	}
	else if(Special[which][0]==7&&!TIMEOUT){
		// Base Key

		for(int door=0;door<2;door++)
			if(!BASEKEY&&OpBase->WithinVicinity(this, door)){
				BASEKEY=1+door;
				CreateAndPlaySound(sBaseKey,dNormal,spNormal);
				SubtractSpecial(which);
			}

		
	}
	else if(Special[which][0]==8){
		// Bullet Reflection
		if(!BulletReflection){
			BulletReflection=TheGame->thisTickCount+10000;
			SubtractSpecial(which);
		}
	}
	else if(Special[which][0]==9&& !Ghost()){					
		// Air Attack, Air Strike
		if(!ThePlane){
			ThePlane=new AirPlane(this);
			SubtractSpecial(which);
			CreateAndPlaySound(sAirStrike,dFar,spNormal);
		}
	}
	else if(Special[which][0]==10&&!GhostActivated()&&!Ghost()){
	// Ghost
		ACTIVATEGHOST=TRUE;
		GoThroughWalls=TRUE;
		SubtractSpecial(which);
		CreateAndPlaySound(sGhost,dFar,spNormal);			
	}else if(Special[which][0]==11){
		// Invisible Bullets
		ShootInvisibleStop=TheGame->thisTickCount+60000;
		SubtractSpecial(which);
	}else if(Special[which][0]==12){
		// Sentry Gun
		if(OKToCreateItemAtDesignatedSpot()){			
			TheSpawnPit->CreateMortal(ItemCreate[0]*32,ItemCreate[1]*32,T_SENTRYGUN,Team);
			SubtractSpecial(which);
			CreateAndPlaySound(sPlaceSentryGun,dNormal,spLow);
		}
	}else if(Special[which][0]==13){
		// Det Pack
		if(DetPack[which][2]==DP_ACTIVE){


			// Destroys and Nullifies the DetPackAnim
			TheGame->RegExp(DetPack[which][0]*32,DetPack[which][1]*32,220,RET_DETPACK,DT_SPECIAL,this);
			DetPack[which][2]=DP_NONE;
			if(!Special[which][1])
				Special[which][0]=0;

		}else if(DetPack[which][2]==DP_NONE){
			CreateAndPlaySound(sCharge,dClose,spLow);
			DetPack[which][0]=GetXs();
			DetPack[which][1]=GetYs();
			DetPack[which][2]=DP_ACTIVE;
			DetPackAnims[which]=TheGame->CreateStaticAnimation(GetXs()*32,GetYs()*32,160,64);
			Special[which][1]--;
		}else if(DetPack[which][2]==DP_DESTROYED){
			CreateAndPlaySound(sDetPackClick,dClose,spLow);
			DetPack[which][2]=DP_NONE;
			if(!Special[which][1])
				Special[which][0]=0;
		}

	}else if(Special[which][0]==14){
		// Medikit
		FullHealth();
		SubtractSpecial(which);
		CreateAndPlaySound(sHealth,dNormal,spLow);
	}else if(Special[which][0]==15){
		// Super Bullets
		SuperBulletStop = TheGame->thisTickCount+30000;
		SubtractSpecial(which);
			// PSN 'Superbullets'
	}else if(Special[which][0]==16){
		// Missile
		if(!MISSILEACTIVE){
			TheMissile = new Missile(this);
			MISSILEACTIVE=which+1;
		}else if(MISSILEACTIVE-1==which){	
			TheMissile->Detonate();
			InactivateMissile();
		}
	}else if(Special[which][0]==17){
		// Darkness
		CreateAndPlaySound(sDarkness,dFar,spNormal);
		Opponent->GoDark();
		SubtractSpecial(which);
	}
	
	
}

void Player::InactivateMissile(){
	if(MISSILEACTIVE)
		SubtractSpecial(MISSILEACTIVE-1);
	MISSILEACTIVE=0;
	SAFE_DELETE(TheMissile);
}

void Player::SubtractSpecial(int which){
	Special[which][1]--;
	if(!Special[which][1])
		Special[which][0]=0;
	
}
