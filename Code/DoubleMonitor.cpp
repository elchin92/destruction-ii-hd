#include "Game.h"
#include "DoubleMonitor.h"
#include "DebugWindow.h"
#include <stdio.h>


DoubleMonitor::DoubleMonitor(char * iname, double * ip, int ix, int iy,int iw){
	DebugMonitor::Initialize();
	p=ip;
	strcpy(name,iname);
	x=ix;y=iy;w=iw;

}

DoubleMonitor::~DoubleMonitor(){
	//DebugMonitor::~DebugMonitor();
	if(nextMonitor)
		delete nextMonitor;
}

void DoubleMonitor::Blit(){
	char output[100];
	sprintf(output,"%s: %d",name,(*p));
	TheGame->Text(x,y,output);
	if(nextMonitor)
		nextMonitor->Blit();
}