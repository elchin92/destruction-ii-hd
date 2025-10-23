// SoundEngine.cpp: implementation of the SoundEngine class.
//
//////////////////////////////////////////////////////////////////////
#include "SoundEngine.h"
#include "SoundSource.h"
#include "DebugWindow.h"
#include "Settings.h"
#include "Game.h"
#include "definitions.h"
#include "Debugger.h"

SoundEngine * TheSoundEngine;




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SoundEngine::SoundEngine() 
{
	ACTIVE=FALSE;
	MaxVolume=0;
	TheSounds=NULL;
	TheActors = NULL;

	UZ2();
	Zero(FirstAddedSound,NumberOfSounds);
	Zero2d(PlayingSounds,NumberOfSounds,2);
	SetWaveNames();
//	DMONITOR(SoundsPlaying);
}

SoundEngine::~SoundEngine()
{
	// Make sure FreeDirectSound is run before deleting!!!!

}

void SoundEngine::SetWaveNames(){

	strcpy(WaveNames[ sStart], "StartRound.wav");
	strcpy(WaveNames[ sWallDown], "WallDown.wav");
	strcpy(WaveNames[ sPlantCharge], "PlantCharge.wav");
	strcpy(WaveNames[ s556], "556_Fire.wav");
	strcpy(WaveNames[ s762], "762_Fire.wav");
	strcpy(WaveNames[ sShotgun], "Shotgun.wav");
	strcpy(WaveNames[ s9mm], "9mm_Fire.wav");
	strcpy(WaveNames[ s12], "12_Fire.wav");
	strcpy(WaveNames[ sBazooka], "Bazooka.wav");
	strcpy(WaveNames[ sAirStrike], "AirStrike.wav");
	strcpy(WaveNames[ sTankHit], "TankHit.wav");
	strcpy(WaveNames[ s1UP], "1UP.wav");
	strcpy(WaveNames[ sHealth], "Health.wav");
	strcpy(WaveNames[ sSmallAmmo], "SmallAmmo.wav");
	strcpy(WaveNames[ sMediumAmmo], "MediumAmmo.wav");
	strcpy(WaveNames[ sBigAmmo], "BigAmmo.wav");
	strcpy(WaveNames[ sCash], "Cash.wav");
	strcpy(WaveNames[ sCashHeap], "CashHeap.wav");
	strcpy(WaveNames[ sShield], "Shield.wav");
	strcpy(WaveNames[ sSpeed], "Speed.wav");
	strcpy(WaveNames[ sInvisibility], "Invis.wav");
	strcpy(WaveNames[ sCharge], "Charge.wav");
	strcpy(WaveNames[ sRapidFire], "RapidFire.wav");
	strcpy(WaveNames[ sDoubleDamage], "DoubleDamage.wav");
	strcpy(WaveNames[ sLookingGlass], "LookingGlass.wav");
	strcpy(WaveNames[ sStop], "Stop.wav");
	strcpy(WaveNames[ sSuperman], "Superman.wav");
	strcpy(WaveNames[ sTreasureChest], "TreasureChest.wav");
	strcpy(WaveNames[ sTrinity], "Trinity.wav");
	strcpy(WaveNames[ sX2], "X2.wav");
	strcpy(WaveNames[ sRedLaser], "RedLaser.wav");
	strcpy(WaveNames[ sBlueLaser], "BlueLaser.wav");
	strcpy(WaveNames[ sGreenLaser], "GreenLaser.wav");
	strcpy(WaveNames[ sRocket], "Rocket.wav");
	strcpy(WaveNames[ sShell], "Shell.wav");
	strcpy(WaveNames[ sSmallExplosion], "SmallExplosion.wav");
	strcpy(WaveNames[ sNormalExplosion], "NormalExplosion.wav");
	strcpy(WaveNames[ sBigExplosion], "BigExplosion.wav");
	strcpy(WaveNames[ sMassiveExplosion], "MassiveExplosion.wav");
	strcpy(WaveNames[ sHugeExplosion], "HugeExplosion.wav");
	strcpy(WaveNames[ sBaseKey], "BaseKey.wav");
	strcpy(WaveNames[ sDarkness], "Darkness.wav");
	strcpy(WaveNames[ sClusterBomb], "ClusterBomb.wav");
	strcpy(WaveNames[ sKamikaze], "Kamikaze.wav");
	strcpy(WaveNames[ sMissile], "Missile.wav");
	strcpy(WaveNames[ sPlaceSentryGun], "PlaceSentryGun.wav");
	strcpy(WaveNames[ sTeleport], "Teleport.wav");
	strcpy(WaveNames[ sPacmanDies], "PMDie.wav");
	strcpy(WaveNames[ sHeartBeat], "HeartBeat.wav");
	strcpy(WaveNames[ sSignal], "Signal.wav");
	strcpy(WaveNames[ sRanOver], "RanOver.wav");
	strcpy(WaveNames[ sLaugh], "Laugh.wav");
	strcpy(WaveNames[ sDeath], "Death.wav");
	strcpy(WaveNames[ sGhost], "Ghost.wav");
	strcpy(WaveNames[ sDetPackClick], "DetPackClick.wav");
	strcpy(WaveNames[ sElectric], "Electric.wav");


}
//-----------------------------------------------------------------------------
// Name: InitDirectSound()
// Desc: Initializes DirectSound
//-----------------------------------------------------------------------------
HRESULT SoundEngine::InitDirectSound()
{


    HRESULT             hr;
    LPDIRECTSOUNDBUFFER pDSBPrimary = NULL;

		
	DBS(3001);

    // Initialize COM
    if( hr = CoInitialize(NULL) )
        return hr;
	DBS(3002);
    // Create IDirectSound using the primary sound device
    if( FAILED( hr = DirectSoundCreate( NULL, &lpDS, NULL ) ) )
        return hr;
	DBS(3003);
    // Set coop level to DSSCL_PRIORITY
    if( FAILED( hr = lpDS->SetCooperativeLevel( TheGame->hWnd, DSSCL_PRIORITY  ) ) )
        return hr;
	DBS(3004);
    // Get the primary buffer 
    DSBUFFERDESC        dsbd;
    ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
    dsbd.dwSize        = sizeof(DSBUFFERDESC);
    dsbd.dwFlags       = DSBCAPS_PRIMARYBUFFER;
    dsbd.dwBufferBytes = 0;
    dsbd.lpwfxFormat   = NULL;
    DBS(3005);
    if( FAILED( hr = lpDS->CreateSoundBuffer( &dsbd, &pDSBPrimary, NULL ) ) )
        return hr;

    // Set primary buffer format to 22kHz and 8-bit mono output.
    WAVEFORMATEX wfx;
    ZeroMemory( &wfx, sizeof(WAVEFORMATEX) ); 
    wfx.wFormatTag      = WAVE_FORMAT_PCM; 
    wfx.nChannels       = 1; 
    wfx.nSamplesPerSec  = 22050; 
    wfx.wBitsPerSample  = 8; 
    wfx.nBlockAlign     = wfx.wBitsPerSample / 8 * wfx.nChannels;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
	DBS(3006);
    if( FAILED( hr = pDSBPrimary->SetFormat(&wfx) ) )
        return hr;
	DBS(3007);
    SAFE_RELEASE( pDSBPrimary );
	DBS(3008);

/*	// Figure out how many channels we have to play with
	LPDSCAPS lpDSCaps=NULL;
    if( FAILED( hr = lpDS->GetCaps(lpDSCaps) ) )
		return hr;
	MaxBuffers = lpDSCaps->dwMaxHwMixingStreamingBuffers;
	DBS(3009);
*/
    return S_OK;
}

void SoundEngine::UpdateSound(){

	BOOL PURESOUNDS = TRUE;

	DBS(30100);

	if(!SoundActivated())
		return;

#ifdef TIMEKEEPING
	TheTimeKeeper->EnterArea(taSoundRelated);
#endif

	int sounds=0;
	SoundSource * ss=TheSounds;
	SoundSource * ssPrev=NULL;

	while(ss){

		sounds++;
		DBS(30130);
		ss->UpdateSound();
		DBS(30131);

		SoundSource * Kill=NULL;

		if(ss->DeleteAble()){

			if(ssPrev){
				ssPrev->nextSoundSource=ss->nextSoundSource;
			}else{
				TheSounds=ss->nextSoundSource;
			}
			Kill=ss;
		}

		if(!Kill)
			ssPrev=ss;
		ss=ss->nextSoundSource;
		if(Kill)
			delete Kill;
	}

	DBS(30101);
#ifdef TIMEKEEPING
	TheTimeKeeper->LeaveArea();
#endif
}
void SoundEngine::NormalizePanning(){

	DBS(345345);

	SoundSource * ss = TheSounds;
	while(ss){
		ss->SetCenterPan();
		ss=ss->nextSoundSource;
	}


}

//-----------------------------------------------------------------------------
// Name: FreeDirectSound()
// Desc: Releases DirectSound 
//-----------------------------------------------------------------------------
HRESULT SoundEngine::FreeDirectSound()
{
//	playLoop play->StopPlaying();


    // Release DirectSound interfaces
    //SAFE_RELEASE( DSNotify );

	SoundSource * ss = TheSounds;

	while(ss){
		SoundSource * kill = ss;
		ss=ss->nextSoundSource;
		delete kill;
	}
	ss = TheActors;
	while(ss){
		ss->StopPlaying();
		ss=ss->nextSoundSource;
	}

    SAFE_RELEASE( lpDS ); 

    // Release COM
    CoUninitialize();

	ACTIVE=FALSE;
    return S_OK;
}




void SoundEngine::ActivateSound(){
	if(!ACTIVE){
		InitDirectSound();
	}
	ACTIVE=TRUE;
}
void SoundEngine::DisableSound(){
	if(ACTIVE)
		FreeDirectSound();
	ACTIVE=FALSE;
}
BOOL SoundEngine::SoundActivated(){return ACTIVE;}
void SoundEngine::IncreaseVolume(){
	// No need to change the volume of the buffers since they
	// are automatically updated when UpdateSound() is called

	TheSettings->IncreaseVolume();
	SetVolume();


}
void SoundEngine::DecreaseVolume(){
	// No need to change the volume of the buffers since they
	// are automatically updated when UpdateSound() is called

	TheSettings->DecreaseVolume();
	SetVolume();
}

void SoundEngine::SetVolume(){ 
	MaxVolume=VOL_MIN+(VOL_DIFMAX*TheSettings->GetVolumeControl()) / TheSettings->GetVolumeControlMax();
}
SoundSource * SoundEngine::PlaySound(Sound snd, SoundPriority sPri, BOOL Loop){

	return PlayLocatedSound(snd,0,0,dInfinite,sPri,Loop);
			
}
SoundSource * SoundEngine::PlaySound(BulletType bType, SoundPriority sPri, BOOL Loop){
	return PlaySound(Ammo2Sound[bType], sPri, Loop);
}


SoundSource * SoundEngine::PlayLocatedSound(Sound snd, int x, int y, DistanceType dType,
							SoundPriority sPri, BOOL Loop){
	DBS(30000);
	if(!SoundActivated()||snd==sVoid)
		return NULL;
	if(!AllowedToPlay(snd, sPri))
		return NULL;

#ifdef TIMEKEEPING
	TheTimeKeeper->EnterArea(taSoundRelated);
#endif
	DBS(30020);
	DBC1("SE: PlaySound()");
	DBS(30021);
	SoundSource * newSS = new SoundSource(snd,sPri,x,y,dType,Loop);


	DBS(30022);
	newSS->nextSoundSource=TheSounds;
	TheSounds=newSS;
	if(newSS->StartPlaying())
		AddToThePlayingSounds(newSS);
	DBS(30023);
#ifdef TIMEKEEPING
	TheTimeKeeper->LeaveArea();
#endif
	DBS(30001);
	return newSS;
}

int SoundEngine::AllowedToPlay(Sound snd, SoundPriority sPri){

	// check pri system here

	int AddWhere=!FirstAddedSound[snd];

	if(PlayingSounds[snd][0]&&PlayingSounds[snd][1]){
		AddWhere=FirstAddedSound[snd];
		PlayingSounds[snd][AddWhere]->StopPlaying();
	}else if(PlayingSounds[snd][AddWhere])
		AddWhere=!AddWhere;

	FirstAddedSound[snd]=!AddWhere;

	PlayingSounds[snd][AddWhere]=NULL;

	return TRUE;
}
void SoundEngine::RemoveFromThePlayingSounds(SoundSource * sSource){
	for(int two=0;two<2;two++)
		if(PlayingSounds[sSource->GetSound()][two]==sSource){
			PlayingSounds[sSource->GetSound()][two]=NULL;
			return;
		}

	Beacon(6667772);

}
void SoundEngine::AddToThePlayingSounds(SoundSource * sSource){
	for(int two=0;two<2;two++)
		if(PlayingSounds[sSource->GetSound()][two]==NULL){
			PlayingSounds[sSource->GetSound()][two]=sSource;
			return;
		}

	Beacon(6667771);
}

/*
void SoundEngine::IncreaseSoundNumber(Sound snd){
	PlayingSounds[snd]++;
	if(PlayingSounds[snd]>=2){
		Beacon(6667772);
	}
}

void SoundEngine::DecreaseSoundNumber(Sound snd){
	PlayingSounds[snd]--;
	if(PlayingSounds[snd]<0){
		Beacon(6667771);
	}
}*/
int SoundEngine::GetVolume(){
	return MaxVolume;
}

void SoundEngine::CleanUp(){

	FreeDirectSound();
}