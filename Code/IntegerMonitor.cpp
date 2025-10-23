#include "Game.h"
#include "IntegerMonitor.h"
#include "DebugWindow.h"
#include <stdio.h>

IntegerMonitor::IntegerMonitor(char * iname, int * ip, int ix, int iy,int iw){
	DebugMonitor::Initialize();
	p=ip;
	strcpy(name,iname);
	x=ix;y=iy;w=iw;
}

IntegerMonitor::~IntegerMonitor(){
	//DebugMonitor::~DebugMonitor();
	if(nextMonitor)
		delete nextMonitor;
}

void IntegerMonitor::Blit(){
	char output[100];
	sprintf(output,"%s: %i",name,(*p));
	TheGame->Text(x,y,output);
	if(nextMonitor)
		nextMonitor->Blit();
}