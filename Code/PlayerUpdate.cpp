#include "Actor.h"
#include "Weapon.h"
#include "Animation.h"
#include "Player.h"
#include "Game.h"
#include "Mortal.h"
#include "NewStore.h"
#include "SpecialClusterBomb.h"
#include "AirPlane.h"
#include "GeneralConstants.h"
#include "TimeConstants.h"
#include "BodyAndHairColor.h"
#include "Debugger.h"
#include <stdio.h>

void Player::ReportKill(Mortal * Dead){


	if(Team!=Dead->GetTeam())
		Kills[Dead->GetMType()]++;

	GiveReward(Dead->GetPrice(Team));

}
void Player::SetName(const char * iName){ // C++20: const char*
	strcpy(Name,iName);
}
void Player::SetTeam(int iTeam){
	Team=iTeam;

}
void Player::SetHairColor(int iHC){
	HairColor=iHC;
}
void Player::DestroyItems(int Xs,int Ys){
	for(int i=0;i<3;i++){
		if(DetPack[i][2]==DP_ACTIVE&&DetPack[i][0]==Xs&&DetPack[i][1]==Ys){

			// DetPackAnim isn't nullified in Specials.cpp!!!

			DetPack[i][2]=DP_DESTROYED;
			DetPackAnims[i]->KILLME=TRUE;
			DetPackAnims[i]=NULL;
		}
		if(	CoverItem[0][i] == Xs && CoverItem[1][i] == Ys){
			CoverItem[0][i]=-1;
			CoverItem[1][i]=-1;
		}
	
	}
}
void Player::SetOpponent(Player * iOp){
	Opponent=iOp;
}
void Player::SetID(int iID){
	ID=iID;
}
void Player::SetBodyColor(int iBC){

	BodyColor=iBC;
}
void Player::SwitchHairColor(){
	HairColor++;
	HairColor%=HairColors;

}
void Player::SwitchBodyColor(){
	BodyColor++;
	BodyColor%=BodyColors;
}

void Player::SetGun(int Gun){


	if(TheGame->Weapons[ActiveGun]==Tazer&&Gun!=ActiveGun){
		Beacon(1810);
		StopTazerSound();
	}
	ActiveGun=Gun;


}

void Player::UpdateStates(){
	BreaksWalls=SupermanStop&&(SupermanStop>TheGame->thisTickCount);

	UpdatePowers();
}
void Player::BecomeSuperman(){
	SupermanStop=TheGame->thisTickCount+PowerTime(SMTime);
	GoThroughWalls=TRUE;
}
void Player::StartDoubling(){DoubleStop=TheGame->thisTickCount+PowerTime(X2Time);}
void Player::DoDoubleDamage(){DoubleDamageStop=TheGame->thisTickCount+PowerTime(DDTime);}
void Player::FireRapidly(){RapidFireStop=TheGame->thisTickCount+PowerTime(RFTime);}

const double SUPERSPEED=8.5;

void Player::UpdatePowers(){
	if(IsFast()&&!SpL()&&!SpR())
		if(TheGame->thisTickCount>SpeedStop&&!KAMIKAZE){
			if(Speed==SUPERSPEED)
				SpeedStop=0;
			else
				GoSlow();
		}
	if(IsShielded()&&!ShL())
		if(TheGame->thisTickCount>ShieldStop)
			BecomeUnshielded();
	if(IsInvisible())
		if(TheGame->thisTickCount>InvisibilityStop)
			TurnVisible();


}
void Player::MightAndMagic(){
	BecomeShielded();
	TurnInvisible();
	GoFast();
	BecomeSuperman();
	FireRapidly();
	DoDoubleDamage();
	StartDoubling();
}
void Player::GoSuperFast(){Speed=SUPERSPEED;}
void Player::BecomeShielded(){
	SHIELD=TRUE;
	ShieldStop=TheGame->thisTickCount+PowerTime();
}
void Player::BecomeUnshielded(){
	SHIELD=FALSE;
	ShieldStop=0;
}
void Player::TurnInvisible(){
	INVISIBLE=TRUE;
	InvisibilityStop=TheGame->thisTickCount+PowerTime();
}
void Player::TurnVisible(){
	INVISIBLE=FALSE;
	InvisibilityStop=0;
}
void Player::GoFast(){
	Speed=4;
	SpeedStop=TheGame->thisTickCount+PowerTime();
}
void Player::GoSlow(){
	Speed=2;
	SpeedStop=0;
}
void Player::SetOpBase(Base * base){
	OpBase=base;
}
void Player::GiveReward(int Reward){
	Cash+=Reward;
}
void Player::GetDonation(int iDonation, Player * Giver){
	Donation[(GetRoundsPlayed()+1)%2]+=iDonation;
	strcpy(Donor[(GetRoundsPlayed()+1)%2],Giver->GetName());
}
void Player::ReceiveDonation(){

	int d=Donation[GetRoundsPlayed()%2];

	if(d){
		GiveReward(d);
		Donation[GetRoundsPlayed()%2]=0;
		char Msg[100];
		sprintf(Msg, "%s gave you $%i!",Donor[GetRoundsPlayed()%2],d);
		TheNewStore->Message(Msg,mcGreen);
	}
}
void Player::GoGhost(){
	ACTIVATEGHOST=FALSE;
	FIRE=FALSE;
	ALTFIRE=FALSE;
}
void Player::GoNormal(){
	GoThroughWalls=FALSE;
}
void Player::SubBullets(BulletType bType){
 
	if(bType==GR_SHELL)
		Type=B_SHELL;

	Bullets[(int)bType]--;

	ReportStat(ST_BULLETS);

}
void Player::StillMineSafe(){
	MINESAFE = MINESAFE && GetXs()==MinePos[0] && GetYs()==MinePos[1];
}
void Player::GoMineSafe(){
	MINESAFE=TRUE;
	MinePos[0]=GetXs();
	MinePos[1]=GetYs();
}
void Player::GiveBullets(BulletType bType, int Amount){


	Bullets[(int)bType]+=Amount;

	if(Bullets[bType]>bbLimit[bType]){
		Bullets[bType]=bbLimit[bType];
	}

}
void Player::SetKey(int Key, int Value){
	theKeys[Key]=Value;
}
void Player::ReportStat(int Stat){
	Statistics[Stat]++;
	
}

void Player::GivePoints(int pts, int DOSTATS){
	GamePoints+=pts;
	if(DOSTATS){
		MatchStats[rsPlayed]++;
		if(pts>=2){
			MatchStats[rsWon]++;		
		}else if(pts==1){
			MatchStats[rsDraw]++;		
		}else{
			MatchStats[rsLost]++;		
		}
	}
}
void Player::SetTimeOut(){
	TIMEOUT=TRUE;
}
void Player::InactivateCharge(){
	ChargePos[2]=FALSE;
}
void Player::UpdateCamera(){

	if(CameraLock>=TheGame->thisTickCount)
		return;

	for(int xy=0;xy<2;xy++){

		Camera[xy]=Pos[xy]-160;
		if(Camera[xy]<0)
			Camera[xy]=0;
		else if(Camera[xy]>(TheGame->WorldSize-1)*11*32)
			Camera[xy]=(TheGame->WorldSize-1)*11*32;
	}
	
}
void Player::LockCamera(int ms){
	CameraLock=TheGame->thisTickCount+ms;
}
void Player::Stop(){
	StopStop=TheGame->thisTickCount+Opponent->PowerTime(STTime);
	NextStopSpin=TheGame->thisTickCount;
	ResetInput();
}
void Player::SetStartPos(int EXISTSINQUADRANTS){

	DP("nana");
	if(EXISTSINQUADRANTS){
		DP("SetPos");
		SetPos(GetStartX(), GetStartY());
	}else{
		DP("AddMeTo...");
		Pos[0]=GetStartX();
		Pos[1]=GetStartY();
		AddMeToCurrentQuadrant();
	}

	if(!Team){
		Dir=4;
	}
	else{
		Dir=0;
	}

}
void Player::GiveATShot(){
	Bullets[B_ATSHOT]=1;
	OWNEDWEAPONS[25]=TRUE;
}
void Player::Detonate(){
	TheGame->RegExp(Pos[0],Pos[1],500,RET_CMEDIUM,DT_SPECIAL,this);	
}
void Player::GoDark(){
	DarkStop=TheGame->thisTickCount+10000;
}


void Player::LooseStates(BOOL BETWEENROUNDS){

	StopStop=0;
	DoubleStop=0;
	DarkStop=0;
	RapidFireStop=0;
	DoubleDamageStop=0;
	SupermanStop=0;
	ShootInvisibleStop=0;
	SuperBulletStop=0;
	GrenadeStop=0;
	CameraLock=0;
	PainReflection=0;
	BulletReflection=0;
	UpdateStates();

	if(!SpR())
		GoSlow();
	TurnVisible();
	BecomeUnshielded();


	GoThroughWalls=FALSE;
	ACTIVATEGHOST=FALSE;
	BASEKEY=FALSE;
	FIRE=FALSE;
	ALTFIRE=FALSE;
	TIMEOUT=FALSE;
	KAMIKAZE=FALSE;


	if(BETWEENROUNDS){

		BLITLIFE=FALSE;
		ChargePos[2]=0;
		for(int i=0;i<3;i++){
			DetPack[i][2]=DP_NONE;
			DetPackAnims[i]=NULL;
			CoverItem[0][i]=-1;
			CoverItem[1][i]=-1;
		}
		InactivateMissile();
		Cluster->InActivate();
		SAFE_DELETE(ThePlane);
	}
}

