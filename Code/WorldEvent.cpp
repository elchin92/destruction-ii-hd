// WorldEvent.cpp: implementation of the WorldEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "WorldEvent.h"
#include "shot.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

WorldEvent::WorldEvent(Actor * iMover, int iMP, int Secondary)
{
	Initialize();

	TheShot=iMover->IsShot();
	if(TheShot){
		st=ST_TOTALSTOP;
	}else{
		st=(StopType)((int)ST_SLIDEPRIMARY+Secondary);
	}
	MP=iMP;
}

WorldEvent::~WorldEvent()
{

}
void WorldEvent::Act(){
	if(TheShot)
		TheShot->KILLME=TRUE;
}
int WorldEvent::KamikazeEvent()
{
	return 1;
}