// ButtonMenuItem.cpp: implementation of the ButtonMenuItem class.
//
//////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "InputEngine.h"
#include "ButtonMenuItem.h"
#include "Functions.h"
#include "Debugger.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ButtonMenuItem::ButtonMenuItem(int ix, int iy, int w, int h, int ShineMargin, LPDIRECTDRAWSURFACE7 * iSelectedSurface, bf iButtonFunction)
{
	Initialize();
	x=ix;y=iy;
	Trigger.top=iy-ShineMargin;
	Trigger.bottom=iy+h+ShineMargin;
	Trigger.left=ix-ShineMargin;
	Trigger.right=ix+w+ShineMargin;
	setMin(&Trigger.top, 0);
	setMax(&Trigger.bottom, 600);
	setMin(&Trigger.left, 0);
	setMax(&Trigger.right, 800);


	SelectedSurface=iSelectedSurface;

	ButtonFunction=iButtonFunction;
}

ButtonMenuItem::~ButtonMenuItem()
{

}

void ButtonMenuItem::InternalUpdateInput(){
	Beacon(523634);
	if(TrueDown(DIK_RETURN))
		(this->ButtonFunction)();
}

void ButtonMenuItem::InternalBlit(int Selected){
	if(!Selected)
		return;

	while (TRUE)
	{
		HRESULT hRet = TheGame->g_pDDSBack->BltFast(Trigger.left, Trigger.top, (*SelectedSurface),
			&Trigger, DDBLTFAST_SRCCOLORKEY);
		if (hRet == DD_OK)
			break;
		if (hRet == DDERR_SURFACELOST){
			hRet = TheGame->RestoreAll();
			if (hRet != DD_OK)
				break;
		}
		else if (hRet != DDERR_WASSTILLDRAWING)
			break;
	}
	
}
