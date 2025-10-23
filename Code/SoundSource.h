#ifndef __SoundSource__ 
#define __SoundSource__

#include "SoundEnum.h"
#include "SoundPriorityEnum.h"
#define BREAKDIST	dFar
#define BREAKPRI	spLow
#define WallSound();	CreateAndPlaySound(sWallDown, BREAKDIST, BREAKPRI);

class Game;
class SoundBuffer;
class SoundEngine;
class SoundStructure;

class SoundSource
{
protected:

	int Pos[2], ActualCenterPos, SX, SY;

	Sound Snd;
	DistanceType sDistance;
	SoundPriority Priority;
	int DELETEME, DELETEABLE, Looping;

//	void AddMeToThePlayingSoundsList();
//	void RemoveMeFromThePlayingSoundsList();

//	int AmPartOfTheList();

public:

	SoundStructure * TheSound;

	// Reserved for the SoundEngine
	SoundSource * nextSoundSource;

	SoundSource();
	SoundSource(Sound iSnd, SoundPriority sPriority, int x, int y, DistanceType iDistance, int Loop);
	virtual ~SoundSource();


	int GetX();
	int GetY();
	int GetPos(int xy);
	int GetXs();
	int GetYs();
	int GetNewXs();
	int GetNewYs();
	int GetActualXPos();
	int GetActualYPos();
	int GetActualPos(int xy);
	int GetActualCenterPos();


	void SetPosNonActor(int x, int y);
	void UpdateS();

	int IsPlaying();
	int DeleteAble();

	DistanceType GetDistance();
	SoundPriority GetPriority();
	Sound GetSound();


	void SetCenterPan();
	void HasStopped();
	void Initialize();
	int StartPlaying();
	void StopPlaying();

	SoundSource * CreateAndPlaySound(Sound snd, DistanceType dType=dNormal, SoundPriority sPri=spLow, int Loop=0);	
	SoundSource * CreateAndPlaySound(BulletType bType, DistanceType dType=dNormal, SoundPriority sPri=spLow, int Loop=0);	

	void UpdateSound();


};
#endif

