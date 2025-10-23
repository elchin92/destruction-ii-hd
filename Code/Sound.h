#include <wtypes.h>
#include <dsound.h>

BOOL AppCreateBasicBuffer( 
		LPDIRECTSOUND lpDirectSound, 
		LPDIRECTSOUNDBUFFER *lplpDsb);

BOOL AppWriteDataToBuffer( 
		LPDIRECTSOUNDBUFFER lpDsb,  
		DWORD dwOffset,          
		LPBYTE lpbSoundData,      
		DWORD dwSoundBytes);