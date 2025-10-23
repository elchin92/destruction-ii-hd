#ifndef __QUADRANTS__
#define __QUADRANTS__

#define NineX (niXs+nLoopX)
#define NineY (niYs+nLoopY)
#define Nine(xy) (xy?NineY:NineX)
#define NinePos(xy) (Nine(xy)*32)
#define NineLoopXY(ixs,iys) for(int nLoopX=0, niXs=(ixs)-1, niYs=(iys)-1 ; nLoopX<3; nLoopX++)	for(int nLoopY=0 ; nLoopY<3; nLoopY++) if(CheckSquare(NineX, NineY))
#define NineLoopDM(Mover) NineLoopXY(Mover->GetXs(),Mover->GetYs())

#define InternalMortalQuadLoop() for(Mortal * Iterator = TheMortalQuadrants[NineX][NineY];Iterator;Iterator=Iterator->nextQuadMortal)
#define InternalShotQuadLoop() for(shot * Iterator = TheShotQuadrants[NineX][NineY];Iterator;Iterator=Iterator->nextQuadShot)

#define ShotQuadLoopDM(Mover) NineLoopDM(Mover) InternalShotQuadLoop()
#define MortalQuadLoopDM(Mover) NineLoopDM(Mover) InternalMortalQuadLoop()
#define NineLoop() NineLoopDM(Mover)
#define ShotQuadLoop() ShotQuadLoopDM(Mover)
#define MortalQuadLoop() MortalQuadLoopDM(Mover)
#define MortalQuadLoopXY(ixs,iys) NineLoopXY(ixs,iys) InternalMortalQuadLoop()
#define TwoWayLoop for(int i=0, twxy=xy;i<1+MovingTwice;i++,twxy=!twxy)
#define twoWayLoop for(int i=0, twxy=xy;i<1+MovingTwice;i++,twxy=!twxy) // C++20: added 'int' for loop variable scoping

#endif