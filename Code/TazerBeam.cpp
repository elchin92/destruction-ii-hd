#include "TazerBeam.h"
#include "Game.h"
#include "Mortal.h"
#include "Player.h"
#include "Functions.h"
#include "Quadrants.h"
#include "Debugger.h"

TazerBeam * TheTazerBeams;

void TazerBeam::KillAll(){
	if(nextTazerBeam){
		nextTazerBeam->KillAll();
		delete nextTazerBeam;
		//nextTazerBeam=NULL;
	}

}	


TazerBeam::TazerBeam(Actor * mAttacker, int Damage,  int Color, int iDistance){

	Initialize();

	Attacker=mAttacker;

	Range=iDistance;

	BeamColor=Color;


	int Owner=mAttacker->GetTeam();
	int Inv=Attacker->IsPlayer()&&Attacker->IsPlayer()->ShootsInvisible();

	BOOL FIRED=FALSE;
	
	if(Attacker->IsMortal()){
		MortalQuadLoopDM(Attacker){
			DP("Found Iterator");
			DP2("Is Player",Iterator->IsPlayer()!=NULL);
			DP2("Is Alive",Iterator->IsAlive());
			DP2("Is Within Range",WithinRange(Iterator));
			DP2("Team",Iterator->GetTeam());
			DP2("Owner",Owner);
			if(Iterator->IsAlive()&&WithinRange(Iterator)&&Owner!=Iterator->GetTeam()){
				
				
				Iterator->Damage(Damage,DT_NRJ,B_NRJ,Attacker->IsMortal());
				
				if(FIRED){		
					for(TazerBeam * temp = this;!Inv&&temp->nextTazerBeam;temp=temp->nextTazerBeam){
						TazerBeam * newBeam = new TazerBeam(Attacker,Iterator,Color);
						temp->nextTazerBeam=newBeam;
					}
				}else{
					Victim=Iterator;
					FIRED=TRUE;
				}
				
			}
		}
	}else{
		//Inv: Aviator, firing at player
		for(int pl=0;pl<2;pl++){
			if(ThePlayers[pl]->IsAlive()&&WithinRange(ThePlayers[pl])){

				ThePlayers[pl]->Damage(Damage,DT_SPECIALUNIT,B_NRJ,NULL);

				if(Victim){
						nextTazerBeam=new TazerBeam(Attacker,ThePlayers[pl],Color);	
				}else{
					Victim=ThePlayers[pl];
				}
			}
		}
	}
	HitSomebody=Victim!=NULL;

	if(Victim==NULL||Inv){
		DP("Tazerbeam hit noone!");
		StopTime=TheGame->thisTickCount;
	}
	else
		DP("Tazerbeam hit!");

}

TazerBeam::TazerBeam(Actor * mAttacker, Mortal * mVictim, int Color){

	Initialize();

	Attacker=mAttacker;
	Victim=mVictim;
	BeamColor=Color;

	HitSomebody=TRUE;
}
int tzcr=1, tzdl=1;
TazerBeam::~TazerBeam(){
	DP2("deleting a tazerbeam",tzdl++);
}

void TazerBeam::Initialize(){
	DP2("Creating a tazerbeam",tzcr++);
	Victim=NULL;
	KILLME=FALSE;	
	nextTazerBeam=NULL;
	StopTime=TheGame->thisTickCount+TAZERTIME;    
}

int TazerBeam::HitAnyone(){return HitSomebody;}
void TazerBeam::Blit(int where){
	Beacon(88455771);

	if(nextTazerBeam)
		nextTazerBeam->Blit(where);

	Beacon(88455772);

		Beacon(88455773);
	if(TheGame->thisTickCount>=StopTime){
		Beacon(88455774);
		KILLME=TRUE;
	}else{
		// Inv: Victim!=NULL
			Beacon(88455775);
		int X=Attacker->GetActualXPos(),Y=Attacker->GetActualYPos();
		if(Attacker->IsPlayer())
			LengthAdjust(&X,&Y,Attacker->Get8Dir(),11,TRUE);
		DP2("X",X);
		DP2("Y",Y);
		DP2("Victim->GetActualXPos()",Victim->GetActualXPos());
				DP2("Victim->GetActualYPos()",Victim->GetActualYPos());
		TheGame->BlitBeamC(X,Y,Victim->GetActualXPos(),Victim->GetActualYPos(),BeamColor,where);
	}

}



int TazerBeam::WithinRange(Mortal * aMortal){
	int x=(aMortal->GetX()-Attacker->GetX());
	int y=(aMortal->GetY()-Attacker->GetY());
	if(aMortal->GetMType()==M_PL)
		LengthAdjust(&x,&y,aMortal->Get8Dir(),11,TRUE);

	return Range*Range> x*x+y*y;
}