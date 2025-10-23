// OptionMenuItem.cpp: implementation of the OptionMenuItem class.
//
//////////////////////////////////////////////////////////////////////
#include "Game.h"
#include "MenuStyle.h"
#include "OptionMenuItem.h"
#include "OptionMenuItemOption.h"
#include "InputEngine.h"
#include "Debugger.h"
#include <stdio.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OptionMenuItem::OptionMenuItem(MenuStyle * iTheMenuStyle, const char * iTitle)
{
	ActiveMenuItem::Initialize();
	TheMenuStyle=iTheMenuStyle; Title=iTitle;
	CurrentOption=NULL;FirstOption=NULL;LastId=0;
}

OptionMenuItem::~OptionMenuItem()
{
	KillEntireCircularLinkedList(OptionMenuItemOption, CurrentOption, nextOption);
}

void OptionMenuItem::SwitchOption(){
	CurrentOption=CurrentOption->nextOption;
	SetSelection(CurrentOption->GetID());
}
void OptionMenuItem::AddOption(const char * iName, int StandardSelection){
	// Adds option last
	// FirstOption is only used for building the menu

	OptionMenuItemOption * NewOption = new OptionMenuItemOption(iName,LastId++);
	if(FirstOption){
		FirstOption->lastOption->nextOption=NewOption;
		NewOption->nextOption=FirstOption;
		NewOption->lastOption=FirstOption->lastOption;
		FirstOption->lastOption=NewOption;
	}else{
		NewOption->nextOption=FirstOption=NewOption->lastOption=NewOption;
		CurrentOption=FirstOption;
	}

	if(StandardSelection){

		CurrentOption=FirstOption->lastOption;
		SetSelection(CurrentOption->GetID());
		
	}

}
void OptionMenuItem::InternalBlit(int Selected){

	char Output[50];

	TheMenuStyle->SetStyle(Selected);

	
	strcpy(Output,Title);

	if(strcmp(Title,""))
		sprintf(Output, "%s: %s",Title,CurrentOption->GetName());
	else
		strcpy(Output,CurrentOption->GetName());

	TheGame->Text(x,y,&Output[0]);
}

void OptionMenuItem::InternalUpdateInput(){
	Beacon(9271535);
	if(TrueDown(DIK_RETURN)){
		SwitchOption();
	}
}

void OptionMenuItem::SetOptionSelection(int newSelection){
	Beacon(927535);

	while(newSelection!=CurrentOption->GetID()){
			Beacon(7000+newSelection);
			Beacon(7000+CurrentOption->GetID());
		CurrentOption=CurrentOption->nextOption;
			Beacon(927536);
	}
}

int OptionMenuItem::IsOption(){return 1;}