
#include "SoundEngine.h"

#include "Player.h"

#include "Tank.h"
#include "Clone.h"
#include "SpecialAirStrike.h"
#include "Game.h"
SpecialAirStrike::SpecialAirStrike(Player * theActivator){
	Activator=theActivator;
}
int SpecialAirStrike::Activate(){


	if(!IsActive()){
		int t;
		int Xpos=(*Activator).GetX(),Ypos=(*Activator).GetY();
		int WorldSize=(*Activator).GetWorldSize();

		AirStart=TheGame->thisTickCount;
		AirDropPos[0]=Activator->GetXs();
		AirDropPos[1]=Activator->GetYs();
		AirFreePos[0]=AirDropPos[0];
		AirFreePos[1]=AirDropPos[1];
		for(int two=0;two<2;two++){
			if(AirDropPos[two]<5)
				AirDropPos[two]=5;
			if(AirDropPos[two]>WorldSize*11-6)
				AirDropPos[two]=WorldSize*11-6;
		}
		AirBombs=11;
		
		t=Activator->GetXs()+11;
		AirPos[0]=32*t;
		t=Activator->GetYs()/32;
		if(t>WorldSize*11-6)
			t=WorldSize*11-6;
		if(t<5)
			t=5;
		AirPos[1]=t*32;
		if(Xpos>(WorldSize*11-1)*32-175)
			AirPos[0]+=Xpos-(WorldSize*11-1)*32+175;
		if(Xpos<175){
			AirPos[0]-=175-Xpos;
		}
		AirStartPos=AirPos[0];

		return TRUE;	
	}

	return FALSE;
	
}

int SpecialAirStrike::IsActive(){
	return AirStart!=0;
}

int * SpecialAirStrike::GetPos(){
	return &AirPos[0];
}

void SpecialAirStrike::InActivate(){
	AirStart=0;
}
void SpecialAirStrike::Fire(){



	if(IsActive()){


		const int sp=10;
		int Bombs=0;
		DWORD t;
		__int64 thisTickCount=TheGame->thisTickCount;
		t=(DWORD)(thisTickCount-AirStart);
		t*=sp;
		t/=TheGame->Dlay;
		
		AirPos[0]=AirStartPos-t;
		t=(DWORD)(thisTickCount-AirStart);
		t-=1500;
		t*=sp;
		t/=32;
		t/=TheGame->Dlay;
		if(t>0){
			if(t>11)
				t=11;
			int Bombs=t-11+AirBombs;
			AirBombs=11-t;
			
			int X,Y;
			while(Bombs){
				
				for(int Blow=0;Blow<11;Blow++){
					X=(AirDropPos[0]-6+Bombs+AirBombs);
					Y=(AirDropPos[1]-5+Blow);
					if(!AirBombs&&!Blow)
						TheSoundEngine->PlayLocatedSound(sBigExplosion,X,Y,dFar,spNormal);
					if( !( X == AirFreePos[0] &&
						   Y == AirFreePos[1] )  ){
						TheGame->RegExp(32*X,32*Y, 2*ChDamage, RET_AIR, DT_SPECIAL,Activator);							
					}
					
					
				}
				Bombs--;
			}
			
			
		}	
		
		// Add Boom Boom
		
		
		
		if(AirPos[0]<=0-160&&AirBombs==0){
			AirStart=0;
		}
	}
}