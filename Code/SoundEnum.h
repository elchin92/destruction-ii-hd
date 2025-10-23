#ifndef SOUNDENUM
#define SOUNDENUM

#include "definitions.h"

enum Sound {sStart,sWallDown,sPlantCharge,s556,s762, sShotgun,
			s9mm,s12,sBazooka,sAirStrike,sTankHit, sRocket, sShell,
			s1UP,sHealth,sSmallAmmo,sMediumAmmo, sBigAmmo,sCash,sCashHeap,
			sShield,sSpeed,sInvisibility,sCharge,sRapidFire, sDoubleDamage, 
			sLookingGlass,sStop,sSuperman,sTreasureChest,sTrinity,sX2,
			sRedLaser,sGreenLaser,sBlueLaser,// Gotta be RGB
			sBigExplosion, sNormalExplosion, sSmallExplosion,
			sHugeExplosion, sMassiveExplosion,
			sBaseKey, sDarkness, sClusterBomb, sKamikaze, sMissile,
			sPlaceSentryGun, sTeleport, sDetPackClick,
			sPacmanDies, sHeartBeat, sSignal,
			sRanOver,sLaugh,sDeath,sGhost,sElectric, NumberOfSounds, sVoid};



const Sound Ammo2Sound[NumberOfBulletTypes]={s9mm, s556, s762, s12, sShell, 
				sShell, sRocket, sRocket,sBazooka, sShotgun, sRedLaser,
				sPlantCharge, sCharge, sCharge,
				sVoid, sShell, sRocket, sMissile}; 
 


#define VOL_TOP		(TheSoundEngine->GetVolume())
#define	VOL_MAX		0
#define VOL_MIN		(-10000)
#define VOL_DIFMAX	(VOL_MAX-VOL_MIN)			
#define VOL_MINDIF	VOL_DIFMAX/50			

#define PAN_CENTER	 0
#define PAN_RIGHT	 10000
#define PAN_LEFT	(-10000)
#define PAN_MINDIF	 1000

#endif