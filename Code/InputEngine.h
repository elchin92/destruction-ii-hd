#ifndef __InputEngine__ 
#define __InputEngine__

#include <dinput.h>

#define KeyboardKeys	256
#define JoyKeyStart		256
#define StartKeyJoyMove JoyKeyStart+32
#define KeysPerJoy		48

#define MaxKeys			(KeyboardKeys+2*KeysPerJoy)
#define JoyKeyMax		(MaxKeys-1)

#define MaxTempJoys		16
class InputEngine
{
private:

	int JoysUsed, JoysFound;

	BYTE						PlayDiks[MaxKeys]; 
	BYTE						ldiks[MaxKeys];

	int DIGITALMOVEMENT[2];
	int Inbetween;
	int TotalKeysUsed();
	void UpdateJoyInput(int joy);

public:

	LPDIRECTINPUT7				g_pDI;         
	LPDIRECTINPUTDEVICE			g_pKeyboard;
   	LPDIRECTINPUTDEVICE2		g_pTempJoystick[16];     
	LPDIRECTINPUTDEVICE2		g_pJoystick[2];   
	LPDIRECTINPUTDEVICE2		CurrentJoystick;   	
	DIDEVCAPS				    g_diDevCaps;
	HWND						hDlg;

	//-----------------------------------------------------------------------------
	// Construction/Destruction
	//-----------------------------------------------------------------------------


	InputEngine();
	~InputEngine();

	//-----------------------------------------------------------------------------
	// DirectInput Related Functions
	//-----------------------------------------------------------------------------

	HRESULT UpdateInputState(HWND hDlg, int iInbetween=FALSE);



	BOOL JoyActive();
	BOOL ReacquireInput(); 
	void UnacquireInput();


	HRESULT InitializeInput();
	void ReleaseInput();
	HRESULT UpdateInput();

	void ZeroInput();

	int DigitalMovement();
	int IsInbetween();
	void SetDigital();
	void IncreaseJoysFound();
	void LookForJoysticks();
	int GetJoysFound();
	int GetJoysUsed();
	//-----------------------------------------------------------------------------
	// In Game Related Functions
	//-----------------------------------------------------------------------------
	
	int GetTrueUp(int DikCode);
	int GetTrueDown(int DikCode);
	int GetDown(int DikCode);
	int GetAnyKeyTrueUp();
	int GetAnyKeyTrueDown();
};
#endif


#define TrueUp(p) (TheInputEngine->GetTrueUp(p))
#define TrueDown(p) (TheInputEngine->GetTrueDown(p))
#define Down(p) (TheInputEngine->GetDown(p))
#define AnyKeyTrueUp() (TheInputEngine->GetAnyKeyTrueUp())
#define AnyKeyTrueDown() (TheInputEngine->GetAnyKeyTrueDown())

extern InputEngine * TheInputEngine;
