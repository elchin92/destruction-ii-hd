#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif    

//-----------------------------------------------------------------------------
// Include files	
//-----------------------------------------------------------------------------

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <mmreg.h>
#include <commdlg.h>
#include <stdio.h>
#include <stdlib.h>


#include "ddex4.h"

#include <io.h>
#include "resource.h"
#include "ddutil.h"
#include <time.h>
#include <assert.h>
#include <stdarg.h>
#include <ddutil.h>
#include <string.h>
#include <fstream>
#include <math.h>
#include <wingdi.h>

using namespace std; // For ofstream, ifstream, endl

#include "GameState.h"
#include "SoundEnum.h"
#include "SoundPriorityEnum.h"
#include "Debugger.h"
#include "DamageConstants.h"
#include "InteractiveHelp.h"
#include "shellapi.h"
#include "PlayerSetup.h"
#include "GameKeeper.h"
#include "GeneralConstants.h"
#include "LastRoundsInfo.h"
#include "FullVersionInfo.h"
#include "ScoreTableEntry.h"
#include "Aviator.h"

// CRITICAL DEBUG: Global constructor to log BEFORE WinMain
// This helps diagnose if the problem is in C++ initialization or in WinMain
struct GlobalInitLogger {
	GlobalInitLogger() {
		FILE* log = fopen("global_init.log", "w");
		if (log) {
			fprintf(log, "[DEBUG] Global constructors executing BEFORE WinMain\n");
			fflush(log);
			fclose(log);
		}
	}
	~GlobalInitLogger() {
		FILE* log = fopen("global_init.log", "a");
		if (log) {
			fprintf(log, "[DEBUG] Global destructors executing AFTER WinMain\n");
			fflush(log);
			fclose(log);
		}
	}
};
static GlobalInitLogger g_initLogger;

Game * TheGame;

int SETTINGDISPLAYMODE;


//-----------------------------------------------------------------------------
// Name: ReleaseAllObjects()
// Desc: Finished with all objects we use; release them
//-----------------------------------------------------------------------------
void
Game::ReleaseAllObjects(void)
{
    if (g_pDD != 0)
    {
		Beacon(70021);
        if (g_pDDSPrimary != NULL)
        {
            g_pDDSPrimary->Release();
            g_pDDSPrimary = NULL;
        }
		Beacon(70022);
        if (g_pDDSBack != NULL)
        {
			// Already released through Primary Release()
            //g_pDDSBack->Release();
            g_pDDSBack = NULL;
        }
		Beacon(70023);
		ReleaseAllSurfaces();
		Beacon(70024);
        if (g_pDDPal != 0)
        {
            g_pDDPal->Release();
            g_pDDPal = 0;
        }
		Beacon(70025);
        g_pDD->Release();
        g_pDD = 0;
		Beacon(70026);
    }

}




//-----------------------------------------------------------------------------
// Name: InitFail()
// Desc: This function is called if an initialization function fails
//-----------------------------------------------------------------------------
HRESULT
Game::InitFail(HWND hWnd, HRESULT hRet, LPCTSTR szError,...)
{
    char                        szBuff[128];
    va_list                     vl;

    va_start(vl, szError);
    vsprintf(szBuff, szError, vl);
    MessageBox(hWnd, szBuff, "InitFail()", MB_OK);

    va_end(vl);
	Quit();
    return hRet;
}

//-----------------------------------------------------------------------------
// Name: RestoreAll()
// Desc: Restore all lost objects
//-----------------------------------------------------------------------------
HRESULT 
Game::RestoreAll(void)
{

	Beacon(20000+GameStatus);

	g_pDDSPrimary->Restore();

	cgs->ReLoadBitmaps();

//	cgs->Setup();

    return DD_OK;
}

//-----------------------------------------------------------------------------
// Name: UpdateFrame()
// Desc: Decide what needs to be blitted next, wait for flip to complete,
//       then flip the buffers.
//-----------------------------------------------------------------------------
void 
Game::UpdateFrame()
{
	DP2("New Frame", GameStatus);

	UpdateTime();
	TheSoundEngine->UpdateSound();


	Beacon(-4);	    	

	// Begin Destruction

	cgs->Update();
	cgs->Blit();
	cgs->BlitMessage();
	TheInputEngine->UpdateInputState(hWnd);

	Beacon(-5);
    // Flip the surfaces
	
	if(!(ThePlay->GetPlayStatus()==PLAYING&&GameStatus==PLAY&&!PAUSE) && !(ThePlay->GetPlayStatus()==ROUNDOVER&&GameStatus==PLAY&&COOL)){
		DP("Regular flip");
		while (TRUE)
		{
			hRet = g_pDDSPrimary->Flip(0, 0 );
			if (hRet == DD_OK){
				DP("Flip OK!");
				break;
			}
			DP("Flip error!");
			if (hRet == DDERR_SURFACELOST)
			{
				hRet = RestoreAll();
				if (hRet != DD_OK)
					break;
			}
			if (hRet != DDERR_WASSTILLDRAWING)
				break;
		}
	}else{
		DP("Special Flip");
	}

	Beacon(-7);
		
}




//-----------------------------------------------------------------------------
// Name: WindowProc()
// Desc: The Main Window Procedure
//-----------------------------------------------------------------------------
LRESULT CALLBACK  // Modern Windows: LRESULT CALLBACK instead of long FAR PASCAL
WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message==WM_DESTROY){
		Beacon(7000);
		// DirectSound (Sound buffers released through SoundSources)
		TheSoundEngine->CleanUp();
				Beacon(7001);
		// DirectInput
		TheInputEngine->ReleaseInput();
				Beacon(7002);
		// DirectDraw
		TheGame->ReleaseAllObjects();
				Beacon(7003);
		// Game Classes 
		delete TheGame;
		TheGame=NULL;
				Beacon(7004);
		CloseBeaconFile();
	}else if(TheGame->MainWindowSwitch(message, wParam, lParam)==0)
		return 0L;
	
    return DefWindowProc(hWnd, message, wParam, lParam);
}

int Game::MainWindowSwitch(UINT message, WPARAM wParam, LPARAM lParam){

    switch (message)
    { 
		
	case WM_ACTIVATEAPP:
		// Pause if minimized

		//ACTIVE=(BOOL)wParam;

		return 0L;
    case WM_COMMAND:   
    case WM_CREATE:
        break;	
    case WM_KEYUP:
    case WM_KEYDOWN:
        // Handle any non-accelerated key commands
		if(wParam==VK_F12)
			Quit();
        return 0;			
    case WM_SETCURSOR:
        // Turn off the cursor since this is a full-screen app
        SetCursor(0);
        return 0;
	
	case WM_ENDSESSION:
		DP("Recevied endsession request");
		Quit();
		return 0;
    case WM_SETFOCUS:

		Beacon(123);

		SetCursor(0);
		ACTIVE=!FOCUSLOST;
        return 0;
    case WM_KILLFOCUS:

		Beacon(321);

		if(SETTINGDISPLAYMODE){
			UserMessage("SetDisplayMode() failed!","Either your video card doesn't support Destruction II's resolution (800x600x8) or (much more likely) there is too little graphics memory available. Try decreasing Windows screen resolution and color depth and restart the game.");
		}
		//ReleaseAll();
		FOCUSLOST=TRUE;
		ACTIVE=FALSE;
		TheInputEngine->ZeroInput();
		TheInputEngine->UnacquireInput();
        if(GameStatus==PLAY&&ThePlay->GetPlayStatus()==PLAYING&&!PAUSE){
			Pause();
		}

        return 0;
    }
	return 1;
}


//-----------------------------------------------------------------------------
// Name: InitApp()
// Desc: Do work required for every instance of the application:
//          Create the window, initialize data
//-----------------------------------------------------------------------------
static HRESULT
InitApp(HINSTANCE hInstance, int nCmdShow)
{

	return TheGame->InitGame(hInstance, nCmdShow);

}



void Game::BeginNewGame(){
	Beacon(2200);
	Beacon(2201);
	Beacon(2202);
	TheGameKeeper->ResetAll();		
	Beacon(2203);

	//PRACTICE=(StartMenu->GetChoice()==1);
	ZeroStats();
	Beacon(2205);

	TheGameKeeper->GiveAllReward(AL_SM);

#ifdef __D2PROTECTED__
	if(AL_RS!=FREENUMBEROFROUNDS && (
		((RegCode[5]&0x0080)?1:0) != (((((( ((RegCode[0]&1)?1:0)&& ((RegCode[1]&512)?1:0)) && ((RegCode[1]&1)?1:0)) || ((RegCode[2]&1)?1:0)) && ((RegCode[4]&4096)?1:0)) )?1:0)
		||
		((RegCode[5]&0x0020)?1:0) != (((((( ((RegCode[1]&32768)?1:0)&& ((RegCode[1]&8)?1:0)) || ((RegCode[2]&16)?1:0)) || ((RegCode[3]&256)?1:0)) && ((RegCode[4]&2048)?1:0)) )?1:0)
		)){

		Beacon(6665);

		Quit();
	}
#endif

	GAMEACTIVE=TRUE;
	TheStart->SetResumeSurfaces();
}




HRESULT Game::SetGameUp() {
	

	Beacon(40);

	if(STARTNEWROUND){
		STARTNEWROUND=FALSE;
		GAMEDONE=FALSE;
		SUDDENDEATH=FALSE;
		DP2("GAMEDONE",GAMEDONE);
		TheGameKeeper->BeginNewRound();
		BeginNewRound();
		ThePlay->SetPlayStatus(PLAYING);
		DP("Setting playing!");
	}


	TheGameKeeper->ResetAllInput();
//	if(!WASPAUSED)
//		Pause();
	
	Beacon(47);

	hRet=0;
	
	
    // Create the offscreen surface, by loading our bitmap.
	
	
	Beacon(48);
#ifdef __D2PROTECTED__	
	if(TheWorldSize!=WS_SMALL && (
		((RegCode[5]&0x1000)?1:0) != (((((( ((RegCode[0]&32)?1:0)&& ((RegCode[1]&8192)?1:0)) && ((RegCode[1]&64)?1:0)) && ((RegCode[2]&1024)?1:0)) && ((RegCode[4]&4)?1:0)) )?1:0)
		||
		((RegCode[5]&0x0010)?1:0) != (((((( ((RegCode[0]&8192)?1:0)&& ((RegCode[0]&4)?1:0)) && ((RegCode[1]&128)?1:0)) || ((RegCode[3]&8192)?1:0)) && ((RegCode[3]&2)?1:0)) )?1:0)
		)){

		Beacon(6668);

		Quit();
	}
#endif
	
	Beacon(49);
	
	TheTimeKeeper->SetLastCallToNow();
	Beacon(491);
#ifdef TIMEKEEPING
	Beacon(492);
	TheTimeKeeper->ResetAll();
	Beacon(493);
	TheTimeKeeper->LeaveArea();
	Beacon(494);
	TheTimeKeeper->EnterArea(taOthers);
	Beacon(495);
#endif
	
	Beacon(250);
	Beacon(50);
	ThePlay->SetPaletteAndGamma(30 + (70*TheSettings->GetGammaControl()) / TheSettings->GetGammaControlMax());

	UPDATESCREEN=3;

	Beacon(-3);
	
	return DD_OK;
		
}

// Notice: Generate world before adding mortals

void Game::GenerateWorld(int TypeOfWorld,int Size){
	

	WorldSize=Size;
	WorldType=TypeOfWorld;
	
	// Collision Detection

	DeleteTheQuadrants();

	OldWorldSize=WorldSize;

	AllocateNewQuadrants();

	// Zero World
		
	BETWEEN=FALSE;

    FrameTickCount=thisTickCount;
	
	// Generating Worldmap
	
	int x,y;
	
	
	for(y=0;y<WorldSize*11;y++)
		for(x=0;x<WorldSize*11;x++){
			for(int d=1;d<3;d++)
				WorldMap[x][y][d]=0;
			
			int TType=AL_TT;
			if(TType==TT_RANDOM){
				TType=rand()%TT_TYPES+TT_LOWEST;
			}


			if(TType==TT_LIGHT){
				if(rand()%10>=4)
					WorldMap[x][y][0]=0;
				else
					WorldMap[x][y][0]=1;
			}
			else if(TType==TT_MODERATE)
				WorldMap[x][y][0]=rand()%2;
			else if(TType==TT_ROUGH){
				if(rand()%10>=4)
					WorldMap[x][y][0]=1;
				else
					WorldMap[x][y][0]=0;
			}

#ifdef __D2PROTECTED__
			if(Size!=5 && !(
				((HiddenSecurity[5]&0x0001)?1:0) == (((((( ((HiddenSecurity[0]&4)?1:0)&& ((HiddenSecurity[2]&8192)?1:0)) || ((HiddenSecurity[3]&64)?1:0)) && ((HiddenSecurity[3]&8)?1:0)) || ((HiddenSecurity[4]&128)?1:0)) )?1:0) &&
				((HiddenSecurity[5]&0x0002)?1:0) == (((((( ((HiddenSecurity[0]&128)?1:0)&& ((HiddenSecurity[1]&16384)?1:0)) && ((HiddenSecurity[2]&4096)?1:0)) || ((HiddenSecurity[2]&32)?1:0)) || ((HiddenSecurity[3]&16)?1:0)) )?1:0) &&
				((HiddenSecurity[5]&0x0004)?1:0) == ((((( ((HiddenSecurity[1]&1024)?1:0)|| ((HiddenSecurity[2]&1)?1:0)) && ((HiddenSecurity[3]&1024)?1:0)) && ((HiddenSecurity[4]&8)?1:0)) )?1:0)
				)){
				Beacon(6661);
				if(!(rand()%4))
					PseudoQuit();
			}
#endif
			// Static maze structure
			if(!(y%2)&&!(x%2))
				WorldMap[x][y][0]=1;
			if((y%2)&&(x%2))
				WorldMap[x][y][0]=0;
	
		}


	// Border
	for(y=0;y<WorldSize*11;y++){
		WorldMap[0][y][0]=1;
		WorldMap[WorldSize*11-1][y][0]=1;
	}
	for(x=0;x<WorldSize*11;x++){
		WorldMap[x][0][0]=1;
		WorldMap[x][WorldSize*11-1][0]=1;
	}

	// Space for Base (1)
	for(y=0;y<5;y++)
		for(x=0;x<5;x++)
			WorldMap[x][y][0]=0;
		
	// Space for Base (2)
	for(y=WorldSize*11-5;y<WorldSize*11;y++)
		for(x=WorldSize*11-5;x<WorldSize*11;x++)
			WorldMap[x][y][0]=0;

	BOOL top,bottom,left,right;

	int WallLife;
	if(AL_ST==ST_WEAK)
		WallLife=150;
	else if(AL_ST==ST_MODERATE)
		WallLife=400;
	else if(AL_ST==ST_STRONG)
		WallLife=700;
	else if(AL_ST==ST_VSTRONG)
		WallLife=1250;

	int SpriteNum;
	for(y=0;y<WorldSize*11;y++)
		for(x=0;x<WorldSize*11;x++)
			if(WorldMap[x][y][0]){
				WorldMap[x][y][1]=WallLife;
				SpriteNum=0;
				
				top=(y>0&&WorldMap[x][y-1][0]!=0);
				bottom=(y<WorldSize*11-1&&WorldMap[x][y+1][0]!=0);
				left=(x>0&&WorldMap[x-1][y][0]!=0);
				right=(x<WorldSize*11-1&&WorldMap[x+1][y][0]!=0);
				
				SpriteNum+=8*top;
				SpriteNum+=4*bottom;
				SpriteNum+=2*left;
				SpriteNum+=right;
				WorldMap[x][y][0]=SpriteNum+1;
			}
		
		

	int Prob=11-3*AL_IT;

	if(MONEYMAZE){

		for(y=1;y<WorldSize*11-4;y+=2)
			for(x=1;x<WorldSize*11-4;x+=2)
				if(!(rand()%Prob)){
					WorldMap[x][y][2]=GenerateItem(GI_MMAZE);
				}
				else
					WorldMap[x][y][2]=0;
				
	}
	else{

		for(y=1;y<WorldSize*11-4;y+=2)
			for(x=1;x<WorldSize*11-4;x+=2)
				if(!(rand()%Prob)){
					WorldMap[x][y][2]=GenerateItem(GI_REGULAR);
				}
				else
					WorldMap[x][y][2]=0;

				
	}

	TheBases[0]->CreateBase();
	TheBases[1]->CreateBase();

}


//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Initialization, message loop
//-----------------------------------------------------------------------------
int PASCAL
WinMain(HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nCmdShow)
{
	// CRITICAL DEBUG: File logging to diagnose immediate exit
	FILE* startLog = fopen("game_startup.log", "w");
	if (startLog) {
		fprintf(startLog, "[DEBUG] WinMain() START\n");
		fflush(startLog);
		fclose(startLog);
	}

    MSG                         msg;
	HRESULT						hRet;

	startLog = fopen("game_startup.log", "a");
	if (startLog) {
		fprintf(startLog, "[DEBUG] Creating TheGame object...\n");
		fflush(startLog);
		fclose(startLog);
	}

	TheGame = new Game();

	startLog = fopen("game_startup.log", "a");
	if (startLog) {
		fprintf(startLog, "[DEBUG] TheGame created successfully\n");
		fflush(startLog);
		fclose(startLog);
	}

	Beacon(-9876);

	startLog = fopen("game_startup.log", "a");
	if (startLog) {
		fprintf(startLog, "[DEBUG] Calling TheGame->CreateObjects()...\n");
		fflush(startLog);
		fclose(startLog);
	}

	TheGame->CreateObjects();

	startLog = fopen("game_startup.log", "a");
	if (startLog) {
		fprintf(startLog, "[DEBUG] TheGame->CreateObjects() completed\n");
		fflush(startLog);
		fclose(startLog);
	}

//	char tada[100];
//	sprintf(tada,"Whisper min = %i, Whisper max = %i",Distances[(int)dWhisper][0],Distances[(int)dWhisper][1]);
//	MessageBox(TheGame->hWnd," Is it correct? ",tada,0);

	startLog = fopen("game_startup.log", "a");
	if (startLog) {
		fprintf(startLog, "[DEBUG] Calling InitApp()...\n");
		fflush(startLog);
		fclose(startLog);
	}

    if (InitApp(hInstance, nCmdShow) != DD_OK){
		startLog = fopen("game_startup.log", "a");
		if (startLog) {
			fprintf(startLog, "[ERROR] InitApp() FAILED!\n");
			fflush(startLog);
			fclose(startLog);
		}
		TheGame->UserMessage("Error","General InitApp Failure");
        return FALSE;
	}

	startLog = fopen("game_startup.log", "a");
	if (startLog) {
		fprintf(startLog, "[DEBUG] InitApp() SUCCESS - entering main loop\n");
		fflush(startLog);
		fclose(startLog);
	}
	
	while (TRUE)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE))
        {
			//SystemParametersInfo(SPI_SCREENSAVERRUNNING, true, &result, 0);

            if (!GetMessage(&msg, 0, 0, 0))
                return msg.wParam;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

		// CRITICAL: Exit loop if TheGame was deleted (on WM_DESTROY)
		if (!TheGame)
			break;

   		if (TheGame->ACTIVE)
        {
			//bub
            TheGame->UpdateFrame();

		}
        else
        {
			hRet=TheGame->g_pDD->TestCooperativeLevel();
            // Make sure we go to sleep if we have nothing else to do
			if(hRet==DDERR_NOEXCLUSIVEMODE){
				WaitMessage();
				do{
					int bGotMsg = PeekMessage(&msg,NULL,0U,0U,PM_REMOVE);
					if(bGotMsg){
						DispatchMessage(&msg);
						hRet=TheGame->g_pDD->TestCooperativeLevel();
					}
				}while(hRet!=DD_OK);
				TheGame->Focus();

			}else
				WaitMessage();
        }
    }
	
	
}



void Game::Focus(){
	if(FOCUSLOST){
		ACTIVE=TRUE;
		Beacon(6204);

		RestoreAll();	

		while(g_pDDSPrimary->Flip(0, DDFLIP_WAIT)==DDERR_SURFACELOST){

			g_pDDSPrimary->Restore();
			Beacon(6205);
		}


		TheInputEngine->ReacquireInput();

		if(GameStatus==PLAY&&ThePlay->GetPlayStatus()==PLAYING){
			if(PAUSE)
				Pause();
		}
		cgs->SetupSurfaces();
		cgs->SetCorrectPaletteAndGamma();
		TheTimeKeeper->SetLastCallToNow();
		FOCUSLOST=FALSE;

	}
}




void Game::KillAll(){
Beacon(443);
	if(TheAnimations){
		TheAnimations->KillAll();
		delete TheAnimations;
		TheAnimations=NULL;
	}

	if(TheShots){
		TheShots->KillAll();
		delete TheShots;
		TheShots=NULL;
	}

	KillEntireLinkedList(FireFlame, TheFireFlames, nextFireFlame);
	KillEntireLinkedList(Aviator, TheAviators, nextAviator);

	if(TheGrenades){
		TheGrenades->KillAll();
		delete TheGrenades;
		TheGrenades=NULL;
	}
	if(TheLaserBeams){
		TheLaserBeams->KillAll();
		delete TheLaserBeams;
		TheLaserBeams=NULL;
	}

	if(TheTazerBeams){
		TheTazerBeams->KillAll();
		delete TheTazerBeams;
		TheTazerBeams=NULL;
	}

	while(	TheMortals ){
		Mortal * kill =TheMortals;
		TheMortals=TheMortals->nextMortal;
		if(kill->GetMType()!=M_PL){
			if(kill->IsShooter())
				delete (Shooter *)kill;
			else
				delete kill;
		}

	}

	while(	TheDeadMortals ){
		Mortal * kill =TheDeadMortals;
		TheDeadMortals=TheDeadMortals->nextDeadMortal;
		if(kill->GetMType()!=M_PL)
			if(kill->IsShooter())
				delete (Shooter *)kill;
			else
				delete kill;
		else{
			kill->nextDeadMortal=NULL;
		}

	}


	Beacon(445);
	TheShooters=NULL;
	TheDeadMortals=NULL;
	TheClones=NULL;
	TheFireFlames=NULL;


	if(INITED)
		return;

	INITED=TRUE;

#ifdef __D2PROTECTED__
	for(int i = 0; i<6;i++ ){

		HiddenSecurity[6]=0;

		for(int j=0;j<4;j++){
			HiddenSecurity[6]*=16;
			if(RegisterCode[i*5+j]>=48&&RegisterCode[i*5+j]<=57)
				HiddenSecurity[6]+=RegisterCode[i*5+j]-48;
			else
				HiddenSecurity[6]+=RegisterCode[i*5+j]-55;

		}
		HiddenSecurity[i]=HiddenSecurity[6];
	}
#endif

/*
	DP2("Match",((HiddenSecurity[5]&0x0001)?1:0) == (((((( ((HiddenSecurity[0]&4)?1:0)&& ((HiddenSecurity[2]&8192)?1:0)) || ((HiddenSecurity[3]&64)?1:0)) && ((HiddenSecurity[3]&8)?1:0)) || ((HiddenSecurity[4]&128)?1:0)) )?1:0));
	DP2("Match",((HiddenSecurity[5]&0x0002)?1:0) == (((((( ((HiddenSecurity[0]&128)?1:0)&& ((HiddenSecurity[1]&16384)?1:0)) && ((HiddenSecurity[2]&4096)?1:0)) || ((HiddenSecurity[2]&32)?1:0)) || ((HiddenSecurity[3]&16)?1:0)) )?1:0));
	DP2("Match",((HiddenSecurity[5]&0x0004)?1:0) == ((((( ((HiddenSecurity[1]&1024)?1:0)|| ((HiddenSecurity[2]&1)?1:0)) && ((HiddenSecurity[3]&1024)?1:0)) && ((HiddenSecurity[4]&8)?1:0)) )?1:0));
	DP2("Match",((HiddenSecurity[5]&0x0008)?1:0) == ((((((( ((HiddenSecurity[0]&512)?1:0)|| ((HiddenSecurity[1]&16)?1:0)) && ((HiddenSecurity[1]&4)?1:0)) || ((HiddenSecurity[2]&512)?1:0)) || ((HiddenSecurity[3]&128)?1:0)) && ((HiddenSecurity[3]&1)?1:0)) )?1:0));
	DP2("Match",((HiddenSecurity[5]&0x0010)?1:0) == (((((( ((HiddenSecurity[0]&8192)?1:0)&& ((HiddenSecurity[0]&4)?1:0)) && ((HiddenSecurity[1]&128)?1:0)) || ((HiddenSecurity[3]&8192)?1:0)) && ((HiddenSecurity[3]&2)?1:0)) )?1:0));
	DP2("Match",((HiddenSecurity[5]&0x0020)?1:0) == (((((( ((HiddenSecurity[1]&32768)?1:0)&& ((HiddenSecurity[1]&8)?1:0)) || ((HiddenSecurity[2]&16)?1:0)) || ((HiddenSecurity[3]&256)?1:0)) && ((HiddenSecurity[4]&2048)?1:0)) )?1:0));
	DP2("Match",((HiddenSecurity[5]&0x0040)?1:0) == (((((( ((HiddenSecurity[0]&256)?1:0)&& ((HiddenSecurity[1]&4096)?1:0)) && ((HiddenSecurity[3]&4096)?1:0)) || ((HiddenSecurity[3]&2048)?1:0)) && ((HiddenSecurity[4]&8)?1:0)) )?1:0));
	DP2("Match",((HiddenSecurity[5]&0x0080)?1:0) == (((((( ((HiddenSecurity[0]&1)?1:0)&& ((HiddenSecurity[1]&512)?1:0)) && ((HiddenSecurity[1]&1)?1:0)) || ((HiddenSecurity[2]&1)?1:0)) && ((HiddenSecurity[4]&4096)?1:0)) )?1:0));
	DP2("Match",((HiddenSecurity[5]&0x0100)?1:0) == ((((((( ((HiddenSecurity[0]&1024)?1:0)|| ((HiddenSecurity[1]&1024)?1:0)) || ((HiddenSecurity[2]&64)?1:0)) && ((HiddenSecurity[4]&8192)?1:0)) && ((HiddenSecurity[4]&256)?1:0)) && ((HiddenSecurity[4]&2)?1:0)) )?1:0));
	DP2("Match",((HiddenSecurity[5]&0x0200)?1:0) == (((((( ((HiddenSecurity[0]&32768)?1:0)&& ((HiddenSecurity[0]&16384)?1:0)) && ((HiddenSecurity[2]&16384)?1:0)) && ((HiddenSecurity[2]&8)?1:0)) || ((HiddenSecurity[3]&4)?1:0)) )?1:0));
	DP2("Match",((HiddenSecurity[5]&0x0400)?1:0) == ((((((( ((HiddenSecurity[0]&4096)?1:0)&& ((HiddenSecurity[1]&2)?1:0)) && ((HiddenSecurity[2]&128)?1:0)) || ((HiddenSecurity[3]&16384)?1:0)) && ((HiddenSecurity[3]&32)?1:0)) && ((HiddenSecurity[4]&64)?1:0)) )?1:0));
	DP2("Match",((HiddenSecurity[5]&0x0800)?1:0) == ((((((( ((HiddenSecurity[0]&2048)?1:0)&& ((HiddenSecurity[1]&256)?1:0)) || ((HiddenSecurity[2]&2048)?1:0)) && ((HiddenSecurity[2]&2)?1:0)) && ((HiddenSecurity[3]&512)?1:0)) && ((HiddenSecurity[4]&1)?1:0)) )?1:0));
	DP2("Match",((HiddenSecurity[5]&0x1000)?1:0) == (((((( ((HiddenSecurity[0]&32)?1:0)&& ((HiddenSecurity[1]&8192)?1:0)) && ((HiddenSecurity[1]&64)?1:0)) && ((HiddenSecurity[2]&1024)?1:0)) && ((HiddenSecurity[4]&4)?1:0)) )?1:0));
	DP2("Match",((HiddenSecurity[5]&0x2000)?1:0) == ((((((( ((HiddenSecurity[0]&64)?1:0)&& ((HiddenSecurity[1]&32)?1:0)) || ((HiddenSecurity[2]&32768)?1:0)) || ((HiddenSecurity[2]&8)?1:0)) && ((HiddenSecurity[2]&4)?1:0)) && ((HiddenSecurity[3]&32768)?1:0)) )?1:0));
	DP2("Match",((HiddenSecurity[5]&0x4000)?1:0) == ((((( ((HiddenSecurity[0]&2)?1:0)|| ((HiddenSecurity[1]&2048)?1:0)) && ((HiddenSecurity[4]&16384)?1:0)) || ((HiddenSecurity[4]&16)?1:0)) )?1:0));
	DP2("Match",((HiddenSecurity[5]&0x8000)?1:0) == (((((( ((HiddenSecurity[0]&16)?1:0)&& ((HiddenSecurity[1]&8)?1:0)) && ((HiddenSecurity[2]&256)?1:0)) && ((HiddenSecurity[3]&2)?1:0)) || ((HiddenSecurity[4]&32)?1:0)) )?1:0));

*/


	Beacon(446);
}




void Game::AddExplosion(int x, int y){
	if(x<-31||y<-31||x>=(WorldSize*11)*32||y>=(WorldSize*11)*32)
		return;

	NewAnimation(8,400,AN_EXP,x,y,0,96);
}


void Game::DoMagic(){

	Beacon(-1);

	DBHI(71298);

	if(!TheFrame||ThePlay->GetPlayStatus()!=PLAYING||PAUSE)
		return;

	DP("LPlayer");
	DP(ThePlayers[0]->GetName());
	DP("RPlayer");
	DP(ThePlayers[1]->GetName());

	MAGICDONE=TRUE;
	
	if(TIMEOUT){
		DBHI(625345);
		for(int pl=0;pl<2;pl++)
			if(ThePlayers[pl]->Ghost()){
				int HP      = ( 1 * TheFrame + HPFrame[pl]) / Dlay;
				HPFrame[pl] = ( 1 * TheFrame + HPFrame[pl]) % Dlay;
	
				ThePlayers[pl]->Damage(HP, DT_SPECIAL, B_NOBULLET, ThePlayers[pl]);
			}
		//if(RoundsCounter)
		//	MessageBox(TheGame->hWnd," General InitApp Failure ","Error",0);
		if(rand()*3 < ((signed)(TheFrame)*WorldSize) )
			TheSpawnPit->SpawnRandomEnemy();

	}else if( AL_TO !=TO_NEVER ){

		if( StartTime + Timer[0] < thisTickCount &&
		ThePlayers[0]->GetLastDeath() + Timer[1] < thisTickCount && ThePlayers[1]->GetLastDeath() + Timer[1] < thisTickCount){
			TIMEOUT=TRUE;
			DBHI(5153235);
			ThePlayers[0]->SetTimeOut();
			ThePlayers[1]->SetTimeOut();
			TheBases[0]->SetTimeOut();
			TheBases[1]->SetTimeOut();
		}
	}
	
	//bobba

	if(ufoTick<thisTickCount){
	
		if(!(rand()%256)){
			
			if(rand()%5){
				// Ufo & Ghost
				if(rand()%2){
										DP("Ufo created");
					// Ufo
					new Aviator(
						11*32+rand()%(11*32*(TheGame->WorldSize-2)),
						11*32+rand()%(11*32*(TheGame->WorldSize-2)), 
						aUfo);
					AddFX();
				}
				else{

					DP("Ghost created");
					int DeadPlayers=0;
					for(Mortal * DeadOne=TheDeadMortals;DeadOne;DeadOne=DeadOne->nextDeadMortal)
						if(DeadOne->GetMType()==M_PL) DeadPlayers++;

					// Ghost
					if(DeadPlayers){
						int SpawnFrom=rand()%DeadPlayers;
						for(Mortal * DeadOne=TheDeadMortals;DeadOne;DeadOne=DeadOne->nextDeadMortal){
							if(DeadOne->GetMType()==M_PL){
								if(SpawnFrom==0){
									new Aviator(DeadOne->GetX(), DeadOne->GetY(), aGhost);
									AddFX();
									break;
								}
								SpawnFrom--;
							}
						}

						
					}
				}
			}else{
				// Pacman
				ActivatePacman();
			}
		}
		ufoTick=thisTickCount+30000;
	}
	


	DBHI(53262);
    // Decide which frame will be blitted next
	for(int Gunnar=0;Gunnar<2;Gunnar++){
		if(ThePlayers[Gunnar]->ThePlane)
			ThePlayers[Gunnar]->ThePlane->Fire();	
		ThePlayers[Gunnar]->GetCluster()->Fire(TheFrame);

		if(ThePlayers[Gunnar]->ChargeActive()&&ThePlayers[Gunnar]->GetChargeStart()+2000<thisTickCount){
			// Charge Explosion
			int * ChargePos = ThePlayers[Gunnar]->GetChargePos();
			RegExp(ChargePos[0]*32,ChargePos[1]*32,ChDamage,RET_CHARGE, DT_CHARGE, ThePlayers[Gunnar]);
			ThePlayers[Gunnar]->InactivateCharge();
		}
	}
	DBHI(52342);	
	if(TheAnimations){
		TheAnimations->TickUp();
		TheAnimations->KillDead();
	}

	DBHI(75685);
#ifdef TIMEKEEPING
	TheTimeKeeper->EnterArea(taMoving);
#endif
	// Shots move first so that they move away from their firer
	for(shot * Presentshot=TheShots;Presentshot;Presentshot=Presentshot->nextshot)
		Presentshot->Move();

	DBHI(72592);
	for(Aviator * PresentAviator=TheAviators;PresentAviator;PresentAviator=PresentAviator->nextAviator)
		PresentAviator->Move();

	DBHI(92795);
	ThePlayers[0]->Move();
	ThePlayers[1]->Move();


	DBHI(23524);
	Mortal * PresentMortal; // C++20: declare before loops so it's accessible in both
	for(PresentMortal=TheMortals;PresentMortal;PresentMortal=PresentMortal->nextMortal)
		if(PresentMortal->GetMType()!=M_PL)
			PresentMortal->Move();

	DBHI(66274);

	ThePlayers[0]->TakeInputAction();
	ThePlayers[0]->Fire();
	ThePlayers[1]->TakeInputAction();
	ThePlayers[1]->Fire();

	DBHI(66264);

	for(Shooter * PresentShooter=TheShooters;PresentShooter;PresentShooter=PresentShooter->nextShooter)
			PresentShooter->Fire();


	DBHI(83484);

	for(Grenade * PresentGrenade=TheGrenades;PresentGrenade;PresentGrenade=PresentGrenade->nextGrenade)
		PresentGrenade->Move();

	DBHI(74578);
	// Kill shots before deleting the Quadrants
	KillShots();

	DBHI(62346);
//	theUfo->Move();
	for(PresentMortal=TheDeadMortals;PresentMortal;PresentMortal=PresentMortal->nextDeadMortal){
		DBHI(62344);
		PresentMortal->MaybeExplode();
		DBHI(62345);
	}
	DBHI(62347);	
	for(player=0;player!=2;player++){
		apl=ThePlayers[player];

		if(!apl->Frozen()){
			// bobba, bubba
			int Margin = 3;
			if(WorldMap[ (apl->GetActualXPos()-Margin-1)/32 ][ (apl->GetActualYPos()-Margin-1)/32 ][1] <= 0 && 
				WorldMap[ (apl->GetActualXPos()-Margin-1)/32 ][ (apl->GetActualYPos()+Margin+1)/32 ][1] <= 0 &&
				WorldMap[ (apl->GetActualXPos()+Margin+1)/32 ][ (apl->GetActualYPos()-Margin-1)/32 ][1] <= 0 &&
				WorldMap[ (apl->GetActualXPos()+Margin+1)/32 ][ (apl->GetActualYPos()+Margin+1)/32 ][1] <= 0){

				// Inside wall
				if(apl->Ghost())
					apl->GoNormal();
			}else{
				if(apl->GhostActivated())
					apl->GoGhost();
			}

			DBHI(32957);
			apl->UpdateStates();
			apl->StillMineSafe();

			// Security 
#ifdef __D2PROTECTED__
			
			if(FIRSTTO && !(
				((HiddenSecurity[5]&0x0400)?1:0) == ((((((( ((HiddenSecurity[0]&4096)?1:0)&& ((HiddenSecurity[1]&2)?1:0)) && ((HiddenSecurity[2]&128)?1:0)) || ((HiddenSecurity[3]&16384)?1:0)) && ((HiddenSecurity[3]&32)?1:0)) && ((HiddenSecurity[4]&64)?1:0)) )?1:0) &&
				((HiddenSecurity[5]&0x0800)?1:0) == ((((((( ((HiddenSecurity[0]&2048)?1:0)&& ((HiddenSecurity[1]&256)?1:0)) || ((HiddenSecurity[2]&2048)?1:0)) && ((HiddenSecurity[2]&2)?1:0)) && ((HiddenSecurity[3]&512)?1:0)) && ((HiddenSecurity[4]&1)?1:0)) )?1:0)
				)){

				Beacon(6662);

				if(!(rand()%10))
					PseudoQuit();
			}
#endif
			
			DBHI(52348);
		}
		
		TheBases[player]->Update();
		DBHI(82346);
	}

#ifdef TIMEKEEPING
	TheTimeKeeper->LeaveArea();
#endif

	// player turns
	
	
	DBHI(64462);		
	if(ThePlayers[0]->Frozen()&& ThePlayers[0]->FrStart()+5000<thisTickCount){
		RealRevive(0);
	}
	if(ThePlayers[1]->Frozen()&&ThePlayers[1]->FrStart()+5000<thisTickCount){
		RealRevive(1);
	}
	DBHI(87204);

	
	if(GAMEDONE){
		FreezeRound();
	}	

}

void Game::FreezeRound(){
	DBUEV(3455277);

	DP("Freezing Round");

	TIMEOUT=FALSE;
	if(MONEYMAZE){
		LASTMM=TRUE;
		MONEYMAZE=FALSE;
	}

	ThePlay->SetPlayStatus(ROUNDOVER);
	DP("Unsetting GameDone");
	GAMEDONE=FALSE;
	COOL=TRUE;	// Used to copy the currentscreen to a surface
	DBUEV(523395);


}

void Game::FreezeScreen(){


	static char VictoryStr[MNC+7];

	if(!COOL){
		BlitOntoBack(&g_pDDSBackground);

		// Print messages
		if (g_pDDSBack->GetDC(&hdc) == DD_OK)
		{
			

			SelectObject(hdc,g_hFontHuge);
			GetClientRect(hWnd, &rcRect);
			
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, cgs->GetBlack());
			SetTextAlign(hdc, TA_CENTER);
			
			GetTextExtentPoint(hdc, VictoryStr, lstrlen(VictoryStr), &size);
			TextOut(hdc, 405, 260, VictoryStr, lstrlen(VictoryStr));
			
			if(RoundWinner==2)
				SetTextColor(hdc, cgs->GetRed());
			else
				SetTextColor(hdc, cgs->GetBlue());
			
			GetTextExtentPoint(hdc, VictoryStr, lstrlen(VictoryStr), &size);
			TextOut(hdc, 395, 250, VictoryStr, lstrlen(VictoryStr));
			
		}
		g_pDDSBack->ReleaseDC(hdc);
		return;
	}



	Beacon(123123);
			
	if(RoundWinner<2)
		sprintf(VictoryStr,"%s wins!",ThePlayers[RoundWinner]->GetName());	
	else
		sprintf(VictoryStr,"Draw!");

	COOL=FALSE;
	Times[1]+=thisTickCount-StartTime;
	TheGame->LoadBitmapToSurface(&g_pDDSBackground,BackgroundBitmap);  // Use custom function instead of Windows API
	BlitOnto(&g_pDDSBackground,&g_pDDSPrimary);
	
}

void Game::EndRound(){
	// stop tazersounds
	KillAll();
	
	// called once, before we leave the gamestate

	TheLastRounds->EndRound(ThePlayers[0]->GetLives(), ThePlayers[1]->GetLives(), RoundWinner, RoundTime-1);
	// Changes ThePlayers[]
	TheGameKeeper->EndRound(RoundWinner);


	TheSettings->SettingsMenu->ValidateData(LOCK_TAKEEFFECTNEXTROUND);
	TheSettings->EnemySetupMenu->ValidateData(LOCK_TAKEEFFECTNEXTROUND);

}
void Game::EndGame(){}

void Game::RealRevive(int who){
	//DED[who]=FALSE;
	if(ThePlayers[who]->NoLivesLeft()||SUDDENDEATH){
		DP2("Setting Game Done",SUDDENDEATH);
		GAMEDONE=TRUE;
		RoundTime=(int)((TheGame->thisTickCount-StartTime)/1000);
		RoundWinner=!who;
	}
	else if(TheSpawnPit->CreateMortalOK(T_PLAYER, ThePlayers[who]->GetStartX(),ThePlayers[who]->GetStartY())){
		ThePlayers[who]->DoRevive();	
	}
	
}


void Game::Pause(){
	if(PAUSE){
		DP("Unpausing");
		if (g_pDDSBackground != 0)
		{
			g_pDDSBackground->Release();
			g_pDDSBackground = 0;
		}

		while (g_pDDSBackground == 0)
			g_pDDSBackground = DDLoadBitmap(g_pDD, BackgroundBitmap, 0, 0);


		PAUSE=FALSE;

		TheTimeKeeper->SetLastCallToNow();
		//Times[1]+=incr;
		for(int pl=0;pl<2;pl++){
			if(!WASFROZEN[pl])
				ThePlayers[pl]->UnFreeze();
			
			//ThePlayers[pl]->Updateapl->PowerTime(incr);
			ThePlayers[pl]->ResetInput();
		}
		ActivateCheat();
	}
	else{
		DP("Pausing");
		if(!ThePlayers[0]->Frozen()){
			ThePlayers[0]->Freeze(thisTickCount);
			WASFROZEN[0]=FALSE;
		}
		else
			WASFROZEN[0]=TRUE;
		if(!ThePlayers[1]->Frozen()){
			ThePlayers[1]->Freeze(thisTickCount);
			WASFROZEN[1]=FALSE;
		}
		else
			WASFROZEN[1]=TRUE;
		
		PAUSE=TRUE;
		COPYBACK=TRUE;
	}
}


void Game::Quit(){
	Beacon(55555556);
	ClearScreen();
	PostMessage(hWnd,WM_DESTROY,0,0);
}



#ifdef __D2PROTECTED__
void Game::Verification(char Name[50]){

	unsigned short c,d;
	int i;
	long prod=1,sum=0;
	for(i=0;i<50&&Name[i]!='\0'&&Name[i];i++){
		if(Name[i]%2){
			sum+=Name[i]*997;
			(prod*=Name[i]+1)/2;
		}
		else{
			sum+=(Name[i]/2)*29*17;
			prod*=Name[i]/2;
		}
		prod++;

	}

	c=(unsigned short)sum;
	d=(unsigned short)prod;

	if(c!=RegCode[2]||d!=RegCode[3]){
		DeleteFile("Data\\Reg.Key");
		DP("Deleting Reg.Key...");
		Quit();
	}


	return;

}

unsigned short * Game::String2Code(char Code[50]){

	unsigned static short code[6];
	
	for(int i = 0; i<6;i++ ){
		unsigned short temp=0;
		for(int j=0;j<4;j++){
			temp*=16;
			if(Code[i*5+j]>=48&&Code[i*5+j]<=57)
				temp+=Code[i*5+j]-48;
			else if(Code[i*5+j]>=65&&Code[i*5+j]<=70)
				temp+=Code[i*5+j]-55;
			else{
				DP("String2Code() failed");
				Quit();
			}
		}
		code[i]=temp;
	}
	return code;
}

#endif


void Game::CantChangeBeep(){

	Beep(500,200);

	CantChangeStart=thisTickCount;

}	


void Game::ZeroStats(){
	Times[0]=thisTickCount;
	SpecialEvents=0;
	for(int pl=0;pl<2;pl++){

		Times[pl]=0;
	}
}

void Game::AddFX(){
	SpecialEvents++;
}





/*
BOOL Game::Allowed( int XDest, int YDest, int XTest, int YTest, int DestMarg, int TestMarg ){

	RECT Dest, Test;

	Dest.left = XDest+TestMarg;
	Dest.top = YDest+TestMarg;
	Dest.right = XDest+31-TestMarg;
	Dest.bottom = YDest+31-TestMarg;


	Test.left = XTest+DestMarg;
	Test.top = YTest+DestMarg;
	Test.right = XTest+31-DestMarg;
	Test.bottom = YTest+31-DestMarg;


	if(InsideRectangle(Dest,Test.left,Test.top))
		return FALSE;
	if(InsideRectangle(Dest,Test.right,Test.top))
		return FALSE;
	if(InsideRectangle(Dest,Test.left,Test.bottom))
		return FALSE;
	if(InsideRectangle(Dest,Test.right,Test.bottom))
		return FALSE;

	return TRUE;
}
BOOL Game::InsideRectangle(RECT Dest, int X, int Y){
	return (Dest.left<=X&&Dest.right>=X&&Dest.top<=Y&&Dest.bottom>=Y);
	
}*/

int Game::LookForRegistrationInfo(){

	DP("No Data\\Reg.Key file found, looking in clipboard for registration info");
	int  COMPLETE=FALSE;

	HANDLE hText;
	char *pText;

	OpenClipboard(NULL);

	hText = GetClipboardData(CF_TEXT);
	if(hText == NULL) {
		DP("No text in clipboard");
	} else {
		
		pText = (char *)GlobalLock(hText);
		DP("Text found!");
		DP(pText);
		int GotUserName=FALSE,Lines=0;
		ofstream RFile( "Data\\Reg.Key");
		for(int c=0;pText[c]!='\0';c++){

			if(pText[c]=='#'&&pText[c+1]==13){
				Lines=4;
				c++;
				DP("Lines lock");
			}
			if(Lines){
				if(pText[c]==13)
					Lines--;
				if(pText[c]==':'&&pText[c+1]==13){
					DP("Name/code lock!");
					c+=3;
					if(pText[c-1]=='\0'){
						// Fail total, eos
						break;
					}

					// Get code
					for(int a=0;TRUE;a++,c++){
						if(pText[c]=='\0'){
							// failed, end of string
							c--;
							break;
						}
						
						if(pText[c]!=10&&pText[c]!=13){
							RFile << pText[c];
						}
						if(pText[c]==13){
							if(GotUserName){
								RFile << " " ;
								COMPLETE=TRUE;
								DP("Done!");
							}else{
								RFile << endl;
								DP("Got name");
								GotUserName=TRUE;
							}
							break;
						}

					}
				}
			}
		
			
		}DP2("Characters",c);
		RFile.close();
		GlobalUnlock(hText);

		if(!COMPLETE){
			DP("incomplete delete");
			DeleteFile("Data\\Reg.Key");
		}
	}

	CloseClipboard();

	return COMPLETE;
}


int Game::Average(int Array[prlength]){

	long sum=0;
	for(int i=0;i<prlength;i++)
		sum+=Array[i]; 
	return (int)(sum/prlength);
}

void Game::UpdateTime(){

	TheFrame=TheTimeKeeper->GetTimeSinceLastCall();

	if(GameStatus==PLAY&&ThePlay->GetPlayStatus()==PLAYING&&PAUSE){
		TheFrame=0;
		return;
	}

	thisTickCount+=TheFrame;
	DP2("TheFrame",TheFrame);
    DP2("tcdif",(int)(GetTickCount()-thisTickCount));

	if(TheFrame>500){
		DP2("Excess time", TheFrame);
		TheFrame=0;
	}
}



HRESULT Game::InitGame(HINSTANCE hInstance, int nCmdShow){

	Beacon(1);
	// One time reset, not when loading games
#ifdef __D2PROTECTED__
	ISREGISTERED=FALSE;
#endif
	JoyMovement=-1;

	Reset();
	SetVariables();

	Beacon(300);
	

	GetCurrentDirectory(256,&WorkingDir[0]);
//	GetModuleFileName(NULL, &WorkingDir[0],256);
//	SetCurrentDirectory(&WorkingDir[0] );
	CreateDirectory("Data",0);

	TheTimeKeeper->SetLastCallToNow();
	UpdateTime();

	Beacon(301);

	srand((int)TheTimeKeeper->GetRealTime());


	// Variable Initialation
	
	FOCUSLOST=FALSE;
	
	Beacon(302);

	SetSurfaces();

	Beacon(300);



	//-----------------------------------------------------------------------------
	// Direct Draw
	//-----------------------------------------------------------------------------
	g_pDD = 0;        // DirectDraw object
	g_pDDSPrimary = 0;// DirectDraw primary surface
	g_pDDSBack = 0;   // DirectDraw back surface
	g_pDDSSprites = 0;    // The Sprite Surface
	g_pDDSBackground = 0;    // The Background Surface
	g_pDDSLife = 0;    // The Life Surface
	g_pDDSEmptyLife = 0;    // The Empty Life Surface
	g_pDDSTankWarner = 0;    // The TankWarner Surface
	g_pDDSBars = 0;    // The Small Bars Surface
	g_pDDSEmptyBar = 0;    // The Empty Bar Surface
	g_pDDSTemporary = 0;    // Where the maps first are blitted
	g_pDDSDroid = 0;
	g_pDDSTank = 0;
	g_pDDPal = 0;     // The primary surface palette

	//-----------------------------------------------------------------------------
	// Direct Input
	//-----------------------------------------------------------------------------

	INITED = FALSE;

    DDSURFACEDESC2              ddsd;
    DDSCAPS2                    ddscaps;
    WNDCLASS                    wc;
    
    // Set up and register window class
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = (HBRUSH )GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NAME;
    wc.lpszClassName = NAME;
    RegisterClass(&wc);
    // Create a window
    hWnd = CreateWindowEx(WS_EX_TOPMOST,
		NAME,
		TITLE,
		WS_POPUP,
		0,
		0,
		GetSystemMetrics(SM_CXSCREEN),
		GetSystemMetrics(SM_CYSCREEN),
		0,
		0,
		hInstance,
		0);
    if (hWnd==0){
		UserMessage("Error","Could not create window ");
        return FALSE;
	}
	Beacon(2);


    ///////////////////////////////////////////////////////////////////////////
    // Create the main DirectDraw object
    ///////////////////////////////////////////////////////////////////////////
    hRet = DirectDrawCreateEx(0, (VOID**)&g_pDD, IID_IDirectDraw7, 0);
    if (hRet != DD_OK){
		UserMessage("Error","DirectDrawCreateEx() FAILED ");
		return InitFail(hWnd, hRet, "DirectDrawCreateEx FAILED");
	}
	Beacon(3);
   
    // Get exclusive mode
    hRet = g_pDD->SetCooperativeLevel(hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
    if (hRet != DD_OK){
		UserMessage("Error"," Destruction was unable to gain exlusive fullscreen mode.\n Please close all fullscreen applications and try again! ");
        return InitFail(hWnd, hRet, "SetCooperativeLevel FAILED");
	}
	Beacon(4);
	SETTINGDISPLAYMODE=TRUE;
    // Set the video mode to Width x Height x Color
    hRet = g_pDD->SetDisplayMode(800, 600, 8, 0, 0);
    if (hRet != DD_OK){
		UserMessage("Error"," SetDisplayMode() Failed! ");
        return InitFail(hWnd, hRet, "SetDisplayMode FAILED");
	}
	SETTINGDISPLAYMODE=FALSE;
	Beacon(5);
    // Create the primary surface with 1 back buffer
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |
		DDSCAPS_FLIP |
		DDSCAPS_COMPLEX;
    ddsd.dwBackBufferCount = 1;
    hRet = g_pDD->CreateSurface(&ddsd, &g_pDDSPrimary, 0);
    if (hRet != DD_OK){
		UserMessage("Error"," CreateSurface() Failed! ");
        return InitFail(hWnd, hRet, "CreateSurface FAILED");
	}

/*	ZeroMemory(&swcaps, sizeof(swcaps));
    ZeroMemory(&hwcaps, sizeof(hwcaps));
	g_pDD->GetCaps(&hwcaps,&swcaps);
	DD_ROTATION=(hwcaps.dwFXCaps & DDFXCAPS_BLTROTATION90) || 
				(swcaps.dwFXCaps & DDFXCAPS_BLTROTATION90);*/
	Beacon(6);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    SetFocus(hWnd);

	hRet = TheInputEngine->InitializeInput();

	if( FAILED(hRet) )
	{
		UserMessage("Error","InitDirectInput() Failed! ");;
		return InitFail(hWnd, hRet, "InitDirectInput FAILED");
	}



	Beacon(7);
    // Get a pointer to the back buffer
    ZeroMemory(&ddscaps, sizeof(ddscaps));
    ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
    hRet = g_pDDSPrimary->GetAttachedSurface(&ddscaps, &g_pDDSBack);
    if (hRet != DD_OK){
		UserMessage("Error","GetAttachedSurface() Failed!");
        return InitFail(hWnd, hRet, "GetAttachedSurface FAILED");
	}
	Beacon(8);

    // Create and set the palette
    g_pDDPal = DDLoadPalette(g_pDD, SpritesBitmap);

    if (g_pDDPal)
        g_pDDSPrimary->SetPalette(g_pDDPal);
	else{
		UserMessage("Error","DDLoadPalette() Failed");
	}

	Beacon(9);
	printf("[DEBUG] Beacon(9) passed - after SetPalette\n"); fflush(stdout);
#ifdef __D2PROTECTED__
	ifstream RFile( "Data\\Reg.Key", ios::in ); // C++20: removed ios::nocreate, filebuf::openprot

	char Tempo[2][50];

	if(	RFile.is_open() && RFile.getline(Tempo[0],50,'\n') && RFile.getline(Tempo[1],50,'\n') ){
		ISREGISTERED=TRUE;
		sprintf(UserCode,"%s\n",Tempo[1]);
		DP("UserName");
		DP(Tempo[0]);
		RegCode=String2Code(UserCode);
		Verification(Tempo[0]);
		sprintf(UserName,"%s",Tempo[0]);
		Beacon(600);
		// Standard registered options
		TheSettings->GetItem(RS)->SetSelection(0);

	}else{

		if(LookForRegistrationInfo()){
			char mmsg[500], mtitle[100], mname[100];
			int a=0;
			mtitle[a++]='L';
			mtitle[a++]='i';
			mtitle[a++]='c';
			mtitle[a++]='e';
			mtitle[a++]='n';
			mtitle[a++]='s';
			mtitle[a++]='e';
			mtitle[a++]=' ';
			mtitle[a++]='A';
			mtitle[a++]='c';
			mtitle[a++]='c';
			mtitle[a++]='e';
			mtitle[a++]='p';
			mtitle[a++]='t';
			mtitle[a++]='e';
			mtitle[a++]='d';
			mtitle[a++]='!';
			mtitle[a++]='\0';
			a=0;
			mmsg[a++]='T';
			mmsg[a++]='h';
			mmsg[a++]='a';
			mmsg[a++]='n';
			mmsg[a++]='k';
			mmsg[a++]=' ';
			mmsg[a++]='y';
			mmsg[a++]='o';
			mmsg[a++]='u';
			mmsg[a++]=' ';
			mmsg[a++]='f';
			mmsg[a++]='o';
			mmsg[a++]='r';
			mmsg[a++]=' ';
			mmsg[a++]='b';
			mmsg[a++]='u';
			mmsg[a++]='y';
			mmsg[a++]='i';
			mmsg[a++]='n';
			mmsg[a++]='g';
			mmsg[a++]=' ';
			mmsg[a++]='D';
			mmsg[a++]='e';
			mmsg[a++]='s';
			mmsg[a++]='t';
			mmsg[a++]='r';
			mmsg[a++]='u';
			mmsg[a++]='c';
			mmsg[a++]='t';
			mmsg[a++]='i';
			mmsg[a++]='o';
			mmsg[a++]='n';
			mmsg[a++]=' ';
			mmsg[a++]='I';
			mmsg[a++]='I';
			mmsg[a++]='!';
			mmsg[a++]='\n';
			mmsg[a++]='\n';
			mmsg[a++]='T';
			mmsg[a++]='h';
			mmsg[a++]='i';
			mmsg[a++]='s';
			mmsg[a++]=' ';
			mmsg[a++]='c';
			mmsg[a++]='o';
			mmsg[a++]='p';
			mmsg[a++]='y';
			mmsg[a++]=' ';
			mmsg[a++]='i';
			mmsg[a++]='s';
			mmsg[a++]=' ';
			mmsg[a++]='l';
			mmsg[a++]='i';
			mmsg[a++]='c';
			mmsg[a++]='e';
			mmsg[a++]='n';
			mmsg[a++]='s';
			mmsg[a++]='e';
			mmsg[a++]='d';
			mmsg[a++]=' ';
			mmsg[a++]='t';
			mmsg[a++]='o';
			mmsg[a++]=' ';
			mmsg[a++]='\0';

			ifstream RFile( "Data\\Reg.Key", ios::in ); // C++20: removed ios::nocreate, filebuf::openprot
			RFile.getline(mname,50,'\n');
			RFile.close();

			strcat(mmsg,mname);
			strcat(mmsg,".");

			UserMessage(mtitle,mmsg, TRUE);
		}
		
		TheSettings->GetItem(SM)->Lock(LOCK_NEEDTOREGISTER);
		TheSettings->GetItem(WS)->Lock(LOCK_NEEDTOREGISTER);
		TheSettings->GetItem(RS)->Lock(LOCK_NEEDTOREGISTER);
		TheSettings->GetItem(VC)->Lock(LOCK_NEEDTOREGISTER);
		TheSettings->GetItem(GT)->Lock(LOCK_NEEDTOREGISTER);
		for(int a=0;a<eTypes;a++){
			for(int b=0;b<eSettings;b++){
				TheSettings->GetEnemySetting((EnemySettingsType)b,(EnemyType)a)->Lock(LOCK_NEEDTOREGISTER);
			}
		}
		// Lock some other things
		// Set Standard selections
	}


	RFile.close();

#endif
	// Create the fonts
	printf("[DEBUG] About to call CreateFont for g_hFont\n"); fflush(stdout);
	g_hFont = CreateFont(25,
		0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, VARIABLE_PITCH,"Arial");
	printf("[DEBUG] CreateFont returned: %p\n", g_hFont); fflush(stdout);
	if(!g_hFont)
		UserMessage("Error","Destruction requires the Arial font!");
	
	g_hFontSmall = CreateFont(15,
		0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, VARIABLE_PITCH,"Arial");
	g_hFontMedium = CreateFont(36,
		0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, VARIABLE_PITCH,"Arial");
	g_hFontBig = CreateFont(48,
		0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, VARIABLE_PITCH,"Arial");
	g_hFontBigPlus = CreateFont(74,
		0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, VARIABLE_PITCH,"Arial");
	g_hFontHuge = CreateFont(100,
		0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, VARIABLE_PITCH,"Arial");
	
	g_hStatsSub = CreateFont(subSize,
		0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_TT_ALWAYS,
		PROOF_QUALITY, FF_SWISS | TMPF_TRUETYPE | VARIABLE_PITCH,"MS Sans Serif");
	g_hStatsLine = CreateFont(lnSize,
		0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_TT_ALWAYS,
		PROOF_QUALITY, FF_SWISS | TMPF_TRUETYPE | VARIABLE_PITCH,"MS Sans Serif");
	
	/* Allocate memory for a LOGFONT structure. */ 

	PLOGFONT plf = (PLOGFONT) LocalAlloc(LPTR, sizeof(LOGFONT)); 


	/* Specify a font typeface name and weight. */ 

	lstrcpy(plf->lfFaceName, "Arial"); 
	plf->lfWeight = FW_NORMAL; 
	plf->lfHeight = 160;
	plf->lfWidth = 60;
	plf->lfEscapement = 900; 

	g_hFontAngled[0] = CreateFontIndirect(plf); 

	plf->lfEscapement = 2700; 

	g_hFontAngled[1] = CreateFontIndirect(plf); 

	LocalFree((LOCALHANDLE) plf);
	printf("[DEBUG] All CreateFont calls completed\n"); fflush(stdout);

	thePen = CreatePen(PS_SOLID,5,RGB(0,0,200));
	aPen = CreatePen(PS_SOLID,5,RGB(100,0,200));
	cPen = CreatePen(PS_SOLID,5,RGB(0,0,150));
	bPen = CreatePen(PS_SOLID,3,RGB(0,0,255));
	lifePen = CreatePen(PS_SOLID,1,RGB(160,160,100));

	laserPens[0] = 	 redPen = CreatePen(PS_SOLID,1,RGB(255,0,0));
	laserPens[1] = greenPen = CreatePen(PS_SOLID,1,RGB(0,255,0));
	laserPens[2] =  bluePen = CreatePen(PS_SOLID,1,RGB(0,0,255));

	RealBrush.lbStyle=BS_HOLLOW;
	theBrush = CreateBrushIndirect(&RealBrush);		
	RealBrush.lbStyle=BS_SOLID;
	RealBrush.lbColor=RGB(100,0,200);
	aBrush = CreateBrushIndirect(&RealBrush);
	RealBrush.lbColor=RGB(255,0,0);
	lifeBrush = CreateBrushIndirect(&RealBrush);
	printf("[DEBUG] All GDI objects (Pen/Brush) created\n"); fflush(stdout);

#ifdef __D2PROTECTED__
	strcpy(RegisterCode,Tempo[1]);
#endif
	RoundsCounter=0;
	ZeroStats();

	KillAll();

	TheControls->LoadControls();
	TheSettings->LoadSettings();

	Beacon(11);
	printf("[DEBUG] About to call TheIntro->ForcedSwitchState(INTRO)\n"); fflush(stdout);

	//TheFullVersionInfo->ForcedSwitchState(FULLVERSION);


	TheIntro->ForcedSwitchState(INTRO);
	printf("[DEBUG] TheIntro->ForcedSwitchState() completed\n"); fflush(stdout);
	
	//int result;
	//SystemParametersInfo(SPI_SCREENSAVERRUNNING, true, &result, 0);

	char tmp[12];

	ifstream FirstTimeFile("Data\\.started");
	FIRSTTIMESTARTUP=!FirstTimeFile.getline(tmp,10,'\n');
	FirstTimeFile.close();
	if(FIRSTTIMESTARTUP){
		ofstream FTF("Data\\.started");
		FTF << "started";
		FTF.close();	
	}



	Beacon(12);

	return DD_OK;

}




	
int Game::DoAttack(Mortal * Attacker, Mortal * Victim){

	return MortalMatrix[Attacker->GetMType()][Victim->GetMType()];

}
int Game::GetThick(int ThickType){

	return ThickArray[ThickType];

}

class HitRecord{ 
public:
	Mortal * Victim;
	int Damage;
	DamageType dType;
	BulletType bType;
	Mortal * Firerer;
	HitRecord * nextHitRecord;

	HitRecord(Mortal * iVictim, int iDamage, DamageType idType, BulletType ibType, Mortal * iFirerer,HitRecord * inextHitRecord);
	~HitRecord();
};
int hrdl=1, hrcr=1;
HitRecord::HitRecord(Mortal * iVictim, int iDamage, DamageType idType, BulletType ibType, Mortal * iFirerer,HitRecord * inextHitRecord){
	Victim=iVictim;Damage=iDamage;dType=idType;bType=ibType;Firerer=iFirerer;nextHitRecord=inextHitRecord;
	DP2("Creating a hr",hrcr++);
}
HitRecord::~HitRecord(){	DP2("Delting a hr",hrdl++);}
HitRecord * HitList=NULL;

void AddToHitList(Mortal * Victim, int Damage, DamageType dType, BulletType bType, Mortal * Firerer){

	HitRecord * List=HitList;
	while(List){
		if(List->Victim==Victim){
			// Already exists
			if(Damage>List->Damage){
				// Use maximum damage
				List->Damage=Damage;
			}
			return;
		}
		List=List->nextHitRecord;
	}
	HitList = new HitRecord(Victim, Damage, dType, bType, Firerer, HitList);
	
}

void ExecuteAndClearHitList(){
	HitRecord * deleter;
	while(HitList){
		HitList->Victim->Damage(HitList->Damage, HitList->dType, HitList->bType, HitList->Firerer);
		deleter = HitList;
		HitList=HitList->nextHitRecord;
		delete deleter;
	}
}
void Game::SingleExplosion(int X, int Y, int Damage, DamageType dType, Mortal * Firerer){

	DP("SingleExp()");
	int Xs=(X+16)/32, Ys=(Y+16)/32;

	if(Xs<0||Ys<0||Xs>=WorldSize*11||Ys>=WorldSize*11)
		return;

	AddExplosion(X,Y);

	if(WorldMap[Xs][Ys][0]<17){
		WorldMap[Xs][Ys][1]=-1;
		WorldMap[Xs][Ys][2]=0;										
	}
	for(int pl=0;pl<2;pl++){
		ThePlayers[pl]->DestroyItems(Xs,Ys);
	}
	
	MortalQuadLoopXY(Xs,Ys){

		if( Iterator->GetActualXPos() >= X && Iterator->GetActualXPos() < X+32 &&
			Iterator->GetActualYPos() >= Y && Iterator->GetActualYPos() < Y+32
			){
				// Major hit
			 AddToHitList(Iterator, Damage, dType, B_NOBULLET, Firerer);
		}else{
			int Thick = Iterator->GetThickness();	
			if( Iterator->GetActualXPos()+Thick >= X && Iterator->GetActualXPos()-Thick < X+32 &&
				Iterator->GetActualYPos()+Thick >= Y && Iterator->GetActualYPos()-Thick < Y+32
				){
				// Minor hit
				AddToHitList(Iterator, Damage/2, dType, B_NOBULLET, Firerer);
			}
				 

		}
	}
		DP("SingleEnd()");

}

void Game::RegExp(int X,int Y, int Damage, RegExpType rType, DamageType dType, Mortal * Firerer){
	DP("RegExp()");
	Sound snd=sNormalExplosion;
	DistanceType dist=dNormal;	
	SoundPriority sp = spLow;

	if(rType==RET_SPLUS||rType==RET_MPLUS){

		snd=sSmallExplosion;
		SingleExplosion(X,Y,(3*Damage)/2, dType, Firerer);
		SingleExplosion(X+32,Y, Damage, dType, Firerer);
		SingleExplosion(X-32,Y, Damage, dType, Firerer);
		SingleExplosion(X,Y+32, Damage, dType, Firerer);
		SingleExplosion(X,Y-32, Damage, dType, Firerer);

	}else if(rType==RET_SX || rType==RET_MX){

		SingleExplosion(X,Y, (3*Damage)/2, dType, Firerer);
		SingleExplosion(X+32,Y+32, Damage, dType, Firerer);
		SingleExplosion(X+32,Y-32, Damage, dType, Firerer);
		SingleExplosion(X-32,Y+32, Damage, dType, Firerer);
		SingleExplosion(X-32,Y-32, Damage, dType, Firerer);

	}else if(rType==RET_GRENADE||rType==RET_CHARGE||rType==RET_MQUAD||rType==RET_SQUAD){

		sp=spNormal;

		SingleExplosion(X,Y, Damage, dType, Firerer);
		SingleExplosion(X+32,Y, Damage, dType, Firerer);
		SingleExplosion(X-32,Y, Damage, dType, Firerer);
		SingleExplosion(X,Y+32, Damage, dType, Firerer);
		SingleExplosion(X,Y-32, Damage, dType, Firerer);
		SingleExplosion(X+32,Y+32, Damage, dType, Firerer);
		SingleExplosion(X+32,Y-32, Damage, dType, Firerer);
		SingleExplosion(X-32,Y+32, Damage, dType, Firerer);
		SingleExplosion(X-32,Y-32, Damage, dType, Firerer);

	}else if(rType>=RET_RADIUSMIN && rType<=RET_RADIUSMAX){

		snd=sMassiveExplosion;
		sp=spNormal;
		dist=dFar;		

		int Radius=RegExpRadiuses[rType-RET_RADIUSMIN];
		if(Radius>8)
			snd=sHugeExplosion;


		double RadQuad=Radius*Radius;
		for(int x=-Radius;x<=Radius;x++)
			for(int y=-Radius;y<=Radius;y++){
				double DistQuad = y*y+x*x;
				if(DistQuad<RadQuad){

					double Extra=(2.0*(RadQuad - DistQuad))/RadQuad;
					SingleExplosion(X+x*32,Y+y*32,(int)( (double)Damage * (1.0+Extra) ),dType,Firerer);
				}

			}
	}else if(rType==RET_CLUSTER){

		snd=sSmallExplosion;
		if(rand()%100<10){
			snd=sNormalExplosion;
			if(rand()%5>3)
				snd=sBigExplosion;
			sp=spNormal;
			dist=dFar;
		}

		SingleExplosion(X,Y, Damage, dType, Firerer);

	}else if(rType==RET_ENTIREWORLD){
		for(int WX=0;WX<TheGame->WorldSize*11;WX++){
			for(int WY=0;WY<TheGame->WorldSize*11;WY++){
				SingleExplosion(WX*32,WY*32, Damage, dType, Firerer);
			}
		}
	}else{
		if(rType==RET_MORTALEXPLOSIONBIG){
			snd=sBigExplosion;
			sp=spNormal;
			dist=dFar;
		}else if(rType==RET_MORTALEXPLOSIONNORMAL){
			dist=dFar;
		}else if(rType==RET_AIR)
			snd=sVoid;

		SingleExplosion(X,Y, Damage, dType, Firerer);
	}

	ExecuteAndClearHitList();

	TheSoundEngine->PlayLocatedSound(snd,X,Y,dist,sp);
		DP("RegExp() end");
}


int Game::GenerateItem(int GIType){

	int Item,Tempo;
	SumOfProb=0;


	if(GIType==GI_MMAZE){

		SumOfProb=Probability[IT_MINE]+Probability[IT_SDOLLAR]+Probability[IT_DDOLLAR]+Probability[IT_CASHHEAP]+Probability[IT_CHEST];
		Tempo = rand()%SumOfProb;

		if(Tempo<=Probability[IT_MINE])
			Item=IT_MINE;
		else{
			Tempo-=Probability[IT_MINE];
			if(Tempo<=Probability[IT_SDOLLAR])
				Item=IT_SDOLLAR;
			else{
				Tempo-=Probability[IT_SDOLLAR];
				if(Tempo<=Probability[IT_DDOLLAR])
					Item=IT_DDOLLAR;
				else{
					Tempo-=Probability[IT_DDOLLAR];
					if(Tempo<=Probability[IT_CASHHEAP])
						Item=IT_CASHHEAP;
					else{
						Tempo-=Probability[IT_CASHHEAP];
						if(Tempo<=Probability[IT_CHEST])
							Item=IT_CHEST;
						else{
							ErrorReport(100);
						}
					}
				}
			}
		}


	}
	else{
		for(int u=0;u<NumberOfItems;u++)
			SumOfProb+=Probability[u];

		if(GIType==GI_NOMINE)
			SumOfProb-=Probability[IT_MINE];
	

		Tempo = rand()%SumOfProb;
		
		for(Item=0;Item<NumberOfItems;Item++){
			if(Tempo<=Probability[Item])
				break;
			Tempo-=Probability[Item];
		}
	}

	return Item+1;
}
int Game::MoneyMaze(){
	return MONEYMAZE;
}

void Game::UpdateGameInput(){

		if(ThePlay->GetPlayStatus()==PLAYING){
			
/*			if(TheInputEngine->IsInbetween()&&!PAUSE){
				for(int twice=0;twice<2;twice++){
					DP("UpdateInputState() cgs->InternalUpdateInput() Player::UpdateInput()");
					ThePlayers[twice]->UpdateInput();	
				}
				return;
			}
*/
			if(TrueDown(DIK_PAUSE) )
				Pause();

			if(Down(DIK_ADD) ){
				TheSoundEngine->IncreaseVolume();
				
			}
			else if( Down(DIK_SUBTRACT)){
				TheSoundEngine->DecreaseVolume();
			}


			if(PAUSE){
				int key=AnyKeyTrueDown();	
				if(key!=-1){
					DP("Found key");

					for(int u=0;u<NoOfCheats;u++){
						if(key==CheatKeys[u][CheatLevel[u]++]){
							DP2("Cheat",u);
							DP2("CheatLevel[u]",CheatLevel[u]);
							if(CheatKeys[u][CheatLevel[u]]==-1){
								TheSoundEngine->PlaySound(sTreasureChest,spHigh);
								DP2("Cheat locked",u);
								CheatActive=u+1;
								CheatLevel[u]=0;
							}				
						}else{
							CheatLevel[u]=0;
						}
					}


				}

				
			}else{
			
			

			#ifdef BETATESTING

				if(TrueUp(DIK_L))
					ActivateBoom();

				if(FALSE){
			if(TrueUp(DIK_W)){
				WorldType++;
				if(WorldType==8)
					WorldType=0;
			}
			if(TrueUp(DIK_T)){
				TheSpawnPit->CreateMortal(32, 32*7, T_TANK+4, 2);
			}
			if(TrueUp(DIK_D)){
//				TheSpawnPit->CreateMortal(32, 32*7, T_DROID+rand()%Units[M_DR], 2);
			}
			if(TrueUp(DIK_I)){
				TheSpawnPit->CreateMortal(32, 32*7, T_PACMAN, 2);
			}
			if(TrueUp(DIK_M)){
				TheSpawnPit->CreateMortal(32, 32*7, T_MACHINE+rand()%Units[M_MA], 2);
			}			
			if(TrueUp(DIK_C)){
				TheSpawnPit->CreateMortal(32, 32*7, T_CAR+rand()%Units[M_CA], 2);
			}
			if(TrueUp(DIK_O)){
				DBC1("Input Alive!");
				Beacon(33223322);
			}
#ifdef DEBUGGING
			if(TrueUp(DIK_HOME))
				dw->Toggle();
#endif
#ifdef TIMEKEEPING
			if(TrueUp(DIK_END))
				TheTimeKeeper->SwitchVisible();
#endif
			// Beta testing keys

			if(TrueUp(DIK_X)){
				RegExp(ThePlayers[0]->GetX(),ThePlayers[0]->GetY(),10,RET_CHUGE,DT_SPECIAL,NULL);
			}
			if(TrueUp(DIK_L)){
				ThePlayers[0]->LockCamera(10000);
			}
			if(TrueUp(DIK_K)){
				new Aviator(100, 100, (AviatorType)(rand()%2));
			}

			if(TrueUp(DIK_F5)){
				DP("F5 pressed");
			}
				}
			#endif // Beta testin


				for(int twice=0;twice<2;twice++){
					DP("UpdateInputState() cgs->InternalUpdateInput() Player::UpdateInput()");
					ThePlayers[twice]->UpdateInput();	
				}
			}
		if(TrueDown(DIK_ESCAPE)){
		 			 
			if(!ESCSUPRESSED){

				ThePlay->SwitchState(START, TRUE);
				return;
			}
			ESCSUPRESSED=FALSE;
		}
		}else if(ThePlay->GetPlayStatus()==ROUNDOVER){
			if(TrueDown(DIK_SPACE)||TrueDown(DIK_RETURN)||TrueDown(DIK_ESCAPE)){
				// Sto that the tazersound shall stop correctly
				// We no longer have need for any actors
				EndRound();
				ThePlay->SwitchState(LASTROUNDS);
			}
		}


		
}




void Game::ErrorReport(int Error){

	ofstream File("Error.report");

	char Name[40];
	struct tm *newtime;
	time_t long_time;
	
	time( &long_time );                /* Get time as long integer. */
	newtime = localtime( &long_time ); /* Convert to local time. */
	
	sprintf(Name, "%.19s", asctime( newtime ));

	File << "Error " << Error << " at " << Name;
	File << endl << "Version: " << "1.00";
	File.close();

	char estr[200],etitle[30];
	sprintf(estr, "An unrecoverable error occured. Please send the file Error.report to bugreport@destruction.nu. It can be found in %s.", WorkingDir);
	sprintf(etitle,"Game Error %i",Error);
	UserMessage(etitle, estr);	


}

void Game::SetActivePlayers(){
	Beacon(111);

	DP("Left Player");
	DP(ThePlayers[0]->GetName());
	DP("Right Player");
	DP(ThePlayers[1]->GetName());

	TheControls->SetControls(ThePlayers[0],ThePlayers[1]);

	TheInputEngine->SetDigital();

	Beacon(TheGameKeeper->GetNumberOfPlayers()+1000);

	TheBases[0]->SetOwner(ThePlayers[0]);
	TheBases[1]->SetOwner(ThePlayers[1]);

	TheBases[0]->SetOpponent(ThePlayers[1]);
	TheBases[1]->SetOpponent(ThePlayers[0]);

	ThePlayers[0]->SetOpBase(TheBases[1]);
	ThePlayers[1]->SetOpBase(TheBases[0]);

	ThePlayers[0]->SetOpponent(ThePlayers[1]);
	ThePlayers[1]->SetOpponent(ThePlayers[0]);

	ThePlayers[0]->SetTeam(0);
	ThePlayers[1]->SetTeam(1);


}
void Game::InterpretSettings(){


	Beacon(23444);
	StartingMoney=AL_SM;

	if(AL_GT==gtOneOnOne)
		NumberOfRoundz=AL_RS;
	else
		NumberOfRoundz=AL_RM;
	FIRSTTO=AL_VC;


	GameSpeed=AL_GS;
	Beacon(235235);
	if(GameSpeed==GS_SLOW)
		Dlay=25;
	else if(GameSpeed==GS_NORMAL)
		Dlay=20;
	else if(GameSpeed==GS_FAST)
		Dlay=15;
	else if(GameSpeed==GS_ULTRA)
		Dlay=10;


	//TheInputEngine->UpdateUseJoy();

	lSoundStatus=SoundStatus;
	SoundStatus=AL_SS;
	Beacon(23454);
	if(SoundStatus!=SS_NOSOUND&&lSoundStatus==SS_NOSOUND)
		TheSoundEngine->ActivateSound();
	else if(SoundStatus==SS_MONO&&(lSoundStatus==SS_STEREO || lSoundStatus==SS_RSTEREO  ))
		TheSoundEngine->NormalizePanning();
	else if(SoundStatus==SS_NOSOUND&&lSoundStatus!=SS_NOSOUND)
		TheSoundEngine->DisableSound();


	Gamma=AL_GA;

	Volume=AL_VL;

	TheSoundEngine->SetVolume();	
	Beacon(234441);

	TheControls->SetControls(ThePlayers[0],ThePlayers[1]);

	Beacon(234444);
}



void Game::DeleteTheQuadrants(){
	// Guard against double-free: return early if already freed
	if (!TheShotQuadrants || !TheMortalQuadrants) return;

	if(OldWorldSize){
		Beacon(77766677);
		for(int d=0;d<OldWorldSize*11;d++){
			delete[] TheShotQuadrants[d];
			delete[] TheMortalQuadrants[d];
		}
		delete[] TheShotQuadrants;
		delete[] TheMortalQuadrants;

		// Clear dangling pointers and reset size to prevent double-free
		TheShotQuadrants = nullptr;
		TheMortalQuadrants = nullptr;
		OldWorldSize = 0;
	}
}
void Game::AllocateNewQuadrants(){

	TheShotQuadrants = new shot**[WorldSize*11];

	int i; // C++20: declare before loops so it's accessible in both
	for ( i = 0; i < WorldSize*11; i++){
		TheShotQuadrants[i] = new shot*[WorldSize*11];
	}
	TheMortalQuadrants = new Mortal**[WorldSize*11];


	for ( i = 0; i < WorldSize*11; i++){
		TheMortalQuadrants[i] = new Mortal*[WorldSize*11];
	}

	for ( int ix = 0; ix < WorldSize*11; ix++){
		for ( int iz = 0; iz < WorldSize*11; iz++){
			TheMortalQuadrants[ix][iz]=0;
			TheShotQuadrants[ix][iz]=0;

		}
	}

}

void Game::SuddenOrDraw(){
	if(TheGameKeeper->AllowDrawInCurrentRound()||MONEYMAZE){
		DP2("Gamedone set bcoz moneymaze ==",MONEYMAZE);
		GAMEDONE=TRUE;
		RoundTime=(int)((TheGame->thisTickCount-StartTime)/1000);
		RoundWinner=2;
	}else{
		SUDDENDEATH=TRUE;
	}
}