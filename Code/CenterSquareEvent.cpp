// CenterSquareEvent.cpp: implementation of the CenterSquareEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "Mortal.h"
#include "Game.h"
#include "MovementObject.h"
#include "CenterSquareEvent.h"
#include "Debugger.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CenterSquareEvent::CenterSquareEvent(Mortal * iMover,int iMP)
{
	Initialize();MP=iMP;Mover=iMover;st=ST_SAVEMP;
}

CenterSquareEvent::~CenterSquareEvent()
{

}

void CenterSquareEvent::Act(){
	Beacon(777999);

	Mover->Lock();

	if(!Mover->ChangeDir(TRUE)){
		st=ST_TOTALSTOP;
	}
}