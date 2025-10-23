/*******************************************************************************
 * SDL2_DirectInputCompat.cpp
 *
 * Implementation of DirectInput 7 → SDL2 Input Compatibility Layer
 *
 * This file implements DirectInput-compatible interfaces using SDL2.
 *******************************************************************************/

#include "SDL2_DirectInputCompat.h"
#include <algorithm>

// ============================================================================
// Global Data Format Definitions
// ============================================================================

const DIDATAFORMAT c_dfDIKeyboard = {
    sizeof(DIDATAFORMAT),
    sizeof(DWORD),
    0x00000002,  // DIDF_RELAXIS
    256,         // Size of keyboard data
    0,
    nullptr
};

const DIDATAFORMAT c_dfDIJoystick2 = {
    sizeof(DIDATAFORMAT),
    sizeof(DWORD),
    0x00000001,  // DIDF_ABSAXIS
    sizeof(DIJOYSTATE2),
    0,
    nullptr
};

// ============================================================================
// Device GUIDs
// ============================================================================

const GUID GUID_SysKeyboard = { 0x12345678, 0x1234, 0x5678, {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0} };
const GUID GUID_SysMouse = { 0x87654321, 0x4321, 0x8765, {0x43, 0x21, 0x87, 0x65, 0x43, 0x21, 0x87, 0x65} };

// ============================================================================
// SDL2InputDevice Implementation
// ============================================================================

HRESULT SDL2InputDevice::SetCooperativeLevel(HWND hwnd, DWORD flags) {
    // SDL2 handles cooperation internally
    // This is mostly a no-op
    return DI_OK;
}

HRESULT SDL2InputDevice::SetDataFormat(const DIDATAFORMAT* format) {
    // Store the data format for later use
    // SDL2 doesn't need specific data formats
    return DI_OK;
}

HRESULT SDL2InputDevice::Acquire() {
    if (type == DEVICE_JOYSTICK || type == DEVICE_GAMEPAD) {
        // Open the first available game controller
        int numJoysticks = SDL_NumJoysticks();
        for (int i = 0; i < numJoysticks; i++) {
            if (SDL_IsGameController(i)) {
                controller = SDL_GameControllerOpen(i);
                if (controller) {
                    joystickID = i;
                    return DI_OK;
                }
            } else {
                joystick = SDL_JoystickOpen(i);
                if (joystick) {
                    joystickID = i;
                    return DI_OK;
                }
            }
        }
    }
    return DI_OK;
}

HRESULT SDL2InputDevice::Unacquire() {
    if (controller) {
        SDL_GameControllerClose(controller);
        controller = nullptr;
    } else if (joystick) {
        SDL_JoystickClose(joystick);
        joystick = nullptr;
    }
    joystickID = -1;
    return DI_OK;
}

HRESULT SDL2InputDevice::GetDeviceState(DWORD size, LPVOID data) {
    if (!data) return DIERR_GENERIC;

    if (type == DEVICE_KEYBOARD) {
        // Get keyboard state
        const Uint8* sdlKeyState = SDL_GetKeyboardState(nullptr);

        // Map SDL keyboard state to DirectInput format
        BYTE* dikState = (BYTE*)data;
        memset(dikState, 0, 256);

        // Map common keys (the DIK_* defines map to SDL scancodes)
        for (int i = 0; i < SDL_NUM_SCANCODES && i < 256; i++) {
            if (sdlKeyState[i]) {
                dikState[i] = 0x80; // Key is pressed
            }
        }

        return DI_OK;
    }
    else if (type == DEVICE_JOYSTICK || type == DEVICE_GAMEPAD) {
        if (size != sizeof(DIJOYSTATE2)) return DIERR_GENERIC;

        DIJOYSTATE2* joyData = (DIJOYSTATE2*)data;
        memset(joyData, 0, sizeof(DIJOYSTATE2));

        if (controller) {
            // Get game controller state
            joyData->lX = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
            joyData->lY = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);
            joyData->lZ = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
            joyData->lRx = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);
            joyData->lRy = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY);
            joyData->lRz = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT);

            // Get button states
            for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX && i < 128; i++) {
                joyData->rgbButtons[i] = SDL_GameControllerGetButton(controller,
                    (SDL_GameControllerButton)i) ? 0x80 : 0x00;
            }

            // Get D-pad as POV hat
            bool up = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP);
            bool down = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
            bool left = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
            bool right = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);

            if (up && right) joyData->rgdwPOV[0] = 4500;  // 45 degrees
            else if (right && down) joyData->rgdwPOV[0] = 13500; // 135 degrees
            else if (down && left) joyData->rgdwPOV[0] = 22500; // 225 degrees
            else if (left && up) joyData->rgdwPOV[0] = 31500; // 315 degrees
            else if (up) joyData->rgdwPOV[0] = 0;    // 0 degrees
            else if (right) joyData->rgdwPOV[0] = 9000;  // 90 degrees
            else if (down) joyData->rgdwPOV[0] = 18000; // 180 degrees
            else if (left) joyData->rgdwPOV[0] = 27000; // 270 degrees
            else joyData->rgdwPOV[0] = 0xFFFFFFFF; // Centered

        } else if (joystick) {
            // Get raw joystick state
            int numAxes = SDL_JoystickNumAxes(joystick);
            if (numAxes > 0) joyData->lX = SDL_JoystickGetAxis(joystick, 0);
            if (numAxes > 1) joyData->lY = SDL_JoystickGetAxis(joystick, 1);
            if (numAxes > 2) joyData->lZ = SDL_JoystickGetAxis(joystick, 2);
            if (numAxes > 3) joyData->lRx = SDL_JoystickGetAxis(joystick, 3);
            if (numAxes > 4) joyData->lRy = SDL_JoystickGetAxis(joystick, 4);
            if (numAxes > 5) joyData->lRz = SDL_JoystickGetAxis(joystick, 5);

            // Get button states
            int numButtons = SDL_JoystickNumButtons(joystick);
            for (int i = 0; i < numButtons && i < 128; i++) {
                joyData->rgbButtons[i] = SDL_JoystickGetButton(joystick, i) ? 0x80 : 0x00;
            }

            // Get POV hat
            int numHats = SDL_JoystickNumHats(joystick);
            if (numHats > 0) {
                Uint8 hat = SDL_JoystickGetHat(joystick, 0);
                if (hat & SDL_HAT_UP) {
                    if (hat & SDL_HAT_RIGHT) joyData->rgdwPOV[0] = 4500;
                    else if (hat & SDL_HAT_LEFT) joyData->rgdwPOV[0] = 31500;
                    else joyData->rgdwPOV[0] = 0;
                } else if (hat & SDL_HAT_DOWN) {
                    if (hat & SDL_HAT_RIGHT) joyData->rgdwPOV[0] = 13500;
                    else if (hat & SDL_HAT_LEFT) joyData->rgdwPOV[0] = 22500;
                    else joyData->rgdwPOV[0] = 18000;
                } else if (hat & SDL_HAT_LEFT) {
                    joyData->rgdwPOV[0] = 27000;
                } else if (hat & SDL_HAT_RIGHT) {
                    joyData->rgdwPOV[0] = 9000;
                } else {
                    joyData->rgdwPOV[0] = 0xFFFFFFFF; // Centered
                }
            }
        }

        return DI_OK;
    }

    return DIERR_GENERIC;
}

HRESULT SDL2InputDevice::Poll() {
    // SDL2 automatically updates input state
    // This is mostly a no-op
    SDL_PumpEvents();
    return DI_OK;
}

HRESULT SDL2InputDevice::GetCapabilities(DIDEVCAPS* caps) {
    if (!caps) return DIERR_GENERIC;

    memset(caps, 0, sizeof(DIDEVCAPS));
    caps->dwSize = sizeof(DIDEVCAPS);
    caps->dwFlags = 0;

    if (type == DEVICE_KEYBOARD) {
        caps->dwDevType = DI8DEVTYPE_KEYBOARD;
        caps->dwAxes = 0;
        caps->dwButtons = 256;
        caps->dwPOVs = 0;
    } else if (type == DEVICE_JOYSTICK || type == DEVICE_GAMEPAD) {
        caps->dwDevType = DI8DEVTYPE_GAMEPAD;
        caps->dwAxes = 6;
        caps->dwButtons = 32;
        caps->dwPOVs = 1;
    }

    return DI_OK;
}

// ============================================================================
// SDL2DirectInput Implementation
// ============================================================================

HRESULT SDL2DirectInput::CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICE* device, IUnknown* unkOuter) {
    if (!device) return DIERR_GENERIC;

    // Check if it's the keyboard GUID
    if (memcmp(&rguid, &GUID_SysKeyboard, sizeof(GUID)) == 0) {
        SDL2InputDevice* keyboardDevice = new SDL2InputDevice(SDL2InputDevice::DEVICE_KEYBOARD);
        *device = keyboardDevice;
        keyboard = keyboardDevice;
        return DI_OK;
    }

    // Otherwise assume it's a joystick
    SDL2InputDevice* joystickDevice = new SDL2InputDevice(SDL2InputDevice::DEVICE_JOYSTICK);
    *device = joystickDevice;

    if (numJoysticks < 16) {
        joysticks[numJoysticks++] = joystickDevice;
    }

    return DI_OK;
}

HRESULT SDL2DirectInput::EnumDevices(DWORD devType, LPDIENUMDEVICESCALLBACK callback, LPVOID ref, DWORD flags) {
    if (!callback) return DIERR_GENERIC;

    // Enumerate keyboard
    if (devType == 0 || devType == DI8DEVTYPE_KEYBOARD) {
        DIDEVICEINSTANCE inst;
        memset(&inst, 0, sizeof(inst));
        inst.dwSize = sizeof(inst);
        inst.guidInstance = GUID_SysKeyboard;
        inst.guidProduct = GUID_SysKeyboard;
        inst.dwDevType = DI8DEVTYPE_KEYBOARD;
        wcscpy_s(inst.tszInstanceName, L"Keyboard");
        wcscpy_s(inst.tszProductName, L"Standard Keyboard");

        if (!callback(&inst, ref)) {
            return DI_OK;
        }
    }

    // Enumerate joysticks/gamepads
    if (devType == 0 || devType == DI8DEVTYPE_JOYSTICK || devType == DI8DEVTYPE_GAMEPAD) {
        int numJoysticks = SDL_NumJoysticks();
        for (int i = 0; i < numJoysticks; i++) {
            DIDEVICEINSTANCE inst;
            memset(&inst, 0, sizeof(inst));
            inst.dwSize = sizeof(inst);
            // Create a unique GUID for each joystick
            inst.guidInstance.Data1 = 0x10000000 + i;
            inst.guidProduct.Data1 = 0x20000000 + i;
            inst.dwDevType = SDL_IsGameController(i) ? DI8DEVTYPE_GAMEPAD : DI8DEVTYPE_JOYSTICK;

            const char* name = SDL_JoystickNameForIndex(i);
            if (name) {
                MultiByteToWideChar(CP_UTF8, 0, name, -1, inst.tszProductName, 260);
                wcscpy_s(inst.tszInstanceName, inst.tszProductName);
            } else {
                swprintf_s(inst.tszProductName, L"Joystick %d", i);
                wcscpy_s(inst.tszInstanceName, inst.tszProductName);
            }

            if (!callback(&inst, ref)) {
                break;
            }
        }
    }

    return DI_OK;
}

HRESULT SDL2DirectInput::Initialize(HINSTANCE inst, DWORD version) {
    // SDL2 input is initialized with SDL_Init
    // This is mostly a no-op
    return DI_OK;
}

// ============================================================================
// Global DirectInput Functions
// ============================================================================

HRESULT DirectInputCreateEx(HINSTANCE inst, DWORD version, REFIID riid, LPVOID* ppvOut, IUnknown* unkOuter) {
    if (!ppvOut) return DIERR_GENERIC;

    // Initialize SDL if not already done
    if (SDL_WasInit(SDL_INIT_GAMECONTROLLER) == 0) {
        if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0) {
            return DIERR_GENERIC;
        }
    }

    SDL2DirectInput* di = new SDL2DirectInput();
    di->Initialize(inst, version);

    *ppvOut = di;
    return DI_OK;
}