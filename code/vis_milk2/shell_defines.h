/*
  LICENSE
  -------
Copyright 2005-2013 Nullsoft, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

  * Neither the name of Nullsoft nor the names of its contributors may be used to
    endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __NULLSOFT_DX9_PLUGIN_SHELL_SHELL_DEFINES_H__
#define __NULLSOFT_DX9_PLUGIN_SHELL_SHELL_DEFINES_H__ 1

#include <cstdint>

// Common types for all platforms
typedef uint32_t DWORD;
typedef uint16_t USHORT;
typedef uint16_t WORD;
typedef unsigned int UINT;
typedef intptr_t LONG_PTR;
typedef intptr_t LRESULT;
typedef uintptr_t UINT_PTR;
typedef UINT_PTR WPARAM;
typedef LONG_PTR LPARAM;

#ifdef _WIN32
#include <windows.h>
#include <process.h>
#include <d3d9.h>
#include <d3dx9.h>
#else
#include "dx_on_gl.h"
// Stubs for Windows types
struct HWND__; typedef HWND__* HWND;
struct HINSTANCE__; typedef HINSTANCE__* HINSTANCE;
struct HFONT__; typedef HFONT__* HFONT;
struct RECT { long left, top, right, bottom; };
struct GUID { uint32_t Data1; uint16_t Data2; uint16_t Data3; uint8_t Data4[8]; };
typedef void* LPVOID;
struct IUnknown { virtual long Release() { return 0; }; };
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h> // for memset
#include <assert.h>
#include <vector>


#define MAX_PATH 260
#define WM_KEYDOWN 0x0100

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

#define D3D_OK 0
#define D3DCOLOR_RGBA(r,g,b,a) ((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff))
#define D3DPT_TRIANGLESTRIP 0
#define D3DPT_TRIANGLELIST 0
#define D3DPT_TRIANGLEFAN 0
#define D3DPT_LINELIST 0
#define D3DPT_LINESTRIP 0
#define D3DPT_POINTLIST 0
#define WFVERTEX_FORMAT 0
#define MYVERTEX_FORMAT 0
#define SPRITEVERTEX_FORMAT 0
#define D3DRS_ALPHABLENDENABLE 0
#define D3DRS_SRCBLEND 0
#define D3DRS_DESTBLEND 0
#define D3DBLEND_SRCALPHA 0
#define D3DBLEND_INVSRCALPHA 0
#define D3DBLEND_ONE 0
#define D3DBLEND_ZERO 0
#define D3DBLEND_INVDESTCOLOR 0
#define D3DBLEND_DESTCOLOR 0
#define D3DCLEAR_TARGET 0
#define D3DRS_SHADEMODE 0
#define D3DSHADE_GOURAUD 0
#define D3DRS_SPECULARENABLE 0
#define D3DRS_CULLMODE 0
#define D3DCULL_NONE 0
#define D3DRS_ZENABLE 0
#define D3DRS_ZWRITEENABLE 0
#define D3DRS_LIGHTING 0
#define D3DRS_COLORVERTEX 0
#define D3DRS_FILLMODE 0
#define D3DFILL_SOLID 0
#define D3DRS_AMBIENT 0
#define D3DRS_CLIPPING 0
#define D3DRS_WRAP0 0
#define D3DSAMP_ADDRESSU 0
#define D3DSAMP_ADDRESSV 0
#define D3DSAMP_ADDRESSW 0
#define D3DTADDRESS_WRAP 0
#define D3DTADDRESS_CLAMP 0
#define D3DSAMP_MAGFILTER 0
#define D3DSAMP_MINFILTER 0
#define D3DSAMP_MIPFILTER 0
#define D3DTEXF_LINEAR 0
#define D3DTEXF_POINT 0
#define D3DTEXF_ANISOTROPIC 0
#define D3DSAMP_MAXANISOTROPY 0
#define D3DTSS_COLOROP 0
#define D3DTOP_MODULATE 0
#define D3DTSS_COLORARG1 0
#define D3DTA_DIFFUSE 0
#define D3DTSS_COLORARG2 0
#define D3DTA_TEXTURE 0
#define D3DTOP_DISABLE 0
#define D3DTSS_ALPHAOP 0
#define D3DTOP_SELECTARG1 0
#define D3DTSS_ALPHAARG1 0
#define D3DTSS_ALPHAARG2 0
#define D3DRS_POINTSIZE 0
#define D3DFMT_R5G6B5 0
#define D3DFMT_X1R5G5B5 0
#define D3DFMT_A1R5G5B5 0
#define D3DFMT_A4R4G4B4 0
#define D3DPBLENDCAPS_INVDESTCOLOR 0
#define D3DPBLENDCAPS_DESTCOLOR 0
#define D3DBLEND_SRCCOLOR 0
#define D3DBLEND_INVSRCCOLOR 0
#define D3DFMT_INDEX16 0

#define DT_SINGLELINE 0
#define DT_CALCRECT 0
#define DT_CENTER 0
#define DEFAULT_CHARSET 0
#define OUT_DEFAULT_PRECIS 0
#define CLIP_DEFAULT_PRECIS 0
#define ANTIALIASED_QUALITY 0
#define DEFAULT_QUALITY 0
#define DEFAULT_PITCH 0

#define ZeroMemory(p, size) memset(p, 0, size)

#define DEFAULT_FULLSCREEN_WIDTH  640
#define DEFAULT_FULLSCREEN_HEIGHT 480
#define MAX_ICON_TEXTURES 8
#define ICON_TEXTURE_SIZE 256
#define DEFAULT_WINDOW_SIZE 0.625f      // as a portion of the width or height of the screen (whichever is smaller)
#define BGR2RGB(x) (((x>>16)&0xFF) | (x & 0xFF00) | ((x<<16)&0xFF0000))

#define NUM_BASIC_FONTS 4
#define  SYS_FONT 0
#define  DEC_FONT 1
#define  HELP_FONT 2
#define  DESK_FONT 3
#define MAX_EXTRA_FONTS 5
typedef enum
{
    SIMPLE_FONT = 0,  // aka 'system' font; should be legible
    DECORATIVE_FONT = 1,
    HELPSCREEN_FONT = 2,
    PLAYLIST_FONT = 3,
    EXTRA_1 = 4,
    EXTRA_2 = 5,
    EXTRA_3 = 6,
    EXTRA_4 = 7,
    EXTRA_5 = 8
}
eFontIndex;

#endif