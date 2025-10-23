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

// Global SDL2 renderer for convenience (set by SDL2DirectDraw)
static SDL_Renderer* g_SDLRenderer = nullptr;

// ============================================================================
// SDL2Surface Implementation (IDirectDrawSurface7)
// ============================================================================

HRESULT SDL2Surface::BltFast(DWORD x, DWORD y, LPDIRECTDRAWSURFACE7 src, RECT* srcRect, DWORD flags) {
    if (!src || !g_SDLRenderer) return DDERR_GENERIC;

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
    // SDL2 doesn't use palettes in the same way
    // Modern graphics use 32-bit RGBA
    return DD_OK;
}

HRESULT SDL2Surface::SetColorKey(DWORD flags, DDCOLORKEY* colorKey) {
    if (colorKey) {
        colorKeyEnabled = true;
        this->colorKey = colorKey->dwColorSpaceLowValue;

        // Convert color key to RGB
        Uint8 r = (this->colorKey >> 16) & 0xFF;
        Uint8 g = (this->colorKey >> 8) & 0xFF;
        Uint8 b = this->colorKey & 0xFF;

        // Set color key for transparency
        SDL_SetTextureColorMod(texture, r, g, b);
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    } else {
        colorKeyEnabled = false;
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_NONE);
    }

    return DD_OK;
}

HRESULT SDL2Surface::Restore() {
    // In SDL2, textures don't get "lost" like DirectDraw surfaces
    // This is mostly a no-op
    return DD_OK;
}

// ============================================================================
// SDL2DirectDraw Implementation (IDirectDraw7)
// ============================================================================

HRESULT SDL2DirectDraw::SetCooperativeLevel(HWND hwnd, DWORD flags) {
    // SDL2 handles window cooperation internally
    // Store fullscreen flag for later use
    fullscreen = (flags & DDSCL_FULLSCREEN) != 0;
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
    if (!desc || !surface) return DDERR_GENERIC;

    int width = desc->dwWidth;
    int height = desc->dwHeight;

    // Default dimensions if not specified
    if (width == 0) width = screenWidth;
    if (height == 0) height = screenHeight;

    // Create SDL texture
    SDL_Texture* texture = SDL_CreateTexture(renderer,
                                            SDL_PIXELFORMAT_ARGB8888,
                                            SDL_TEXTUREACCESS_TARGET,
                                            width, height);
    if (!texture) return DDERR_GENERIC;

    // Create our surface wrapper
    SDL2Surface* newSurface = new SDL2Surface(texture, width, height);

    // Handle primary surface with back buffer
    if (desc->dwFlags & DDSD_CAPS) {
        if (desc->dwFlags & DDSCAPS_PRIMARYSURFACE) {
            primarySurface = newSurface;

            // Create back buffer if requested
            if (desc->dwFlags & DDSD_BACKBUFFERCOUNT && desc->dwBackBufferCount > 0) {
                SDL_Texture* backTex = SDL_CreateTexture(renderer,
                                                        SDL_PIXELFORMAT_ARGB8888,
                                                        SDL_TEXTUREACCESS_TARGET,
                                                        width, height);
                if (backTex) {
                    backBuffer = new SDL2Surface(backTex, width, height);
                }
            }
        }
    }

    *surface = newSurface;
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
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) < 0) {
        return DDERR_GENERIC;
    }

    // Create window
    Uint32 windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
    if (fullscreen) {
        windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }

    window = SDL_CreateWindow("Destruction II - Resurrection",
                             SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                             width, height, windowFlags);
    if (!window) {
        SDL_Quit();
        return DDERR_GENERIC;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1,
                                 SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return DDERR_GENERIC;
    }

    // Store renderer globally for surface operations
    g_SDLRenderer = renderer;

    screenWidth = width;
    screenHeight = height;
    this->fullscreen = fullscreen;

    return DD_OK;
}

// ============================================================================
// Global DirectDraw Functions
// ============================================================================

HRESULT DirectDrawCreateEx(GUID* guid, LPVOID* ddraw, REFIID iid, IUnknown* unkOuter) {
    if (!ddraw) return DDERR_GENERIC;

    SDL2DirectDraw* dd = new SDL2DirectDraw();

    // Initialize with default settings
    HRESULT hr = dd->Initialize(nullptr, 800, 600, false);
    if (FAILED(hr)) {
        delete dd;
        return hr;
    }

    *ddraw = dd;
    return DD_OK;
}

// ============================================================================
// Helper Functions (ddutil.h implementations)
// ============================================================================

IDirectDrawPalette* DDLoadPalette(IDirectDraw7* dd, LPCSTR bitmapPath) {
    // SDL2 doesn't use palettes - return a dummy palette
    return new SDL2Palette();
}

IDirectDrawSurface7* DDLoadBitmap(IDirectDraw7* dd, LPCSTR bitmapPath, int width, int height) {
    if (!g_SDLRenderer || !bitmapPath) return nullptr;

    // Load surface from file
    SDL_Surface* loadedSurface = IMG_Load(bitmapPath);
    if (!loadedSurface) {
        // Try with .bmp extension if not specified
        std::string path(bitmapPath);
        if (path.find('.') == std::string::npos) {
            path += ".bmp";
            loadedSurface = IMG_Load(path.c_str());
        }
    }

    if (!loadedSurface) return nullptr;

    // Convert surface to texture
    SDL_Texture* texture = SDL_CreateTextureFromSurface(g_SDLRenderer, loadedSurface);

    int w = loadedSurface->w;
    int h = loadedSurface->h;
    SDL_FreeSurface(loadedSurface);

    if (!texture) return nullptr;

    // Create surface wrapper
    return new SDL2Surface(texture, w, h);
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
    if (!surface) return DDERR_GENERIC;

    SDL2Surface* sdlSurface = static_cast<SDL2Surface*>(surface);

    DDCOLORKEY colorKey;
    colorKey.dwColorSpaceLowValue = DDColorMatch(surface, rgb);
    colorKey.dwColorSpaceHighValue = colorKey.dwColorSpaceLowValue;

    return sdlSurface->SetColorKey(0, &colorKey);
}