// GraphicalSliderMenuItem.cpp: implementation of the GraphicalSliderMenuItem class.
//
//////////////////////////////////////////////////////////////////////
#include "Game.h"
#include "MenuStyle.h"
#include "GraphicalSliderMenuItem.h"
#include <stdio.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

 GraphicalSliderMenuItem::GraphicalSliderMenuItem(MenuStyle *  iTheMenuStyle, const char * iTitle, int iStartNumber,  int iMax){
	SliderMenuItem::Initialize();
	TheMenuStyle=iTheMenuStyle; Title=iTitle;
	Max=iMax;
	InternalSelection=ExternalSelection=iStartNumber;Min=0;MaxPointer=&Max;

}

GraphicalSliderMenuItem::~GraphicalSliderMenuItem()
{

}

void GraphicalSliderMenuItem::InternalBlit(int Selected){

	char Output[150];

	TheMenuStyle->SetStyle(Selected);

	if(strcmp(Title,"")){
		if(TheMenuStyle->GetMenuAlign()==TA_CENTER)
			sprintf(Output, "%s:                           ", Title);
		else
			sprintf(Output, "%s:", Title);
		TheGame->Text(x,y,&Output[0]);
	}

	TheMenuStyle->SetUnselectedStyle();
	TheMenuStyle->SetTemporaryMenuAlign(TA_LEFT);
	for(int j=0;j<(*MaxPointer);j++)
		Output[j]='I';
	Output[j]='\0';

	TheGame->Text(x+TheMenuStyle->GetSlideAdjust(),y,Output);

	TheMenuStyle->SetSelectedStyle();
	TheMenuStyle->SetTemporaryMenuAlign(TA_LEFT);
	Output[InternalSelection]='\0';
	TheGame->Text(x+TheMenuStyle->GetSlideAdjust(),y,&Output[0]);
}