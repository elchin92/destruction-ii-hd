#include <Math.h>
#include "Clone.h"
#include "Player.h"
#include "SpecialClusterBomb.h"
#include "Game.h"

#define 	ClusterBooms 300
#define		ClusterDamage 200

SpecialClusterBomb::SpecialClusterBomb(Player * theActivator){
	Activator=theActivator;
}
int SpecialClusterBomb::Activate(){


		// Cluster Bomb
	if(!ClusterStart){
		ClusterStart=TheGame->thisTickCount;
		ClusterPos[0]=(Activator->GetXs());
		ClusterPos[1]=(Activator->GetYs());
		Booms=ClusterBooms;
		Ms=0;
	
		return TRUE;	
	}

	return FALSE;
	
}

int SpecialClusterBomb::IsActive(){
	return ClusterStart!=0;
}

void SpecialClusterBomb::Fire(long TheFrame){
	if(IsActive()){
		// cLUSTER eXPLOSION
		__int64 thisTickCount=TheGame->thisTickCount;

		if(ClusterStart+1000<thisTickCount){

			long Factor = (rand() % (Booms+10));

			int SilkBooms = ( Factor * TheFrame + Ms) / (TheGame->Dlay*80);
			Ms		  = ( Factor * TheFrame + Ms) % (TheGame->Dlay*80);
			
			if(SilkBooms){
				const int MaxDist=7,SafeDist=2;
				BOOL Boom[MaxDist*2+1][MaxDist*2+1];
				for(int X=ClusterPos[0]-MaxDist,XX=0;X<=ClusterPos[0]+MaxDist;X++,XX++)
					for(int Y=ClusterPos[1]-MaxDist,YY=0;Y<=ClusterPos[1]+MaxDist;Y++,YY++){
						Boom[XX][YY]=FALSE;
						if(X>=0&&Y>=0&&X<=TheGame->WorldSize*11-1&&Y<=TheGame->WorldSize*11-1){
							int Xdist,Ydist;
							double Dist;
							Xdist=ClusterPos[0]-X;
							Ydist=ClusterPos[1]-Y;
							Xdist*=Xdist;
							Ydist*=Ydist;
							Dist=sqrt(Xdist+Ydist);
							if(Dist<=(double)MaxDist+0.5&&Dist>=SafeDist){
								
								Boom[XX][YY]=TRUE;
							}
						}
					}
					int RandX,RandY;
					if(SilkBooms>Booms)
						SilkBooms=Booms;
					while(SilkBooms){
						
						RandX=(rand()%(MaxDist*2+1));
						RandY=(rand()%(MaxDist*2+1));
						if(Boom[RandX][RandY]){
							if(rand()<Booms){
								RandX-=MaxDist;
								RandY-=MaxDist;
								RandX+=ClusterPos[0];
								RandY+=ClusterPos[1];
								
								TheGame->RegExp(32*RandX,32*RandY, ClusterDamage, RET_CLUSTER, DT_SPECIAL,Activator);
								Booms--;
								SilkBooms--;
							}
						}
					}		
			}
		}
		if(!Booms)
			ClusterStart=0;
	}
}

int * SpecialClusterBomb::GetPos(){
	return &ClusterPos[0];
}

__int64 SpecialClusterBomb::GetTime(){
	return ClusterStart;
}
void SpecialClusterBomb::InActivate(){
	ClusterStart=0;
}