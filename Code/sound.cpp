#include <windows.h>
#include <mmreg.h>
#include "SDL2_DirectSoundCompat.h"


BOOL AppCreateBasicBuffer( 
		LPDIRECTSOUND lpDirectSound, 
		LPDIRECTSOUNDBUFFER *lplpDsb) 
{ 
	PCMWAVEFORMAT pcmwf; 
	DSBUFFERDESC dsbdesc; 
	HRESULT hr; 
	// Set up wave format structure. 
	memset(&pcmwf, 0, sizeof(PCMWAVEFORMAT)); 
	pcmwf.wf.wFormatTag = WAVE_FORMAT_PCM; 
	pcmwf.wf.nChannels = 2; 
	pcmwf.wf.nSamplesPerSec = 22050; 
	pcmwf.wf.nBlockAlign = 4; 
	pcmwf.wf.nAvgBytesPerSec = 
		pcmwf.wf.nSamplesPerSec * pcmwf.wf.nBlockAlign; 
	pcmwf.wBitsPerSample = 8; 
	// Set up DSBUFFERDESC structure. 
	memset(&dsbdesc, 0, sizeof(DSBUFFERDESC)); // Zero it out. 
	dsbdesc.dwSize = sizeof(DSBUFFERDESC); 
	// Need default controls (pan, volume, frequency). 
	dsbdesc.dwFlags = DSBCAPS_CTRLVOLUME; 
	// 3-second buffer. 
	dsbdesc.dwBufferBytes = 3 * pcmwf.wf.nAvgBytesPerSec; 
	dsbdesc.lpwfxFormat = (LPWAVEFORMATEX)&pcmwf; 
	// Create buffer. 
	hr = lpDirectSound->CreateSoundBuffer(&dsbdesc, lplpDsb, NULL); 
	if SUCCEEDED(hr) 
	{ 
		// Valid interface is in *lplpDsb. 
		return TRUE; 
	} 
	else 
	{ 
		// Failed. 
		*lplpDsb = NULL; 
		return FALSE; 
	} 
} 

BOOL AppWriteDataToBuffer( 
    LPDIRECTSOUNDBUFFER lpDsb,  // the DirectSound buffer
    DWORD dwOffset,             // our own write cursor
    LPBYTE lpbSoundData,        // start of our data
    DWORD dwSoundBytes)         // size of block to copy
{ 
    LPVOID  lpvPtr1; 
    DWORD   dwBytes1; 
    LPVOID  lpvPtr2; 
    DWORD   dwBytes2; 
    HRESULT hr; 


    hr = (lpDsb)->Lock(dwOffset, dwSoundBytes, &lpvPtr1, 
        &dwBytes1, &lpvPtr2, &dwBytes2, 0); 
 
    // If DSERR_BUFFERLOST is returned, restore and retry lock. 
    if (DSERR_BUFFERLOST == hr) 
    { 
        lpDsb->Restore(); 
        hr = lpDsb->Lock(dwOffset, dwSoundBytes, &lpvPtr1, 
        &dwBytes1, &lpvPtr2, &dwBytes2, 0); 
    } 
    if SUCCEEDED(hr) 
    { 
        // Write to pointers. 
        CopyMemory(lpvPtr1, lpbSoundData, dwBytes1); 
        if (NULL != lpvPtr2) 
        { 
            CopyMemory(lpvPtr2, lpbSoundData+dwBytes1, dwBytes2); 
        } 
        // Release the data back to DirectSound. 
        hr = lpDsb->Unlock(lpvPtr1, dwBytes1, lpvPtr2, 
            dwBytes2); 
        if SUCCEEDED(hr) 
        { 
            // Success. 
            return TRUE; 
        } 
    } 
    // Lock, Unlock, or Restore failed. 
    return FALSE; 
} 
