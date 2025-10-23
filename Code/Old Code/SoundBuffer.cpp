// SoundBuffer.cpp: implementation of the SoundBuffer class.
//
//////////////////////////////////////////////////////////////////////

#include "SoundBuffer.h"
#include "SoundEngine.h"
#include "SoundSource.h"
#include "DebugWindow.h"
#include "Game.h"

#define DESIRED_BUFFER_LENGTH 500
#define DESIRED_PIECE_LENGTH 50
#define DESIRED_PIECES_PER_S (1000/(DESIRED_BUFFER_LENGTH/PIECES_PER_BUFFER))


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SoundBuffer::SoundBuffer(char * filename,SoundSource * iTheSource, int Loop)
{
	TheSource=iTheSource;
	TheWave=NULL;
	WaveName=filename;
	TheBuffer=NULL;
	LOOPING=Loop;
	BufferSize      = 0;
    PieceSize       = 0;
	PieceProgress	= 0;
    NextWriteOffset = 0;
    Progress        = 0;
    LastPos         = 0;
	IS_PLAYING=FALSE;
	IS_PAUSED=FALSE;

	CreateSound();

	nextSoundBuffer=TheGame->TheEngine->TheBuffers;
	if(nextSoundBuffer)
		nextSoundBuffer->prevSoundBuffer=this;
	TheGame->TheEngine->TheBuffers=this;
	prevSoundBuffer=NULL;

}

SoundBuffer::~SoundBuffer()
{
	DBS(31000);	
	// Must only be deleted by soundsource

	DBS(31020);	
	DestroyWave();
	DBS(31021);	
	DestroyBuffer();
	DBS(31022);	
	if(nextSoundBuffer)
		nextSoundBuffer->prevSoundBuffer=prevSoundBuffer;
	if(prevSoundBuffer)
		prevSoundBuffer->nextSoundBuffer=nextSoundBuffer;
	if(TheGame->TheEngine->TheBuffers==this)
		TheGame->TheEngine->TheBuffers=nextSoundBuffer;

	DBS(31001);	

}
void SoundBuffer::DestroyBuffer(){
	if(TheBuffer){
		TheBuffer->Release();
		delete TheBuffer;
		TheBuffer=NULL;
	}
}

void SoundBuffer::DestroyWave(){
	if( TheWave ){
		delete TheWave;
		TheWave=NULL;
	}
}

HRESULT SoundBuffer::CreateSound(){

	DBS(31100);	
	HRESULT hr;

	DestroyWave();
	DBS(31120);	
	TheWave = new CWaveSoundRead();

	char fullName[40];
	sprintf(fullName,"%s\\%s",WaveDir,WaveName);

	hr=TheWave->Open( fullName );
	DP(&fullName[0]);
	DBS(31121);	
	if( FAILED( hr ) )
	{
		TheGame->UserMessage("Bad wave file", fullName);			
	}
	else {
		hr=CreateStreamingBuffer();
	}
	DBS(31101);	
	return hr;

}




//-----------------------------------------------------------------------------
// Name: CreateStreamingBuffer()
// Desc: Creates a streaming buffer, and the notification events to handle
//       filling it as sound is played
//-----------------------------------------------------------------------------
HRESULT SoundBuffer::CreateStreamingBuffer()
{
    HRESULT hr; 

	DBS(18001);

	DestroyBuffer();

	DBS(18002);

    // This engine works by dividing a streaming buffer into approximately pieces
    // of DESIRED_PIECE_LENGTH unless the buffer is shorter than 
	// DESIRED_BUFFER_LENGTH ms. If that is the case then that sound is played 
	// without looping.
	// Otherwise it starts by filling up the entire buffer with wave data. Once
	// it has played a piece, it fills the played piece up with new wave data which
	// will be played once the buffer has looped around.
	// UpdateSound needs to be called at least once every DESIRED_PIECE_LENGTH ms.

	// Smallest atomic piece of the buffer that is a sample (size/sample)
    DWORD nBlockAlign = (DWORD)(TheWave->m_pwfx->nBlockAlign);
    INT nSamplesPerSec = TheWave->m_pwfx->nSamplesPerSec;

    // Get the total size
	DWORD TotalSize	= (TheWave->m_ckInRiff.cksize+nBlockAlign-1);

	DWORD ms = (1000*TotalSize)/(nSamplesPerSec*nBlockAlign);
	if(ms>DESIRED_BUFFER_LENGTH){
		BufferPieces=DESIRED_BUFFER_LENGTH/DESIRED_PIECE_LENGTH;
		// (Size per second / Piece per second)
		PieceSize = (nSamplesPerSec * nBlockAlign) / BufferPieces;
		DBS(9999001);
	}else{
		// If buffer pieces == 1, we will ignore the piece process and play without looping
		BufferPieces=1;
		PieceSize=TotalSize;
		DBS(9999002);
	}

	// The PieceSize should be an integer multiple of nBlockAlign
	PieceSize -= PieceSize % nBlockAlign;

	// Calculate number of pieces to play
	Pieces = (TotalSize+PieceSize-1)/PieceSize;

    // The buffersize should approximately DESIRED_BUFFER_LENGTH ms of wav data
	// unless the wave is shorter than that.
    BufferSize  = PieceSize * BufferPieces;
    FoundEnd    = FALSE;
    Progress    = 0;
    LastPos     = 0;

    // Set up the direct sound buffer, and only request the flags needed
    // since each requires some overhead and limits if the buffer can 
    // be hardware accelerated

    DSBUFFERDESC dsbd;
    ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
    dsbd.dwSize        = sizeof(DSBUFFERDESC);
    dsbd.dwFlags       = DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GETCURRENTPOSITION2;
    dsbd.dwBufferBytes = BufferSize;
    dsbd.lpwfxFormat   = TheWave->m_pwfx;

	DBS(18004);
    // Create a DirectSound buffer
    while(FAILED( hr = TheGame->TheEngine->lpDS->CreateSoundBuffer( &dsbd, &TheBuffer, NULL ) ) ){
		if(hr!=DSERR_OUTOFMEMORY )
			return hr;
			DBCE("Couldn't create soundbuffer!");
		//TheGame->TheEngine->ReleaseOne();
	}
	DBS(18005);
    return S_OK;
}



//-----------------------------------------------------------------------------
// Name: HasFoundEnd()
// Desc: Returns FoundEnd
//-----------------------------------------------------------------------------

BOOL SoundBuffer::HasFoundEnd(){return FoundEnd;}

//-----------------------------------------------------------------------------
// Name: IsPlaying()
// Desc: Returns TRUE if StartPlaying() has been called and Stop() hasn't 
//		 been called afterwards.
//-----------------------------------------------------------------------------

BOOL SoundBuffer::IsPlaying(){return IS_PLAYING;}

//-----------------------------------------------------------------------------
// Name: IsActuallyPlaying()
// Desc: Checks whether the buffer actually still is playing
//-----------------------------------------------------------------------------

BOOL SoundBuffer::IsActuallyPlaying(){
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
void SoundBuffer::UpdateBuffer(){

	DBS(31200);
	
	if(TheBuffer){
	
		if(BufferPieces==1){
			
			DBS(31220);
				//DBS(444446);
			// We do no need to fill up more wave data since the buffer already
			// contains all wave data.
			if(!IsActuallyPlaying()){
				
				DBS(31221);
				TheSource->SoundHasStopped();
			}
			return;
		}
		
		DBS(31222);
		if(UpdateProgress()!=S_OK){
			
			DBS(31223);
			RestoreBuffers();
		}
	}

	DBS(31201);
}
//-----------------------------------------------------------------------------
// Name: UpdateProgress()
// Desc: Checks if more wave data needs to be written and does that if needed.
//-----------------------------------------------------------------------------
HRESULT SoundBuffer::UpdateProgress()
{

	DBS(31300);
    HRESULT hr;
    DWORD   dwPlayPos;
    DWORD   dwWritePos;
    DWORD   dwPlayed;

    if( FAILED( hr = TheBuffer->GetCurrentPosition( &dwPlayPos, &dwWritePos ) ) )
        return hr;
	DBS(31320);
    if( dwPlayPos < LastPos ){
        dwPlayed = BufferSize - LastPos + dwPlayPos;
    }else
        dwPlayed = dwPlayPos - LastPos;

    Progress += dwPlayed;
    LastPos = dwPlayPos;

	// While we have played a whole piece, fill the space up
	// with new wave data.
	while(Progress>(PieceProgress+1-BufferPieces)*PieceSize && 
		  PieceProgress<Pieces){
		DBS(31330);
		// Write a piece
		WriteMoreWaveData();
		PieceProgress++;
	}
	DBS(31321);
	if(	Progress>=Pieces*PieceSize && !LOOPING ){
		TheSource->SoundHasStopped();
		DBS(31322);
		DBS(7777777);
	}
//	DBS(10000000+Progress);
//	DBS(20000000+Pieces*PieceSize);
	DBS(31301);
    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: RestoreBuffers()
// Desc: Restore lost buffers and fill them up with sound if possible
//-----------------------------------------------------------------------------
HRESULT SoundBuffer::RestoreBuffers()
{
    HRESULT hr;

    if( NULL == TheBuffer )
        return S_OK;

    DWORD dwStatus;
    if( FAILED( hr = TheBuffer->GetStatus( &dwStatus ) ) )
        return hr;

    if( dwStatus & DSBSTATUS_BUFFERLOST )
    {
        // Since the app could have just been activated, then
        // DirectSound may not be giving us control yet, so 
        // the restoring the buffer may fail.  
        // If it does, sleep until DirectSound gives us control.
        do 
        {
            hr = TheBuffer->Restore();
            if( hr == DSERR_BUFFERLOST )
                Sleep( 10 );
        }
        while( hr = TheBuffer->Restore() );

        if( FAILED( hr = FillBuffer( ) ) )
            return hr;
    }

    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: StopBuffer()
// Desc: Stop the DirectSound buffer
//-----------------------------------------------------------------------------
HRESULT SoundBuffer::StopBuffer() 
{
	DBS(31400);
	HRESULT hr=S_OK;
    if( NULL != TheBuffer ){
		IS_PLAYING=FALSE;
		FoundEnd=TRUE;
		DBS(31420);
		TheBuffer->Stop();
		DBS(31421);
		DestroyWave();
		DBS(31422);
		DestroyBuffer();
	}
	DBS(31401);
    return hr;
}
//-----------------------------------------------------------------------------
// Name: PauseBuffer()
// Desc: Pause the DirectSound buffer
//-----------------------------------------------------------------------------
HRESULT SoundBuffer::PauseBuffer() 
{
    if( NULL != TheBuffer ){
		IS_PLAYING=FALSE;
		IS_PAUSED=TRUE;
		return TheBuffer->Stop();

	}

    return S_OK;
}
//-----------------------------------------------------------------------------
// Name: WriteToBuffer()
// Desc: Writes wave data to the streaming DirectSound buffer 
//-----------------------------------------------------------------------------
HRESULT SoundBuffer::WriteToBuffer( VOID* pbBuffer, DWORD dwBufferLength )
{
	DBS(31500);
    HRESULT hr;
    UINT nActualBytesWritten;

    if( !FoundEnd)
    {
        // Fill the DirectSound buffer with WAV data
        if( FAILED( hr = TheWave->Read(dwBufferLength, 
                                                 (BYTE*) pbBuffer, 
                                                 &nActualBytesWritten ) ) )
            return hr;
    }
    else
    {
        // Fill the DirectSound buffer with silence
        FillMemory( pbBuffer, dwBufferLength, 
                    (BYTE)( TheWave->m_pwfx->wBitsPerSample == 8 ? 128 : 0 ) );
        nActualBytesWritten = dwBufferLength;
    }

    // If the number of bytes written is less than the 
    // amount we requested, we have a short file.
    if( nActualBytesWritten < dwBufferLength )
    {
        if( !LOOPING ) 
        {
            FoundEnd = TRUE;

            // Fill in silence for the rest of the buffer.
            FillMemory( (BYTE*) pbBuffer + nActualBytesWritten, 
                        dwBufferLength - nActualBytesWritten, 
                        (BYTE)(TheWave->m_pwfx->wBitsPerSample == 8 ? 128 : 0 ) );
        }
        else
        {
            // We are looping.
            UINT nWritten = nActualBytesWritten;    // From previous call above.
            while( nWritten < dwBufferLength )
            {  
                // This will keep reading in until the buffer is full. For very short files.
                if( FAILED( hr = TheWave->Reset() ) )
                    return hr;

                if( FAILED( hr = TheWave->Read( (UINT)dwBufferLength - nWritten,
                                                          (BYTE*)pbBuffer + nWritten,
                                                          &nActualBytesWritten ) ) )
                    return hr;

                nWritten += nActualBytesWritten;
            } 
        } 
    }
	DBS(31501);
    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: PlayBuffer()
// Desc: Play the DirectSound buffer
//-----------------------------------------------------------------------------
HRESULT SoundBuffer::PlayBuffer()
{
	DBS(31600);
	HRESULT hr;

	if( TheBuffer == NULL )
		CreateSound();
	DBS(31620);
	IS_PLAYING=TRUE;

//	DBS(17001);



	if(!IS_PAUSED){
		DBS(31621);
		VOID*   pbBuffer = NULL;

		if( NULL == TheBuffer )
			return E_FAIL;

		// Restore the buffers if they are lost
		if( FAILED( hr = RestoreBuffers() ) )
			return hr;
		DBS(31622);
		// Fill the entire buffer with wave data
		if( FAILED( hr = FillBuffer( ) ) )
			return hr;


	}

    // Play with the LOOPING flag since the streaming buffer
    // wraps around before the entire WAV is played unless
	// there is only one buffer piece
	DWORD Flags = DSBPLAY_LOOPING;
	if(BufferPieces==1)
		Flags=0;

	DBS(31623);
    if( FAILED( hr = TheBuffer->Play( 0, 0, Flags ) ) )
        return hr;

	IS_PAUSED=FALSE;
	DBS(31601);
    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: FillBuffer()
// Desc: Fills the DirectSound buffer completely up 
//-----------------------------------------------------------------------------
HRESULT SoundBuffer::FillBuffer()
{
    HRESULT hr;
    VOID*   pbBuffer = NULL;
    DWORD   dwBufferLength;

    if( NULL == TheBuffer )
        return E_FAIL;

    FoundEnd = FALSE;
    NextWriteOffset = 0; // Start writing from zero as soon as the first piece has played
    Progress = 0;
	PieceProgress=BufferPieces;
    LastPos  = 0;

    // Reset the wav file to the beginning
    TheWave->Reset();
    TheBuffer->SetCurrentPosition( 0 );

    // Lock the buffer down, 
    if( FAILED( hr = TheBuffer->Lock( 0, BufferSize, 
                                        &pbBuffer, &dwBufferLength, 
                                        NULL, NULL, 0L ) ) )
        return hr;

    // Fill the buffer with wav data 
    if( FAILED( hr = WriteToBuffer( pbBuffer, dwBufferLength ) ) )
        return hr;

    // Now unlock the buffer
    TheBuffer->Unlock( pbBuffer, dwBufferLength, NULL, 0 );


    return S_OK;
}



//-----------------------------------------------------------------------------
// Name: WriteMoreWaveData()
// Desc: Writes another piece of data to the buffer
//-----------------------------------------------------------------------------
HRESULT SoundBuffer::WriteMoreWaveData() 
{
    HRESULT hr;
    VOID* pbBuffer  = NULL;
    DWORD dwBufferLength;

    // Lock the buffer down
    if( FAILED( hr = TheBuffer->Lock( NextWriteOffset, PieceSize, 
                                        &pbBuffer, &dwBufferLength, NULL, NULL, 0L ) ) )
        return hr;

    // Fill the buffer with wav data 
    if( FAILED( hr = WriteToBuffer( pbBuffer, dwBufferLength ) ) )
        return hr;

    // Now unlock the buffer
    TheBuffer->Unlock( pbBuffer, dwBufferLength, NULL, 0 );
    pbBuffer = NULL;

    NextWriteOffset += PieceSize; 
    NextWriteOffset %= BufferSize; // Circular buffer

    return S_OK;
}

void SoundBuffer::SetVolume(long Volume){
	DBS(31700);
	TheBuffer->SetVolume(Volume);
	DBS(31701);
}
void SoundBuffer::SetPan(long Pan){
	DBS(31702);
	TheBuffer->SetPan(Pan);
	DBS(31703);
}