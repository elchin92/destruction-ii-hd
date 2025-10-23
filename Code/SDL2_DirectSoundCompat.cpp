/*******************************************************************************
 * SDL2_DirectSoundCompat.cpp
 *
 * Implementation of DirectSound → SDL2_mixer Compatibility Layer
 *
 * This file implements DirectSound-compatible interfaces using SDL2_mixer.
 *******************************************************************************/

#include "SDL2_DirectSoundCompat.h"
#include <algorithm>
#include <cmath>

// ============================================================================
// Helper Functions
// ============================================================================

// Convert DirectSound volume (-10000 to 0 in hundredths of dB) to SDL_mixer (0-128)
static int ConvertVolume(LONG dsVolume) {
    if (dsVolume <= -10000) return 0;
    if (dsVolume >= 0) return MIX_MAX_VOLUME;

    // DirectSound uses hundredths of dB: -10000 = silence, 0 = full volume
    // Formula: 10^(dB/2000) where dB is in hundredths
    double factor = pow(10.0, dsVolume / 2000.0);
    return static_cast<int>(factor * MIX_MAX_VOLUME);
}

// Convert SDL_mixer volume (0-128) to DirectSound (-10000 to 0)
static LONG ConvertToDirectSoundVolume(int mixVolume) {
    if (mixVolume <= 0) return -10000;
    if (mixVolume >= MIX_MAX_VOLUME) return 0;

    double factor = static_cast<double>(mixVolume) / MIX_MAX_VOLUME;
    return static_cast<LONG>(2000.0 * log10(factor));
}

// Convert DirectSound pan (-10000 to +10000) to SDL_mixer (0-254)
static int ConvertPan(LONG dsPan) {
    // DirectSound: -10000 = full left, 0 = center, +10000 = full right
    // SDL_mixer: 0 = full left, 127 = center, 254 = full right
    if (dsPan <= -10000) return 0;
    if (dsPan >= 10000) return 254;

    return static_cast<int>(127 + (dsPan * 127.0 / 10000.0));
}

// Convert SDL_mixer pan (0-254) to DirectSound (-10000 to +10000)
static LONG ConvertToDirectSoundPan(int mixPan) {
    return static_cast<LONG>((mixPan - 127) * 10000.0 / 127.0);
}

// ============================================================================
// SDL2SoundBuffer Implementation
// ============================================================================

HRESULT SDL2SoundBuffer::GetCaps(LPDSBCAPS caps) {
    if (!caps) return DSERR_INVALIDPARAM;

    caps->dwSize = sizeof(DSBCAPS);
    caps->dwFlags = flags;
    caps->dwBufferBytes = bufferSize;
    caps->dwUnlockTransferRate = 0;
    caps->dwPlayCpuOverhead = 0;

    return DS_OK;
}

HRESULT SDL2SoundBuffer::GetCurrentPosition(LPDWORD playPos, LPDWORD writePos) {
    if (!playPos && !writePos) return DSERR_INVALIDPARAM;

    // SDL_mixer doesn't provide exact position tracking
    // Return approximations
    if (playPos) *playPos = 0;
    if (writePos) *writePos = 0;

    return DS_OK;
}

HRESULT SDL2SoundBuffer::GetFormat(LPWAVEFORMATEX waveFormat, DWORD size, LPDWORD written) {
    if (!format) return DSERR_GENERIC;

    if (waveFormat && size >= sizeof(WAVEFORMATEX)) {
        memcpy(waveFormat, format, sizeof(WAVEFORMATEX));
    }

    if (written) {
        *written = sizeof(WAVEFORMATEX);
    }

    return DS_OK;
}

HRESULT SDL2SoundBuffer::GetVolume(LPLONG volume) {
    if (!volume) return DSERR_INVALIDPARAM;

    *volume = ConvertToDirectSoundVolume(this->volume);
    return DS_OK;
}

HRESULT SDL2SoundBuffer::GetPan(LPLONG pan) {
    if (!pan) return DSERR_INVALIDPARAM;

    *pan = ConvertToDirectSoundPan(this->pan);
    return DS_OK;
}

HRESULT SDL2SoundBuffer::GetFrequency(LPDWORD frequency) {
    if (!frequency) return DSERR_INVALIDPARAM;
    if (!format) return DSERR_GENERIC;

    *frequency = format->nSamplesPerSec;
    return DS_OK;
}

HRESULT SDL2SoundBuffer::GetStatus(LPDWORD status) {
    if (!status) return DSERR_INVALIDPARAM;

    *status = 0;

    if (isLost) {
        *status |= DSBSTATUS_BUFFERLOST;
        return DS_OK;
    }

    if (channel >= 0 && Mix_Playing(channel)) {
        *status |= DSBSTATUS_PLAYING;

        // Check if looping (we track this in Play() call)
        // Note: SDL_mixer doesn't expose loop status directly
    }

    return DS_OK;
}

HRESULT SDL2SoundBuffer::Initialize(LPDIRECTSOUND ds, LPDSBUFFERDESC desc) {
    // Already initialized in CreateSoundBuffer
    return DS_OK;
}

HRESULT SDL2SoundBuffer::Lock(DWORD offset, DWORD bytes, LPVOID* audioPtr1, LPDWORD audioBytes1,
                               LPVOID* audioPtr2, LPDWORD audioBytes2, DWORD flags) {
    if (!audioPtr1 || !audioBytes1) return DSERR_INVALIDPARAM;

    if (isLocked) return DSERR_GENERIC;

    // Allocate streaming buffer if needed
    if (!streamBuffer) {
        streamSize = bufferSize;
        streamBuffer = new Uint8[streamSize];
        memset(streamBuffer, 0, streamSize);
    }

    // Handle wrapping
    if (offset + bytes <= streamSize) {
        // No wrap
        *audioPtr1 = streamBuffer + offset;
        *audioBytes1 = bytes;
        if (audioPtr2) *audioPtr2 = nullptr;
        if (audioBytes2) *audioBytes2 = 0;
    } else {
        // Wrap around
        *audioPtr1 = streamBuffer + offset;
        *audioBytes1 = streamSize - offset;
        if (audioPtr2) *audioPtr2 = streamBuffer;
        if (audioBytes2) *audioBytes2 = bytes - *audioBytes1;
    }

    isLocked = true;
    return DS_OK;
}

HRESULT SDL2SoundBuffer::Play(DWORD reserved1, DWORD priority, DWORD flags) {
    if (isPrimary) {
        // Primary buffer doesn't play
        return DS_OK;
    }

    if (!chunk) return DSERR_GENERIC;

    // Determine loop count
    int loops = (flags & DSBPLAY_LOOPING) ? -1 : 0;

    // Play on any available channel
    channel = Mix_PlayChannel(-1, chunk, loops);

    if (channel < 0) {
        return DSERR_GENERIC;
    }

    // Apply volume and pan
    Mix_Volume(channel, volume);
    Mix_SetPanning(channel, 254 - pan, pan);  // SDL_mixer uses (left, right)

    return DS_OK;
}

HRESULT SDL2SoundBuffer::SetCurrentPosition(DWORD newPos) {
    // SDL_mixer doesn't support seeking within a sound
    // This is mostly a no-op
    return DS_OK;
}

HRESULT SDL2SoundBuffer::SetFormat(LPWAVEFORMATEX waveFormat) {
    if (!waveFormat) return DSERR_INVALIDPARAM;

    if (!format) {
        format = new WAVEFORMATEX;
    }

    memcpy(format, waveFormat, sizeof(WAVEFORMATEX));
    return DS_OK;
}

HRESULT SDL2SoundBuffer::SetVolume(LONG volume) {
    this->volume = ConvertVolume(volume);

    // Apply to currently playing sound
    if (channel >= 0) {
        Mix_Volume(channel, this->volume);
    }

    return DS_OK;
}

HRESULT SDL2SoundBuffer::SetPan(LONG pan) {
    this->pan = ConvertPan(pan);

    // Apply to currently playing sound
    if (channel >= 0) {
        int left = 254 - this->pan;
        int right = this->pan;
        Mix_SetPanning(channel, left, right);
    }

    return DS_OK;
}

HRESULT SDL2SoundBuffer::SetFrequency(DWORD frequency) {
    // SDL_mixer doesn't support changing frequency of individual chunks
    // This would require resampling the audio data
    return DS_OK;
}

HRESULT SDL2SoundBuffer::Stop() {
    if (channel >= 0) {
        Mix_HaltChannel(channel);
        channel = -1;
    }

    return DS_OK;
}

HRESULT SDL2SoundBuffer::Unlock(LPVOID audioPtr1, DWORD audioBytes1, LPVOID audioPtr2, DWORD audioBytes2) {
    if (!isLocked) return DSERR_GENERIC;

    // If we have stream data, create a Mix_Chunk from it
    if (streamBuffer) {
        // Free old chunk if it exists
        if (chunk) {
            Mix_FreeChunk(chunk);
        }

        // Create new chunk from buffer
        // Note: Mix_QuickLoad_WAV doesn't copy data, so we keep streamBuffer alive
        chunk = Mix_QuickLoad_WAV(streamBuffer);

        if (!chunk) {
            return DSERR_GENERIC;
        }
    }

    isLocked = false;
    return DS_OK;
}

HRESULT SDL2SoundBuffer::Restore() {
    // SDL_mixer buffers don't get "lost"
    isLost = false;
    return DS_OK;
}

// ============================================================================
// SDL2DirectSound Implementation
// ============================================================================

HRESULT SDL2DirectSound::CreateSoundBuffer(LPDSBUFFERDESC desc, LPLPDIRECTSOUNDBUFFER buffer, IUnknown* unkOuter) {
    if (!desc || !buffer) return DSERR_INVALIDPARAM;

    SDL2SoundBuffer* newBuffer = new SDL2SoundBuffer();

    newBuffer->flags = desc->dwFlags;
    newBuffer->bufferSize = desc->dwBufferBytes;

    // Check if this is a primary buffer
    if (desc->dwFlags & DSBCAPS_PRIMARYBUFFER) {
        newBuffer->isPrimary = true;
        // Primary buffer doesn't need actual audio data
        *buffer = newBuffer;
        buffers.push_back(newBuffer);
        return DS_OK;
    }

    // Secondary buffer - store format
    if (desc->lpwfxFormat) {
        newBuffer->format = new WAVEFORMATEX;
        memcpy(newBuffer->format, desc->lpwfxFormat, sizeof(WAVEFORMATEX));
    }

    // Buffer will be filled via Lock/Unlock or loaded separately

    *buffer = newBuffer;
    buffers.push_back(newBuffer);

    return DS_OK;
}

HRESULT SDL2DirectSound::GetCaps(LPDSCAPS caps) {
    if (!caps) return DSERR_INVALIDPARAM;

    memset(caps, 0, sizeof(DSCAPS));
    caps->dwSize = sizeof(DSCAPS);

    // Report capabilities
    caps->dwFlags = DSCAPS_PRIMARYSTEREO | DSCAPS_PRIMARY16BIT |
                    DSCAPS_SECONDARYSTEREO | DSCAPS_SECONDARY16BIT |
                    DSCAPS_CONTINUOUSRATE;

    caps->dwMinSecondarySampleRate = 11025;
    caps->dwMaxSecondarySampleRate = 48000;
    caps->dwPrimaryBuffers = 1;
    caps->dwMaxHwMixingAllBuffers = 32;  // SDL_mixer default channels

    return DS_OK;
}

HRESULT SDL2DirectSound::DuplicateSoundBuffer(LPDIRECTSOUNDBUFFER original, LPLPDIRECTSOUNDBUFFER duplicate) {
    if (!original || !duplicate) return DSERR_INVALIDPARAM;

    SDL2SoundBuffer* origBuffer = static_cast<SDL2SoundBuffer*>(original);
    SDL2SoundBuffer* newBuffer = new SDL2SoundBuffer();

    // Copy properties
    newBuffer->flags = origBuffer->flags;
    newBuffer->bufferSize = origBuffer->bufferSize;
    newBuffer->isPrimary = origBuffer->isPrimary;
    newBuffer->volume = origBuffer->volume;
    newBuffer->pan = origBuffer->pan;

    // Copy format
    if (origBuffer->format) {
        newBuffer->format = new WAVEFORMATEX;
        memcpy(newBuffer->format, origBuffer->format, sizeof(WAVEFORMATEX));
    }

    // Share the same Mix_Chunk (SDL_mixer handles this)
    newBuffer->chunk = origBuffer->chunk;

    *duplicate = newBuffer;
    buffers.push_back(newBuffer);

    return DS_OK;
}

HRESULT SDL2DirectSound::SetCooperativeLevel(HWND hwnd, DWORD level) {
    this->hwnd = hwnd;
    this->cooperativeLevel = level;

    // SDL_mixer handles audio system cooperation internally
    return DS_OK;
}

HRESULT SDL2DirectSound::Compact() {
    // SDL_mixer manages its own memory
    return DS_OK;
}

HRESULT SDL2DirectSound::GetSpeakerConfig(LPDWORD config) {
    if (!config) return DSERR_INVALIDPARAM;

    // Default to stereo
    *config = DSSPEAKER_STEREO;
    return DS_OK;
}

HRESULT SDL2DirectSound::SetSpeakerConfig(DWORD config) {
    // SDL_mixer configuration is set at initialization
    return DS_OK;
}

HRESULT SDL2DirectSound::Initialize(GUID* guid) {
    if (initialized) {
        return DS_OK;  // Already initialized
    }

    // Initialize SDL audio if not already done
    if (SDL_WasInit(SDL_INIT_AUDIO) == 0) {
        if (SDL_Init(SDL_INIT_AUDIO) < 0) {
            return DSERR_GENERIC;
        }
    }

    // Initialize SDL_mixer
    // Format: 44.1kHz, 16-bit, stereo, 2048 byte chunks
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        return DSERR_GENERIC;
    }

    // Allocate mixing channels
    Mix_AllocateChannels(32);  // Support up to 32 simultaneous sounds

    initialized = true;
    return DS_OK;
}

// ============================================================================
// Global DirectSound Functions
// ============================================================================

HRESULT DirectSoundCreate(GUID* guid, LPDIRECTSOUND* ds, IUnknown* unkOuter) {
    if (!ds) return DSERR_INVALIDPARAM;

    SDL2DirectSound* newDS = new SDL2DirectSound();

    // Initialize immediately
    HRESULT hr = newDS->Initialize(guid);
    if (FAILED(hr)) {
        delete newDS;
        return hr;
    }

    *ds = newDS;
    return DS_OK;
}
