// SoundBuffer.h: interface for the SoundBuffer class.
//
//////////////////////////////////////////////////////////////////////
#include "wavread.h"
#include <dsound.h>
#ifndef __SoundBuffer__ 
#define __SoundBuffer__


class Game;
class SoundSource;
class SoundBuffer  
{
private:

	DWORD               BufferSize;
	DWORD               PieceSize;
	DWORD               NextWriteOffset;
	DWORD               Progress;
	DWORD               LastPos;
	BOOL                IS_PLAYING, FoundEnd, IS_PAUSED;

	char *				WaveName;

	int					PieceProgress;
	int					Pieces;
	int					LOOPING;
	int					BufferPieces;

	LPDIRECTSOUNDBUFFER TheBuffer;

	CWaveSoundRead*     TheWave;

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


public:

	SoundBuffer * nextSoundBuffer;
	SoundBuffer * prevSoundBuffer;

	SoundSource * TheSource;

	SoundBuffer(Game* theGame, char * filename,SoundSource * iTheSoundSource, int Loop);
	~SoundBuffer();


	HRESULT StopBuffer();
	HRESULT PauseBuffer();
	HRESULT PlayBuffer();


	BOOL IsPlaying();
	BOOL HasFoundEnd();
	void UpdateBuffer();
	void SetVolume(long Volume);
	void SetPan(long Pan);


};

#endif 