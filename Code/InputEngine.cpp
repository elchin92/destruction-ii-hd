// InputEngine.cpp: implementation of the InputEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "InputEngine.h"
#include "ddex4.h"
#include <stdio.h>

//#include <windef.h>

InputEngine * TheInputEngine;

char str[100];

//#include "Game.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

InputEngine::InputEngine()
{
	g_pDI       = NULL;         
	g_pKeyboard = NULL;    
	g_pJoystick[0] = NULL;     
	g_pJoystick[1] = NULL;     
	for(int i=0;i<MaxTempJoys;i++){
		g_pTempJoystick[i] = NULL;   
	}
	hDlg		= NULL;
	JoysUsed=0;
}

InputEngine::~InputEngine()
{

}
void InputEngine::IncreaseJoysFound(){JoysFound++;}
int InputEngine::GetJoysFound(){return JoysFound;}
int InputEngine::GetJoysUsed(){return JoysUsed;}
//-----------------------------------------------------------------------------
// Name: EnumJoysticksCallback()
// Desc: Called once for each enumerated joystick. If we find one, create a
//       device interface on it so we can play with it.
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance,
                                     VOID* pContext )
{
    HRESULT hr;


	hr = TheInputEngine->g_pDI->CreateDeviceEx( pdidInstance->guidInstance, IID_IDirectInputDevice2,
								(VOID**)&TheInputEngine->g_pTempJoystick[TheInputEngine->GetJoysFound()], 0 );

	if( FAILED(hr) )
		return DIENUM_CONTINUE;

	TheInputEngine->IncreaseJoysFound();

    // Stop enumeration. Note: we're just taking the first joystick we get. You
    // could store all the enumerated joysticks and let the user pick.

	if(TheInputEngine->GetJoysFound()<MaxTempJoys)
		return DIENUM_CONTINUE;

	return DIENUM_STOP;
}



//-----------------------------------------------------------------------------
// Name: EnumAxesCallback()
// Desc: Callback function for enumerating the axes on a joystick
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumAxesCallback( const DIDEVICEOBJECTINSTANCE* pdidoi,
                                VOID* pContext )
{
    HWND hDlg = (HWND)pContext;

    DIPROPRANGE diprg; 
    diprg.diph.dwSize       = sizeof(DIPROPRANGE); 
    diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
    diprg.diph.dwHow        = DIPH_BYOFFSET; 
    diprg.diph.dwObj        = pdidoi->dwOfs; // Specify the enumerated axis
    diprg.lMin              = -1000; 
    diprg.lMax              = +1000; 
    

	// Set the range for the axis
	if( FAILED( TheInputEngine->CurrentJoystick->SetProperty( DIPROP_RANGE, &diprg.diph ) ) ){
		sprintf(str,"SetProperty() failed.");
		TheGame->UserMessage("Joystick Error", str,FALSE);
		return DIENUM_STOP;
	}

    return DIENUM_CONTINUE;
}


//-----------------------------------------------------------------------------
// Name: InitInput()
// Desc: Initialize the DirectInput variables.
//-----------------------------------------------------------------------------
HRESULT InputEngine::InitializeInput()
{
    HRESULT hr;
	
    // Register with the DirectInput subsystem and get a pointer
    // to a IDirectInput interface we can use.
    hr = DirectInputCreateEx( (HINSTANCE)GetWindowLong( TheGame->hWnd, GWL_HINSTANCE ), DIRECTINPUT_VERSION,IID_IDirectInput7, (LPVOID*)&g_pDI, 0 );
    if( FAILED(hr) ) 
        return hr;

	Beacon(100);
	
	LookForJoysticks();

    // Obtain an interface to the system keyboard device.
    hr = g_pDI->CreateDevice( GUID_SysKeyboard, &g_pKeyboard, 0 );
    if( FAILED(hr) ){
		TheGame->UserMessage("Keyboard Error", "CreateDevice() failed");
        return hr;
	}

	Beacon(106);
    // Set the data format to "keyboard format" - a predefined data format 
    //
    // A data format specifies which controls on a device we
    // are interested in, and how they should be reported.
    //
    // This tells DirectInput that we will be passing an array
    // of 256 bytes to IDirectInputDevice::GetDeviceState.
    hr = g_pKeyboard->SetDataFormat( &c_dfDIKeyboard );
    if( FAILED(hr) ) {
		TheGame->UserMessage("Keyboard Error", "SetDataFormat() failed");
        return hr;
	}
	Beacon(107);
    // Set the cooperativity level to let DirectInput know how
    // this device should interact with the system and with other
    // DirectInput applications.
    hr = g_pKeyboard->SetCooperativeLevel( TheGame->hWnd, 
		DISCL_EXCLUSIVE | DISCL_FOREGROUND );
    if( FAILED(hr) ) {
		TheGame->UserMessage("Keyboard Error", "SetCooperativeLevel() failed");
        return hr;
	}
	Beacon(108);
	
	DIGITALMOVEMENT[0]=FALSE;
	DIGITALMOVEMENT[1]=FALSE;

	g_pKeyboard->Acquire();

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: UnacquireInput()
// Desc: Acquire or unacquire the keyboard, depending on if the app is active
//       Input device must be acquired before the GetDeviceState is called
//-----------------------------------------------------------------------------
void InputEngine::UnacquireInput()
{
	for(int joy=0;joy<2;joy++)
		if( g_pJoystick[joy] )
		{
			g_pJoystick[joy]->Unacquire();
		}
    
	// Nothing to do if g_pKeyboard is 0
    if( g_pKeyboard )
		g_pKeyboard->Unacquire();


}


//-----------------------------------------------------------------------------
// Name: SetAcquire()
// Desc: Acquire or unacquire the keyboard, depending on if the app is active
//       Input device must be acquired before the GetDeviceState is called
//-----------------------------------------------------------------------------
/*HRESULT SetAcquire( HWND hDlg )
{
    if( USEJOY && g_pJoystick )
    {
        if( g_bActive ) 
            g_pJoystick->Acquire();
        else 
            g_pJoystick->Unacquire();
    }
    
	// Nothing to do if g_pKeyboard is 0
    if( 0 == g_pKeyboard )
        return S_FALSE;
	
    if( g_bActive ) 
    {
        // Acquire the input device 
        g_pKeyboard->Acquire();
    } 
    else 
    {
        // Unacquire the input device 
        g_pKey
		board->Unacquire();
    }
	g_pMouse->Acquire();

    return S_OK;
}

*/

void InputEngine::UpdateJoyInput(int joy){

	HRESULT hr;
    DIJOYSTATE  js;           // DInput joystick state 

	DP2("Updating joy #",joy+1);
	if(g_pJoystick[joy]){
		do
		{
			// Poll the device to read the current state
			hr = g_pJoystick[joy]->Poll();
			if( FAILED(hr) ){
				DP("Joy poll failed 666");
				sprintf(str,"Poll() failed. Device: %i",joy+1);
				TheGame->UserMessage("Joystick Error", str,FALSE);
				break;
			}
			
			// Get the input's device state
			hr = g_pJoystick[joy]->GetDeviceState( sizeof(DIJOYSTATE), &js );
			
			if( hr == DIERR_INPUTLOST )
			{
				DP("Joy GetDeviceState() input lost");
				// DInput is telling us that the input stream has been
				// interrupted. We aren't tracking any state between polls, so
				// we don't have any special reset that needs to be done. We
				// just re-acquire and try again.
				
				hr = g_pJoystick[joy]->Acquire();
				if( FAILED(hr) ){ 
					DP("Joy Acquire() failed 666");
					sprintf(str,"Input lost and Acquire() failed. Device: %i",joy+1);
					TheGame->UserMessage("Joystick Error", str,FALSE);
					break;
				}
			}
		}
		while( DIERR_INPUTLOST == hr );
	
		if( !FAILED(hr) )  
		{

			DP("Joy OK");
			int offset = JoyKeyStart+joy*KeysPerJoy;

			for( int i = 0; i < 32; i++ )
				PlayDiks[offset+i]=( js.rgbButtons[i] & 0x80 );
			
			int x=js.lX,y=js.lY;
			
			int cd=offset+32;

			if(DIGITALMOVEMENT[joy]){
				int ay=abs(y),ax=abs(x);
				const int sensitivity=280;
				// Right
				PlayDiks[cd++]=(ay>sensitivity?(x>ay-sensitivity):(x>=sensitivity-ay))*0x80;
				// Left
				PlayDiks[cd++]=(ay>sensitivity?(x<sensitivity-ay):(x<=ay-sensitivity))*0x80;
				// Down
				PlayDiks[cd++]=(ax>sensitivity?(y>ax-sensitivity):(y>=sensitivity-ax))*0x80;
				// Up
				PlayDiks[cd++]=(ax>sensitivity?(y<sensitivity-ax):(y<=ax-sensitivity))*0x80;


				
			}else{
				PlayDiks[cd++]=(x>500)*0x80;
				PlayDiks[cd++]=(x<-500)*0x80;
				PlayDiks[cd++]=(y>500)*0x80;
				PlayDiks[cd++]=(y<-500)*0x80;
				
			}

			PlayDiks[cd++]=(js.lZ>500)*0x80;
			PlayDiks[cd++]=(js.lZ<-500)*0x80;
			PlayDiks[cd++]=(js.rglSlider[0]>500)*0x80;
			PlayDiks[cd++]=(js.rglSlider[0]<-500)*0x80;

			PlayDiks[cd++]=(js.rglSlider[1]>500)*0x80;
			PlayDiks[cd++]=(js.rglSlider[1]<-500)*0x80;
			PlayDiks[cd++]=(js.lRx>500)*0x80;
			PlayDiks[cd++]=(js.lRx<-500)*0x80;

			PlayDiks[cd++]=(js.lRy>500)*0x80;
			PlayDiks[cd++]=(js.lRy<-500)*0x80;
			PlayDiks[cd++]=(js.lRz>500)*0x80;
			PlayDiks[cd++]=(js.lRz<-500)*0x80;

		}else{
			DP2("Something wrong with joy 666",hr);
			sprintf(str,"General GetDeviceState() failed. Device: %i. HRESULT: %i",joy+1, hr);
			TheGame->UserMessage("Joystick Error", str,FALSE);	
		}

	}

}

int InputEngine::IsInbetween(){
	return Inbetween;
}

int InputEngine::TotalKeysUsed(){
	return KeyboardKeys + KeysPerJoy*JoysUsed;
}
//-----------------------------------------------------------------------------
// Name: UpdateInputState()
// Desc: Get the input device's state and display it.
//-----------------------------------------------------------------------------
HRESULT InputEngine::UpdateInputState( HWND hWnd , int iInbetween){

	Inbetween=iInbetween;
    HRESULT     hr;

	DP("UpdateInputState() #1");


	for(int joy=0;joy<JoysUsed;joy++){
		DP2("UpdateInputState() j#%i",joy+1);
		UpdateJoyInput(joy);
	}
	DP("UpdateInputState() #2");	
	
	hDlg=hWnd;
	
	if( g_pKeyboard ){
		DP("UpdateInputState() k#1");
		
        BYTE    diks[KeyboardKeys];   // DirectInput keyboard state buffer 
		
        // Get the input's device state, and put the state in dims
        hr = g_pKeyboard->GetDeviceState( sizeof(diks), &diks );
        if( FAILED(hr) )  
        {
			DP("UpdateInputState() GetDeviceState() Failed 666");
			TheGame->UserMessage("InputStream interrupted", "(re)Acquire() failed",FALSE);
            // DirectInput is telling us that the input stream has been
            // interrupted.  We aren't tracking any state between polls, so
            // we don't have any special reset that needs to be done.
            // We just re-acquire and try again.
			
            // If input is lost then acquire and keep trying 
            hr = g_pKeyboard->Acquire();
            while( hr == DIERR_INPUTLOST || 
				hr == E_ACCESSDENIED  ) 
            {
				DP("UpdateInputState() Acquiring()");
                hr = g_pKeyboard->Acquire();
            }
			
            if( FAILED(hr) ){  
				TheGame->UserMessage("Keyboard Error", "(re)Acquire() failed",FALSE);
                return hr;
			}

			TheGame->UserMessage("Keyboard Error", "GetDeviceState() failed",FALSE);
        }else{
			DP("UpdateInputState() k#2 Copying diks");
			for(int y=0;y<KeyboardKeys;y++)
				PlayDiks[y]=diks[y];		
		}

		DP("UpdateInputState() k#3");	
		
	}


	DP("UpdateInputState() #3");

#ifdef __D2PROTECTED__
	if(TheGame->INITED){
		
		DP("UpdateInputState() Inited");
		if((TheGame->RoundsCounter>=6) && !(
			((TheGame->HiddenSecurity[5]&0x0008)?1:0) == ((((((( ((TheGame->HiddenSecurity[0]&512)?1:0)|| ((TheGame->HiddenSecurity[1]&16)?1:0)) && ((TheGame->HiddenSecurity[1]&4)?1:0)) || ((TheGame->HiddenSecurity[2]&512)?1:0)) || ((TheGame->HiddenSecurity[3]&128)?1:0)) && ((TheGame->HiddenSecurity[3]&1)?1:0)) )?1:0) &&
			((TheGame->HiddenSecurity[5]&0x0040)?1:0) == (((((( ((TheGame->HiddenSecurity[0]&256)?1:0)&& ((TheGame->HiddenSecurity[1]&4096)?1:0)) && ((TheGame->HiddenSecurity[3]&4096)?1:0)) || ((TheGame->HiddenSecurity[3]&2048)?1:0)) && ((TheGame->HiddenSecurity[4]&8)?1:0)) )?1:0) &&
			((TheGame->HiddenSecurity[5]&0x0100)?1:0) == ((((((( ((TheGame->HiddenSecurity[0]&1024)?1:0)|| ((TheGame->HiddenSecurity[1]&1024)?1:0)) || ((TheGame->HiddenSecurity[2]&64)?1:0)) && ((TheGame->HiddenSecurity[4]&8192)?1:0)) && ((TheGame->HiddenSecurity[4]&256)?1:0)) && ((TheGame->HiddenSecurity[4]&2)?1:0)) )?1:0)
			) ){
			
			DP("UpdateInputState() PseudoQuit()");

			Beacon(6664);
			if(rand()%2)
				PseudoQuit();	
		}
		
	}
#endif

	cgs->UpdateInput();
	

	DP("UpdateInputState() #4");
	for(int Y=0;Y<TotalKeysUsed();Y++)
		ldiks[Y]=PlayDiks[Y];	

	return S_OK;
}


void InputEngine::ReleaseInput(){
    // Unacquire and release any DirectInputDevice objects.
	for(int joy=0;joy<2;joy++)
		if( 0 != g_pJoystick[joy] ) 
		{
			// Unacquire the device one last time just in case 
			// the app tried to exit while the device is still acquired.
			g_pJoystick[joy]->Unacquire();
			g_pJoystick[joy]->Release();
			g_pJoystick[joy] = 0;
		}
    // Unacquire and release any DirectInputDevice objects.
    if( g_pKeyboard ) 
    {
        // Unacquire the device one last time just in case 
        // the app tried to exit while the device is still acquired.
        g_pKeyboard->Unacquire();
        g_pKeyboard->Release();
        g_pKeyboard = 0;
    }

    // Release any DirectInput objects.
    if( g_pDI ) 
    {
        g_pDI->Release();
        g_pDI = 0;
    }
}

void InputEngine::SetDigital(){
	for(int joy=0;joy<2;joy++){
		BOOL TEMP;
		DIGITALMOVEMENT[joy]=FALSE;

		int StartDikJoyMove = JoyKeyStart + 32 + KeysPerJoy*joy;
		for(int pl=0;pl<2;pl++){
			TEMP=TRUE;
			for(int key=0;key<4;key++)
				TEMP=TEMP&&TheControls->GetKey(key,pl)>=StartKeyJoyMove
				&&         TheControls->GetKey(key,pl)<=StartKeyJoyMove+4;

			DIGITALMOVEMENT[joy]=DIGITALMOVEMENT[joy]||TEMP;
		}
	}
	//SetJoyMove();
}


/*
void InputEngine::SetJoyMove(){

	BOOL TEMP;
	JoyMovement=-1;
	for(int pl=0;pl<2;pl++){
		TEMP=TRUE;
		for(int key=0;key<5;key++)
			TEMP=TEMP&&TheControls->GetKey(key,pl)>=256;
		if(TEMP)
			JoyMovement=pl;
	}
}*/
void InputEngine::ZeroInput(){
	for(int Y=0;Y<TotalKeysUsed();Y++)
		PlayDiks[Y]=ldiks[Y]=0;
}

int InputEngine::GetAnyKeyTrueUp(){

	for(int Key=0;Key<TotalKeysUsed();Key++)
		if(GetTrueUp(Key))
			return Key;

	return -1;
}

int InputEngine::GetAnyKeyTrueDown(){

	for(int Key=0;Key<TotalKeysUsed();Key++)
		if(GetTrueDown(Key))
			return Key;

	return -1;
}

int InputEngine::GetTrueUp(int DikCode){  return !(PlayDiks[DikCode] & 0x80) &&  (ldiks[DikCode] & 0x80);}
int InputEngine::GetTrueDown(int DikCode){return  (PlayDiks[DikCode] & 0x80) && !(ldiks[DikCode] & 0x80);}
int InputEngine::GetDown(int DikCode){return (PlayDiks[DikCode] & 0x80);}

/*--------------------------------------------------------------------------
| ReacquireInput
|
| Reacquires the current input device.  If Acquire() returns S_FALSE,
| that means
| that we are already acquired and that DirectInput did nothing.
*-------------------------------------------------------------------------*/
BOOL InputEngine::ReacquireInput(void)
{
    HRESULT hRes;

    // if we have a current device
    if(g_pKeyboard)
    {
       // acquire the device
       hRes = IDirectInputDevice_Acquire(g_pKeyboard);
       if(SUCCEEDED(hRes))
       {
		   for(int joy=0;joy<JoysUsed;joy++){
				hRes = g_pJoystick[joy]->Acquire();	
				if(FAILED(hRes)){
					sprintf(str,"ReacquireInput() failed. Device: %i",joy+1);
					TheGame->UserMessage("Joystick Error", str,FALSE);
					return FALSE;
				}
		   }
          // acquisition successful
          return TRUE;
       }
       else
       {
		   TheGame->UserMessage("Keyboard Error", "ReacquireInput() failed",FALSE);
          // acquisition failed
          return FALSE;
       }
    }
    else
    {
       // we don't have a current device
       return FALSE;
    }

}

BOOL InputEngine::JoyActive(){

	for(int pl=0;pl<2;pl++)
		for(int i=0;i<INGAMEKEYS;i++)
			if(TheControls->GetKey(i,pl)>=JoyKeyStart &&
			   TheControls->GetKey(i,pl)<=JoyKeyMax )
				return TRUE;

	return FALSE;

}


 // Look for simple joysticks

void InputEngine::LookForJoysticks(){

	JoysUsed=0;
    JoysFound=0;
	Beacon(923043);

	if(g_pJoystick[0]){
		g_pJoystick[0]->Unacquire();
		g_pJoystick[0]=NULL;
	}
	if(g_pJoystick[1]){
		g_pJoystick[1]->Unacquire();
		g_pJoystick[1]=NULL;
	}
	HRESULT hr = g_pDI->EnumDevices( DIDEVTYPE_JOYSTICK, EnumJoysticksCallback, 0, DIEDFL_ATTACHEDONLY );

	DP2("jf",JoysFound);
	if(FAILED(hr)){
		sprintf(str,"EnumDevices() failed");
		TheGame->UserMessage("Joystick Error", str, FALSE);
	}

	Beacon(923044);
	for(int joy=0;joy<JoysFound&&JoysUsed<2;joy++){

		BOOL Go=TRUE;

		while(Go){

			CurrentJoystick=g_pTempJoystick[joy];
			Beacon(101);

			hr = CurrentJoystick->SetDataFormat( &c_dfDIJoystick );
			if( FAILED(hr) ){
				sprintf(str,"SetDataFormat() failed. Device: %i",joy+1);
				TheGame->UserMessage("Joystick Error", str, FALSE);
				break;
			}
			Beacon(102);
			// Set the cooperative level to let DInput know how this device should
			// interact with the system and with other DInput applications.
			hr = CurrentJoystick->SetCooperativeLevel( TheGame->hWnd, DISCL_EXCLUSIVE|DISCL_FOREGROUND );
			if( FAILED(hr) ) {
				sprintf(str,"SetProperty() failed. Device: %i",joy+1);
				TheGame->UserMessage("Joystick Error", str,FALSE);
				break;
			}
			Beacon(103);
			// Determine how many axis the joystick has (so we don't error out setting
			// properties for unavailable axis)
			g_diDevCaps.dwSize = sizeof(DIDEVCAPS);
			hr = CurrentJoystick->GetCapabilities(&g_diDevCaps);
			if ( FAILED(hr) ){ 
				sprintf(str,"GetCapabilites() failed. Device: %i",joy+1);
				TheGame->UserMessage("Joystick Error", str,FALSE);
				break;
			}
			Beacon(104);
			// Enumerate the axes of the joyctick and set the range of each axis. Note:
			// we could just use the defaults, but we're just trying to show an example
			// of enumerating device objects (axes, buttons, etc.).

			//    g_pTempJoystick->EnumObjects( EnumAxesCallback, (VOID*)g_pTempJoystick, DIDFT_AXIS );
			CurrentJoystick->EnumObjects( EnumAxesCallback, (VOID*)TheGame->hWnd, DIDFT_AXIS );	
			CurrentJoystick->Acquire();

			if(CurrentJoystick==0){
				sprintf(str,"EnumAxesCallback() failed. Device: %i",joy+1);
				TheGame->UserMessage("Joystick Error", str,FALSE);
				break;		
			}
			Beacon(105);
			Go=FALSE;
			g_pJoystick[JoysUsed]=CurrentJoystick;
			JoysUsed++;
			DP2("joy id ok", JoysUsed);
		}
	}
}
