#include "TimeKeeper.h"
#include "DebugWindow.h"

//#define DEBUGGING

#ifdef DEBUGGING
#define DMONITOR(p) dw->AddMonitor("p",&(p)); 
#define DBC1(p) {dw->Broadcast(Channel1,(p));}
#define DBC2(p) {dw->Broadcast(Channel2,(p));}
#define DBC3(p) {dw->Broadcast(Channel3,(p));}
#define DBCE(p) {dw->Broadcast(ErrorChannel,(p));}
extern DebugWindow * dw;
#else
#define DMONITOR(p);
#define DBC1(p);
#define DBC2(p);
#define DBC3(p);
#define DBCE(p);

#endif

//#define BETATESTING


/*#define TheGame->Beacon(p); "#ifdef BETATESTING TheGame->BetaBeacon(p); #endif"
#define Beacon(p); "#ifdef BETATESTING TheGame->BetaBeacon(p); #endif"
#define TheGame->BeaconRect(p); "#ifdef BETATESTING TheGame->BetaBeaconRect(p); #endif"
*/




#ifndef BETATESTING
#define Beacon(p); {}
#define BeaconRect() {}
#define DP(a); {}
#define DP2(a,b); {}
#define CloseBeaconFile();
#else

void Beacon(int Number);
void BeaconRect();
void DP(const char * output);
void DP2(const char * output, int i);
void CloseBeaconFile();
#endif
