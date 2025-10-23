// SelectKeyMenuItem.cpp: implementation of the SelectKeyMenuItem class.
//
//////////////////////////////////////////////////////////////////////
#include "Game.h"
#include "Menu.h"
#include "MenuStyle.h"
#include "SelectKeyMenuItem.h"
#include "Controls.h"
#include "InputEngine.h"
#include "DIK_CODES.h"
#include "Debugger.h"
#include <stdio.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SelectKeyMenuItem::SelectKeyMenuItem(MenuStyle * iTheMenuStyle, const char * iTitle, int iid, int iside)
{
	ActiveMenuItem::Initialize();
	TheMenuStyle=iTheMenuStyle; Title=iTitle;
	BlinkStart=0;
	id=iid;
	side=iside;
	xAdjust=TheMenuStyle->GetSelectKeyAdjust();

}

SelectKeyMenuItem::~SelectKeyMenuItem()
{

}


void SelectKeyMenuItem::InternalBlit(int Selected){

	char Output[50];

	TheMenuStyle->SetStyle(Selected);
	sprintf(Output, "%s:",Title);

	if(!BlinkStart||(TheGame->thisTickCount-BlinkStart)%400>200)
		TheGame->Text(x+TheMenuStyle->GetSelectKeyAdjust(),y,TheControls->GetCurrentSideKeyName(id));


	TheGame->Text(x,y,&Output[0]);
}

void SelectKeyMenuItem::InternalUpdateInput(){
	Beacon(8227505);

	if(BlinkStart){

		int Key=AnyKeyTrueDown();

		if(Key>=0){
			DP2("Keyy",Key);
			DP(DIK2STR(Key));
			if(Key==DIK_ESCAPE){

				AbortCatchKey();
				return;
			}

			if(TheControls->KeyFree(Key,id,side)){
				DP("Key free");

				TheControls->SetKey(id, Key,side);
				AbortCatchKey();

				if(nextActiveMenuItem)
					ReturnNext=TRUE;

			}else{
				DP("Key taken");
			}

		}
	}else{
		if(TrueDown(DIK_RETURN))
			CatchKey();

	}
}

void SelectKeyMenuItem::CatchKey(){
	BlinkStart=TheGame->thisTickCount;
	CatchingEscape=TRUE;
	MoveToNeighbours=FALSE;
}

void SelectKeyMenuItem::AbortCatchKey(){
	BlinkStart=0;
	CatchingEscape=FALSE;
	MoveToNeighbours=TRUE;
}
int SelectKeyMenuItem::GetID(){
	return id;
}