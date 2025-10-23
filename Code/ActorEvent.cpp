#include "Game.h"
#include "ActorEvent.h"
#include "Mortal.h"
#include "shot.h"
#include "MovementObject.h"

ActorEvent::ActorEvent(int iMP, Actor * iMover, Actor * iCollided, int Secondary){
	// Inv: At least one is a mortal
	Initialize();
	MP=iMP;
	st=(StopType)((int)ST_SLIDEPRIMARY+Secondary);

	Collided=iCollided;
	Mover=iMover;
}

ActorEvent::~ActorEvent()
{

}
int ActorEvent::KamikazeEvent()
{
	return 1;
}
void ActorEvent::Act(){
	// Inv: At least one is a mortal
	if(Collided->IsShot()){
		if(Collided->IsShot()->DontHitThisOne()==Mover){
			st=ST_CONTINUE;
			return;
		}
		if(	((shot *)Collided)->HitMortal(Mover->IsMortal()))
			st=ST_TOTALSTOP;
	}else if(Mover->IsShot()){
		if(Mover->IsShot()->DontHitThisOne()==Collided){
			st=ST_CONTINUE;
			return;
		}
		if(((shot *)Mover)->HitMortal(Collided->IsMortal()));
			st=ST_TOTALSTOP;
	}	
	else{
		switch( (Mover->IsMortal())->MortalInteraction(Collided->IsMortal())){
		case WAY_TURN:
		case WAY_STOP:
			if(Mover->IsPlayer()){
				// So that we can slide
				return;
			}
		case WAY_DIED:
				st=ST_TOTALSTOP;
				return;

		case WAY_KILL:
			st=ST_CONTINUE;
			return;

				
		}
	}
}
