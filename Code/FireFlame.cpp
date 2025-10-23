// FireFlame.cpp: implementation of the FireFlame class.
//
//////////////////////////////////////////////////////////////////////
#define FlameTime 50

#include "Game.h"
#include "Shooter.h"
#include "FireFlame.h"
#include "Flame.h"
#include "Functions.h"

FireFlame * TheFireFlames;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FireFlame::FireFlame(Shooter * iOwner, int iLeftRightShot, BulletType ibType, int LaserColor)
{

	Owner=iOwner;
	LeftRightShot=iLeftRightShot;
	FlameStop=TheGame->thisTickCount + FlameTime-1;
	Frames=2;
	FlameType=BulletType2Flame[ibType];
	if(FlameType==fRedLaser)
		FlameType=LaserColor2Flame[LaserColor];
	nextFireFlame=NULL;
	KILLME=FALSE;
	AddThisToLinkedList(TheFireFlames, nextFireFlame);
}

FireFlame::~FireFlame()
{

}

void FireFlame::Blit(){


	if(nextFireFlame)
		nextFireFlame->Blit();

	if(TheGame->thisTickCount>=FlameStop||!Owner->IsAlive()){
		KILLME=TRUE;
		return;
	}

	TheGame->BlankSurfaceArea(TempFlamePos[0],TempFlamePos[1],FlameThickness[FlameType],FlameThickness[FlameType] ,&TheGame->g_pDDSFlames);

	int FlameTick=Frames - (Frames*(int)(FlameStop - TheGame->thisTickCount))/FlameTime - 1;
	int Rotated = Owner->Get8Dir()%2;

	TheGame->rcRect.left  = FlameSourcePos[FlameType][0]+Rotated*FlameThickness[FlameType];
	TheGame->rcRect.top   = FlameSourcePos[FlameType][1]+FlameThickness[FlameType]*FlameTick;  
	TheGame->rcRect.right =	TheGame->rcRect.left+FlameThickness[FlameType];
	TheGame->rcRect.bottom=	TheGame->rcRect.top+FlameThickness[FlameType];
	
	TheGame->BlitOntoSetRc(TempFlamePos[0],TempFlamePos[1],&TheGame->g_pDDSFlames,&TheGame->g_pDDSFlames, 0, Owner->Get4Dir()); // Rotate-Blit

	TheGame->rcRect.left  = TempFlamePos[0];
	TheGame->rcRect.top   = TempFlamePos[1];  
	TheGame->rcRect.right =	TheGame->rcRect.left+FlameThickness[FlameType];
	TheGame->rcRect.bottom=	TheGame->rcRect.top +FlameThickness[FlameType];

	int BlitX=Owner->GetActualXPos()+FlameAdjust[Owner->Get8Dir()][0]*FlameThickness[FlameType]/2;
	int BlitY=Owner->GetActualYPos()+FlameAdjust[Owner->Get8Dir()][1]*FlameThickness[FlameType]/2;

	LengthAdjust(&BlitX,&BlitY,Owner->Get8Dir(),Owner->GetPipeLength(LeftRightShot));
	SideAdjust(&BlitX,&BlitY,Owner->Get8Dir(),LeftRightShot*Owner->GetPipeDistance(Rotated)+Owner->GetPipeSideAdjust());

	TheGame->BGOS(BlitX,BlitY,&TheGame->g_pDDSFlames);

}