#include "ddraw.h"

#ifndef __GameState__ 
#define __GameState__



#define mcRed RGB(255,0,0)
#define mcGreen RGB(0,255,0)
#define mcBlue RGB(0,0,255)
#define mcWhite RGB(255,255,255)
#define mcGrey RGB(200,200,200)

enum gState{PLAY, START, ABOUT, SETTINGS, NEWSTORE,
			 LOAD, SAVE, INTRO, GETREADY, CONTROLS,
			 LOOKAWAY,PLAYERSELECTER, STATISTICS,
			 HELP,SETTINGSEXPLANATION, PLAYERSETUP, NEXTROUND,
			 FULLVERSION, LEAGUE, LASTROUNDS, GETTINGSTARTED,
			 ENEMYENCYCLOPEDIA, ITEMENCYCLOPEDIA, DETAILEDINSTRUCTIONS,
			 CUP, BUYNOW,
			 NumberOfGameStates, NoState};
class Game;
class InfoBox;

const int					MaximumNumberOfSurfaces=21;

class GameState
{
private:

	__int64 MsgStop;
	COLORREF StaticMsgColor;



protected:

	// Messaging
	char StaticMsg[100]; // internal message string
	char Msg[100]; // internal message string

	InfoBox * UserMsgBox;
	HFONT MsgFont;

	void CreateStandardUserMsgBox();
	void CreateWideScreenUserMsgBox();
	void SetStaticMsgColor(COLORREF iColor);
	void iMessage(COLORREF iColor);

	void DisableMsg();

	int Brightness, SwitchingState, BitmapToUsePaletteFrom;
	gState NextState, ReturnState;

	const char *				StateBitmaps[MaximumNumberOfSurfaces];
	LPDIRECTDRAWSURFACE7		* StateSurfaces[MaximumNumberOfSurfaces];
	int NumberOfStateSurfaces;

	HRESULT LoadBitmaps();

	virtual void InternalUpdateInput()=0;

	void InternalSwitchState(gState State, int SubMenu=0);
	void AddBitmapAndSurface(const char * Bitmap, LPDIRECTDRAWSURFACE7 * Surface);

public:

	virtual ~GameState();
	void SetCorrectPaletteAndGamma();
	void SetPaletteAndGamma(int Brightness);
	void SetReturnState(gState iReturnState);
	void UpdateInput();
	void Update();
	void Initialize();
	void BlitMessage();

	HRESULT ReLoadBitmaps();
	LPDIRECTDRAWSURFACE7 * GetSurface(int i);
	int GetNumberOfSurfaces();

	virtual int IsFading();
	void ReturnToMotherState();
	void ForcedSwitchState(gState State, int SubMenu=0);
	virtual void SwitchState(gState State, int SubMenu=0);
	virtual void Setup()=0;
	virtual void Blit()=0;
	virtual void SetSurfaces()=0;
	virtual void SetupSurfaces();

	virtual COLORREF ContrastAdjust(COLORREF rgb);
	virtual COLORREF ContrastAdjust(int r, int g, int b);
	virtual COLORREF GetWhite();	
	virtual COLORREF GetRed();
	virtual COLORREF GetGreen();
	virtual COLORREF GetBlue();
	virtual COLORREF GetBlack();

	
	void Message(char * iMsg1, char * iMsg2, COLORREF iColor);
	void Message(char * iMsg, COLORREF iColor);


};
#endif

extern	LPDIRECTDRAWPALETTE         g_pDDPal;     // The primary surface palette
extern 	gState	GameStatus;
extern	GameState * cgs;