#include "Game.h"
#include "FadingGameState.h"
#include "staticchar.h"
#include "InfoBox.h"
#include "Debugger.h"
#include <stdio.h>
#include <ddraw.h>
#include "ddutil.h"
gState	GameStatus;
GameState * cgs;

LPDIRECTDRAWPALETTE         g_pDDPal;

void GameState::Initialize(){
	Brightness=100;
	SwitchingState=FALSE;
	BitmapToUsePaletteFrom=0;
	NumberOfStateSurfaces=0;
	UserMsgBox=NULL;
	strcpy(StaticMsg, "");
	StaticMsgColor=mcWhite;
	MsgStop=0;
	ReturnState=NoState;

}


GameState::~GameState(){
	if(UserMsgBox)
		SAFE_DELETE(UserMsgBox);
	DeleteObject(MsgFont);
}
COLORREF GameState::ContrastAdjust(int r, int g, int b){
	return RGB(r,g,b);
}

COLORREF GameState::GetWhite(){
	return RGB(255,255,255);
}
COLORREF GameState::GetRed(){
	return RGB(255,0,0);
}
COLORREF GameState::GetGreen(){
	return RGB(0,255,0);
}
COLORREF GameState::GetBlue(){
	return RGB(0,0,255);
}
COLORREF GameState::GetBlack(){
	return RGB(0,0,0);
}
COLORREF GameState::ContrastAdjust(COLORREF rgb){return rgb;}

void GameState::SetupSurfaces(){}

void GameState::SwitchState(gState State, int SubMenu){
	InternalSwitchState(State, SubMenu);

}
void GameState::ForcedSwitchState(gState State, int SubMenu){

	InternalSwitchState(State, SubMenu);
}
void GameState::InternalSwitchState(gState State, int SubMenu){

	if(SubMenu)
		TheGame->GameStates[State]->SetReturnState(GameStatus);

	GameStatus=State;
	cgs=TheGame->GameStates[State];
	cgs->DisableMsg();
	Beacon(333000);
	TheGame->PrepareNewScreen();
	Beacon(333001);
	cgs->LoadBitmaps();
	Beacon(333002);

	if(cgs->IsFading()){
		Beacon(3330021);
		((FadingGameState *)cgs)->InitializeFadeIn();
	} 
	Beacon(333003);
	cgs->SetCorrectPaletteAndGamma();
	Beacon(333004);
	cgs->Setup();
	cgs->SetupSurfaces();
	Beacon(333005);

	SwitchingState=FALSE;

}
void GameState::Update(){
	if(IsFading())
		((FadingGameState *)this)->UpdateFade();
}
void GameState::SetReturnState(gState iReturnState){
	ReturnState=iReturnState;
}


void GameState::UpdateInput(){
	if(!SwitchingState){
	
		DP("UpdateInputState() cgs->UpdateInput() #1");
		InternalUpdateInput();
	}
}
int GameState::IsFading(){return 0;}

void GameState::ReturnToMotherState(){if(ReturnState!=NoState)SwitchState(ReturnState,FALSE);}

HRESULT GameState::LoadBitmaps(){


	HRESULT hRet=DD_OK;
	for(int i=0; i<NumberOfStateSurfaces;i++){
		g_pDDPal = DDLoadPalette(TheGame->g_pDD, StateBitmaps[i]);
		if (g_pDDPal)
			TheGame->g_pDDSPrimary->SetPalette(g_pDDPal);
		TheGame->LoadBitmapToSurface(StateSurfaces[i],StateBitmaps[i]);
		if((*StateSurfaces[i])==NULL){
			char debug[70];
			sprintf(debug, " GameState: %i\n Bitmap: %i", GameStatus, i);
			TheGame->UserMessage("LoadBitmaps() failed", debug);
		}
	}


	return DD_OK;

}
HRESULT GameState::ReLoadBitmaps(){
	for(int i=0; i<NumberOfStateSurfaces;i++){
		HRESULT hRet = (*StateSurfaces[i])->Restore();

		if (hRet == DD_OK)
			DDReLoadBitmap((*StateSurfaces[i]), StateBitmaps[i]);
		else
			return hRet;
		DDSetColorKey((*StateSurfaces[i]), RGB(0, 0, 0));
	}
	return DD_OK;
}

LPDIRECTDRAWSURFACE7 * GameState::GetSurface(int i){
	return StateSurfaces[i];
}

void GameState::SetCorrectPaletteAndGamma(){
	Beacon(5269203);
	SetPaletteAndGamma(Brightness);
}

void GameState::SetPaletteAndGamma(int setBrightness){
	// 100 = Normal brightness

	// Load the palette

	LPDIRECTDRAWPALETTE tPal=NULL;

	if(NumberOfStateSurfaces)
		tPal=DDLoadPalette(TheGame->g_pDD, StateBitmaps[BitmapToUsePaletteFrom]);
	else
		tPal=DDLoadPalette(TheGame->g_pDD, SpritesBitmap);

	if(!tPal){
		Beacon(66616);
		return;
	}
	Beacon(6610000);
	if(setBrightness==100){

		if(g_pDDPal){
			g_pDDPal->Release();
		}
		g_pDDPal=tPal;
		TheGame->g_pDDSPrimary->SetPalette(tPal);
		return;
	}
	Beacon(6620000);

	PALETTEENTRY                pe[256];

    if (tPal->GetEntries(0, 0, 256, pe) != DD_OK){
		tPal->Release();
        return;
	}
	Beacon(6630000);
	if(!g_pDDPal){
		g_pDDPal=tPal;
		TheGame->g_pDDSPrimary->SetPalette(g_pDDPal);
	}else
		tPal->Release();	

    for (int i = 0; i < 256; i++)
    {
        pe[i].peRed   = (BYTE)(( (setBrightness)*(int)(pe[i].peRed)   )/100);
		pe[i].peGreen = (BYTE)(( (setBrightness)*(int)(pe[i].peGreen) )/100);
		pe[i].peBlue  = (BYTE)(( (setBrightness)*(int)(pe[i].peBlue)  )/100);
		//pe[i].peFlags=PC_RESERVED;

    }
	
	Beacon(6640000);
    if (g_pDDPal->SetEntries(0, 0, 256, pe) != DD_OK){
			Beacon(6650000);
      return;
	}

}

void GameState::AddBitmapAndSurface(const char * Bitmap, LPDIRECTDRAWSURFACE7 * Surface){
	StateSurfaces[NumberOfStateSurfaces]=Surface;
	StateBitmaps[NumberOfStateSurfaces++]=Bitmap;
}

int GameState::GetNumberOfSurfaces(){return NumberOfStateSurfaces;}













void GameState::SetStaticMsgColor( COLORREF iColor){
	StaticMsgColor=iColor;
}

void GameState::CreateWideScreenUserMsgBox(){
	MsgFont=CreateFont(20,
		0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, VARIABLE_PITCH,"Arial");

	UserMsgBox = new InfoBox(35,570, 730, 24, &MsgFont, mcWhite,TA_CENTER);

}void GameState::CreateStandardUserMsgBox(){
	MsgFont=CreateFont(24,
		0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, VARIABLE_PITCH,"Arial");

	UserMsgBox = new InfoBox(32,550, 383, 30, &MsgFont, mcWhite,TA_CENTER);

}
void GameState::Message(char * iMsg1, char * iMsg2, COLORREF iColor){
	char oMsg[200];
	sprintf(oMsg,"%s%s",iMsg1,iMsg2);
	Message(oMsg,iColor);
}
void GameState::Message(char * iMsg, COLORREF iColor){
	strcpy(Msg,iMsg);
	iMessage(iColor);
}

void GameState::iMessage(COLORREF iColor){
	UserMsgBox->SetInfoBoxTextColor(iColor);
	MsgStop=TheGame->thisTickCount+5000;
}

void GameState::BlitMessage(){

	if(!UserMsgBox)
		return;

	if(UserMsgBox->BlankAndPrepareBox(0,0)){
		if((TheGame->thisTickCount<MsgStop))
			UserMsgBox->Print(3,3, Msg);
		else if(StaticMsg[0]!='\0'){
			UserMsgBox->SetInfoBoxTextColor(StaticMsgColor);
			UserMsgBox->Print(3,3, StaticMsg);			
		}
	}
	UserMsgBox->CloseBox();
		
}

void GameState::DisableMsg(){MsgStop=0;}