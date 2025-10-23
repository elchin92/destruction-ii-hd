// NumericalSliderMenuItem.cpp: implementation of the NumericalSliderMenuItem class.
//
//////////////////////////////////////////////////////////////////////
#include "Game.h"
#include "MenuStyle.h"
#include "NumericalSliderMenuItem.h"
#include <stdio.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NumericalSliderMenuItem::NumericalSliderMenuItem(MenuStyle * iTheMenuStyle, const char * iTitle, int iStartNumber, int iMin, int iMax, int iMulitiplier, const char * unit, int pre){ // C++20: const char*
	SliderMenuItem::Initialize();
	TheMenuStyle=iTheMenuStyle; Title=iTitle;
	Max=iMax;
	Unit=unit;
	Pre=pre;
	InternalSelection=ExternalSelection=iStartNumber;Min=iMin;MaxPointer=&Max; Multiplier=iMulitiplier;

}

NumericalSliderMenuItem::NumericalSliderMenuItem(MenuStyle * iTheMenuStyle, const char * iTitle, int iStartNumber, int iMin, int * iMaxPointer, int iMulitiplier){
	SliderMenuItem::Initialize();
	Pre=0;Unit=NULL;
	TheMenuStyle=iTheMenuStyle; Title=iTitle;
	InternalSelection=ExternalSelection=iStartNumber;Min=iMin;MaxPointer=iMaxPointer; Multiplier=iMulitiplier;
}

NumericalSliderMenuItem::~NumericalSliderMenuItem()
{

}

void NumericalSliderMenuItem::InternalBlit(int Selected){

	char Output[80], append[25];

	TheMenuStyle->SetStyle(Selected);

	strcpy(Output,Title);

	if(strcmp(Title,""))
		strcat(Output,": ");

	if(InternalSelection==0 && UnlimitedZero() ){
		sprintf(append, "Unlimited");
	}else{
		sprintf(append, "%i",InternalSelection);
		if(Unit){
			if(Pre){
				char tappend[25];
				sprintf(tappend,"%s%s",Unit, append);
				strcpy(append,tappend);
			}else
				strcat(append,Unit);
		}else{

		}

		
	}

	strcat(Output, append);

	TheGame->Text(x,y,&Output[0]);

}

