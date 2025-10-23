#include "TimeKeeper.h"
#include "Game.h"

TimeKeeper * TheTimeKeeper;

const int sx=0,sy=400,width=300,height=200,border=10,xdist=35,ydist=15, xspace=120;

TimeKeeper::TimeKeeper(){

	for(int a=0;a<NumberOfTimeIntervals;a++){
		for(int b=0;b<NumberOfTimeAreas;b++){
			TimeSpent[a][b][0]=0;
			TimeSpent[a][b][1]=0;
		}
		LastReset[a]=0;
		TimeSwitch[a]=0;
	}
	TickFrame[TKINTERNAL]=0;
	TickFrame[TKEXTERNAL]=0;
	VISIBLE=FALSE;
	ACTIVE=FALSE;

	QueryPerformanceFrequency(&TicksPerSecond);
	if(TicksPerSecond.QuadPart==0)
		TheGame->UserMessage("Error: Hardware missing","Destruction requires a hardware High Performance Counter and cannot find one.");
}
TimeKeeper::~TimeKeeper(){}

void TimeKeeper::ResetAll(){

	SetLastCallToNow(TKINTERNAL);

	for(int a=0;a<NumberOfTimeIntervals;a++){
		LastReset[a]=GetRealTime();
		TimeSwitch[a]=0;
	}
	ACTIVE=TRUE;
#ifdef DEBUGGING

	dw->AddMonitor("ca",&ca);
	dw->AddMonitor("TimeSwitch[LastSec]",&TimeSwitch[LastSec]);

#endif
}


void TimeKeeper::Blit(){
	if(!VISIBLE||!ACTIVE)
		return;

	TheGame->BlankArea(sx,sy,width,height);

	HDC hdc;

	CalculateSums();

	if (TheGame->g_pDDSBack->GetDC(&hdc) == DD_OK){

		SetBkMode(hdc,TRANSPARENT);
		SelectObject(hdc,TheGame->g_hFontSmall);
		SetTextAlign(hdc, TA_LEFT );
		SetTextColor(hdc, cgs->GetBlue());

		int x=sx+border+xspace,y=sy+border;
		for(int a=0;a<NumberOfTimeIntervals;a++,x+=xdist){
			TheGame->Text(x,y,TimeIntervalNames[a]);
		}
		y=sy+border+2*ydist;
		for(int b=0;b<NumberOfTimeAreas;b++, y+=ydist){
			x=sx+border;
			TheGame->Text(x,y,TimeAreaNames[b]);
			x+=xspace;
			for(int a=0;a<NumberOfTimeIntervals;a++,x+=xdist){
				TheGame->IntText(x,y,GetPercentage(a,b));
				TheGame->Text(x+25,y,"%");				
			}
		}
		

	}
	TheGame->g_pDDSBack->ReleaseDC(hdc);	

}

int TimeKeeper::GetPercentage(int ti, int ta){
	return (100*TimeSpent[ti][ta][(!TimeSwitch[ti])*(ti!=AllTime) ])/Sums[ti];
}

void TimeKeeper::SwitchVisible(){VISIBLE=!VISIBLE;}

void TimeKeeper::CalculateSums(){
	for(int a=0;a<NumberOfTimeIntervals;a++){
		Sums[a]=0;
		for(int b=0;b<NumberOfTimeAreas;b++)
			Sums[a]+=TimeSpent[a][b][(!TimeSwitch[a])*(a!=AllTime)];
		if(Sums[a]==0)
			Sums[a]=1;
	}

}

void TimeKeeper::Update(){
	if(!ACTIVE)
		return;
	__int64 CurrentTime = GetRealTime();

	for(int a=0;a<NumberOfTimeIntervals;a++){

		if(CurrentTime-LastReset[a]>TimeIntervals[a]){

			LastReset[a]=CurrentTime;
			TimeSwitch[a]=!TimeSwitch[a];	
			for(int b=0;b<NumberOfTimeAreas;b++){
				TimeSpent[a][b][TimeSwitch[a]]=0;
			}


		}

	}
}

// No beacons in here!!!!!!!!!!!!!!
void TimeKeeper::EnterArea(TimeArea ta){
	if(!ACTIVE)
		return;
	ca++;
	AreaDepth[ca]=ta;
}
// No beacons in here!!!!!!!!!!!!!!

void TimeKeeper::LeaveArea(){
	if(!ACTIVE)
		return;
	if(ca>=0){
		int TimeSpentInArea = GetTimeSinceLastCall(TKINTERNAL);
		for(int a=0;a<NumberOfTimeIntervals;a++){
			TimeSpent[a][AreaDepth[ca]][TimeSwitch[a]]+=TimeSpentInArea;
		}
	}
	ca--;
}

void TimeKeeper::SetLastCallToNow(int Internal){
	QueryPerformanceCounter(&TimeTicks[Internal]);
	TickFrame[Internal]=0;
}
int TimeKeeper::GetTimeSinceLastCall(int Internal){

	LastTimeTicks[Internal]=TimeTicks[Internal];
	QueryPerformanceCounter(&TimeTicks[Internal]);
	__int64 Ticks = TimeTicks[Internal].QuadPart-LastTimeTicks[Internal].QuadPart;

	long ticks             = (long)((1000*Ticks+(__int64)TickFrame[Internal])/TicksPerSecond.QuadPart);	
	TickFrame[Internal]    = (long)((1000*Ticks+(__int64)TickFrame[Internal])%TicksPerSecond.QuadPart);
	return ticks;
}

__int64 TimeKeeper::GetRealTime(){
	QueryPerformanceCounter(&RealTime);
	return (1000*RealTime.QuadPart)/TicksPerSecond.QuadPart;

}