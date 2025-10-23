// SwitchMenuItem.cpp: implementation of the SwitchMenuItem class.
//
//////////////////////////////////////////////////////////////////////

#include "MenuStyle.h"
#include "Game.h"
#include "SwitchMenuItem.h"
#include "InputEngine.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SwitchMenuItem::SwitchMenuItem(MenuStyle * iTheMenuStyle, const char * iTitle,  gState iNextState, int iSubMenu)
{
	ActiveMenuItem::Initialize();
	TheMenuStyle=iTheMenuStyle; Title=iTitle;
	nextState=iNextState;SubMenu=iSubMenu;
}

SwitchMenuItem::~SwitchMenuItem()
{

}

void SwitchMenuItem::InternalBlit(int Selected){

	TheMenuStyle->SetStyle(Selected);

	TheGame->Text(x,y,Title);

}

void SwitchMenuItem::InternalUpdateInput(){
	if(TrueDown(DIK_RETURN)){
		cgs->SwitchState(nextState,SubMenu);
	}
}
