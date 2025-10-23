#ifndef __SDL2_DIRECTINPUT_COMPAT_H__
#define __SDL2_DIRECTINPUT_COMPAT_H__

/*******************************************************************************
 * SDL2_DirectInputCompat.h
 *
 * DirectInput 7 → SDL2 Input Compatibility Layer
 *
 * This header provides DirectInput-like types and interfaces using SDL2 input
 * as the backend. This allows porting legacy DirectInput code to modern SDL2
 * with minimal source code changes.
 *
 * Architecture:
 * - DirectInput keyboard → SDL_Keyboard API
 * - DirectInput joystick → SDL_GameController API
 * - DIK codes → SDL_Scancode mapping
 *******************************************************************************/

// ===========================================================================
// CRITICAL: Prevent inclusion of real DirectInput headers!
// Must be defined BEFORE any Windows headers that might transitively include them
// ===========================================================================
#ifndef _DINPUT_H
#define _DINPUT_H
#endif
#ifndef __DINPUT_INCLUDED__
#define __DINPUT_INCLUDED__
#endif
#ifndef __dinput_h__
#define __dinput_h__
#endif

#include <SDL2/SDL.h>
#include <windows.h>
#include <unknwn.h>  // For IUnknown
#include <unordered_map>
#include <array>
#include <memory>

// ============================================================================
// DirectInput Constants and Types
// ============================================================================

// DirectInput HRESULT codes
#ifndef DI_OK
#define DI_OK                           S_OK
#endif

#ifndef DIERR_GENERIC
#define DIERR_GENERIC                   E_FAIL
#endif

// Device types
#define DI8DEVTYPE_KEYBOARD            0x00000001
#define DI8DEVTYPE_JOYSTICK            0x00000002
#define DI8DEVTYPE_GAMEPAD             0x00000003

// Cooperative level flags
#define DISCL_EXCLUSIVE                0x00000001
#define DISCL_NONEXCLUSIVE             0x00000002
#define DISCL_FOREGROUND               0x00000004
#define DISCL_BACKGROUND               0x00000008

// Data format flags
#define DIDFT_BUTTON                   0x0000000C
#define DIDFT_AXIS                     0x00000003

// Enumeration callback return values
#define DIENUM_CONTINUE                1
#define DIENUM_STOP                    0

// DirectInput version
#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION            0x0800
#endif

// Property constants - use static GUID instead of macro to allow taking address
static const GUID DIPROP_RANGE_GUID = {1, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0}};
#define DIPROP_RANGE                   (&DIPROP_RANGE_GUID)

// Property header flags
#define DIPH_BYOFFSET                  2

// DirectInput error codes
#define DIERR_INPUTLOST                0x8007001E
#define DIERR_NOTACQUIRED              0x8007000C

// Device type constants
#define DIDEVTYPE_JOYSTICK             4
#define DIEDFL_ATTACHEDONLY            0x00000001

// GUIDs for device interfaces
static const GUID IID_IDirectInputDevice2 = {0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0}};
static const GUID IID_IDirectInput7 = {0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0}};

// Windows constant (for GetWindowLong)
#ifndef GWLP_HINSTANCE
#define GWLP_HINSTANCE                 (-6)
#endif
#ifndef GWL_HINSTANCE
#define GWL_HINSTANCE                  GWLP_HINSTANCE
#endif

// ============================================================================
// DirectInput Property Structures
// ============================================================================

typedef struct DIPROPHEADER {
    DWORD dwSize;
    DWORD dwHeaderSize;
    DWORD dwObj;
    DWORD dwHow;
} DIPROPHEADER, *LPDIPROPHEADER;

typedef struct DIPROPRANGE {
    DIPROPHEADER diph;
    LONG lMin;
    LONG lMax;
} DIPROPRANGE, *LPDIPROPRANGE;

// Const pointer typedef for property header
typedef const DIPROPHEADER* LPCDIPROPHEADER;

// Device object instance structure (forward declaration for callback)
typedef struct DIDEVICEOBJECTINSTANCE {
    DWORD dwSize;
    GUID guidType;
    DWORD dwOfs;
    DWORD dwType;
    DWORD dwFlags;
    TCHAR tszName[260];
    DWORD dwFFMaxForce;
    DWORD dwFFForceResolution;
    WORD wCollectionNumber;
    WORD wDesignatorIndex;
    WORD wUsagePage;
    WORD wUsage;
    DWORD dwDimension;
    WORD wExponent;
    WORD wReportId;
} DIDEVICEOBJECTINSTANCE, *LPDIDEVICEOBJECTINSTANCE;

// Callback function pointer for EnumObjects
typedef BOOL (CALLBACK *LPDIENUMDEVICEOBJECTSCALLBACK)(LPDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);

// ============================================================================
// DirectInput Keyboard Scan Codes (DIK_*)
// Map these to SDL_Scancode
// ============================================================================

// Arrow keys
#define DIK_UP              SDL_SCANCODE_UP
#define DIK_DOWN            SDL_SCANCODE_DOWN
#define DIK_LEFT            SDL_SCANCODE_LEFT
#define DIK_RIGHT           SDL_SCANCODE_RIGHT

// Main keyboard
#define DIK_ESCAPE          SDL_SCANCODE_ESCAPE
#define DIK_1               SDL_SCANCODE_1
#define DIK_2               SDL_SCANCODE_2
#define DIK_3               SDL_SCANCODE_3
#define DIK_4               SDL_SCANCODE_4
#define DIK_5               SDL_SCANCODE_5
#define DIK_6               SDL_SCANCODE_6
#define DIK_7               SDL_SCANCODE_7
#define DIK_8               SDL_SCANCODE_8
#define DIK_9               SDL_SCANCODE_9
#define DIK_0               SDL_SCANCODE_0
#define DIK_MINUS           SDL_SCANCODE_MINUS
#define DIK_EQUALS          SDL_SCANCODE_EQUALS
#define DIK_BACK            SDL_SCANCODE_BACKSPACE
#define DIK_BACKSPACE       DIK_BACK            // Alias for DIK_BACK (DirectInput compatibility)
#define DIK_TAB             SDL_SCANCODE_TAB
#define DIK_Q               SDL_SCANCODE_Q
#define DIK_W               SDL_SCANCODE_W
#define DIK_E               SDL_SCANCODE_E
#define DIK_R               SDL_SCANCODE_R
#define DIK_T               SDL_SCANCODE_T
#define DIK_Y               SDL_SCANCODE_Y
#define DIK_U               SDL_SCANCODE_U
#define DIK_I               SDL_SCANCODE_I
#define DIK_O               SDL_SCANCODE_O
#define DIK_P               SDL_SCANCODE_P
#define DIK_LBRACKET        SDL_SCANCODE_LEFTBRACKET
#define DIK_RBRACKET        SDL_SCANCODE_RIGHTBRACKET
#define DIK_RETURN          SDL_SCANCODE_RETURN
#define DIK_LCONTROL        SDL_SCANCODE_LCTRL
#define DIK_A               SDL_SCANCODE_A
#define DIK_S               SDL_SCANCODE_S
#define DIK_D               SDL_SCANCODE_D
#define DIK_F               SDL_SCANCODE_F
#define DIK_G               SDL_SCANCODE_G
#define DIK_H               SDL_SCANCODE_H
#define DIK_J               SDL_SCANCODE_J
#define DIK_K               SDL_SCANCODE_K
#define DIK_L               SDL_SCANCODE_L
#define DIK_SEMICOLON       SDL_SCANCODE_SEMICOLON
#define DIK_APOSTROPHE      SDL_SCANCODE_APOSTROPHE
#define DIK_GRAVE           SDL_SCANCODE_GRAVE
#define DIK_LSHIFT          SDL_SCANCODE_LSHIFT
#define DIK_BACKSLASH       SDL_SCANCODE_BACKSLASH
#define DIK_Z               SDL_SCANCODE_Z
#define DIK_X               SDL_SCANCODE_X
#define DIK_C               SDL_SCANCODE_C
#define DIK_V               SDL_SCANCODE_V
#define DIK_B               SDL_SCANCODE_B
#define DIK_N               SDL_SCANCODE_N
#define DIK_M               SDL_SCANCODE_M
#define DIK_COMMA           SDL_SCANCODE_COMMA
#define DIK_PERIOD          SDL_SCANCODE_PERIOD
#define DIK_SLASH           SDL_SCANCODE_SLASH
#define DIK_RSHIFT          SDL_SCANCODE_RSHIFT
#define DIK_MULTIPLY        SDL_SCANCODE_KP_MULTIPLY
#define DIK_LMENU           SDL_SCANCODE_LALT
#define DIK_SPACE           SDL_SCANCODE_SPACE
#define DIK_CAPITAL         SDL_SCANCODE_CAPSLOCK
#define DIK_F1              SDL_SCANCODE_F1
#define DIK_F2              SDL_SCANCODE_F2
#define DIK_F3              SDL_SCANCODE_F3
#define DIK_F4              SDL_SCANCODE_F4
#define DIK_F5              SDL_SCANCODE_F5
#define DIK_F6              SDL_SCANCODE_F6
#define DIK_F7              SDL_SCANCODE_F7
#define DIK_F8              SDL_SCANCODE_F8
#define DIK_F9              SDL_SCANCODE_F9
#define DIK_F10             SDL_SCANCODE_F10
#define DIK_NUMLOCK         SDL_SCANCODE_NUMLOCKCLEAR
#define DIK_SCROLL          SDL_SCANCODE_SCROLLLOCK
#define DIK_NUMPAD7         SDL_SCANCODE_KP_7
#define DIK_NUMPAD8         SDL_SCANCODE_KP_8
#define DIK_NUMPAD9         SDL_SCANCODE_KP_9
#define DIK_SUBTRACT        SDL_SCANCODE_KP_MINUS
#define DIK_NUMPAD4         SDL_SCANCODE_KP_4
#define DIK_NUMPAD5         SDL_SCANCODE_KP_5
#define DIK_NUMPAD6         SDL_SCANCODE_KP_6
#define DIK_ADD             SDL_SCANCODE_KP_PLUS
#define DIK_NUMPAD1         SDL_SCANCODE_KP_1
#define DIK_NUMPAD2         SDL_SCANCODE_KP_2
#define DIK_NUMPAD3         SDL_SCANCODE_KP_3
#define DIK_NUMPAD0         SDL_SCANCODE_KP_0
#define DIK_DECIMAL         SDL_SCANCODE_KP_PERIOD
#define DIK_F11             SDL_SCANCODE_F11
#define DIK_F12             SDL_SCANCODE_F12

// Extended keys
#define DIK_NUMPADENTER     SDL_SCANCODE_KP_ENTER
#define DIK_RCONTROL        SDL_SCANCODE_RCTRL
#define DIK_DIVIDE          SDL_SCANCODE_KP_DIVIDE
#define DIK_SYSRQ           SDL_SCANCODE_SYSREQ
#define DIK_RMENU           SDL_SCANCODE_RALT
#define DIK_PAUSE           SDL_SCANCODE_PAUSE
#define DIK_HOME            SDL_SCANCODE_HOME
#define DIK_PRIOR           SDL_SCANCODE_PAGEUP
#define DIK_END             SDL_SCANCODE_END
#define DIK_NEXT            SDL_SCANCODE_PAGEDOWN
#define DIK_INSERT          SDL_SCANCODE_INSERT
#define DIK_DELETE          SDL_SCANCODE_DELETE

// ============================================================================
// Forward Declarations
// ============================================================================

struct SDL2DirectInput;
struct SDL2InputDevice;

// ============================================================================
// Type Definitions
// ============================================================================

// DirectInput interface pointers
typedef SDL2DirectInput*    IDirectInput7;
typedef SDL2DirectInput*    IDirectInput;
typedef SDL2InputDevice*    IDirectInputDevice;
typedef SDL2InputDevice*    IDirectInputDevice2;

// Legacy pointer types (note: IDirectInputDevice is already a pointer!)
typedef SDL2DirectInput*     LPDIRECTINPUT7;
typedef SDL2InputDevice*     LPDIRECTINPUTDEVICE;   // NOT IDirectInputDevice*!
typedef SDL2InputDevice*     LPDIRECTINPUTDEVICE2;  // NOT IDirectInputDevice2*!

// DirectInput COM-style method macros (for legacy code compatibility)
#define IDirectInputDevice_Acquire(p)           (p)->Acquire()
#define IDirectInputDevice_Unacquire(p)         (p)->Unacquire()
#define IDirectInputDevice_GetDeviceState(p,a,b) (p)->GetDeviceState(a,b)
#define IDirectInputDevice_Poll(p)              (p)->Poll()

typedef unsigned char        BYTE;

// ============================================================================
// DirectInput Structures
// ============================================================================

// Device capabilities
struct DIDEVCAPS {
    DWORD dwSize;
    DWORD dwFlags;
    DWORD dwDevType;
    DWORD dwAxes;
    DWORD dwButtons;
    DWORD dwPOVs;
    DWORD dwFFSamplePeriod;
    DWORD dwFFMinTimeResolution;
    DWORD dwFirmwareRevision;
    DWORD dwHardwareRevision;
    DWORD dwFFDriverVersion;
};

// Joystick state
// Data format structures
typedef struct DIOBJECTDATAFORMAT {
    const GUID *pguid;
    DWORD dwOfs;
    DWORD dwType;
    DWORD dwFlags;
} DIOBJECTDATAFORMAT, *LPDIOBJECTDATAFORMAT;

typedef struct DIDATAFORMAT {
    DWORD dwSize;
    DWORD dwObjSize;
    DWORD dwFlags;
    DWORD dwDataSize;
    DWORD dwNumObjs;
    LPDIOBJECTDATAFORMAT rgodf;
} DIDATAFORMAT, *LPDIDATAFORMAT;

// Predefined data format for joystick (stub - actual implementation would have object list)
static DIDATAFORMAT c_dfDIJoystick_stub = {
    sizeof(DIDATAFORMAT),
    sizeof(DIOBJECTDATAFORMAT),
    0x00000002,  // DIDF_ABSAXIS
    80,          // Data size for DIJOYSTATE
    0,           // Number of objects (stub)
    nullptr      // Object format array (stub)
};
#define c_dfDIJoystick  (&c_dfDIJoystick_stub)

// Simple joystick state (DirectInput 1-5 compatible)
struct DIJOYSTATE {
    LONG    lX;
    LONG    lY;
    LONG    lZ;
    LONG    lRx;
    LONG    lRy;
    LONG    lRz;
    LONG    rglSlider[2];
    DWORD   rgdwPOV[4];
    BYTE    rgbButtons[32];
};

// Extended joystick state (DirectInput 7+)
struct DIJOYSTATE2 {
    LONG    lX;
    LONG    lY;
    LONG    lZ;
    LONG    lRx;
    LONG    lRy;
    LONG    lRz;
    LONG    rglSlider[2];
    DWORD   rgdwPOV[4];
    BYTE    rgbButtons[128];
    LONG    lVX;
    LONG    lVY;
    LONG    lVZ;
    LONG    lVRx;
    LONG    lVRy;
    LONG    lVRz;
    LONG    rglVSlider[2];
    LONG    lAX;
    LONG    lAY;
    LONG    lAZ;
    LONG    lARx;
    LONG    lARy;
    LONG    lARz;
    LONG    rglASlider[2];
    LONG    lFX;
    LONG    lFY;
    LONG    lFZ;
    LONG    lFRx;
    LONG    lFRy;
    LONG    lFRz;
    LONG    rglFSlider[2];
};

// Device instance
struct DIDEVICEINSTANCE {
    DWORD   dwSize;
    GUID    guidInstance;
    GUID    guidProduct;
    DWORD   dwDevType;
    WCHAR   tszInstanceName[260];
    WCHAR   tszProductName[260];
    GUID    guidFFDriver;
    WORD    wUsagePage;
    WORD    wUsage;
};

// Enum devices callback
typedef BOOL (CALLBACK *LPDIENUMDEVICESCALLBACK)(const DIDEVICEINSTANCE*, LPVOID);

// Predefined data formats
extern const DIDATAFORMAT c_dfDIKeyboard;
extern const DIDATAFORMAT c_dfDIJoystick2;

// ============================================================================
// SDL2InputDevice - DirectInputDevice Wrapper
// ============================================================================

struct SDL2InputDevice {
    enum DeviceType {
        DEVICE_KEYBOARD,
        DEVICE_JOYSTICK,
        DEVICE_GAMEPAD
    };

    DeviceType type;
    SDL_GameController* controller;
    SDL_Joystick* joystick;
    int joystickID;

    // Keyboard state
    std::array<Uint8, SDL_NUM_SCANCODES> keyboardState;

    // Joystick state
    DIJOYSTATE2 joyState;

    int refCount;

    SDL2InputDevice(DeviceType t)
        : type(t), controller(nullptr), joystick(nullptr), joystickID(-1), refCount(1) {
        keyboardState.fill(0);
        memset(&joyState, 0, sizeof(joyState));
    }

    ~SDL2InputDevice() {
        if (controller) {
            SDL_GameControllerClose(controller);
        } else if (joystick) {
            SDL_JoystickClose(joystick);
        }
    }

    // COM-like interface
    ULONG AddRef() { return ++refCount; }
    ULONG Release() {
        if (--refCount == 0) {
            delete this;
            return 0;
        }
        return refCount;
    }

    // DirectInput methods
    HRESULT SetCooperativeLevel(HWND hwnd, DWORD flags);
    HRESULT SetDataFormat(const DIDATAFORMAT* format);
    HRESULT Acquire();
    HRESULT Unacquire();
    HRESULT GetDeviceState(DWORD size, LPVOID data);
    HRESULT Poll();
    HRESULT SetProperty(REFGUID rguidProp, LPCDIPROPHEADER pdiph) {
        // Property setting stub - SDL doesn't require property configuration
        // Just return success to keep original code happy
        return DI_OK;
    }
    HRESULT EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACK lpCallback, LPVOID pvRef, DWORD dwFlags) {
        // Enumerate device objects stub - for joystick axis enumeration
        // Return success - actual enumeration implementation would go here
        return DI_OK;
    }
    HRESULT GetCapabilities(DIDEVCAPS* caps);
};

// ============================================================================
// SDL2DirectInput - Main DirectInput7 Wrapper
// ============================================================================

struct SDL2DirectInput {
    LPDIRECTINPUTDEVICE keyboard;
    LPDIRECTINPUTDEVICE2 joysticks[16];
    int numJoysticks;
    int refCount;

    SDL2DirectInput() : keyboard(nullptr), numJoysticks(0), refCount(1) {
        memset(joysticks, 0, sizeof(joysticks));
    }

    ~SDL2DirectInput() {
        if (keyboard) {
            keyboard->Release();
        }
        for (int i = 0; i < numJoysticks; i++) {
            if (joysticks[i]) {
                joysticks[i]->Release();
            }
        }
    }

    // COM-like interface
    ULONG AddRef() { return ++refCount; }
    ULONG Release() {
        if (--refCount == 0) {
            delete this;
            return 0;
        }
        return refCount;
    }

    // DirectInput methods
    HRESULT CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICE* device, IUnknown* unkOuter);
    HRESULT CreateDeviceEx(REFGUID rguid, REFIID riid, LPVOID* device, IUnknown* unkOuter) {
        // CreateDeviceEx is just an extended version of CreateDevice
        return CreateDevice(rguid, (LPDIRECTINPUTDEVICE*)device, unkOuter);
    }
    HRESULT EnumDevices(DWORD devType, LPDIENUMDEVICESCALLBACK callback, LPVOID ref, DWORD flags);
    HRESULT Initialize(HINSTANCE inst, DWORD version);
};

// ============================================================================
// Global DirectInput Functions
// ============================================================================

HRESULT DirectInputCreateEx(HINSTANCE inst, DWORD version, REFIID riid, LPVOID* ppvOut, IUnknown* unkOuter);

// Standard GUIDs for devices
extern const GUID GUID_SysKeyboard;
extern const GUID GUID_SysMouse;

#endif // __SDL2_DIRECTINPUT_COMPAT_H__