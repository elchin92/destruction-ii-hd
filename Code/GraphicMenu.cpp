// GraphicMenu.cpp: implementation of the GraphicMenu class.
//
//////////////////////////////////////////////////////////////////////
#include "Game.h"
#include "GraphicMenu.h"
#include "InputEngine.h"
#include "Debugger.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


void GraphicMenu::UpdateInput(){

		if(TrueDown(DIK_UP)&&Choice/Cols)
			Choice-=Cols;
		else if(TrueDown(DIK_DOWN)&&Choice/Cols!=(NumberOfMenuItems-1)/Cols)
			Choice+=Cols;
		else if(TrueDown(DIK_LEFT)&&Choice%Cols)
				Choice--;
		else if(TrueDown(DIK_RIGHT)&&Choice%Cols<=Cols-2)
				Choice++;

		if(Choice<0)
			Choice=0;
		if(Choice>=NumberOfMenuItems)
			Choice=NumberOfMenuItems-1;
}
int GraphicMenu::GetChoice(){return Choice;}
void GraphicMenu::BlitMenu(){



	int Dest=BlitDim[1]*Choice;

	RECT rcRect;
	HRESULT hRet;


	rcRect.left = 0;
	rcRect.top = 0;
	rcRect.right = BackDim[0];
	rcRect.bottom = BackDim[1];

	while (TRUE)
	{
	Beacon(34534);
		hRet = TheGame->g_pDDSBack->BltFast(BackPos[0], BackPos[1], (*BackgroundSurface),
			&rcRect, DDBLTFAST_NOCOLORKEY);
		if (hRet == DD_OK)
			break;
		if (hRet == DDERR_SURFACELOST){
			hRet = TheGame->RestoreAll();
			if (hRet != DD_OK)
				return;
		}
		if (hRet != DDERR_WASSTILLDRAWING)
			return;
	}
	Beacon(34535);
	if(Choice!=-1){
	Beacon(34536);
		rcRect.left = 0;
		rcRect.top = Dest;
		rcRect.right = BlitDim[0];
		rcRect.bottom = BlitDim[1]+Dest;

		while (TRUE)
		{
				Beacon(345398);
			hRet = TheGame->g_pDDSBack->BltFast(MenuPos[0], MenuPos[1]+Dest, (*SelectedSurface),
				&rcRect, DDBLTFAST_NOCOLORKEY);
				Beacon(345399);
			if (hRet == DD_OK)
				break;
			if (hRet == DDERR_SURFACELOST){
				hRet = TheGame->RestoreAll();
				if (hRet != DD_OK)
					return;
			}
			if (hRet != DDERR_WASSTILLDRAWING)
				return;
		}
		Beacon(34537);
	}


}

GraphicMenu::	GraphicMenu(int Columns, int Items,
		LPDIRECTDRAWSURFACE7 * BlitSource, int X, int Y, int Width, int Height,
		LPDIRECTDRAWSURFACE7 * Background, int bX, int bY, int bWidth, int bHeight){

	Choice=-1;
	NumberOfMenuItems=0;

	Cols=Columns;
	NumberOfMenuItems=Items;
	SelectedSurface=BlitSource;
	MenuPos[0]=X;
	MenuPos[1]=Y;
	BlitDim[0]=Width;
	BlitDim[1]=Height;
	BackgroundSurface=Background;
	BackPos[0]=bX;
	BackPos[1]=bY;
	BackDim[0]=bWidth;
	BackDim[1]=bHeight;
}

GraphicMenu::~GraphicMenu()
{

}

