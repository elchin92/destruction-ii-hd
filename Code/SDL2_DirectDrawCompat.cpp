/*******************************************************************************
 * SDL2_DirectDrawCompat.cpp
 *
 * Implementation of DirectDraw 7 → SDL2 Compatibility Layer
 *
 * This file implements the DirectDraw-compatible interfaces using SDL2.
 * The goal is to minimize changes to the original Destruction II code.
 *******************************************************************************/

#include "SDL2_DirectDrawCompat.h"
#include <SDL2/SDL_image.h>
#include <algorithm>
#include <cstring>
#include <string>
#include <cstdarg>

// DirectDraw error codes not defined in Win32 headers
#ifndef DDERR_INVALIDPARAMS
#define DDERR_INVALIDPARAMS E_INVALIDARG
#endif

// Global SDL2 renderer for convenience (set by SDL2DirectDraw)
static SDL_Renderer* g_SDLRenderer = nullptr;

// CRITICAL FIX: Add file logging for bitmap loading debugging
static FILE* g_bitmapLog = nullptr;

static void LogBitmap(const char* format, ...) {
    // CRITICAL FIX: Use va_copy to safely use va_list twice!
    // Using va_list after va_end() causes heap corruption!

    va_list args;
    va_start(args, format);

    // Print to console first
    va_list args_copy;
    va_copy(args_copy, args);
    vprintf(format, args_copy);
    va_end(args_copy);
    fflush(stdout);

    // Then print to file
    if (!g_bitmapLog) {
        g_bitmapLog = fopen("bitmap_loading.log", "w");
        if (g_bitmapLog) {
            fprintf(g_bitmapLog, "=== Bitmap Loading Log ===\n");
            fflush(g_bitmapLog);
        }
    }
    if (g_bitmapLog) {
        vfprintf(g_bitmapLog, format, args);
        fflush(g_bitmapLog);
    }

    va_end(args);
}

// ============================================================================
// SDL2Surface Implementation (IDirectDrawSurface7)
// ============================================================================

HRESULT SDL2Surface::BltFast(DWORD x, DWORD y, LPDIRECTDRAWSURFACE7 src, RECT* srcRect, DWORD flags) {
    static int bltCount = 0;
    if (bltCount < 3) {
        printf("[DEBUG] BltFast() called, dest=%p, src=%p, pos=(%d,%d)\n",
               this, src, x, y); fflush(stdout);
        bltCount++;
    }

    if (!src || !g_SDLRenderer) return DDERR_GENERIC;

    SDL2Surface* srcSurface = static_cast<SDL2Surface*>(src);

    // CRITICAL FIX: Set render target to THIS surface (destination)
    // In DirectDraw, BltFast renders TO the surface it's called on
    SDL_SetRenderTarget(g_SDLRenderer, this->texture);

    SDL_Rect sdlSrcRect;
    if (srcRect) {
        sdlSrcRect.x = srcRect->left;
        sdlSrcRect.y = srcRect->top;
        sdlSrcRect.w = srcRect->right - srcRect->left;
        sdlSrcRect.h = srcRect->bottom - srcRect->top;
    } else {
        sdlSrcRect.x = 0;
        sdlSrcRect.y = 0;
        sdlSrcRect.w = srcSurface->width;
        sdlSrcRect.h = srcSurface->height;
    }

    SDL_Rect sdlDestRect;
    sdlDestRect.x = x;
    sdlDestRect.y = y;
    sdlDestRect.w = sdlSrcRect.w;
    sdlDestRect.h = sdlSrcRect.h;

    // Handle color key transparency
    if (srcSurface->colorKeyEnabled && !(flags & DDBLTFAST_NOCOLORKEY)) {
        SDL_SetTextureBlendMode(srcSurface->texture, SDL_BLENDMODE_BLEND);
    }

    // Perform the blit
    if (SDL_RenderCopy(g_SDLRenderer, srcSurface->texture, &sdlSrcRect, &sdlDestRect) != 0) {
        return DDERR_GENERIC;
    }

    // NOTE: Do NOT reset render target here!
    // Each BltFast call will set its own target at the beginning.
    // Flip() will set target to screen when needed.

    return DD_OK;
}

HRESULT SDL2Surface::Blt(RECT* destRect, LPDIRECTDRAWSURFACE7 src, RECT* srcRect, DWORD flags, DDBLTFX* bltFx) {
    if (!g_SDLRenderer) return DDERR_GENERIC;

    // Handle fill operations (no source surface)
    if (!src && bltFx) {
        // Fill with solid color
        Uint8 r = (bltFx->dwFillColor >> 16) & 0xFF;
        Uint8 g = (bltFx->dwFillColor >> 8) & 0xFF;
        Uint8 b = bltFx->dwFillColor & 0xFF;

        SDL_SetRenderTarget(g_SDLRenderer, texture);
        SDL_SetRenderDrawColor(g_SDLRenderer, r, g, b, 255);

        if (destRect) {
            SDL_Rect sdlRect = { destRect->left, destRect->top,
                                destRect->right - destRect->left,
                                destRect->bottom - destRect->top };
            SDL_RenderFillRect(g_SDLRenderer, &sdlRect);
        } else {
            SDL_RenderClear(g_SDLRenderer);
        }

        SDL_SetRenderTarget(g_SDLRenderer, nullptr);
        return DD_OK;
    }

    if (!src) return DDERR_GENERIC;

    SDL2Surface* srcSurface = static_cast<SDL2Surface*>(src);

    SDL_Rect sdlSrcRect;
    if (srcRect) {
        sdlSrcRect.x = srcRect->left;
        sdlSrcRect.y = srcRect->top;
        sdlSrcRect.w = srcRect->right - srcRect->left;
        sdlSrcRect.h = srcRect->bottom - srcRect->top;
    } else {
        sdlSrcRect.x = 0;
        sdlSrcRect.y = 0;
        sdlSrcRect.w = srcSurface->width;
        sdlSrcRect.h = srcSurface->height;
    }

    SDL_Rect sdlDestRect;
    if (destRect) {
        sdlDestRect.x = destRect->left;
        sdlDestRect.y = destRect->top;
        sdlDestRect.w = destRect->right - destRect->left;
        sdlDestRect.h = destRect->bottom - destRect->top;
    } else {
        sdlDestRect.x = 0;
        sdlDestRect.y = 0;
        sdlDestRect.w = width;
        sdlDestRect.h = height;
    }

    // Set render target to this surface
    SDL_SetRenderTarget(g_SDLRenderer, texture);

    // Perform the blit
    if (SDL_RenderCopy(g_SDLRenderer, srcSurface->texture, &sdlSrcRect, &sdlDestRect) != 0) {
        SDL_SetRenderTarget(g_SDLRenderer, nullptr);
        return DDERR_GENERIC;
    }

    SDL_SetRenderTarget(g_SDLRenderer, nullptr);
    return DD_OK;
}

HRESULT SDL2Surface::Lock(RECT* rect, DDSURFACEDESC2* desc, DWORD flags, HANDLE event) {
    if (!desc || isLocked) return DDERR_GENERIC;

    // Create a software surface if we don't have one
    if (!surface) {
        surface = SDL_CreateRGBSurface(0, width, height, 32,
                                       0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
        if (!surface) return DDERR_GENERIC;
    }

    // Read pixels from texture to surface
    SDL_SetRenderTarget(g_SDLRenderer, texture);
    SDL_RenderReadPixels(g_SDLRenderer, nullptr, SDL_PIXELFORMAT_ARGB8888,
                        surface->pixels, surface->pitch);
    SDL_SetRenderTarget(g_SDLRenderer, nullptr);

    // Fill out the surface description
    desc->dwSize = sizeof(DDSURFACEDESC2);
    desc->dwHeight = height;
    desc->dwWidth = width;
    desc->lPitch = surface->pitch;
    desc->lpSurface = surface->pixels;

    isLocked = true;
    return DD_OK;
}

HRESULT SDL2Surface::Unlock(RECT* rect) {
    if (!isLocked || !surface) return DDERR_GENERIC;

    // Update texture from surface
    SDL_UpdateTexture(texture, nullptr, surface->pixels, surface->pitch);

    isLocked = false;
    return DD_OK;
}

HRESULT SDL2Surface::GetDC(HDC* hdc) {
    // HDC operations are not directly supported in SDL2
    // This is a stub that returns a fake HDC
    // The game will need to be modified if it relies on GDI drawing
    *hdc = (HDC)0x12345678; // Fake HDC
    return DD_OK;
}

HRESULT SDL2Surface::ReleaseDC(HDC hdc) {
    // Stub for HDC release
    return DD_OK;
}

HRESULT SDL2Surface::SetPalette(LPDIRECTDRAWPALETTE palette) {
    printf("[DEBUG] SetPalette() called with palette: %p\n", palette); fflush(stdout);
    // SDL2 doesn't use palettes in the same way
    // Modern graphics use 32-bit RGBA
    printf("[DEBUG] SetPalette() returning OK\n"); fflush(stdout);
    return DD_OK;
}

HRESULT SDL2Surface::SetColorKey(DWORD flags, DDCOLORKEY* colorKey) {
    if (colorKey) {
        colorKeyEnabled = true;
        this->colorKey = colorKey->dwColorSpaceLowValue;

        // Convert color key to RGB (DirectDraw format: 0x00RRGGBB)
        Uint8 keyR = (this->colorKey >> 16) & 0xFF;
        Uint8 keyG = (this->colorKey >> 8) & 0xFF;
        Uint8 keyB = this->colorKey & 0xFF;

        // CRITICAL FIX: Implement proper color key by modifying alpha channel
        // Read texture pixels, set alpha=0 for matching colors, alpha=255 for others

        int w, h;
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);

        // Allocate pixel buffer (RGBA8888 = 4 bytes per pixel)
        Uint32* pixels = new Uint32[w * h];

        // Set this texture as render target and read its pixels
        SDL_SetRenderTarget(g_SDLRenderer, texture);
        if (SDL_RenderReadPixels(g_SDLRenderer, nullptr, SDL_PIXELFORMAT_RGBA8888, pixels, w * 4) == 0) {
            // Process each pixel: if RGB matches key, set alpha=0, else alpha=255
            // CRITICAL: On little-endian (Windows), RGBA8888 as Uint32 = 0xAABBGGRR
            // So we need to read bytes in correct order
            Uint8* pixelBytes = (Uint8*)pixels;

            for (int i = 0; i < w * h; i++) {
                // Read RGBA bytes in memory order (little-endian safe)
                Uint8 r = pixelBytes[i * 4 + 0];  // R is first byte
                Uint8 g = pixelBytes[i * 4 + 1];  // G is second byte
                Uint8 b = pixelBytes[i * 4 + 2];  // B is third byte
                Uint8 a = pixelBytes[i * 4 + 3];  // A is fourth byte

                // If pixel color matches key color, make it transparent
                if (r == keyR && g == keyG && b == keyB) {
                    pixelBytes[i * 4 + 3] = 0;    // Set alpha = 0 (transparent)
                } else {
                    pixelBytes[i * 4 + 3] = 255;  // Set alpha = 255 (opaque)
                }
            }

            // Update texture with modified pixels
            SDL_UpdateTexture(texture, nullptr, pixels, w * 4);
        }

        // Restore render target to screen
        SDL_SetRenderTarget(g_SDLRenderer, nullptr);

        delete[] pixels;

        // Enable alpha blending for transparency to work
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    } else {
        colorKeyEnabled = false;
        // When disabling color key, just disable blending (keep pixel data as-is)
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_NONE);
    }

    return DD_OK;
}

HRESULT SDL2Surface::Restore() {
    // In SDL2, textures don't get "lost" like DirectDraw surfaces
    // This is mostly a no-op
    return DD_OK;
}

HRESULT SDL2Surface::GetFlipStatus(DWORD flags) {
    // SDL2 doesn't have explicit flip status (always ready in modern graphics)
    return DD_OK;
}

HRESULT SDL2Surface::Flip(LPDIRECTDRAWSURFACE7 backBuffer, DWORD flags) {
    // In DirectDraw, Flip swaps primary and backbuffer
    // In SDL2, we need to:
    // 1. Copy backbuffer texture to screen (renderer target)
    // 2. Present the frame

    static int flipCount = 0;

    // File logging for first 5 flips (printf doesn't work in GUI apps)
    if (flipCount < 5) {
        FILE* log = fopen("directdraw_init.log", "a");
        if (log) {
            fprintf(log, "[DEBUG] Flip() called #%d, backBuffer=%p, attachedBackBuffer=%p\n",
                   flipCount, backBuffer, attachedBackBuffer);
            fflush(log);
            fclose(log);
        }
        flipCount++;
    }

    if (!g_SDLRenderer) {
        FILE* log = fopen("directdraw_init.log", "a");
        if (log) {
            fprintf(log, "[ERROR] Flip() - g_SDLRenderer is NULL!\n");
            fflush(log);
            fclose(log);
        }
        return DDERR_GENERIC;
    }

    // Get the backbuffer (either from parameter or attached surface)
    SDL2Surface* backBuf = nullptr;
    if (backBuffer) {
        backBuf = static_cast<SDL2Surface*>(backBuffer);
    } else if (attachedBackBuffer) {
        backBuf = attachedBackBuffer;
    }

    // Set render target to screen (NULL = window)
    if (SDL_SetRenderTarget(g_SDLRenderer, nullptr) != 0) {
        if (flipCount <= 5) {
            FILE* log = fopen("directdraw_init.log", "a");
            if (log) {
                fprintf(log, "[ERROR] Flip() - SDL_SetRenderTarget failed: %s\n", SDL_GetError());
                fflush(log);
                fclose(log);
            }
        }
    }

    // Copy backbuffer to screen
    if (backBuf && backBuf->texture) {
        if (flipCount <= 5) {
            FILE* log = fopen("directdraw_init.log", "a");
            if (log) {
                fprintf(log, "[DEBUG] Flip() - copying backbuffer (%p) to screen\n", backBuf->texture);
                fflush(log);
                fclose(log);
            }
        }
        if (SDL_RenderCopy(g_SDLRenderer, backBuf->texture, nullptr, nullptr) != 0) {
            if (flipCount <= 5) {
                FILE* log = fopen("directdraw_init.log", "a");
                if (log) {
                    fprintf(log, "[ERROR] SDL_RenderCopy failed: %s\n", SDL_GetError());
                    fflush(log);
                    fclose(log);
                }
            }
        }
    }

    // Present the rendered frame to the window
    if (flipCount <= 5) {
        FILE* log = fopen("directdraw_init.log", "a");
        if (log) {
            fprintf(log, "[DEBUG] Calling SDL_RenderPresent()...\n");
            fflush(log);
            fclose(log);
        }
    }

    SDL_RenderPresent(g_SDLRenderer);

    if (flipCount <= 5) {
        FILE* log = fopen("directdraw_init.log", "a");
        if (log) {
            const char* err = SDL_GetError();
            fprintf(log, "[DEBUG] SDL_RenderPresent() completed, error: %s\n",
                   (err && err[0]) ? err : "none");
            fflush(log);
            fclose(log);
        }
    }

    return DD_OK;
}

HRESULT SDL2Surface::GetSurfaceDesc(DDSURFACEDESC2* desc) {
    if (!desc) return DDERR_INVALIDPARAMS;

    // Fill in basic surface info
    desc->dwSize = sizeof(DDSURFACEDESC2);
    desc->dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_PITCH;
    desc->dwWidth = width;
    desc->dwHeight = height;
    desc->lPitch = width * 4; // Assume 32-bit RGBA

    // Fill in pixel format (32-bit RGBA)
    desc->ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
    desc->ddpfPixelFormat.dwRGBBitCount = 32;
    desc->ddpfPixelFormat.dwRBitMask = 0x00FF0000;
    desc->ddpfPixelFormat.dwGBitMask = 0x0000FF00;
    desc->ddpfPixelFormat.dwBBitMask = 0x000000FF;
    desc->ddpfPixelFormat.dwRGBAlphaBitMask = 0xFF000000;

    return DD_OK;
}

HRESULT SDL2Surface::GetAttachedSurface(DDSCAPS2* caps, LPDIRECTDRAWSURFACE7* surface) {
    printf("[DEBUG] GetAttachedSurface() called\n"); fflush(stdout);

    if (!surface || !caps) {
        printf("[ERROR] GetAttachedSurface() - INVALID PARAMS\n"); fflush(stdout);
        return DDERR_INVALIDPARAMS;
    }

    // Check if requesting backbuffer
    if (caps->dwCaps & DDSCAPS_BACKBUFFER) {
        if (attachedBackBuffer) {
            // CRITICAL: DirectDraw COM behavior - GetAttachedSurface MUST call AddRef()
            // Backbuffer will be released by:
            // 1. Primary destructor (decrements refCount)
            // 2. ReleaseAllSurfaces() (final release)
            attachedBackBuffer->AddRef();
            *surface = attachedBackBuffer;
            printf("[DEBUG] GetAttachedSurface() - OK (AddRef called, refCount=%d)\n",
                   attachedBackBuffer->refCount);
            fflush(stdout);
            return DD_OK;
        } else {
            // No backbuffer attached
            *surface = nullptr;
            printf("[ERROR] GetAttachedSurface() - NO BACKBUFFER!\n"); fflush(stdout);
            return DDERR_GENERIC;
        }
    }

    // Other surface types not supported
    *surface = nullptr;
    printf("[DEBUG] GetAttachedSurface() - Unknown caps\n"); fflush(stdout);
    return DDERR_GENERIC;
}

// ============================================================================
// SDL2Palette Implementation
// ============================================================================

HRESULT SDL2Palette::GetEntries(DWORD flags, DWORD start, DWORD count, PALETTEENTRY* outEntries) {
    if (!outEntries || start + count > 256) {
        return DDERR_INVALIDPARAMS;
    }

    // Copy palette entries
    for (DWORD i = 0; i < count; i++) {
        DWORD color = entries[start + i];
        outEntries[i].peRed   = (color >> 16) & 0xFF;
        outEntries[i].peGreen = (color >> 8) & 0xFF;
        outEntries[i].peBlue  = color & 0xFF;
        outEntries[i].peFlags = 0;
    }

    return DD_OK;
}

HRESULT SDL2Palette::SetEntries(DWORD flags, DWORD start, DWORD count, PALETTEENTRY* inEntries) {
    if (!inEntries || start + count > 256) {
        return DDERR_INVALIDPARAMS;
    }

    // Set palette entries
    for (DWORD i = 0; i < count; i++) {
        entries[start + i] = (inEntries[i].peRed << 16) |
                             (inEntries[i].peGreen << 8) |
                             inEntries[i].peBlue;
    }

    return DD_OK;
}

// ============================================================================
// SDL2DirectDraw Implementation (IDirectDraw7)
// ============================================================================

HRESULT SDL2DirectDraw::SetCooperativeLevel(HWND hwnd, DWORD flags) {
    // File logging since printf() doesn't work in GUI apps
    FILE* log = fopen("directdraw_init.log", "a");
    if (log) {
        fprintf(log, "[DEBUG] SetCooperativeLevel() called, hwnd=%p, flags=0x%X\n", hwnd, flags);
        fflush(log);
        fclose(log);
    }

    // CRITICAL FIX: Initialize SDL with HWND on first call
    if (hwnd && !window) {
        log = fopen("directdraw_init.log", "a");
        if (log) {
            fprintf(log, "[DEBUG] First SetCooperativeLevel call - initializing SDL with HWND=%p\n", hwnd);
            fflush(log);
            fclose(log);
        }

        bool wantsFullscreen = (flags & DDSCL_FULLSCREEN) != 0;
        HRESULT hr = Initialize(hwnd, 800, 600, wantsFullscreen);
        if (FAILED(hr)) {
            log = fopen("directdraw_init.log", "a");
            if (log) {
                fprintf(log, "[ERROR] Initialize() FAILED in SetCooperativeLevel!\n");
                fflush(log);
                fclose(log);
            }
            return hr;
        }

        log = fopen("directdraw_init.log", "a");
        if (log) {
            fprintf(log, "[DEBUG] Initialize() OK in SetCooperativeLevel\n");
            fflush(log);
            fclose(log);
        }
    }

    // Store Win32 window handle
    if (hwnd && !originalWin32Window) {
        originalWin32Window = hwnd;
    }

    // DON'T hide Win32 window if we're using it for rendering
    // (SDL_CreateWindowFrom renders into the Win32 window)

    // FORCE WINDOWED MODE for debugging - ignore fullscreen flag
    fullscreen = false;

    // Keep window in windowed mode
    if (window) {
        SDL_SetWindowFullscreen(window, 0);  // 0 = windowed
    }

    log = fopen("directdraw_init.log", "a");
    if (log) {
        fprintf(log, "[DEBUG] SetCooperativeLevel() SUCCESS\n");
        fflush(log);
        fclose(log);
    }

    return DD_OK;
}

HRESULT SDL2DirectDraw::SetDisplayMode(DWORD width, DWORD height, DWORD bpp, DWORD refreshRate, DWORD flags) {
    screenWidth = width;
    screenHeight = height;

    if (window) {
        SDL_SetWindowSize(window, width, height);

        if (fullscreen) {
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        } else {
            SDL_SetWindowFullscreen(window, 0);
        }
    }

    return DD_OK;
}

HRESULT SDL2DirectDraw::CreateSurface(DDSURFACEDESC2* desc, LPDIRECTDRAWSURFACE7* surface, void* unkOuter) {
    printf("[DEBUG] CreateSurface() called\n"); fflush(stdout);

    if (!desc || !surface) return DDERR_GENERIC;

    int width = desc->dwWidth;
    int height = desc->dwHeight;

    // Default dimensions if not specified
    if (width == 0) width = screenWidth;
    if (height == 0) height = screenHeight;

    printf("[DEBUG] Creating surface: %dx%d\n", width, height); fflush(stdout);

    // Create SDL texture with RGBA8888 format (compatible with loaded bitmaps)
    SDL_Texture* texture = SDL_CreateTexture(renderer,
                                            SDL_PIXELFORMAT_RGBA8888,
                                            SDL_TEXTUREACCESS_TARGET,
                                            width, height);
    if (!texture) {
        printf("[ERROR] SDL_CreateTexture FAILED!\n"); fflush(stdout);
        return DDERR_GENERIC;
    }

    // Create our surface wrapper
    SDL2Surface* newSurface = new SDL2Surface(texture, width, height);

    // Handle primary surface with back buffer
    if (desc->dwFlags & DDSD_CAPS) {
        if (desc->ddsCaps.dwCaps & DDSCAPS_PRIMARYSURFACE) {
            printf("[DEBUG] Creating PRIMARY surface\n"); fflush(stdout);
            primarySurface = newSurface;

            // Create back buffer if flippable (double buffering)
            if ((desc->ddsCaps.dwCaps & DDSCAPS_FLIP) &&
                (desc->dwFlags & DDSD_BACKBUFFERCOUNT) &&
                (desc->dwBackBufferCount > 0)) {
                printf("[DEBUG] Creating BACKBUFFER\n"); fflush(stdout);
                SDL_Texture* backTex = SDL_CreateTexture(renderer,
                                                        SDL_PIXELFORMAT_RGBA8888,
                                                        SDL_TEXTUREACCESS_TARGET,
                                                        width, height);
                if (backTex) {
                    SDL2Surface* backBuf = new SDL2Surface(backTex, width, height);
                    // CRITICAL: Store backbuffer in primary surface so GetAttachedSurface() can find it
                    newSurface->attachedBackBuffer = backBuf;
                    backBuffer = backBuf;  // Also store in DirectDraw object for global access
                    printf("[DEBUG] Backbuffer created and attached OK\n"); fflush(stdout);
                } else {
                    printf("[ERROR] Backbuffer texture creation FAILED!\n"); fflush(stdout);
                }
            }
        }
    }

    *surface = newSurface;
    printf("[DEBUG] CreateSurface() SUCCESS\n"); fflush(stdout);
    return DD_OK;
}

HRESULT SDL2DirectDraw::CreatePalette(DWORD flags, PALETTEENTRY* entries, LPDIRECTDRAWPALETTE* palette, void* unkOuter) {
    if (!palette) return DDERR_INVALIDPARAMS;

    // Create new palette object
    SDL2Palette* newPalette = new SDL2Palette();

    // Copy palette entries if provided
    if (entries) {
        for (int i = 0; i < 256; i++) {
            // C++20: Convert PALETTEENTRY to DWORD (RGBA packed)
            newPalette->entries[i] = (entries[i].peRed) | (entries[i].peGreen << 8) | (entries[i].peBlue << 16) | (entries[i].peFlags << 24);
        }
    }

    *palette = newPalette;
    return DD_OK;
}

HRESULT SDL2DirectDraw::RestoreDisplayMode() {
    if (window) {
        SDL_SetWindowFullscreen(window, 0);
    }
    return DD_OK;
}

HRESULT SDL2DirectDraw::FlipToGDISurface() {
    // Present the current frame
    SDL_RenderPresent(renderer);
    return DD_OK;
}

HRESULT SDL2DirectDraw::Initialize(HWND hwnd, int width, int height, bool fullscreen) {
    // File logging since printf() doesn't work in GUI apps
    FILE* log = fopen("directdraw_init.log", "a");
    if (log) {
        fprintf(log, "[DEBUG] Initialize() START, hwnd=%p, size=%dx%d\n", hwnd, width, height);
        fflush(log);
        fclose(log);
    }

    // Disable HiDPI scaling - we want exact pixel control like Windows XP
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "1");
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");  // Nearest pixel sampling

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) < 0) {
        log = fopen("directdraw_init.log", "a");
        if (log) {
            fprintf(log, "[ERROR] SDL_Init FAILED!\n");
            fflush(log);
            fclose(log);
        }
        return DDERR_GENERIC;
    }
    log = fopen("directdraw_init.log", "a");
    if (log) {
        fprintf(log, "[DEBUG] SDL_Init OK\n");
        fflush(log);
        fclose(log);
    }

    // CRITICAL FIX: Use existing Win32 window instead of creating a new one
    // This prevents the "two windows" problem
    printf("[DEBUG] Attaching SDL to existing Win32 window (HWND=%p)\n", hwnd); fflush(stdout);

    if (hwnd) {
        // Attach SDL2 to the existing Win32 window
        window = SDL_CreateWindowFrom((void*)hwnd);
        if (!window) {
            printf("[ERROR] SDL_CreateWindowFrom FAILED: %s\n", SDL_GetError()); fflush(stdout);
            printf("[FALLBACK] Creating standalone SDL window\n"); fflush(stdout);

            // Fallback: create standalone window
            Uint32 windowFlags = SDL_WINDOW_SHOWN;
            if (fullscreen) {
                windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
            }
            window = SDL_CreateWindow("Destruction II - Resurrection",
                                     SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                     width, height, windowFlags);
        } else {
            printf("[DEBUG] SDL_CreateWindowFrom OK\n"); fflush(stdout);
        }
    } else {
        // No HWND provided, create standalone
        printf("[DEBUG] No HWND, creating standalone SDL window\n"); fflush(stdout);
        Uint32 windowFlags = SDL_WINDOW_SHOWN;
        if (fullscreen) {
            windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
        }
        window = SDL_CreateWindow("Destruction II - Resurrection",
                                 SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                 width, height, windowFlags);
    }

    if (!window) {
        printf("[ERROR] Failed to create window\n"); fflush(stdout);
        SDL_Quit();
        return DDERR_GENERIC;
    }
    printf("[DEBUG] Window created successfully\n"); fflush(stdout);

    // Check actual window size
    int actualWidth, actualHeight;
    SDL_GetWindowSize(window, &actualWidth, &actualHeight);
    printf("[DEBUG] Window size: requested=%dx%d, actual=%dx%d\n", width, height, actualWidth, actualHeight); fflush(stdout);

    // Create renderer
    // CRITICAL FIX: Use SOFTWARE renderer for maximum DirectDraw compatibility
    // Hardware renderers may not support SDL_TEXTUREACCESS_TARGET on all drivers
    // For 800x600 2D game from 2003, software rendering is more than fast enough
    log = fopen("directdraw_init.log", "a");
    if (log) {
        fprintf(log, "[DEBUG] Creating SDL_Renderer with SOFTWARE mode...\n");
        fflush(log);
        fclose(log);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (!renderer) {
        log = fopen("directdraw_init.log", "a");
        if (log) {
            fprintf(log, "[ERROR] SDL_CreateRenderer FAILED: %s\n", SDL_GetError());
            fflush(log);
            fclose(log);
        }
        SDL_DestroyWindow(window);
        SDL_Quit();
        return DDERR_GENERIC;
    }

    log = fopen("directdraw_init.log", "a");
    if (log) {
        fprintf(log, "[DEBUG] SDL_CreateRenderer SUCCESS (SOFTWARE mode)\n");
        fflush(log);
        fclose(log);
    }

    // Check renderer output size (may differ on HiDPI displays)
    int outputWidth, outputHeight;
    SDL_GetRendererOutputSize(renderer, &outputWidth, &outputHeight);

    log = fopen("directdraw_init.log", "a");
    if (log) {
        fprintf(log, "[DEBUG] Renderer output size: %dx%d\n", outputWidth, outputHeight);
        fflush(log);
        fclose(log);
    }

    // CRITICAL FIX: Set logical size to game resolution (800x600)
    // This makes SDL automatically scale the 800x600 game content to fit the window/screen
    // Without this, the game renders at native monitor resolution (e.g., 1920x1080) and looks wrong
    if (SDL_RenderSetLogicalSize(renderer, width, height) != 0) {
        log = fopen("directdraw_init.log", "a");
        if (log) {
            fprintf(log, "[WARNING] SDL_RenderSetLogicalSize failed: %s\n", SDL_GetError());
            fflush(log);
            fclose(log);
        }
    } else {
        log = fopen("directdraw_init.log", "a");
        if (log) {
            fprintf(log, "[DEBUG] SDL_RenderSetLogicalSize set to %dx%d (game will auto-scale)\n", width, height);
            fflush(log);
            fclose(log);
        }
    }

    // Store renderer globally for surface operations
    g_SDLRenderer = renderer;

    screenWidth = width;
    screenHeight = height;
    this->fullscreen = fullscreen;

    log = fopen("directdraw_init.log", "a");
    if (log) {
        fprintf(log, "[DEBUG] Initialize() SUCCESS (stored: %dx%d)\n", screenWidth, screenHeight);
        fflush(log);
        fclose(log);
    }
    return DD_OK;
}

// ============================================================================
// Global DirectDraw Functions
// ============================================================================

HRESULT DirectDrawCreateEx(GUID* guid, LPVOID* ddraw, REFIID iid, IUnknown* unkOuter) {
    // File logging since printf() doesn't work in GUI apps
    FILE* log = fopen("directdraw_init.log", "a");
    if (log) {
        fprintf(log, "[DEBUG] DirectDrawCreateEx() called\n");
        fflush(log);
        fclose(log);
    }

    if (!ddraw) return DDERR_GENERIC;

    SDL2DirectDraw* dd = new SDL2DirectDraw();

    // CRITICAL: Do NOT call Initialize() here!
    // Initialize() will be called later in SetCooperativeLevel() when HWND is available
    // DirectDrawCreateEx just creates the object - window creation happens in SetCooperativeLevel

    *ddraw = dd;

    log = fopen("directdraw_init.log", "a");
    if (log) {
        fprintf(log, "[DEBUG] DirectDrawCreateEx() SUCCESS - waiting for SetCooperativeLevel()\n");
        fflush(log);
        fclose(log);
    }

    return DD_OK;
}

// ============================================================================
// Helper Functions (ddutil.h implementations)
// ============================================================================

IDirectDrawPalette* DDLoadPalette(IDirectDraw7* dd, LPCSTR bitmapPath) {
    printf("[DEBUG] DDLoadPalette() called for: %s\n", bitmapPath ? bitmapPath : "(null)"); fflush(stdout);
    // SDL2 doesn't use palettes - return a dummy palette
    SDL2Palette* pal = new SDL2Palette();
    printf("[DEBUG] DDLoadPalette() returning palette: %p\n", pal); fflush(stdout);
    return pal;
}

IDirectDrawSurface7* DDLoadBitmap(IDirectDraw7* dd, LPCSTR bitmapPath, int width, int height) {
    // CRITICAL DEBUG: Log current working directory on first call
    static bool firstCall = true;
    if (firstCall) {
        char cwd[1024];
        if (GetCurrentDirectoryA(sizeof(cwd), cwd)) {
            LogBitmap("[DEBUG] Current working directory: %s\n", cwd);
        }
        firstCall = false;
    }

    LogBitmap("[DEBUG] DDLoadBitmap() called for: %s\n", bitmapPath ? bitmapPath : "(null)");

    if (!g_SDLRenderer) {
        LogBitmap("[ERROR] DDLoadBitmap() - g_SDLRenderer is NULL!\n");
        return nullptr;
    }
    if (!bitmapPath) {
        LogBitmap("[ERROR] DDLoadBitmap() - bitmapPath is NULL!\n");
        return nullptr;
    }

    // Load surface from file
    // Try multiple paths: as-is, with .bmp, and in Bitmaps/ subdirectory
    SDL_Surface* loadedSurface = nullptr;
    std::string path(bitmapPath);

    // 1. Try as-is first
    LogBitmap("[DEBUG] Calling IMG_Load(%s)\n", bitmapPath);
    loadedSurface = IMG_Load(bitmapPath);

    // 2. Try with .bmp extension
    if (!loadedSurface && path.find('.') == std::string::npos) {
        std::string pathWithExt = path + ".bmp";
        LogBitmap("[DEBUG] IMG_Load failed, trying with .bmp extension: %s\n", pathWithExt.c_str());
        loadedSurface = IMG_Load(pathWithExt.c_str());
    }

    // 3. Try in Bitmaps/ subdirectory
    if (!loadedSurface) {
        std::string bitmapPath2 = "Bitmaps/" + path;
        LogBitmap("[DEBUG] Still failed, trying in Bitmaps/ subdirectory: %s\n", bitmapPath2.c_str());
        loadedSurface = IMG_Load(bitmapPath2.c_str());

        // 4. Try Bitmaps/ + .bmp
        if (!loadedSurface && path.find('.') == std::string::npos) {
            bitmapPath2 = "Bitmaps/" + path + ".bmp";
            LogBitmap("[DEBUG] Trying Bitmaps/ with .bmp: %s\n", bitmapPath2.c_str());
            loadedSurface = IMG_Load(bitmapPath2.c_str());
        }
    }

    // 5. CRITICAL FIX: Try in bin/Debug/Bitmaps/ (when working directory is build/)
    if (!loadedSurface) {
        std::string bitmapPath3 = "bin/Debug/Bitmaps/" + path;
        LogBitmap("[DEBUG] Still failed, trying bin/Debug/Bitmaps/: %s\n", bitmapPath3.c_str());
        loadedSurface = IMG_Load(bitmapPath3.c_str());

        // 6. Try bin/Debug/Bitmaps/ + .bmp
        if (!loadedSurface && path.find('.') == std::string::npos) {
            bitmapPath3 = "bin/Debug/Bitmaps/" + path + ".bmp";
            LogBitmap("[DEBUG] Trying bin/Debug/Bitmaps/ with .bmp: %s\n", bitmapPath3.c_str());
            loadedSurface = IMG_Load(bitmapPath3.c_str());
        }
    }

    // 7. CRITICAL FIX: Try in bin/Release/Bitmaps/ (when working directory is build/)
    if (!loadedSurface) {
        std::string bitmapPath4 = "bin/Release/Bitmaps/" + path;
        LogBitmap("[DEBUG] Still failed, trying bin/Release/Bitmaps/: %s\n", bitmapPath4.c_str());
        loadedSurface = IMG_Load(bitmapPath4.c_str());

        // 8. Try bin/Release/Bitmaps/ + .bmp
        if (!loadedSurface && path.find('.') == std::string::npos) {
            bitmapPath4 = "bin/Release/Bitmaps/" + path + ".bmp";
            LogBitmap("[DEBUG] Trying bin/Release/Bitmaps/ with .bmp: %s\n", bitmapPath4.c_str());
            loadedSurface = IMG_Load(bitmapPath4.c_str());
        }
    }

    if (!loadedSurface) {
        LogBitmap("[ERROR] DDLoadBitmap() - IMG_Load failed for %s: %s\n", bitmapPath, SDL_GetError());
        return nullptr;
    }

    LogBitmap("[DEBUG] IMG_Load OK, surface: %dx%d, format=%s\n",
           loadedSurface->w, loadedSurface->h,
           SDL_GetPixelFormatName(loadedSurface->format->format));

    int w = loadedSurface->w;
    int h = loadedSurface->h;

    // CRITICAL FIX: Convert surface to RGBA8888 format if needed
    // Old games use 8-bit paletted BMPs, need to convert to RGBA
    SDL_Surface* convertedSurface = loadedSurface;
    if (loadedSurface->format->format != SDL_PIXELFORMAT_RGBA8888 &&
        loadedSurface->format->format != SDL_PIXELFORMAT_ARGB8888) {
        LogBitmap("[DEBUG] Converting surface from %s to RGBA8888\n",
               SDL_GetPixelFormatName(loadedSurface->format->format));
        convertedSurface = SDL_ConvertSurfaceFormat(loadedSurface, SDL_PIXELFORMAT_RGBA8888, 0);
        SDL_FreeSurface(loadedSurface);  // Free original

        if (!convertedSurface) {
            LogBitmap("[ERROR] DDLoadBitmap() - SDL_ConvertSurfaceFormat failed: %s\n", SDL_GetError());
            return nullptr;
        }
        LogBitmap("[DEBUG] Conversion OK\n");
    }

    // CRITICAL FIX: Create texture with TARGET access so it can be used as render target
    // SDL_CreateTextureFromSurface creates STATIC textures which cannot be render targets!
    LogBitmap("[DEBUG] Creating texture with TARGET access (%dx%d)\n", w, h);
    SDL_Texture* texture = SDL_CreateTexture(g_SDLRenderer,
                                             SDL_PIXELFORMAT_RGBA8888,
                                             SDL_TEXTUREACCESS_TARGET,
                                             w, h);

    if (!texture) {
        LogBitmap("[ERROR] DDLoadBitmap() - SDL_CreateTexture failed: %s\n", SDL_GetError());
        SDL_FreeSurface(convertedSurface);
        return nullptr;
    }

    // Copy loaded image data into the texture
    LogBitmap("[DEBUG] Copying surface data to texture\n");
    if (SDL_UpdateTexture(texture, nullptr, convertedSurface->pixels, convertedSurface->pitch) != 0) {
        LogBitmap("[ERROR] DDLoadBitmap() - SDL_UpdateTexture failed: %s\n", SDL_GetError());
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(convertedSurface);
        return nullptr;
    }

    SDL_FreeSurface(convertedSurface);

    LogBitmap("[DEBUG] Texture created, creating SDL2Surface wrapper\n");
    // Create surface wrapper
    SDL2Surface* result = new SDL2Surface(texture, w, h);
    LogBitmap("[DEBUG] DDLoadBitmap() returning surface: %p\n", result);
    return result;
}

HRESULT DDReLoadBitmap(IDirectDrawSurface7* surface, LPCSTR bitmapPath) {
    if (!surface || !bitmapPath || !g_SDLRenderer) return DDERR_GENERIC;

    SDL2Surface* sdlSurface = static_cast<SDL2Surface*>(surface);

    // Load new image
    SDL_Surface* loadedSurface = IMG_Load(bitmapPath);
    if (!loadedSurface) {
        std::string path(bitmapPath);
        if (path.find('.') == std::string::npos) {
            path += ".bmp";
            loadedSurface = IMG_Load(path.c_str());
        }
    }

    if (!loadedSurface) return DDERR_GENERIC;

    // Update texture
    SDL_UpdateTexture(sdlSurface->texture, nullptr,
                     loadedSurface->pixels, loadedSurface->pitch);
    SDL_FreeSurface(loadedSurface);

    return DD_OK;
}

HRESULT DDCopyBitmap(IDirectDrawSurface7* surface, HBITMAP hbm, int x, int y, int width, int height) {
    // This would require Windows GDI to SDL conversion
    // Not implemented for now - the game will need modification if it uses this
    return DDERR_GENERIC;
}

DWORD DDColorMatch(IDirectDrawSurface7* surface, COLORREF rgb) {
    // Convert Windows COLORREF to SDL color format
    return (0xFF000000 | ((rgb & 0xFF) << 16) | (rgb & 0xFF00) | ((rgb & 0xFF0000) >> 16));
}

HRESULT DDSetColorKey(IDirectDrawSurface7* surface, COLORREF rgb) {
    printf("[DEBUG] DDSetColorKey() called, surface=%p, rgb=0x%06X\n", surface, rgb); fflush(stdout);

    if (!surface) {
        printf("[ERROR] DDSetColorKey() - surface is NULL!\n"); fflush(stdout);
        return DDERR_GENERIC;
    }

    SDL2Surface* sdlSurface = static_cast<SDL2Surface*>(surface);

    DDCOLORKEY colorKey;
    colorKey.dwColorSpaceLowValue = DDColorMatch(surface, rgb);
    colorKey.dwColorSpaceHighValue = colorKey.dwColorSpaceLowValue;

    printf("[DEBUG] DDSetColorKey() calling SetColorKey\n"); fflush(stdout);
    HRESULT result = sdlSurface->SetColorKey(0, &colorKey);
    printf("[DEBUG] DDSetColorKey() returning\n"); fflush(stdout);
    return result;
}