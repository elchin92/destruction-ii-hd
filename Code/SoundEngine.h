#include <dsound.h>
#include "wavread.h"
#include "SoundEnum.h"
#include "SoundPriorityEnum.h"

#ifndef __SoundEngine__ 
#define __SoundEngine__

static char WaveDir[] = "Sound";

//#define DEBUGSOUND

#ifdef DEBUGSOUND
#define DBS(p) {TheGame->Beacon(p);}
#else
#define DBS(p);
#endif


class Game;
class SoundSource;
class SoundBuffer;

class SoundEngine
{
private:

	BOOL ACTIVE;
	int MaxBuffers;
	int MaxVolume;


	void SetWaveNames();
	HRESULT InitDirectSound();
	HRESULT FreeDirectSound();


	SoundSource * PlayingSounds[NumberOfSounds][2];
	int FirstAddedSound[NumberOfSounds];

public:

	SoundSource * TheActors;
	SoundSource * TheSounds;


	LPDIRECTSOUND           lpDS;
	char WaveNames[NumberOfSounds][30];

	

	SoundEngine();
	~SoundEngine();


	SoundSource * PlaySound(Sound snd,						// Plays a sound at full volume
				   SoundPriority spPri=spLow, BOOL Loop=FALSE);	

	SoundSource * PlaySound(BulletType bType,				// Plays a sound at full volume
				   SoundPriority spPri=spLow, BOOL Loop=FALSE);	

	SoundSource * PlayLocatedSound(Sound snd, int x, int y,	// Plays a sound located at
 						 DistanceType dType=dNormal,// the given position 
						 SoundPriority sPri=spLowest, BOOL Loop=FALSE);

	void ActivateSound();
	void DisableSound();
	void NormalizePanning();

	void IncreaseVolume();
	void DecreaseVolume();
	void SetVolume();



	// Must be called periodacally, at least once every 100 ms!
	void UpdateSound();

	void CleanUp();

	void RemoveFromThePlayingSounds(SoundSource * sSource);
	void AddToThePlayingSounds(SoundSource * sSource);
	int AllowedToPlay(Sound snd, SoundPriority sPri);
	int SoundActivated();
	int GetVolume();

};
#endif

extern SoundEngine * TheSoundEngine;