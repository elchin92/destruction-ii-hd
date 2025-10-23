#include "Player.h"
#include "Clone.h"
#include "SpecialClone.h"
#include "Game.h"
#include "Spawner.h"

SpecialClone::SpecialClone(Player * theActivator){
	Activator=theActivator;
}
int SpecialClone::Activate(){

		
	int X=Activator->GetXs(),Y=Activator->GetYs();
	int WorldSize = Activator->GetWorldSize();

	int Dir = Activator->Get8Dir();

	int Margin=1;

	if(Dir==7||Dir==0||Dir==1)
		Y-=Margin;
	if(Dir==1||Dir==2||Dir==3)
		X+=Margin;
	if(Dir==3||Dir==4||Dir==5)
		Y+=Margin;
	if(Dir==5||Dir==6||Dir==7)
		X-=Margin;	
			
	
	if(X>=0&&Y>=0&&X<WorldSize*11&&Y<WorldSize*11&&TheGame->WorldMap[X][Y][1]<=0 ){
		if(TheSpawnPit->CreateMortalOK(T_CLONE,X*32,Y*32)){
			// Clone Copy
			new Clone(X*32,Y*32,Activator->Get4Dir(),Activator->GetTeam());
	
			return TRUE;
		}
	}

	return FALSE;	

}

