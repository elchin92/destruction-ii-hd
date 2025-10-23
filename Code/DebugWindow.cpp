#include "Game.h"


#include "StringMonitor.h"
#include "IntegerMonitor.h"
#include "DoubleMonitor.h"
#include "Debugger.h"
#include <stdio.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DebugWindow::DebugWindow()
{

	TheMonitors=NULL;
	MonitorCount=0;
	VISIBLE=FALSE;

	x=400;
	y=0;
	width=400;
	height=400;
	borderwidth=6;

	theChannels[(int)ErrorChannel]=new StringMonitor("Error Channel",&(theMessages[ErrorChannel][0]), getNextX(), getNextY(), getMaxWidth());
	sprintf(theMessages[(int)ErrorChannel],"Uninitiated");
	AddMonitor(theChannels[(int)ErrorChannel]);

	theChannels[(int)Channel1]=new StringMonitor("Channel 1",&(theMessages[Channel1][0]), getNextX(), getNextY(), getMaxWidth());
	sprintf(theMessages[(int)Channel1],"Uninitiated");
	AddMonitor(theChannels[(int)Channel1]);

	theChannels[(int)Channel2]=new StringMonitor("Channel 2",&(theMessages[Channel2][0]), getNextX(), getNextY(), getMaxWidth());
	sprintf(theMessages[(int)Channel2],"Uninitiated");
	AddMonitor(theChannels[(int)Channel2]);

	theChannels[(int)Channel3]=new StringMonitor("Channel 3",&(theMessages[Channel3][0]), getNextX(), getNextY(), getMaxWidth());
	sprintf(theMessages[(int)Channel3],"Uninitiated");
	AddMonitor(theChannels[(int)Channel3]);

}

DebugWindow::~DebugWindow()
{
	// Recursive delete
	if(TheMonitors)
		delete TheMonitors;
}

void DebugWindow::Blit(){
	if(!VISIBLE)
		return;

	TheGame->BlankArea(x,y,width,height);

	HDC hdc;
	if (TheGame->g_pDDSBack->GetDC(&hdc) == DD_OK){

		SetBkMode(hdc,TRANSPARENT);
		SelectObject(hdc,TheGame->g_hFont);
		SetTextAlign(hdc, TA_LEFT );
		SetTextColor(hdc, cgs->GetWhite());

		if(TheMonitors)
			TheMonitors->Blit();

	}
	TheGame->g_pDDSBack->ReleaseDC(hdc);	


}

void DebugWindow::AddMonitor(char * name, int * p){
	AddMonitor(new IntegerMonitor(name,p,getNextX(),getNextY(),getMaxWidth()));
}
void DebugWindow::AddMonitor(char * name, char * p){
	AddMonitor(new StringMonitor(name,p,getNextX(),getNextY(),getMaxWidth()));
}
void DebugWindow::AddMonitor(char * name, double * p){
	AddMonitor(new DoubleMonitor(name,p,getNextX(),getNextY(),getMaxWidth()));
}

void DebugWindow::AddMonitor(DebugMonitor * dm){

	if(MonitorCount>MaxMonitors){
		Broadcast(ErrorChannel, "Monitors exceeds max");
		return;
	}

	MonitorCount++;

	dm->nextMonitor=TheMonitors;	
	TheMonitors=dm;

}




int DebugWindow::getNextX(){return x+borderwidth;}
int DebugWindow::getNextY(){return y+borderwidth+MonitorCount*(fh+2);}
int DebugWindow::getMaxWidth(){return width-2*borderwidth;}

void DebugWindow::Broadcast(channel chan, char * msg){
	

	Beacon(2355);
	
	strcpy(theMessages[(int)chan],msg);}

void DebugWindow::Broadcast(channel chan, int msg){
	

	Beacon(2355);
	char intg[100];
	sprintf(intg,"%i",msg);
	strcpy(theMessages[(int)chan],intg);
}
void DebugWindow::Broadcast(channel chan, double msg){
	

	Beacon(2355);
	char dbl[100];
	sprintf(dbl,"%d",msg);
	strcpy(theMessages[(int)chan],dbl);
}
void DebugWindow::Toggle(){
	if(VISIBLE)
		TheGame->BlankArea(x,y,width,height);

	VISIBLE=!VISIBLE;
}