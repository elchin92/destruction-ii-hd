#ifndef __SoundStructure__ 
#define __SoundStructure__

#include "SoundEnum.h"
#include "SoundPriorityEnum.h"
#include "wavread.h"
#include "SDL2_DirectSoundCompat.h"
#define BREAKDIST	dFar
#define BREAKPRI	spLow
#define WallSound();	CreateAndPlaySound(sWallDown, BREAKDIST, BREAKPRI);

#define DESIRED_BUFFER_LENGTH 1000
#define DESIRED_PIECE_LENGTH 200
#define DESIRED_PIECES_PER_S (1000/(DESIRED_BUFFER_LENGTH/PIECES_PER_BUFFER))

class Game;
class SoundBuffer;
class SoundEngine;
class SoundSource;

class SoundStructure
{
protected:

	int LOOPING;
	long LastVolume, LastPan;

	// Buffer&play related

	CWaveSoundRead*     TheWave;
	char *				WaveName;

	LPDIRECTSOUNDBUFFER TheBuffer;

	DWORD               BufferSize;
	DWORD               PieceSize;
	DWORD               NextWriteOffset;
	DWORD               Progress;
	DWORD               LastPos;

	int					PieceProgress;
	int					Pieces;
	int					BufferPieces;

	BOOL                IS_PLAYING, FoundEnd, Stopped;





	BOOL IsActuallyPlaying();

	void DestroyBuffer();
	void DestroyWave();

	HRESULT UpdateProgress();
	HRESULT WriteToBuffer(VOID* pbBuffer, DWORD dwBufferLength );
	HRESULT RestoreBuffers();
	HRESULT CreateStreamingBuffer();

	HRESULT FillBuffer();
	HRESULT WriteMoreWaveData();
	HRESULT CreateSound();

	HRESULT NotifySourceAndDestroySound();
	HRESULT DestroySound();

	void CreateBuffer();
	void SetVolume(long Volume);
	void SetPan(long Pan);

public:

	SoundSource * TheSource;


	SoundStructure(SoundSource * theSource,char * filename, int Loop);
	~SoundStructure();


	HRESULT StopBuffer();
	HRESULT PauseBuffer();
	HRESULT PlayBuffer();
	HRESULT UpdateBuffer();

	BOOL IsPlaying();
	BOOL HasFoundEnd();

	void UpdatePanAndVolume();
	void SetCenterPan();


};
#endif
