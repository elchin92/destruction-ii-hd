#include "Actor.h"
#include "Player.h"
#include "Game.h"
#include "SpecialClusterBomb.h"
#include "SpecialClone.h"
#include "GraphicsEffects.h"
#include "Missile.h"
#include "Weapon.h"
#include "PlayerBlit.h"
#include "staticchar.h"
#include "Settings.h"
#include "GeneralConstants.h"
#include "BodyAndHairColor.h"
#include "Debugger.h"
#include "AirPlane.h"
#include <stdio.h>


Player * ThePlayers[2];

void Player::SetVariables(){
	Cluster = new SpecialClusterBomb(this);
	sClone = new SpecialClone(this);
	// CRITICAL: Initialize pointers to prevent crash on uninitialized memory
	ThePlane = nullptr;
	TheMissile = nullptr;
}

Player::Player(int num, int ADDMORTAL){


	SaveableClass::Initialize("Player");
	RegisterItem("Name",&Name,sizeof(Name));
	RegisterItem("Cash",&Cash,sizeof(Cash));
	RegisterItem("Special",&Special[0][0],sizeof(Special));
	RegisterItem("Bullets",&Bullets[0],sizeof(Bullets));
	RegisterItem("Status",&Status[0],sizeof(Status));
	RegisterItem("Armor",&Armor[0],sizeof(Armor));
	RegisterItem("MoneySpent",&MoneySpent[0],sizeof(MoneySpent));
	RegisterItem("Statistics",&Statistics[0],sizeof(Statistics));
	RegisterItem("Items",&Items[0],sizeof(Items));
	RegisterItem("Deaths",&Deaths[0],sizeof(Deaths));
	RegisterItem("Kills",&Kills[0],sizeof(Kills));
	RegisterItem("Score",&Score[0],sizeof(Score));
	RegisterItem("BodyColor",&BodyColor,sizeof(BodyColor));
	RegisterItem("HairColor",&HairColor,sizeof(HairColor));
	RegisterItem("GamePoints",&GamePoints,sizeof(GamePoints));
	RegisterItem("ActiveGun",&ActiveGun,sizeof(ActiveGun));
	RegisterItem("Donation",&Donation[0],sizeof(Donation));
	RegisterItem("CashBefore",&CashBefore,sizeof(CashBefore));
	RegisterItem("ChargesB4",&ChargesB4,sizeof(ChargesB4));
	RegisterItem("LivesB4",&LivesB4,sizeof(LivesB4));
	RegisterItem("ID",&ID,sizeof(ID));
	RegisterItem("RoundStats",&RoundStats[0],sizeof(RoundStats));
	RegisterItem("MatchStats",&MatchStats[0],sizeof(MatchStats));
	RegisterItem("Donor",&Donor[0],sizeof(Donor));
	RegisterItem("OWNEDWEAPONS",&OWNEDWEAPONS[0],sizeof(OWNEDWEAPONS));
	RegisterItem("DUAL",&DUAL,sizeof(DUAL));



	Shooter::Initialize(ADDMORTAL, FALSE);
	PipeDistance=4;

	Team=num;
	sprintf(Name,"Player %i",num+1);	

	HairColor=0;
	BodyColor=0;

	Beacon(447);
	Thickness=PLAYERTHICKNESS;
	SEEKER=FALSE;
	ExplodeType=EXP_NONE;

	SetVariables();

	Beacon(448);
	PLSETBLIT;
	Beacon(445);
}

Player::~Player(){
		DP("Player destructor called");
	SAFE_DELETE(Cluster);
	SAFE_DELETE(sClone);
	SAFE_DELETE(ThePlane);
	SAFE_DELETE(TheMissile);

}


__int64 Player::FrStart(){
	return FreezeStart;
}
int Player::Frozen(){
	return isFrozen;
}

void Player::Freeze(__int64 FrStart){
	FreezeStart=FrStart;
	isFrozen=TRUE;
}
void Player::UnFreeze(){
	isFrozen=FALSE;
}

void Player::GoToMenu(int RESET){
	if(RESET){
		Status[SPR]=FALSE;
		Status[SPL]=FALSE;
		Status[SHL]=FALSE;
	}
	Lives=0;
}

void Player::Reset(){

	// Does not reset the team nor the keys
	// You cannot reset before setting variables

	MISSILEACTIVE=0;
	InactivateMissile();
	UP=0;DOWN=0;LEFT=0;RIGHT=0;FIRE=0;ALTFIRE=0;LOCKEDSHOT[0]=LOCKEDSHOT[1]=0;
	LeftRightShot=1;
	Lives=0;Cash=0;Frame=0;
	MaxHealth=BaseHealth;ActiveGun=0;
	LastDeath=0;
	LooseStates();
	CashBefore=0;ChargesB4=0;LivesB4=0;
	DUAL=FALSE;moneymult=1;
	GoThroughWalls=FALSE; ACTIVATEGHOST=FALSE;
	BASEKEY=0;TIMEOUT=FALSE;
	MINESAFE=FALSE;
	ChargeStart=0;
	GamePoints=0;

	UZ2();
	Zero(Kills,NumberOfMortals);
	Zero(RoundStats,NumberOfRoundsStats);
	Zero(MatchStats,NumberOfRoundsStats);
	Zero(Donation,2);
	Zero2d(Donor,MNC+1,2);
	Zero(Status,MISCITEMS);
	Zero(Armor,NumberOfArmorTypes);
	Zero(Items,30);
	Zero(OWNEDWEAPONS,30);
	Zero2d(Deaths,NumberOfDamageTypes,3);
	Zero(Bullets,PlayerBulletTypes);
	Zero(Statistics,NumberOfPlayerStats);
	Zero(MoneySpent,5);
	Zero2d(Special,3,2);
	Zero(MinePos,2);
	Zero(Score,5);

	Beacon(452);
	// Starting Equipment
	if(AL_SE){
		// Basic
		ActiveGun=0;
		Bullets[B_9]=200;

		Bullets[B_MINE]=3;
		Bullets[GR_GRENADE]=2;
		Bullets[B_CHARGE]=10;
		OWNEDWEAPONS[0]=TRUE;
	}else{
		// Advanced
		ActiveGun=2;
			Beacon(453);
		Bullets[B_9]=200;

		Bullets[B_MINE]=3;
		Bullets[B_TMINE]=1;
		Bullets[GR_GRENADE]=5;
		Bullets[B_CHARGE]=20;
		OWNEDWEAPONS[2]=TRUE;	
		Bullets[B_ATSHOT]=1;
			Beacon(454);
		OWNEDWEAPONS[25]=TRUE; // Atshot
		OWNEDWEAPONS[23]=TRUE; // tazer
		Bullets[B_NRJ]=50;
		Special[0][0]=9;
		Special[0][1]=1;
		Special[1][0]=2;
		Special[1][1]=1;
		Special[2][0]=4;
		Special[2][1]=1;
			Beacon(455);

	}
#ifdef BETATESTING



	if(FALSE){
		Armor[AT_WEST]=3;
		Armor[AT_HELMET]=3;
		Special[0][0]=5;
		Special[0][1]=20;
		Special[1][0]=12;
		Special[1][1]=300;

		Bullets[B_556]=200;
			Bullets[B_762]=202;
				Bullets[B_12]=201;
					Bullets[B_SH]=240;
						Bullets[B_SHELL]=20;
Bullets[B_APRKT]=20;
Bullets[B_ATRKT]=20;
	Bullets[B_ATSHOT]=100;
		OWNEDWEAPONS[0]=1+Team;
		OWNEDWEAPONS[1]=1;
		OWNEDWEAPONS[2]=1;
		OWNEDWEAPONS[4]=TRUE;
		OWNEDWEAPONS[3]=TRUE; 
		OWNEDWEAPONS[10]=TRUE; // tazer
		OWNEDWEAPONS[19]=TRUE;
		OWNEDWEAPONS[22]=TRUE;
		OWNEDWEAPONS[23]=TRUE;
		OWNEDWEAPONS[25]=TRUE;
		OWNEDWEAPONS[26]=TRUE;
		Bullets[B_NRJ]=20000;

		Status[3]=TRUE; // Speed Life
		//Status[5]=TRUE;
		Status[7]=TRUE;
		Status[8]=TRUE;
		Status[9]=TRUE;
		Status[10]=TRUE;
		Status[14]=TRUE;
		Status[15]=TRUE;
		Status[16]=TRUE;
		Special[0][0]=9;
		Special[0][1]=10;

	}
#endif
	UnFreeze();
	CalculateMaxHealth();
	FullHealth();

}
void Player::LoadCorpseSprites(){
	// Inv: g_pDDSSprites already loaded
	TheGame->BlankSurfaceArea(224,32*Team,32,32,&TheGame->g_pDDSSprites);

	TheGame->rcRect.left = 192;
	TheGame->rcRect.top = 64;
	TheGame->rcRect.right = TheGame->rcRect.left+32;
	TheGame->rcRect.bottom = TheGame->rcRect.top+32;

	TheGame->BlitOntoSetRc(224, 32*Team, &TheGame->g_pDDSSprites, &TheGame->g_pDDSSprites);

	TheGame->rcRect.left = 224;
	TheGame->rcRect.top = 32*Team;
	TheGame->rcRect.right = TheGame->rcRect.left+32;
	TheGame->rcRect.bottom = TheGame->rcRect.top+32;

	TheGame->ScaleSurface(&TheGame->g_pDDSSprites,105,BodyScales[GetBodyColor()][0],BodyScales[GetBodyColor()][1],6);

}

void Player::LoadCharacterSprites(){

	TheGame->LoadBitmapToSurface(&TheGame->g_pDDSTemporary, PlayerBitmap);


	TheGame->rcRect.left = 0;
	TheGame->rcRect.top = 0;
	TheGame->rcRect.right = PlayerBlitWidth;
	TheGame->rcRect.bottom = PlayerBlitHeight;
	
	// Set Hair colors
	TheGame->ScaleSurface(&TheGame->g_pDDSTemporary,76,HairScales[GetHairColor()][0],HairScales[GetHairColor()][1],4);
	// Set Body and Aim colors
	TheGame->ScaleSurface(&TheGame->g_pDDSTemporary,105,BodyScales[GetBodyColor()][0],BodyScales[GetBodyColor()][1],6);


	TheGame->BlankSurfaceArea(PlayerBlitStartX,PlayerBlitStartY,PlayerBlitWidth,PlayerBlitHeight,&TheGame->g_pDDSPlayer);

	// Copy Character
	TheGame->rcRect.top=0;
	TheGame->rcRect.left=0;
	TheGame->rcRect.bottom=PlayerBlitHeight;
	TheGame->rcRect.right=PlayerBlitWidth;

	TheGame->BlitOntoSetRc(PlayerBlitStartX, PlayerBlitStartY, &TheGame->g_pDDSPlayer, &TheGame->g_pDDSTemporary);

	TheGame->g_pDDSTemporary->Release();
	TheGame->g_pDDSTemporary=NULL;



}

void Player::LoadWeaponSprites(){

	if(GameStatus!=PLAY)
		return;
	TheGame->BlankSurfaceArea(WeaponBlitX,WeaponBlitY,32*2,32,&TheGame->g_pDDSPlayer);

	TheGame->LoadBitmapToSurface(&TheGame->g_pDDSTemporary,WeaponSprBitmap);

	TheGame->rcRect.top=32*(ActiveGun/4);
	TheGame->rcRect.left=32*2*(ActiveGun%4);
	TheGame->rcRect.bottom=TheGame->rcRect.top+32;
	TheGame->rcRect.right=TheGame->rcRect.left+32*2;

	TheGame->BlitOntoSetRc(WeaponBlitX,WeaponBlitY, &TheGame->g_pDDSPlayer ,&TheGame->g_pDDSTemporary);

	TheGame->g_pDDSTemporary->Release();
	TheGame->g_pDDSTemporary=NULL;


}
Player * Player::IsPlayer(){return this;}


void Player::PostLoad(int PerformLoad){
	if(PerformLoad){
		CalculateMaxHealth();
	}

}