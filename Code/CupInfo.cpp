// CupInfo.cpp: implementation of the CupInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "InputEngine.h"
#include "CupInfo.h"
#include "InfoBox.h"
#include "GameKeeper.h"
#include "Player.h"

CupInfo * TheCupInfo;


const int CupHolders[15][2] = {
	{659,318},
	{475,199},
	{475,438},
	{290,135},
	{290,255},
	{290,376},
	{290,496},
	{104,108},
	{104,168},
	{104,228},
	{104,288},
	{104,349},
	{104,409},
	{104,469},
	{104,529}


};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CupInfo::CupInfo(){
	Initialize();

	CupFont=CreateFont(15,
		0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, VARIABLE_PITCH,"Arial");

}

CupInfo::~CupInfo(){
	DeleteObject(CupFont);
}



void CupInfo::Blit(){
	TheGame->BlitOntoBack(&TheGame->g_pDDSBackground);

	const int dist=16;
	HPEN rdPen = CreatePen(PS_SOLID,3,RGB(255,0,0));
	SelectObject(TheGame->hdc,rdPen);
	if (TheGame->g_pDDSBack->GetDC(&TheGame->hdc) == DD_OK){
		SetBkMode(TheGame->hdc, TRANSPARENT);
		SelectObject(TheGame->hdc,CupFont);
		SetTextAlign(TheGame->hdc, TA_CENTER );	
		SetTextColor(TheGame->hdc,  cgs->ContrastAdjust(255, 255, 255));	
		for(int i=0;i<15;i++){
			for(int bot=0;bot<2;bot++){
				TheGame->Text(CupHolders[i][0],CupHolders[i][1]+2+bot*dist,TheGameKeeper->GetBoxName(i,bot));
			}	
		}
		const int lw=47;
		SelectObject(TheGame->hdc,rdPen);
		for(i=0;i<15;i++){
			int w=TheGameKeeper->GetBoxWinner(i);
			if(w){
				w--;
				MoveToEx(TheGame->hdc,CupHolders[i][0]-lw-2,CupHolders[i][1]+26-w*dist,0);
				LineTo(TheGame->hdc,CupHolders[i][0]+lw-1,CupHolders[i][1]+26-w*dist);	
			}
		}
		
	}TheGame->g_pDDSBack->ReleaseDC(TheGame->hdc);
	DeleteObject(rdPen);
}

void CupInfo::Setup(){

}

void CupInfo::SetSurfaces(){
	AddBitmapAndSurface("Cup", &TheGame->g_pDDSBackground);


}

void CupInfo::InternalUpdateInput(){
	if(TrueDown(DIK_ESCAPE)){
		SwitchState(START,TRUE);
		return;
	}
	if(TrueDown(DIK_SPACE)||TrueDown(DIK_RETURN)){
			if(TheGameKeeper->GameOver())
				SwitchState(STATISTICS);
			else
				SwitchState(NEXTROUND);
		return;
	}
}