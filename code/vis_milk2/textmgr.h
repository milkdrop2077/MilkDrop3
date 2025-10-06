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

#ifndef GEISS_TEXT_DRAWING_MANAGER
#define GEISS_TEXT_DRAWING_MANAGER 1

#include "md_defines.h"
#include "shell_defines.h"

// Stubs for Windows/DirectX types
typedef RECT* LPRECT;

#define MAX_MSGS 4096

typedef struct
{
    char* msg;
    LPD3DXFONT pfont;
    RECT rect;
    DWORD flags;
    DWORD color;
    DWORD bgColor;
    int added, deleted;
    void* prev_dark_box_ptr;
}
td_string;

class CTextManager
{
public:
    CTextManager();
    ~CTextManager();

    void Init(LPDIRECT3DDEVICE9 lpDevice, IDirect3DTexture9* lpTextSurface, int bAdditive);
    void Finish();

    int  DrawTextA(LPD3DXFONT pFont, const char* szText, int len, RECT* pRect, DWORD flags, DWORD color, bool bBlackBox, DWORD boxColor=0xFF000000);
    int  DrawTextW(LPD3DXFONT pFont, const wchar_t* szText, int len, RECT* pRect, DWORD flags, DWORD color, bool bBlackBox, DWORD boxColor=0xFF000000);
    void DrawBox(LPRECT pRect, DWORD boxColor);
    void DrawDarkBox(LPRECT pRect) { DrawBox(pRect, 0xFF000000); }
    void DrawNow();
    void ClearAll();

protected:
    LPDIRECT3DDEVICE9    m_lpDevice;
    IDirect3DTexture9*   m_lpTextSurface;
    int                  m_blit_additively;

    int       m_nMsg[2];
    td_string m_msg[2][MAX_MSGS];
    char*     m_next_msg_start_ptr;
    int       m_b;
};

#endif