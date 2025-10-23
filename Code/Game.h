#ifndef __Game__ 
#define __Game__




#include <ddraw.h>
#include "definitions.h"
#include "SaveableClass.h"
#include "GameState.h"

class Base;
class Weapon;
//class Clone;
class Actor;
class Animation;
class shot;
class Ufo;
class Settings;
class Grenade;
class SpecialClusterBomb;
class SpecialTank;
class SpecialClone;
class Weapon;
class LaserBeam;
class TazerBeam;
class Player;
class Shooter;
class Mortal;
class Menu;
class GraphicMenu;
class GameState;
class Settings;
class GameSetup;
class PCSetup;
class Stats;
class Stores;
class Intro;
class Controls;
class Start;
class Clone;
class SoundEngine;
class MovementObject;


	//-----------------------------------------------------------------------------
	// Variable Constants
	//-----------------------------------------------------------------------------
	const int					CredMax=10;
	const int					MaxVolume=25,MaxGamma=25;
	const int					MaxWorldSize=21; // Copy of this number in player.h, and SpecialX.h
	const int					MaxLik=20;
	const int					prlength=100;
	const int					lnSize=9,subSize=12;
	const int					FX=256,FY=96;	// Free Square
	const int					TotalNumberOfSurfaces=45;

	const int	BaseSize=5; // Not fully functional. Check ddex4.cpp code (Item & tank creation)

	// Note that adding misc items requires some manipulation due to the special xtra life code
	const int	AccMin=5;
	const int	NumberOfItems=30;
	const long	MinVolume=3300; // Ranges from 10000 (=Silent) to 0 (=MAX) 
	const long	MaxVolumeChange=3000;// Ranges from 10000 (=Max) to 0 (=None)
	const BOOL	BUYSEVERALATSHOTS=FALSE; // Not functional!


	class Game : public SaveableClass
{
private:


	//-----------------------------------------------------------------------------
	// Menu & Blit Variables
	//-----------------------------------------------------------------------------

	HRESULT						hRet;
	DDBLTFX						ddbltfx;
	COLORREF					RadarColors[NumberOfMortals][2];




	//-----------------------------------------------------------------------------
	// Private Variables
	//-----------------------------------------------------------------------------

	int							Null[2];
	int							yup, Satan,lTimeLeft;
	int							lastWorld, WorldType;
	int							GameSpeed;
	int							MaxRounds, BPoint;
	int							test[4];	
	int							TankType,BlitLife[2];
	int							ExpSlow;
	int							Temporary,HPFrame[2];
	int							Timez[4][100];
	int							TheWorldSize;	
	int							Pointer;	
	int							sWidth,sHeight, FollowX, FollowY, OverRun;
	int							SoundStatus,CameFrom,lSoundStatus;
	int							CHANGE[4][2];
	int							sflop[2];
	int							Switch;
	int							OtherHealth[2];
	int							JoyMovement;
	int							TrCnt;
	int							RoundTime;
	int							ThickArray[ThicknessTypes];
	int							lBarz[2][3],Barz[2][3], GUIPUPS[2][5],lGUIPUPS[2][5];
	int							MS;
	int							MortalMatrix[NumberOfMortals][NumberOfMortals];
	int							player;
	int							CheatKeys[NoOfCheats][30];
	int							CheatLevel[NoOfCheats];
	int							CheatActive;

	long						Timer[2]; // Total, Play

	BOOL						SETVOLUME;
	BOOL						TIMEOUT;
	BOOL						REVERSED;
	BOOL						COORDS;
	BOOL						BETWEEN;
	BOOL						UPDATETEXT[2][6],BLITBAR[2][3], BLITPUPS[2][5],UPDATETIMELIMIT;
	BOOL						GAMEDONE,READY[2];
	BOOL						COOL, PAUSE,SUDDENDEATH;
	BOOL						KILL,SETGAMMA;
	BOOL						MONEYMAZE, FOCUSLOST;
	BOOL						PAINTED,MAGICDONE;
	BOOL						WASFROZEN[2],WASPAUSED;

	__int64		                FrameTickCount, CantChangeStart;
	__int64						FrameCount,ufoTick,IncrStart,DecrStart;
	__int64						StopTime;
	__int64						TankKillStart[2];

	char						Message1[100] ;
	char						Message2[100] ;
	char						Message3[100] ;
	char						Message4[200] ;
	char						RegisterCode[60];
	char						PlayerBullets[40];
	char						PlayerSecondary[40];
	char						PlayerCharges[35];
	char						PlayerGrenades[35];
	char						PlayerLives[35];
#ifdef __D2PROTECTED__
	char						UserName[50],UserCode[50];
#endif
	char						Strs[2][6][8];

	//-----------------------------------------------------------------------------
	// Internal Functions
	//-----------------------------------------------------------------------------

	void FreezeRound();

	void CantChangeBeep();
	void GenerateWorld(int TypeOfWorld,int Size);
	void BlowUp(int X, int Y, int * ChargePos, int Who);
	void RealRevive(int who);
	void Revive(int who);
	void Pause();
	void KillShots();
	void RegisterBeep();
	void UpdateTheFrame();
#ifdef __D2PROTECTED__
	void Verification(char Name[50]);
#endif
	void SetSurfaces();
	void DeleteTheQuadrants();
	void AllocateNewQuadrants();
	void SuddenOrDraw();
	int LookForRegistrationInfo();

	int	Average(int Array[prlength]);
	int TestDirection(Mortal * Mover, int Direction);

	void ActivateCheat();
	void (Game::*(CheatFunctions[NoOfCheats]))();

	void ActivatePacman();
	void ActivateRocket();
	void ActivatePrey();
	void ActivateBoom();
	void ActivateSpeed();
	void ActivateMight();

	double NewAsk(double Acc);



public:


	//-----------------------------------------------------------------------------
	// Game Functions
	//-----------------------------------------------------------------------------

	Game();
	~Game();

	int MainWindowSwitch(UINT message, WPARAM wParam, LPARAM lParam);

	void CreateObjects();
	void Reset();
  	void ReleaseAllObjects(void);
	void Quit();

	HRESULT InitGame(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitFail(HWND hWnd, HRESULT hRet, LPCTSTR szError,...);
	HRESULT SetGameUp();
	HRESULT RestoreAll(void);

	//-----------------------------------------------------------------------------
	// Public Variables
	//-----------------------------------------------------------------------------

	HWND                        hWnd;
	HDC                         hdc;
	SIZE                        size;
	BOOL						ACTIVE;



	Weapon * Weapons[30];

	Weapon *					ActiveWeapon;



	Player *					apl;

	// The GameStates

	GameState * GameStates[NumberOfGameStates];


	int							OldWorldSize, WorldSize, Volume, Gamma;
	int							WeaponAdjust[8][2], PRACTICE;
	int							Probability[NumberOfItems], ActivePlayer;
	int							SumOfProb, StartingMoney;
	int							Dlay, Xstart,Ystart, RoundsCounter, SpecialEvents;
	int							WorldMap[MaxWorldSize*11+1][MaxWorldSize*11+1][3];
	int							ColorKey;
	int							HiddenSecurity[10], Choice, CPlayer;
	int							NumberOfRoundz, RoundWinner;
	long						TheFrame;
	BOOL						STEREO, LASTMM;
	BOOL						INITED, FIRSTTIMESTARTUP, SETWORLD;
	BOOL						FIRSTTO, GAMEACTIVE;
	BOOL						ESCSUPRESSED;
#ifdef __D2PROTECTED__
	BOOL						ISREGISTERED;
#endif
	BOOL						UPDATESCREEN, STARTNEWROUND;
	BOOL						COPYBACK;

	__int64						thisTickCount;
	__int64						StartTime;
	__int64						Times[2];
	COLORREF					UnSelectedColor,SelectedColor,SelectedBGColor;
	
	char						WorkingDir[256];


	//-----------------------------------------------------------------------------
	// External Functions
	//-----------------------------------------------------------------------------

	int DoAttack(Mortal * Attacker, Mortal * Victim);
	int GenerateItem(int GIType);
	int MoneyMaze();
	int GetScreenPos(int xy, int who);
	int IsPaused();
	int IsSuddenDeath();
	int GetThick(int ThickType);

	void InternalSetupSurfaces();
	void FreezeScreen();
	void BeginNewGame();
	void BeginNewRound();
	void EndRound();
	void EndGame();
	void KillDeadAnims();
	void KillAll();
	void ZeroStats();
	void UpdateFrame();
	void AddExplosion(int x, int y);
	void RegExp(int X,int Y, int Damage, RegExpType rType, DamageType dType, Mortal * Firerer);
	void SingleExplosion(int X, int Y, int Damage, DamageType dType, Mortal * Firerer);
	void AddShot(int LeftRightShot, double SpeedOrColor, double AccOrAngle, BulletType bType, int Damage, int Alt, Shooter * Firerer);
	void Shoot(int Ammo, int x, int y);
	void ErrorReport(int Error);
	void UserMessage(char * title, char * msg, int mboxandquit=TRUE);
	void AddFX();
	void Focus();
	void SetCamera();
	void SetVariables();
	void InterpretSettings();
	void SetActivePlayers();
	void DoMagic();
	void UpdateTime();
	HINSTANCE GoToURL(char * URL, int QUITGAME=FALSE);

#ifdef __D2PROTECTED__
	unsigned short * String2Code(char Code[50]);
	unsigned short * RegCode;
#endif


	//-----------------------------------------------------------------------------
	// Functions found in Blit.cpp
	//-----------------------------------------------------------------------------

	void BlitIt();
	void BlitBack();
	void BlitGUI(int player);
	void BlitMap(int player);
	void PrintText();
	void PreparePlayerSprites();
	Animation * NewAnimation(int iFrames, int iPlayTime, int iAnimType,int x, int y, int BlitX, int BlitY,int Reverse=0);
	Animation * CreateStaticAnimation(int X, int Y, int BlitX, int BlitY);


	//-----------------------------------------------------------------------------
	// Functions found in BlitEngine.cpp
	//-----------------------------------------------------------------------------


	void BGO(int x, int y, BYTE BlitColor=0);
	void BGOS(int x,int y, LPDIRECTDRAWSURFACE7 * BlitSource, BYTE BlitColor=0);
	void BGOSL(int x,int y, int life, LPDIRECTDRAWSURFACE7 * BlitSource, BYTE BlitColor =0);
	void BGOSNC(int x,int y, LPDIRECTDRAWSURFACE7 * BlitSource);
	void BlitBeam(int * Pos, int * EndPos, int Color,int where);
	void BlitRadarDot(Mortal * PresentMortal, int ALIVE);
	void BlitBeamC(int X, int Y, int EndX, int EndY, int Color,int where);
	void Text(int x, int y, LPCSTR Text);
	void MenuText(int x, int y, LPCSTR Text, BOOL SELECTED);
	void IntText(int x, int y, int number);
	void LoadBitmapToSurface(LPDIRECTDRAWSURFACE7 * Surface, const char * Bitmap);
	void BlitOntoBack(LPDIRECTDRAWSURFACE7 * BlitSource);
	void BlitOnto(LPDIRECTDRAWSURFACE7 * BlitDestination, LPDIRECTDRAWSURFACE7 * BlitSource);
	void BlitCoords(int l, int t, int r, int b , int x, int y, LPDIRECTDRAWSURFACE7 * BlitSource);
	void BlitOntoSetRc(int x, int y, LPDIRECTDRAWSURFACE7 * Destinaton, LPDIRECTDRAWSURFACE7 * BlitSource, BYTE BlitColor=0, DWORD rot=0);
	void BlitSetRc(int x, int y, LPDIRECTDRAWSURFACE7 * BlitSource, BYTE BlitColor=0, DWORD rot=0);
	void BlitSetRcCC(int x, int y, LPDIRECTDRAWSURFACE7 * BlitSource, DWORD rot=0);
	void BlankArea(int l, int t, int w, int h);
	void BlankArea(RECT * iRect);
	void BlankSurfaceArea(int l, int t, int w, int h, LPDIRECTDRAWSURFACE7 * Surface);
	void BlankSurfaceArea(RECT * bRect, LPDIRECTDRAWSURFACE7 * Surface);

	void CopyRoutineSingleColor(int x, int y, LPDIRECTDRAWSURFACE7 * Destination, LPDIRECTDRAWSURFACE7 * BlitSource,int rot, BYTE BlitColor);
	void CopyRoutineNoCC(int x, int y, LPDIRECTDRAWSURFACE7 * Destination, LPDIRECTDRAWSURFACE7 * BlitSource,int rot);
	void CopyRoutine(int x, int y, LPDIRECTDRAWSURFACE7 * Destination, LPDIRECTDRAWSURFACE7 * BlitSource,int rot);

	// Doesn't work!!!
	void CreateBlankSurface(int width, int height, LPDIRECTDRAWSURFACE7 * Surface);
	void ScaleSurface(LPDIRECTDRAWSURFACE7 * Surface, 
				  int OriginalColor, int DesiredColor,
				  int Skip, int NumberOfColors);

	void BlankScreen();
	void FlipAndWait();
	void ClearScreen();
	void UpdateGameInput();
	void PrepareNewScreen();
	void FixImage(LPDIRECTDRAWSURFACE7 * Surface);
	void ReleaseAllSurfaces();



	//-----------------------------------------------------------------------------
	// Public DirectDraw Related Variables
	//-----------------------------------------------------------------------------

	LPDIRECTDRAW7               g_pDD ;        // DirectDraw object

	LPDIRECTDRAWSURFACE7        g_pDDSPrimary ;// DirectDraw primary surface
	LPDIRECTDRAWSURFACE7        g_pDDSLife ;    // The Life Surface
	LPDIRECTDRAWSURFACE7        g_pDDSEmptyLife ;    // The Empty Life Surface
	LPDIRECTDRAWSURFACE7        g_pDDSGUIPowerUps ;
	LPDIRECTDRAWSURFACE7        g_pDDSTankWarner ;    // The TankWarner Surface
	LPDIRECTDRAWSURFACE7        g_pDDSBars ;    // The Small Bars Surface
	LPDIRECTDRAWSURFACE7        g_pDDSEmptyBar ;    // The Empty Bar Surface
	LPDIRECTDRAWSURFACE7        g_pDDSItem ;
	LPDIRECTDRAWSURFACE7        g_pDDSTemporary ;    // Where the maps first are blitted
	LPDIRECTDRAWSURFACE7		g_pDDSSprites ;    // For Misc Sprites
	LPDIRECTDRAWSURFACE7        g_pDDSDroid ;
	LPDIRECTDRAWSURFACE7        g_pDDSTank ;
	LPDIRECTDRAWSURFACE7        g_pDDSRTank ;
	LPDIRECTDRAWSURFACE7        g_pDDSPlayer ;
	LPDIRECTDRAWSURFACE7        g_pDDSPlayerAssembly;
	LPDIRECTDRAWSURFACE7        g_pDDSFlames ;
	LPDIRECTDRAWSURFACE7        g_pDDSMachine ;
	LPDIRECTDRAWSURFACE7        g_pDDSMachineGun ;
	LPDIRECTDRAWSURFACE7        g_pDDSCar ;
	LPDIRECTDRAWSURFACE7        g_pDDSGrenade ;
	LPDIRECTDRAWSURFACE7        g_pDDSBullets ;
	LPDIRECTDRAWSURFACE7        g_pDDSLevels ;
	LPDIRECTDRAWSURFACE7		g_pDDSBack ;   // DirectDraw back surface
	LPDIRECTDRAWSURFACE7        g_pDDSBackground ;    // The Background Surface

	LPDIRECTDRAWSURFACE7		* AllSurfaces[TotalNumberOfSurfaces];

	DDCAPS						hwcaps,swcaps;
	int							DD_ROTATION;


	//-----------------------------------------------------------------------------
	// Public Drawing Related Variables
	//-----------------------------------------------------------------------------

	HFONT                       g_hFontMedium, g_hFont,g_hFontSmall,g_hFontBig,g_hFontBigPlus,g_hFontHuge;
	HFONT						g_hStatsSub,g_hStatsLine,g_hFontAngled[2];
	HPEN						thePen,aPen,bPen,cPen, lifePen, greenPen, redPen, bluePen;
	HPEN						laserPens[3];
	HBRUSH						theBrush,aBrush, lifeBrush;
	LOGBRUSH					RealBrush;
	RECT						rcRect, tcRect;


};

extern Game * TheGame;






#endif


