// FullVersionInfo.cpp: implementation of the FullVersionInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "Menu.h"
#include "NewStore.h"
#include "InputEngine.h"
#include "FullVersionInfo.h"
#include "InfoBox.h"
#include "ButtonMenuItem.h"
#include "Debugger.h"
#include <stdio.h>

FullVersionInfo * TheFullVersionInfo;


void OnBuyNow(){
#ifdef __DIGITALRIVERRELEASE__
	TheGame->Quit();
	TheGame->GoToURL("http://www.destruction.nu/buyviadigitalriver.html");
#else
	TheFullVersionInfo->SwitchState(BUYNOW,TRUE);
#endif
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FullVersionInfo::FullVersionInfo(){
	Initialize();

	LockStop=0;

	static HFONT tFont=	CreateFont(25,
						  0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
						  ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						  DEFAULT_QUALITY, VARIABLE_PITCH,"Arial");

	CountDownMsgBox=new InfoBox(200, 463, 400, 30, &tFont, mcRed, TA_CENTER);

	ButtonMenu = new Menu();

	ButtonMenu->AddMenuItem(new ButtonMenuItem(275, 512, 526-275, 581-512, 17, &TheGame->g_pDDSLife, OnBuyNow));

	BackgroundSurface=&TheGame->g_pDDSBackground;

}

FullVersionInfo::~FullVersionInfo(){
 delete CountDownMsgBox;
}



void FullVersionInfo::InternalBlit(){

	if(LockStop){
		
		if(LockStop>=TheGame->thisTickCount){
			CountDownMsgBox->BlankAndPrepareBox(0,0);
			sprintf(tstr, "Delay Time Left: %is",(LockStop-TheGame->thisTickCount)/1000+1);
			CountDownMsgBox->Print(0,6,tstr);
		}
		else{
			CountDownMsgBox->OpenBox();
			SetBkMode(TheGame->hdc, TRANSPARENT);
			CountDownMsgBox->Print(0,0,"Press <escape> to quit or <return> for more information!");
		}
		CountDownMsgBox->CloseBox();
	}
}

void FullVersionInfo::Setup(){
	Beacon(3455);
}

void FullVersionInfo::SetCountDown(){
	LockStop=TheGame->thisTickCount+16000;
}


void FullVersionInfo::SetSurfaces(){
	AddBitmapAndSurface("FullVersion", &TheGame->g_pDDSBackground);
	AddBitmapAndSurface("FullVersionS", &TheGame->g_pDDSLife);

}

void FullVersionInfo::InternalUpdateInput(){
	if(LockStop>=TheGame->thisTickCount)
		return;

	if(TrueDown(DIK_ESCAPE)){

#ifdef __DIGITALRIVERRELEASE__
		TheGame->Quit();
#else
		if(LockStop){
			DP("Quitted through FullVersionInfo");
			TheGame->Quit();
		}else
			ReturnToMotherState();
#endif
		return;
	}

	ButtonMenu->UpdateInput();
		
}