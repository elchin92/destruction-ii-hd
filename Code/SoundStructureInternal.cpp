#include "Game.h"
#include "SoundEngine.h"
#include "SoundSource.h"
#include "SoundStructure.h"
#include "DebugWindow.h"
#include "Functions.h"
#include "Player.h"
#include "Settings.h"
#include "Menu.h"
#include "Debugger.h"

//-----------------------------------------------------------------------------
// Name: CreateSound()
// Desc: Opens the wave to be read and calls CreateStreamingBuffer().
//-----------------------------------------------------------------------------
HRESULT SoundStructure::CreateSound(){

	DBS(31100);	
	HRESULT hr;

	DestroyWave();
	DBS(31120);	
	TheWave = new CWaveSoundRead();

	hr=TheWave->Open( WaveName );
	DBS(31121);	
	if( FAILED( hr ) )
	{
		Beacon(11111);
		DP(WaveName);
		TheGame->UserMessage("Bad wave file", WaveName);

	}
	else {
		hr=CreateStreamingBuffer();
		if(SUCCEEDED( hr )){
			LastVolume=VOL_MAX;
			UpdatePanAndVolume();
		}
	}
	DBS(31101);	

	return hr;

}



//-----------------------------------------------------------------------------
// Name: CreateStreamingBuffer()
// Desc: Creates a streaming buffer, and the notification events to handle
//       filling it as sound is played
//-----------------------------------------------------------------------------
HRESULT SoundStructure::CreateStreamingBuffer()
{
    HRESULT hr; 

	DBS(18001);

	SAFE_RELEASE(TheBuffer);

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

    // Get the total size of the buffer
	DWORD TotalSize	= (TheWave->m_ckInRiff.cksize+nBlockAlign-1);

	// Get the total length of the buffer
	DWORD ms = (1000*TotalSize)/(nSamplesPerSec*nBlockAlign);

	DBS(22000000+ms);
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

	if(BufferPieces==1 && Pieces!=1)
		DBS(66600010);
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
    dsbd.dwFlags       = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLPAN  | DSBCAPS_CTRLVOLUME ;
    dsbd.dwBufferBytes = BufferSize;
    dsbd.lpwfxFormat   = TheWave->m_pwfx;

	DBS(18002);
    // Create a DirectSound buffer
    while(FAILED( hr = TheSoundEngine->lpDS->CreateSoundBuffer( &dsbd, &TheBuffer, NULL ) ) ){

		DBS(6660001);
		DBS(18015);
			DBCE("Couldn't create soundbuffer!");
			DBS(18016);
		if(hr!=DSERR_OUTOFMEMORY )
			return hr;
	
	}
	DBS(18003);
    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: FillBuffer()
// Desc: Fills the DirectSound buffer completely up 
//-----------------------------------------------------------------------------
HRESULT SoundStructure::FillBuffer()
{
    HRESULT hr;
    VOID*   pbBuffer = NULL;
    DWORD   dwBufferLength;

    VOID* pbBuffer2  = NULL;
    DWORD dwBufferLength2;

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

	DBS(39090);
    // Lock the buffer down, 
    if( FAILED( hr = TheBuffer->Lock( 0, BufferSize, 
                                        &pbBuffer, &dwBufferLength, 
                                        &pbBuffer2, &dwBufferLength2, 0L ) ) )
        return hr;

    // Fill the buffer with wav data 
    if( FAILED( hr = WriteToBuffer( pbBuffer, dwBufferLength ) ) )
        return hr;

    // Fill the buffer with wav data, this is supposed to do nothing
	// since dwBufferLength2 is supposed to be zero;
    if( FAILED( hr = WriteToBuffer( pbBuffer2, dwBufferLength2 ) ) )
        return hr;

	if(dwBufferLength2!=0)
		DBS(6660009);
	DBS(39002);
    // Now unlock the buffer
    TheBuffer->Unlock( pbBuffer, dwBufferLength,pbBuffer2, dwBufferLength2);

	DBS(39091);
    return S_OK;
}



//-----------------------------------------------------------------------------
// Name: WriteMoreWaveData()
// Desc: Writes another piece of data to the buffer
//-----------------------------------------------------------------------------
HRESULT SoundStructure::WriteMoreWaveData() 
{
    HRESULT hr;
    VOID* pbBuffer  = NULL;
    DWORD dwBufferLength;

    VOID* pbBuffer2  = NULL;
    DWORD dwBufferLength2;

	DBS(39000);
	if (TheBuffer==NULL)
		DBS(6660009);
    // Lock the buffer down
    if( FAILED( hr = TheBuffer->Lock( NextWriteOffset, PieceSize, 
                                        &pbBuffer, &dwBufferLength, &pbBuffer2, &dwBufferLength2, 0L ) ) )
        return hr;
	DBS(39001);
    // Fill the buffer with wav data 
    if( FAILED( hr = WriteToBuffer( pbBuffer, dwBufferLength ) ) )
        return hr;
    // Fill the buffer with wav data, this is supposed to do nothing
	// since dwBufferLength2 is supposed to be zero;
    if( FAILED( hr = WriteToBuffer( pbBuffer2, dwBufferLength2 ) ) )
        return hr;

	if(dwBufferLength2!=0)
		DBS(6660007);
	DBS(39002);
    // Now unlock the buffer
    TheBuffer->Unlock( pbBuffer, dwBufferLength,pbBuffer2, dwBufferLength2);
    pbBuffer = NULL;
	DBS(39003);
    NextWriteOffset += PieceSize; 
    NextWriteOffset %= BufferSize; // Circular buffer

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: WriteToBuffer()
// Desc: Writes wave data to the streaming DirectSound buffer 
//-----------------------------------------------------------------------------
HRESULT SoundStructure::WriteToBuffer( VOID* pbBuffer, DWORD dwBufferLength )
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
				DBS(6660008);
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
// Name: UpdateProgress()
// Desc: Checks if more wave data needs to be written and does that if needed.
//-----------------------------------------------------------------------------
HRESULT SoundStructure::UpdateProgress()
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

	if(LOOPING){
		while(Progress>=PieceSize){
			WriteMoreWaveData();
			Progress-=PieceSize;
		}
	}else{
		// While we haven't played a whole piece, fill the space up
		// with new wave data.
		while(Progress>(PieceProgress+1-BufferPieces)*PieceSize && 
			  PieceProgress<Pieces ){
			DBS(31330);
			// Write a piece
			if(FAILED(WriteMoreWaveData())){
				DBS(6660006);
			}
			PieceProgress++;
		}
		DBS(31321);
		if(	Progress>=PieceSize*Pieces  ){
			DBS(31322);
			NotifySourceAndDestroySound();
			DBS(7777777);
		}
	}



	DBS(31301);
    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: RestoreBuffers()
// Desc: Restore lost buffers and fill them up with sound if possible
//-----------------------------------------------------------------------------
HRESULT SoundStructure::RestoreBuffers()
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


void SoundStructure::SetVolume(long Volume){
	DBS(31700);

	if(abs(LastVolume-Volume) >= VOL_MINDIF){
		LastVolume=Volume;
		TheBuffer->SetVolume(Volume);
	}	

	DBS(31701);
}

void SoundStructure::SetPan(long Pan){
	DBS(31702);

	if(abs(LastPan-Pan) >= PAN_MINDIF){
		LastPan=Pan;
		TheBuffer->SetPan(Pan);
	}	
	
	DBS(31703);
}
HRESULT SoundStructure::NotifySourceAndDestroySound(){
	HRESULT hr = DestroySound();
	TheSource->HasStopped();

	return hr;
}

HRESULT SoundStructure::DestroySound(){


	HRESULT hr;

	DBS(31000);	
	hr = StopBuffer();
	DBS(31021);
	SAFE_RELEASE(TheBuffer);
	DBS(31022);
	DestroyWave();
	DBS(31001);	

	return hr;
}


void SoundStructure::UpdatePanAndVolume(){

	

	// Inv: TheBuffer exists
	if(TheBuffer==NULL){
		DBCE("Buffer null");
		return;
	}

	long Volumes[2]={VOL_TOP,VOL_TOP};

	DistanceType TheDistance = TheSource->GetDistance();
	if(TheDistance!=dInfinite){
		// Calculate Volumes
		for(int i=0;i<2;i++){
			Player * pl = ThePlayers[i];
			int Distance=(int)dist(pl->GetActualXPos()-TheSource->GetActualXPos(), pl->GetActualYPos()-TheSource->GetActualYPos() );
			if(Distance<=DIST_MIN)
				Volumes[i]=VOL_TOP;
			else if(Distance>=DIST_MAX)
				Volumes[i]=VOL_MIN;
			else
				// The Volume falls linearly from VOL_TOP to VOL_MIN
				// when going from MIN_DIST to MAX_DIST
				Volumes[i]=VOL_TOP - ( (VOL_TOP-VOL_MIN) * (Distance-DIST_MIN) )/DIST_DIFMAX;
			
		}
	}

	long Volume=max(Volumes[0],Volumes[1]);

	// Don't change the volume unless it has changed significantly
	if(abs(LastVolume-Volume) >= VOL_MINDIF){
		LastVolume=Volume;
		TheBuffer->SetVolume(Volume);
	}



	// If mono, don't bother panning

	if(AL_SS==SS_MONO)
		return;

	// Calculate Pan
	long Pan=(PAN_LEFT*(Volumes[0]-VOL_MIN)+PAN_RIGHT*(Volumes[1]-VOL_MIN))/(VOL_TOP-VOL_MIN);
	
	// Reverse Stereo
	if(AL_SS==SS_RSTEREO)
		Pan=-Pan;

	// Don't change the pan unless it has changed significantly
	if(abs(LastPan-Pan) >= PAN_MINDIF || (Pan!=LastPan && (Pan == PAN_LEFT || Pan == PAN_RIGHT) )){
		LastPan=Pan;
		TheBuffer->SetPan(Pan);
	}
	
	
}