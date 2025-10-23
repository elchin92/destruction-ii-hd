// SoundStructure.cpp: implementation of the SoundStructure class.
//
//////////////////////////////////////////////////////////////////////

#include "SoundStructure.h"
#include "SoundEngine.h"
#include "SoundSource.h"
#include "DebugWindow.h"
#include "Game.h"
#include "Functions.h"
#include "Player.h"
#include "Settings.h"
#include "Menu.h"
#include "Debugger.h"

int stdl=1, stcr=1;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SoundStructure::SoundStructure(SoundSource * theSource, char * filename, int Loop){

	TheSource=theSource;
	WaveName=filename;
	TheBuffer=NULL;
	TheWave=NULL;
	LOOPING=Loop;

	BufferSize      = 0;
    PieceSize       = 0;
	PieceProgress	= 0;
    NextWriteOffset = 0;
    Progress        = 0;
    LastPos         = 0;
	LastVolume		= 1000000000; // so that they definetly wil be updated
	LastPan			= 1000000000; 
	Stopped=FALSE;
	IS_PLAYING=FALSE;


	//CreateSound();

	DP2("Creating a struct",stcr++);
}

SoundStructure::~SoundStructure()
{
	DP2("Deleting a struct",stdl++);
	DestroySound();

}

void SoundStructure::DestroyWave(){
	if( TheWave ){
		TheWave->Close();
		SAFE_DELETE(TheWave);
	}
}




//-----------------------------------------------------------------------------
// Name: HasStopped()
// Desc: Returns Stopped
//-----------------------------------------------------------------------------

//BOOL SoundStructure::HasStopped(){return Stopped;}

//-----------------------------------------------------------------------------
// Name: HasFoundEnd()
// Desc: Returns FoundEnd
//-----------------------------------------------------------------------------

BOOL SoundStructure::HasFoundEnd(){return FoundEnd;}

//-----------------------------------------------------------------------------
// Name: IsPlaying()
// Desc: Returns TRUE if StartPlaying() has been called and Stop() hasn't 
//		 been called afterwards.
//-----------------------------------------------------------------------------

BOOL SoundStructure::IsPlaying(){return IS_PLAYING;}

//-----------------------------------------------------------------------------
// Name: IsActuallyPlaying()
// Desc: Checks whether the buffer actually still is playing
//-----------------------------------------------------------------------------

BOOL SoundStructure::IsActuallyPlaying(){
	if(!TheBuffer)
		return FALSE;
	DWORD BufferStatus;
	TheBuffer->GetStatus(&BufferStatus);
	// We want to return a 1 or 0
	if(BufferStatus&DSBSTATUS_PLAYING)
		return TRUE;
	return FALSE;

}
//-----------------------------------------------------------------------------
// Name: UpdateBuffer()
// Desc: This must be called called on regular intervals.
//-----------------------------------------------------------------------------
HRESULT SoundStructure::UpdateBuffer(){

	HRESULT hr;

	DBS(31200);
	
	if(TheBuffer){
	

		DBS(31222);
		if(FAILED(hr=UpdateProgress())){
			
			DBS(6660004);
			DBS(31223);
			DBCE("UpdateProgress() failed");
			return RestoreBuffers();
		}
		DBS(31223);
		UpdatePanAndVolume();
	}


	return S_OK;

	DBS(31201);
}


//-----------------------------------------------------------------------------
// Name: StopBuffer()
// Desc: Stop the DirectSound buffer
//-----------------------------------------------------------------------------
HRESULT SoundStructure::StopBuffer() 
{
	DBS(31400);
    if( NULL != TheBuffer ){

		FoundEnd=TRUE;
		DBS(31420);
		if(FAILED(TheBuffer->Stop())){
			DBCE("Buffer Stop() Failed");
			DBS(6660005);
		}

	}
	DBS(31401);
    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: PlayBuffer()
// Desc: Play the DirectSound buffer
//-----------------------------------------------------------------------------
HRESULT SoundStructure::PlayBuffer()
{
	DBS(31600);
	HRESULT hr;

	if( TheBuffer == NULL )
		CreateSound();


	DBS(31621);
	if( NULL == TheBuffer )
		return E_FAIL;

	// Restore the buffers if they are lost
	if( FAILED( hr = RestoreBuffers() ) )
		return hr;

	DBS(31622);
	// Fill the entire buffer with wave data
	if( FAILED( hr = FillBuffer( ) ) )
		return hr;


    // Play with the LOOPING flag since the streaming buffer
    // wraps around before the entire WAV is played unless
	// there is only one buffer piece
	DWORD Flags = DSBPLAY_LOOPING;
	if(BufferPieces==1&&!LOOPING)
		Flags=0;

	DBS(31623);
    if( FAILED( hr = TheBuffer->Play( 0, 0, Flags ) ) ){
		DBCE("Play Buffer Failed");
        return hr;
	}


	DBS(31620);
	IS_PLAYING=TRUE;

	DBS(31601);
    return S_OK;
}




void SoundStructure::SetCenterPan(){
	if(TheBuffer)
		TheBuffer->SetPan(0);
}

/*

*/
