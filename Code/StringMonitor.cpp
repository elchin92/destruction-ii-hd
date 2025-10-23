#include "Game.h"
#include "StringMonitor.h"
#include "DebugWindow.h"
#include <stdio.h>

StringMonitor::StringMonitor(const char * iname, char * ip, int ix, int iy,int iw){
	DebugMonitor::Initialize();
	p=ip;
	strcpy(name,iname);
	x=ix;y=iy;w=iw;

}

StringMonitor::~StringMonitor(){
	//DebugMonitor::~DebugMonitor();
	if(nextMonitor)
		delete nextMonitor;
}

void StringMonitor::Blit(){


	char output[100];
	sprintf(output,"%s: %s",name,p);
	TheGame->Text(x,y,output);
	if(nextMonitor)
		nextMonitor->Blit();
}