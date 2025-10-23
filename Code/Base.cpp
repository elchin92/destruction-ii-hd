#include "Base.h"
#include "Player.h"
#include "Definitions.h"
#include "constants.h"
#include "Game.h"

Base * TheBases[2];

Base::Base(int theCorner){

	Corner=theCorner;
	WorldSize=&(TheGame->WorldSize);
}

Base::~Base(){}

void Base::CreateBase(){

	TIMEOUT=FALSE;

	Rolling[0]=400;
	Rolling[1]=400;

	Door[0]=CLOSED;
	Door[1]=CLOSED;


	if(Corner){
		Coords[0]=(*WorldSize)*11-BaseSize;
		Coords[1]=(*WorldSize)*11-BaseSize;

		DoorCoords[0][0]=Coords[0];
		DoorCoords[0][1]=Coords[1]+BaseSize-2;	

		DoorCoords[1][0]=Coords[0]+BaseSize-2;
		DoorCoords[1][1]=Coords[1];

	}
	else{
		Coords[0]=0;
		Coords[1]=0;

		DoorCoords[0][0]=Coords[0]+BaseSize-1;
		DoorCoords[0][1]=Coords[1]+1;
		
		DoorCoords[1][0]=Coords[0]+1;
		DoorCoords[1][1]=Coords[1]+BaseSize-1;

	}

	// Set the base sprites

	// Corners


	TheGame->WorldMap[Coords[0]][Coords[1]][0]=42;
	TheGame->WorldMap[Coords[0]+BaseSize-1][Coords[1]][0]=43;
	TheGame->WorldMap[Coords[0]][Coords[1]+BaseSize-1][0]=45;
	TheGame->WorldMap[Coords[0]+BaseSize-1][Coords[1]+BaseSize-1][0]=46;

	// Side Walls
	
	for(int j=1;j<BaseSize-1;j++){
		TheGame->WorldMap[Coords[0]][Coords[1]+j][0]=41;
		TheGame->WorldMap[Coords[0]+BaseSize-1][Coords[1]+j][0]=41;

		TheGame->WorldMap[Coords[0]+j][Coords[1]][0]=44;
		TheGame->WorldMap[Coords[0]+j][Coords[1]+BaseSize-1][0]=44;
	}

	// Doors

	TheGame->WorldMap[DoorCoords[1][0]][DoorCoords[1][1]][0]=47;
	TheGame->WorldMap[DoorCoords[0][0]][DoorCoords[0][1]][0]=48;


	// Clear space inside Base

	for(int a=1;a<BaseSize-1;a++){
		for(int b=1;b<BaseSize-1;b++){	
			TheGame->WorldMap[Coords[0]+a][Coords[0]+b][0]=0;
			TheGame->WorldMap[Coords[0]+a][Coords[0]+b][1]=0;
			TheGame->WorldMap[Coords[0]+a][Coords[0]+b][2]=0;
		}
	}

	UpdateBarriers();

}
void Base::Update(){
	for(int i=0;i<2;i++){

		if((Door[i]==OPEN||Door[i]==OPENING)&&!Sense(i)){
			Door[i]=CLOSING;
		}
		if((Door[i]==CLOSED||Door[i]==CLOSING)&&Sense(i)){
			Door[i]=OPENING;
		}
		if(Door[i]==OPENING){
		
			Rolling[i]-=TheGame->TheFrame;
			if(Rolling[i]<=0){
				Rolling[i]=0;
				Door[i]=OPEN;
			}

		}

		if(Door[i]==CLOSING){
		
			Rolling[i]+=TheGame->TheFrame;

			if(Rolling[i]>=400){
				Rolling[i]=400;
				Door[i]=CLOSED;
			}
		}
	}

	UpdateBarriers();
}

int Base::Sense(int door){

	if(TIMEOUT)
		return TRUE;

	if(WithinVicinity(Owner, door) )
		return TRUE;
	if ((*Opponent).BaseKey(door))
			return TRUE;
	for(Mortal * PresentMortal=TheMortals;PresentMortal;PresentMortal=PresentMortal->nextMortal){
		if(PresentMortal->GetXs()==DoorCoords[door][0] && PresentMortal->GetYs()==DoorCoords[door][1] && PresentMortal->GetSize()==0)
			return TRUE;
		
	}		
	return FALSE;
}

int Base::WithinVicinity(Player * Who, int door){

	return (DoorCoords[door][0]<=Who->GetXs()+1 && DoorCoords[door][0]>=Who->GetXs()-1 && 
			DoorCoords[door][1]<=Who->GetYs()+1 && DoorCoords[door][1]>=Who->GetYs()-1);
	
}

void Base::UpdateBarriers(){

	for(int i=0;i<BaseSize;i++){
		TheGame->WorldMap[Coords[0]+i][Coords[1]][1]=10000;
		TheGame->WorldMap[Coords[0]+i][Coords[1]+BaseSize-1][1]=10000;
	}
	for(int j=1;j<BaseSize-1;j++){
		TheGame->WorldMap[Coords[0]][Coords[1]+j][1]=10000;
		TheGame->WorldMap[Coords[0]+BaseSize-1][Coords[1]+j][1]=10000;
	}
	for(int door=0;door<2;door++){
		if( Rolling[door]>275 ){
			TheGame->WorldMap[DoorCoords[door][0]][DoorCoords[door][1]][1]=10000;			
		}
		else
			TheGame->WorldMap[DoorCoords[door][0]][DoorCoords[door][1]][1]=0;
	}
}

void Base::SetTimeOut(){
	TIMEOUT=TRUE;
}

int Base::GetTicks(int door){

	return 4-Rolling[door]/100;
}
void Base::SetOwner(Player * iOwner){
	Owner=iOwner;
}
void Base::SetOpponent(Player * iOpponent){
	Opponent=iOpponent;
}
