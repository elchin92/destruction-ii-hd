#include "Player.h"
#include "SpecialTank.h"
#include "Game.h"
#include "Spawner.h"

// Not updated !!!!!!!!!!!!!!!!!!! Not In use!!!!!!!!!!!

SpecialTank::SpecialTank(Player * theActivator){
	Activator=theActivator;
}
int SpecialTank::Activate(){
	return ActivateIt(0);
}
int SpecialTank::ActivateIt(int Type){

	int X=((*Activator).GetX()+16)/32,Y=((*Activator).GetY()+16)/32;
	int WorldSize = (*Activator).GetWorldSize();

	int Dir = Activator->Get4Dir();

	if(Dir==7||Dir==0||Dir==1)
		Y--;
	if(Dir==1||Dir==2||Dir==3)
		X++;
	if(Dir==3||Dir==4||Dir==5)
		Y++;
	if(Dir==5||Dir==6||Dir==7)
		X--;	
			
	
	if(X>=0&&Y>=0&&X<WorldSize*11&&Y<WorldSize*11&&TheGame->WorldMap[X][Y][1]<=0 &&
		TheSpawnPit->CreateMortalOK((ThicknessType)(T_TANK+Type),X,Y) ){


			TheSpawnPit->CreateMortal(X*32,Y*32,Type,Activator->GetTeam());


			return TRUE;
	}
	

	return FALSE;	

}

