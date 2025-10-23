#include <wtypes.h>
#include "SDL2_DirectSoundCompat.h"

BOOL AppCreateBasicBuffer( 
		LPDIRECTSOUND lpDirectSound, 
		LPDIRECTSOUNDBUFFER *lplpDsb);

BOOL AppWriteDataToBuffer( 
		LPDIRECTSOUNDBUFFER lpDsb,  
		DWORD dwOffset,          
		LPBYTE lpbSoundData,      
		DWORD dwSoundBytes);