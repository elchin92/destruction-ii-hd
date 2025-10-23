#include "Game.h"
#include "Channel.h"
#include <stdio.h>

Channel::Channel(const char * iname, const char * imsg, int ix, int iy,int iw){
	DebugMonitor::Initialize();
	strcpy(msg,imsg);
	strcpy(name,iname);

	x=ix;y=iy;w=iw;
	//TheGame=theGame;
}

Channel::~Channel(){
	//DebugMonitor::~DebugMonitor();
	if(nextMonitor)
		delete nextMonitor;
}

void Channel::Blit(){

	char output[100];
	sprintf(output,"%s: %s",name,msg);
	if(nextMonitor)
		nextMonitor->Blit();
};