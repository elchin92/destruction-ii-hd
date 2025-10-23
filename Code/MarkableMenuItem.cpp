// MarkableMenuItem.cpp: implementation of the MarkableMenuItem class.
//
//////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "MenuStyle.h"
#include "Menu.h"
#include "InputEngine.h"
#include "MarkableMenuItem.h"
#include "Debugger.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MarkableMenuItem::MarkableMenuItem(Menu * iMotherMenu, const char * iTitle, int ixm, int iym, int iid, LPDIRECTDRAWSURFACE7 * mSurface)
{
	ActiveMenuItem::Initialize();
	MotherMenu=iMotherMenu;
	TheMenuStyle=MotherMenu->GetMenuStyle(); Title=iTitle;
	xm=ixm;ym=iym;id=iid;
	MARKED=FALSE;
	MarkedSurface=mSurface;
	nextMarked=NULL;
}

MarkableMenuItem::~MarkableMenuItem()
{

}
void MarkableMenuItem::InternalBlit(int Selected){

	TheMenuStyle->SetStyle(Selected);
	TheGame->Text(x,y,Title);

	if(MARKED){
		TheGame->g_pDDSBack->ReleaseDC(TheGame->hdc);
		DDSURFACEDESC2          ddsd;
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
		HRESULT hr =(*MarkedSurface)->GetSurfaceDesc(&ddsd);


		TheGame->rcRect.left = 0;
		TheGame->rcRect.top = 0;
		TheGame->rcRect.right = ddsd.dwWidth;
		TheGame->rcRect.bottom = ddsd.dwHeight;

		TheGame->BlitSetRc(x+xm-TheMenuStyle->GetXAdjust(),y+ym,MarkedSurface);
		TheGame->g_pDDSBack->GetDC(&TheGame->hdc);
	}
}

void MarkableMenuItem::InternalUpdateInput(){
	Beacon(2352734);
	if(TrueDown(DIK_RETURN)){
		Beacon(53452534);
		if(MotherMenu->GetMultipleMarks()&&MARKED){
				Beacon(73454646);
			Unmark();
		}else if(!MARKED){
				Beacon(967345);
			Mark();	
		}
	}
}

void MarkableMenuItem::Unmark(){
	MARKED=FALSE;
	RemoveThisFromLinkedList(MarkableMenuItem, MotherMenu->TheMarkableItems, nextMarked);
}
void MarkableMenuItem::Mark(){
	MARKED=TRUE;
	if(!MotherMenu->GetMultipleMarks()&&MotherMenu->TheMarkableItems)
		MotherMenu->TheMarkableItems->Unmark();
	AddThisToLinkedList(MotherMenu->TheMarkableItems, nextMarked);

}
int MarkableMenuItem::GetID(){return id;}

int MarkableMenuItem::IsMarked(){return MARKED;}
