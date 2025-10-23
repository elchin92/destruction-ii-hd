#ifndef __SDL2_DIRECTDRAW_COMPAT_H__
#define __SDL2_DIRECTDRAW_COMPAT_H__

/*******************************************************************************
 * SDL2_DirectDrawCompat.h
 *
 * DirectDraw 7 → SDL2 Compatibility Layer
 *
 * This header provides DirectDraw-like types and interfaces using SDL2 as the
 * backend. This allows porting legacy DirectDraw code to modern SDL2 with
 * minimal source code changes.
 *
 * Architecture:
 * - DirectDraw surfaces → SDL_Texture
 * - DirectDraw palettes → Ignored (SDL2 uses 32-bit RGBA)
 * - Blitting operations → SDL_RenderCopy
 * - Color keys → SDL_SetTextureColorMod + SDL_SetTextureBlendMode
 *******************************************************************************/

#include <SDL2/SDL.h>
#include <memory>
#include <vector>
#include <unordered_map>
#include <windows.h> // For HRESULT, RECT, COLORREF, etc.
#include <unknwn.h>  // For IUnknown

// Prevent inclusion of real DirectDraw headers - we're replacing it!
#ifndef _DDRAW_H
#define _DDRAW_H
#endif

// ============================================================================
// DirectDraw HRESULT Codes (Minimal Set)
// ============================================================================

#ifndef DD_OK
#define DD_OK                           S_OK
#endif

#ifndef DDERR_SURFACELOST
#define DDERR_SURFACELOST               MAKE_HRESULT(1, 0x876, 450)
#endif

#ifndef DDERR_GENERIC
#define DDERR_GENERIC                   E_FAIL
#endif

// ============================================================================
// Forward Declarations
// ============================================================================

struct SDL2Surface;
struct SDL2Palette;
struct SDL2DirectDraw;

// ============================================================================
// Type Definitions (DirectDraw → SDL2 Mapping)
// ============================================================================

// DirectDraw interface pointers → Our compatibility layer
typedef SDL2DirectDraw*     IDirectDraw7;
typedef SDL2Surface*        IDirectDrawSurface7;
typedef SDL2Palette*        IDirectDrawPalette;

// Legacy pointer types
typedef IDirectDraw7*       LPDIRECTDRAW7;
typedef IDirectDrawSurface7* LPDIRECTDRAWSURFACE7;
typedef IDirectDrawPalette* LPDIRECTDRAWPALETTE;

// ============================================================================
// DirectDraw Structures
// ============================================================================

// Surface description (simplified)
struct DDSURFACEDESC2 {
    DWORD       dwSize;
    DWORD       dwFlags;
    DWORD       dwHeight;
    DWORD       dwWidth;
    union {
        LONG    lPitch;
        DWORD   dwLinearSize;
    };
    DWORD       dwBackBufferCount;
    LPVOID      lpSurface;  // Locked surface pointer
};

// Color key structure (must be defined before DDBLTFX)
struct DDCOLORKEY {
    DWORD       dwColorSpaceLowValue;
    DWORD       dwColorSpaceHighValue;
};

// Blit FX structure (for special blitting effects)
struct DDBLTFX {
    DWORD       dwSize;
    DWORD       dwDDFX;
    DWORD       dwROP;
    DWORD       dwDDROP;
    DWORD       dwRotationAngle;
    DWORD       dwZBufferOpCode;
    DWORD       dwZBufferLow;
    DWORD       dwZBufferHigh;
    DWORD       dwZBufferBaseDest;
    DWORD       dwZDestConstBitDepth;
    union {
        DWORD   dwZDestConst;
        LPDIRECTDRAWSURFACE7 lpDDSZBufferDest;
    };
    DWORD       dwZSrcConstBitDepth;
    union {
        DWORD   dwZSrcConst;
        LPDIRECTDRAWSURFACE7 lpDDSZBufferSrc;
    };
    DWORD       dwAlphaEdgeBlendBitDepth;
    DWORD       dwAlphaEdgeBlend;
    DWORD       dwReserved;
    DWORD       dwAlphaDestConstBitDepth;
    union {
        DWORD   dwAlphaDestConst;
        LPDIRECTDRAWSURFACE7 lpDDSAlphaDest;
    };
    DWORD       dwAlphaSrcConstBitDepth;
    union {
        DWORD   dwAlphaSrcConst;
        LPDIRECTDRAWSURFACE7 lpDDSAlphaSrc;
    };
    union {
        DWORD   dwFillColor;
        DWORD   dwFillDepth;
        DWORD   dwFillPixel;
        LPDIRECTDRAWSURFACE7 lpDDSPattern;
    };
    DDCOLORKEY  ddckDestColorkey;
    DDCOLORKEY  ddckSrcColorkey;
};

// (DDCOLORKEY already defined above before DDBLTFX)

// ============================================================================
// SDL2Surface - DirectDrawSurface7 Wrapper
// ============================================================================

struct SDL2Surface {
    SDL_Texture*    texture;        // The actual SDL2 texture
    SDL_Surface*    surface;        // Software surface (for Lock/Unlock)
    int             width;
    int             height;
    DWORD           colorKey;       // Transparent color
    bool            colorKeyEnabled;
    bool            isLocked;

    // RefCounting for COM-like interface
    int             refCount;

    SDL2Surface(SDL_Texture* tex, int w, int h)
        : texture(tex), surface(nullptr), width(w), height(h),
          colorKey(0), colorKeyEnabled(false), isLocked(false), refCount(1) {}

    ~SDL2Surface() {
        if (texture) SDL_DestroyTexture(texture);
        if (surface) SDL_FreeSurface(surface);
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

    // DirectDraw methods
    HRESULT BltFast(DWORD x, DWORD y, LPDIRECTDRAWSURFACE7 src, RECT* srcRect, DWORD flags);
    HRESULT Blt(RECT* destRect, LPDIRECTDRAWSURFACE7 src, RECT* srcRect, DWORD flags, DDBLTFX* bltFx);
    HRESULT Lock(RECT* rect, DDSURFACEDESC2* desc, DWORD flags, HANDLE event);
    HRESULT Unlock(RECT* rect);
    HRESULT GetDC(HDC* hdc);
    HRESULT ReleaseDC(HDC hdc);
    HRESULT SetPalette(LPDIRECTDRAWPALETTE palette);
    HRESULT SetColorKey(DWORD flags, DDCOLORKEY* colorKey);
    HRESULT Restore();
};

// ============================================================================
// SDL2Palette - DirectDrawPalette Wrapper
// ============================================================================

struct SDL2Palette {
    DWORD           entries[256];
    int             refCount;

    SDL2Palette() : refCount(1) {
        memset(entries, 0, sizeof(entries));
    }

    ULONG AddRef() { return ++refCount; }
    ULONG Release() {
        if (--refCount == 0) {
            delete this;
            return 0;
        }
        return refCount;
    }
};

// ============================================================================
// SDL2DirectDraw - Main DirectDraw7 Wrapper
// ============================================================================

struct SDL2DirectDraw {
    SDL_Window*     window;
    SDL_Renderer*   renderer;

    LPDIRECTDRAWSURFACE7 primarySurface;
    LPDIRECTDRAWSURFACE7 backBuffer;
    LPDIRECTDRAWPALETTE  palette;

    int             screenWidth;
    int             screenHeight;
    bool            fullscreen;
    int             refCount;

    SDL2DirectDraw()
        : window(nullptr), renderer(nullptr),
          primarySurface(nullptr), backBuffer(nullptr), palette(nullptr),
          screenWidth(800), screenHeight(600), fullscreen(false), refCount(1) {}

    ~SDL2DirectDraw() {
        if (renderer) SDL_DestroyRenderer(renderer);
        if (window) SDL_DestroyWindow(window);
    }

    ULONG AddRef() { return ++refCount; }
    ULONG Release() {
        if (--refCount == 0) {
            delete this;
            return 0;
        }
        return refCount;
    }

    // DirectDraw methods
    HRESULT SetCooperativeLevel(HWND hwnd, DWORD flags);
    HRESULT SetDisplayMode(DWORD width, DWORD height, DWORD bpp, DWORD refreshRate, DWORD flags);
    HRESULT CreateSurface(DDSURFACEDESC2* desc, LPDIRECTDRAWSURFACE7* surface, void* unkOuter);
    HRESULT RestoreDisplayMode();
    HRESULT FlipToGDISurface();

    // Helper methods
    HRESULT Initialize(HWND hwnd, int width, int height, bool fullscreen);
};

// ============================================================================
// Global DirectDraw Compatibility Functions
// ============================================================================

// Create DirectDraw instance
HRESULT DirectDrawCreateEx(GUID* guid, LPVOID* ddraw, REFIID iid, IUnknown* unkOuter);

// Helper functions (from ddutil.h)
IDirectDrawPalette*  DDLoadPalette(IDirectDraw7* dd, LPCSTR bitmapPath);
IDirectDrawSurface7* DDLoadBitmap(IDirectDraw7* dd, LPCSTR bitmapPath, int width, int height);
HRESULT DDReLoadBitmap(IDirectDrawSurface7* surface, LPCSTR bitmapPath);
HRESULT DDCopyBitmap(IDirectDrawSurface7* surface, HBITMAP hbm, int x, int y, int width, int height);
DWORD DDColorMatch(IDirectDrawSurface7* surface, COLORREF rgb);
HRESULT DDSetColorKey(IDirectDrawSurface7* surface, COLORREF rgb);

// ============================================================================
// DirectDraw Constants
// ============================================================================

// Cooperative level flags
#define DDSCL_NORMAL                    0x00000008
#define DDSCL_EXCLUSIVE                 0x00000010
#define DDSCL_FULLSCREEN                0x00000011

// Surface description flags
#define DDSD_CAPS                       0x00000001
#define DDSD_HEIGHT                     0x00000002
#define DDSD_WIDTH                      0x00000004
#define DDSD_PITCH                      0x00000008
#define DDSD_BACKBUFFERCOUNT            0x00000020

// Surface caps
#define DDSCAPS_PRIMARYSURFACE          0x00000200
#define DDSCAPS_BACKBUFFER              0x00000004
#define DDSCAPS_COMPLEX                 0x00000008
#define DDSCAPS_FLIP                    0x00000010
#define DDSCAPS_OFFSCREENPLAIN          0x00000040

// Blit flags
#define DDBLT_WAIT                      0x01000000
#define DDBLTFAST_WAIT                  0x00000010
#define DDBLTFAST_NOCOLORKEY            0x00000000

// Lock flags
#define DDLOCK_WAIT                     0x00000001
#define DDLOCK_READONLY                 0x00000010
#define DDLOCK_WRITEONLY                0x00000020

#endif // __SDL2_DIRECTDRAW_COMPAT_H__
