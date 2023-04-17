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

#include "DXContext.h"
#include "utility.h"
#include "shell_defines.h"
#include <strsafe.h>

DXContext::DXContext(LPDIRECT3DDEVICE9 device, D3DPRESENT_PARAMETERS* d3dpp, HWND hwnd, wchar_t* szIniFile)
{
    m_szWindowCaption[0] = 0;
    m_hwnd = hwnd;
    m_lpDevice = device;
    m_d3dpp = d3dpp;
    m_hmod_d3d9 = NULL;
    m_hmod_d3dx9 = NULL;
    m_zFormat = D3DFMT_UNKNOWN;
    for (int i = 0; i<MAX_DXC_ADAPTERS; i++)
        m_orig_windowed_mode_format[i] = D3DFMT_UNKNOWN;
    m_ordinal_adapter = D3DADAPTER_DEFAULT;
    m_winamp_minimized = 0;
    m_truly_exiting = 0;
    m_bpp = 0;
    m_frame_delay = 0;
    StringCbCopyW(m_szIniFile, sizeof(m_szIniFile), szIniFile);
    m_szDriver[0] = 0;
    m_szDesc[0] = 0;
    m_lastErr = S_OK;
    m_ready = FALSE;
}

DXContext::~DXContext()
{
    Internal_CleanUp();
}

void DXContext::Internal_CleanUp()
{
    m_ready = FALSE;
}

BOOL DXContext::Internal_Init(DXCONTEXT_PARAMS *pParams, BOOL bFirstInit)
{
    memcpy(&m_current_mode, pParams, sizeof(DXCONTEXT_PARAMS));

    memset(&m_caps, 0, sizeof(m_caps));
    m_lpDevice->GetDeviceCaps(&m_caps);
    m_bpp = 32;

    m_ready = TRUE;
    return TRUE;
}

BOOL DXContext::StartOrRestartDevice(DXCONTEXT_PARAMS *pParams)
{

    // call this to [re]initialize the DirectX environment with new parameters.
    // examples: startup; toggle windowed/fullscreen mode; change fullscreen resolution;
    //   and so on.
    // be sure to clean up all your DirectX stuff first (textures, vertex buffers,
    //   D3DX allocations, etc.) and reallocate it afterwards!

    // note: for windowed mode, 'pParams->disp_mode' (w/h/r/f) is ignored.

    if (!m_ready)
    {
        // first-time init: create a fresh new device
        return Internal_Init(pParams, TRUE);
    }
    else
    {
        // re-init: preserve the DX9 object (m_lpD3D),
        // but destroy and re-create the DX9 device (m_lpDevice).
        m_ready = FALSE;

        //SafeRelease(m_lpDevice);
        // but leave the D3D object!

        //		RestoreWinamp();
        return Internal_Init(pParams, FALSE);
    }
}

HWND DXContext::GetHwnd() { return m_hwnd; }
bool DXContext::TempIgnoreDestroyMessages() { return false; }
void DXContext::SaveWindow() { }

void DXContext::WriteSafeWindowPos()
{
    WritePrivateProfileIntW(64, L"nMainWndTop", m_szIniFile, L"settings");
    WritePrivateProfileIntW(64, L"nMainWndLeft", m_szIniFile, L"settings");
    WritePrivateProfileIntW(64 + 256, L"nMainWndRight", m_szIniFile, L"settings");
    WritePrivateProfileIntW(64 + 256, L"nMainWndBottom", m_szIniFile, L"settings");
    WritePrivateProfileIntW(64, L"avs_wx", m_szIniFile, L"settings");
    WritePrivateProfileIntW(64, L"avs_wy", m_szIniFile, L"settings");
    WritePrivateProfileIntW(256, L"avs_ww", m_szIniFile, L"settings");
    WritePrivateProfileIntW(256, L"avs_wh", m_szIniFile, L"settings");
}

bool DXContext::OnUserResizeWindow(RECT *new_window_rect, RECT *new_client_rect)
{
    // call this function on WM_EXITSIZEMOVE when running windowed.
    // don't bother calling this when fullscreen.
    // be sure to clean up all your DirectX stuff first (textures, vertex buffers,
    //   D3DX allocations, etc.) and reallocate it afterwards!

    if (!m_ready)
        return FALSE;

    if ((m_client_width == new_client_rect->right - new_client_rect->left) &&
        (m_client_height == new_client_rect->bottom - new_client_rect->top) &&
        (m_window_width == new_window_rect->right - new_window_rect->left) &&
        (m_window_height == new_window_rect->bottom - new_window_rect->top))
    {
        return TRUE;
    }

    m_ready = FALSE;

    m_window_width = new_window_rect->right - new_window_rect->left;
    m_window_height = new_window_rect->bottom - new_window_rect->top;
    m_client_width = m_REAL_client_width = new_client_rect->right - new_client_rect->left;
    m_client_height = m_REAL_client_height = new_client_rect->bottom - new_client_rect->top;

    m_d3dpp->BackBufferWidth = m_client_width;
    m_d3dpp->BackBufferHeight = m_client_height;
    if (m_lpDevice->Reset(m_d3dpp) != D3D_OK)
    {
        WriteSafeWindowPos();
        m_lastErr = DXC_ERR_RESIZEFAILED;
        return FALSE;
    }

    SetViewport();
    m_ready = TRUE;
    return TRUE;
}

void DXContext::SetViewport()
{
    D3DVIEWPORT9 v;
    v.X = 0;
    v.Y = 0;
    v.Width = m_client_width;
    v.Height = m_client_height;
    v.MinZ = 0.0f;
    v.MaxZ = 1.0f;
    m_lpDevice->SetViewport(&v);
}