#include "WallEvent.h"
#include "Actor.h"
#include "Debugger.h"

WallEvent::WallEvent(int iMP, int iXS, int iYS, Actor * iCollider, int Secondary){
	Initialize();
	MP=iMP;
	DP2("  Wall MP:",MP);
	XS=iXS;
	YS=iYS;
	Collider=iCollider;
	st=(StopType)((int)ST_SLIDEPRIMARY+Secondary);
	
}
WallEvent::~WallEvent(){}
int WallEvent::KamikazeEvent()
{
	return 1;
}
void WallEvent::Act(){
	if(Collider->MoveThroughWall(XS, YS)){
		st=ST_CONTINUE;
		DP("Allowed to move through wall");
		Beacon(XS);
		Beacon(YS);
		return;
	}
	

}