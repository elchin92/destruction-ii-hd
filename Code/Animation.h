#include "Actor.h"
#ifndef __Animation__ 
#define __Animation__

class Animation  : public Actor
{
private:
	int FrameTime, AnimType, Reverse;
	__int64 StartTime;
public:
	int KILLME;
	Animation * nextAnimation;
	Animation(int iFrames, int iPlayTime, int iAnimType,int x, int y, int BlitX, int BlitY, int iReverse);
	~Animation();
	void TickUp();
	void Blit(int iAnimType);
	void Move();
	void KillAll();
	void KillDead();
	void ShiftLeft();
	void ShiftRight();

};

extern	Animation *					TheAnimations;
#endif 