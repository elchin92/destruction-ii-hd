// NewSquareEvent.cpp: implementation of the NewSquareEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "NewSquareEvent.h"
#include "Player.h"
#include "Clone.h"
#include "Game.h"
#include "DamageConstants.h"
#include "Debugger.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NewSquareEvent::NewSquareEvent(int iMP, Actor * iMover)
{
	Initialize();
	st=ST_SAVEMP;
	MP=iMP;
	Mover=iMover;
}

NewSquareEvent::~NewSquareEvent()
{

}

void NewSquareEvent::Act(){

	Mover->FixQuadrantShit();

	if(Mover->IsMortal()){



		if(Mover->IsPlayer()){
			// PLAYER
			Player * apl = Mover->IsPlayer();
			if(!apl->IsKamikaze() && ! apl->Ghost()){


				int X=apl->GetXs(),Y=apl->GetYs();

				// Any Items to collect?
				if(TheGame->WorldMap[X][Y][2] ){

					Beacon(7771229);			
					Player * CoverOwner=NULL;
					for(int pl=0;pl<2;pl++)
						if(ThePlayers[pl]->QueryCoverItem(X,Y))
							CoverOwner=ThePlayers[pl];
				
					if(CoverOwner){
						apl->Kill(CoverOwner, DT_SPECIAL);
						TheGame->WorldMap[X][Y][2]=0;
						TheGame->RegExp(X*32,Y*32, CoverDamage, RET_COVER, DT_SPECIAL,CoverOwner);
						
						apl->CreateAndPlaySound(sLaugh,dFar,spNormal);
						
					}else{
						apl->CollectItem(TheGame->WorldMap[X][Y][2]);
						apl->HitMines();
					}
				}

			}
		}else{
			Mover->IsMortal()->HitMines();
			if(Mover->IsClone()){
				Mover->IsClone()->CollectItem();
			}
		
		}
			

	}

}
