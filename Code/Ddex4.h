/*
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <dinput.h>
#include <dsound.h>
#include <mmreg.h>
#include <commdlg.h>
#include <ddraw.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>

#include "resource.h"
#include "ddutil.h"*/

#include "Debugger.h"

#include "Game.h"
#include "Quadrants.h"
#include "Ufo.h"
#include "AirPlane.h"
#include "SpecialClusterbomb.h"
#include "Base.h"
#include "Player.h"
#include "shot.h"
#include "Animation.h"
#include "Grenade.h"
#include "SoundEngine.h"
#include "LaserBeam.h"
#include "TazerBeam.h"
#include "Weapon.h"
#include "GraphicsEffects.h"
#include "StrConversions.h"
#include "Clone.h"
#include "GraphicMenu.h"
#include "Menu.h"
#include "ActiveMenuItem.h"
#include "TimeKeeper.h"
#include "About.h"
#include "Spawner.h"
#include "GetReady.h"
#include "Intro.h"
#include "Load.h"
#include "Save.h"
#include "Play.h"
#include "PlayerSelecter.h"

#include "Settings.h"
#include "Start.h"
#include "Stats.h"
#include "About.h"
#include "Controls.h"
#include "InputEngine.h"
#include "FireFlame.h"

#include "Functions.h"
#include "MovementObject.h"
#include "staticchar.h"

	// Compile Settings


// Debug Level

#define HIDEBUG



#ifdef HIDEBUG
	#define MEDDEBUG
#endif
#ifdef MEDDEBUG
	#define LODEBUG
#endif


#ifdef LODEBUG
	#define DBUEV(p) {Beacon(p);}
#else
	#define DBUEV(p);
#endif

#ifdef HIDEBUG
	#define DBHI(p) {Beacon(p);}
#else
	#define DBHI(p)
#endif
