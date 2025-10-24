#ifndef __SDL2_DIRECTSOUND_COMPAT_H__
#define __SDL2_DIRECTSOUND_COMPAT_H__

/*******************************************************************************
 * SDL2_DirectSoundCompat.h
 *
 * DirectSound → SDL2_mixer Compatibility Layer
 *
 * This header provides DirectSound-like types and interfaces using SDL2_mixer
 * as the backend. This allows porting legacy DirectSound code to modern SDL2
 * with minimal source code changes.
 *
 * Architecture:
 * - DirectSound device → SDL_mixer initialization
 * - DirectSoundBuffer → Mix_Chunk + Mix_Channel
 * - Volume/Pan controls → Mix_Volume + Mix_SetPanning
 * - Streaming buffers → SDL_RWops + Mix_LoadWAV_RW
 *******************************************************************************/

// ===========================================================================
// CRITICAL: Prevent inclusion of real DirectSound headers!
// Must be defined BEFORE any Windows headers that might transitively include them
// ===========================================================================
#ifndef _DSOUND_H
#define _DSOUND_H
#endif
#ifndef __DSOUND_INCLUDED__
#define __DSOUND_INCLUDED__
#endif
#ifndef __dsound_h__
#define __dsound_h__
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <windows.h>
#include <mmsystem.h>  // For WAVEFORMATEX, LPWAVEFORMATEX
#include <unknwn.h>    // For IUnknown
#include <vector>
#include <unordered_map>

// ============================================================================
// DirectSound HRESULT Codes
// ============================================================================

#ifndef DS_OK
#define DS_OK                           S_OK
#endif

#ifndef DSERR_GENERIC
#define DSERR_GENERIC                   E_FAIL
#endif

#ifndef DSERR_BUFFERLOST
#define DSERR_BUFFERLOST                MAKE_HRESULT(1, 0x878, 150)
#endif

#ifndef DSERR_INVALIDPARAM
#define DSERR_INVALIDPARAM              E_INVALIDARG
#endif

#ifndef DSERR_OUTOFMEMORY
#define DSERR_OUTOFMEMORY               E_OUTOFMEMORY
#endif

// ============================================================================
// Forward Declarations
// ============================================================================

struct SDL2SoundBuffer;
struct SDL2DirectSound;

// ============================================================================
// Type Definitions (DirectSound → SDL2 Mapping)
// ============================================================================

// CRITICAL: IDirectSound/IDirectSoundBuffer are base types (not pointers)
// This matches original DirectSound API where IDirectSound is a class, not a pointer
typedef SDL2DirectSound      IDirectSound;
typedef SDL2SoundBuffer      IDirectSoundBuffer;

typedef IDirectSound*        LPDIRECTSOUND;         // Single pointer
typedef IDirectSoundBuffer*  LPDIRECTSOUNDBUFFER;   // Single pointer
typedef IDirectSoundBuffer** LPLPDIRECTSOUNDBUFFER; // Double pointer (for output params)

// ============================================================================
// DirectSound Structures
// ============================================================================

// Wave format structure - already defined by Windows SDK mmeapi.h
// We just need to make sure WAVE_FORMAT_PCM is defined
#ifndef WAVE_FORMAT_PCM
#define WAVE_FORMAT_PCM 1
#endif

// NOTE: WAVEFORMATEX is provided by Windows SDK (mmeapi.h)
// No need to redefine it here

// DirectSound capabilities
struct DSCAPS {
    DWORD dwSize;
    DWORD dwFlags;
    DWORD dwMinSecondarySampleRate;
    DWORD dwMaxSecondarySampleRate;
    DWORD dwPrimaryBuffers;
    DWORD dwMaxHwMixingAllBuffers;
    DWORD dwMaxHwMixingStaticBuffers;
    DWORD dwMaxHwMixingStreamingBuffers;
    DWORD dwFreeHwMixingAllBuffers;
    DWORD dwFreeHwMixingStaticBuffers;
    DWORD dwFreeHwMixingStreamingBuffers;
    DWORD dwMaxHw3DAllBuffers;
    DWORD dwMaxHw3DStaticBuffers;
    DWORD dwMaxHw3DStreamingBuffers;
    DWORD dwFreeHw3DAllBuffers;
    DWORD dwFreeHw3DStaticBuffers;
    DWORD dwFreeHw3DStreamingBuffers;
    DWORD dwTotalHwMemBytes;
    DWORD dwFreeHwMemBytes;
    DWORD dwMaxContigFreeHwMemBytes;
    DWORD dwUnlockTransferRateHwBuffers;
    DWORD dwPlayCpuOverheadSwBuffers;
    DWORD dwReserved1;
    DWORD dwReserved2;
};

typedef DSCAPS* LPDSCAPS;

// DirectSoundBuffer capabilities
struct DSBCAPS {
    DWORD dwSize;
    DWORD dwFlags;
    DWORD dwBufferBytes;
    DWORD dwUnlockTransferRate;
    DWORD dwPlayCpuOverhead;
};

typedef DSBCAPS* LPDSBCAPS;

// DirectSoundBuffer description
struct DSBUFFERDESC {
    DWORD          dwSize;
    DWORD          dwFlags;
    DWORD          dwBufferBytes;
    DWORD          dwReserved;
    LPWAVEFORMATEX lpwfxFormat;
};

typedef DSBUFFERDESC* LPDSBUFFERDESC;

// ============================================================================
// SDL2SoundBuffer - DirectSoundBuffer Wrapper
// ============================================================================

struct SDL2SoundBuffer {
    Mix_Chunk*      chunk;          // SDL_mixer sound chunk
    int             channel;        // SDL_mixer channel (-1 = not playing)
    DWORD           bufferSize;     // Size in bytes
    LPWAVEFORMATEX  format;         // Wave format
    DWORD           flags;          // Buffer flags
    bool            isPrimary;      // Is this the primary buffer?
    bool            isLost;         // Has buffer been lost?

    // Streaming support
    Uint8*          streamBuffer;   // For Lock/Unlock operations
    DWORD           streamSize;
    bool            isLocked;

    // Volume and pan settings
    int             volume;         // 0-128 (SDL_mixer scale)
    int             pan;            // 0-254 (0=left, 127=center, 254=right)

    int             refCount;

    SDL2SoundBuffer()
        : chunk(nullptr), channel(-1), bufferSize(0), format(nullptr),
          flags(0), isPrimary(false), isLost(false),
          streamBuffer(nullptr), streamSize(0), isLocked(false),
          volume(MIX_MAX_VOLUME), pan(127), refCount(1) {}

    ~SDL2SoundBuffer() {
        if (chunk) Mix_FreeChunk(chunk);
        if (format) delete format;
        if (streamBuffer) delete[] streamBuffer;
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

    // DirectSoundBuffer methods
    HRESULT GetCaps(LPDSBCAPS caps);
    HRESULT GetCurrentPosition(LPDWORD playPos, LPDWORD writePos);
    HRESULT GetFormat(LPWAVEFORMATEX waveFormat, DWORD size, LPDWORD written);
    HRESULT GetVolume(LPLONG volume);
    HRESULT GetPan(LPLONG pan);
    HRESULT GetFrequency(LPDWORD frequency);
    HRESULT GetStatus(LPDWORD status);
    HRESULT Initialize(LPDIRECTSOUND ds, LPDSBUFFERDESC desc);
    HRESULT Lock(DWORD offset, DWORD bytes, LPVOID* audioPtr1, LPDWORD audioBytes1,
                 LPVOID* audioPtr2, LPDWORD audioBytes2, DWORD flags);
    HRESULT Play(DWORD reserved1, DWORD priority, DWORD flags);
    HRESULT SetCurrentPosition(DWORD newPos);
    HRESULT SetFormat(LPWAVEFORMATEX waveFormat);
    HRESULT SetVolume(LONG volume);
    HRESULT SetPan(LONG pan);
    HRESULT SetFrequency(DWORD frequency);
    HRESULT Stop();
    HRESULT Unlock(LPVOID audioPtr1, DWORD audioBytes1, LPVOID audioPtr2, DWORD audioBytes2);
    HRESULT Restore();
};

// ============================================================================
// SDL2DirectSound - Main DirectSound Wrapper
// ============================================================================

struct SDL2DirectSound {
    bool            initialized;
    HWND            hwnd;
    DWORD           cooperativeLevel;

    std::vector<SDL2SoundBuffer*> buffers;

    int             refCount;

    SDL2DirectSound()
        : initialized(false), hwnd(nullptr), cooperativeLevel(0), refCount(1) {}

    ~SDL2DirectSound() {
        // NOTE: DO NOT release buffers here!
        // Buffers are manually released by SoundEngine::FreeDirectSound()
        // before lpDS->Release() is called. Releasing them here causes double-free!

        // Only cleanup SDL mixer
        if (initialized) {
            Mix_CloseAudio();
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

    // DirectSound methods
    HRESULT CreateSoundBuffer(LPDSBUFFERDESC desc, LPLPDIRECTSOUNDBUFFER buffer, IUnknown* unkOuter);
    HRESULT GetCaps(LPDSCAPS caps);
    HRESULT DuplicateSoundBuffer(LPDIRECTSOUNDBUFFER original, LPLPDIRECTSOUNDBUFFER duplicate);
    HRESULT SetCooperativeLevel(HWND hwnd, DWORD level);
    HRESULT Compact();
    HRESULT GetSpeakerConfig(LPDWORD config);
    HRESULT SetSpeakerConfig(DWORD config);
    HRESULT Initialize(GUID* guid);
};

// ============================================================================
// Global DirectSound Functions
// ============================================================================

HRESULT DirectSoundCreate(GUID* guid, LPDIRECTSOUND* ds, IUnknown* unkOuter);

// ============================================================================
// DirectSound Constants and Flags
// ============================================================================

// Cooperative levels
#define DSSCL_NORMAL                1
#define DSSCL_PRIORITY              2
#define DSSCL_EXCLUSIVE             3
#define DSSCL_WRITEPRIMARY          4

// Buffer capabilities flags
#define DSBCAPS_PRIMARYBUFFER       0x00000001
#define DSBCAPS_STATIC              0x00000002
#define DSBCAPS_LOCHARDWARE         0x00000004
#define DSBCAPS_LOCSOFTWARE         0x00000008
#define DSBCAPS_CTRLFREQUENCY       0x00000020
#define DSBCAPS_CTRLPAN             0x00000040
#define DSBCAPS_CTRLVOLUME          0x00000080
#define DSBCAPS_CTRLDEFAULT         0x000000E0
#define DSBCAPS_CTRLALL             0x000000E0
#define DSBCAPS_STICKYFOCUS         0x00004000
#define DSBCAPS_GETCURRENTPOSITION2 0x00010000

// Play flags
#define DSBPLAY_LOOPING             0x00000001

// Status flags
#define DSBSTATUS_PLAYING           0x00000001
#define DSBSTATUS_BUFFERLOST        0x00000002
#define DSBSTATUS_LOOPING           0x00000004

// Lock flags
#define DSBLOCK_FROMWRITECURSOR     0x00000001

// Device capabilities flags
#define DSCAPS_PRIMARYMONO          0x00000001
#define DSCAPS_PRIMARYSTEREO        0x00000002
#define DSCAPS_PRIMARY8BIT          0x00000004
#define DSCAPS_PRIMARY16BIT         0x00000008
#define DSCAPS_CONTINUOUSRATE       0x00000010
#define DSCAPS_EMULDRIVER           0x00000020
#define DSCAPS_CERTIFIED            0x00000040
#define DSCAPS_SECONDARYMONO        0x00000100
#define DSCAPS_SECONDARYSTEREO      0x00000200
#define DSCAPS_SECONDARY8BIT        0x00000400
#define DSCAPS_SECONDARY16BIT       0x00000800

// Speaker configurations
#define DSSPEAKER_HEADPHONE         1
#define DSSPEAKER_MONO              2
#define DSSPEAKER_QUAD              3
#define DSSPEAKER_STEREO            4
#define DSSPEAKER_SURROUND          5

#endif // __SDL2_DIRECTSOUND_COMPAT_H__
