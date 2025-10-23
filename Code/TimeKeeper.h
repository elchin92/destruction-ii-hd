
#ifndef __TimeKeeper__ 
#define __TimeKeeper__

#include <windows.h>

//#define TIMEKEEPING


#define TKEXTERNAL				0
#define TKINTERNAL				1



#define AllTime					0
#define LastSec					1
#define LastFiveSec				2
#define LastMinute				3
#define NumberOfTimeIntervals	4



const int TimeIntervals[NumberOfTimeIntervals]={2147483647, 1000, 5000, 60000};

const char TimeIntervalNames[NumberOfTimeIntervals][30]={"All", "1 s", "5 s", "1 min"};


enum TimeArea{taBlitting, taMoving, taIdleing, taInput, taSoundRelated, taOthers, taBeacon,NumberOfTimeAreas};

const char TimeAreaNames[NumberOfTimeAreas][30]={"Blitting", "Moving", "Idleing", "Waiting For Input", "Sound Related", "Others", "Beacon"};

#define MaxAreaDepth		10


class TimeKeeper
{
private:

	// Timekeeping related

	int VISIBLE, ACTIVE;
	int TimeSpent[NumberOfTimeIntervals][NumberOfTimeAreas][2];
	int Sums[NumberOfTimeIntervals];
	int TimeSwitch[NumberOfTimeIntervals];
	__int64 LastReset[NumberOfTimeIntervals];
	TimeArea	AreaDepth[MaxAreaDepth];
	int ca;

	int GetPercentage(int ti, int ta);

	// Time request related

	long TickFrame[2];
	LARGE_INTEGER	RealTime, TimeTicks[2], LastTimeTicks[2], TicksPerSecond;



public:

	TimeKeeper();
	~TimeKeeper();


	// Timekeeping related

	void Blit();
	void Update();
	void EnterArea(TimeArea ta);
	void LeaveArea();
	void ResetAll();
	void SwitchVisible();
	void CalculateSums();

	// Time request related

	__int64 GetRealTime();
	int GetTimeSinceLastCall(int Internal=TKEXTERNAL);
	void SetLastCallToNow(int Internal=TKEXTERNAL);
};

extern TimeKeeper * TheTimeKeeper;

#endif
