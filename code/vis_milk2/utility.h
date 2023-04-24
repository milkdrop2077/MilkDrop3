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

#ifndef __NULLSOFT_DX9_PLUGIN_SHELL_UTILITY_H__
#define __NULLSOFT_DX9_PLUGIN_SHELL_UTILITY_H__ 1

#include <windows.h>
#include <crtdefs.h>
#include <d3d9.h>
#include <d3dx9.h>

#define SafeRelease(x) { if (x) {x->Release(); x=NULL;} }
#define SafeDelete(x) { if (x) {delete x; x=NULL;} }
#define IsNullGuid(lpGUID) ( ((int*)lpGUID)[0]==0 && ((int*)lpGUID)[1]==0 && ((int*)lpGUID)[2]==0 && ((int*)lpGUID)[3]==0 )
#define DlgItemIsChecked(hDlg, nIDDlgItem) ((SendDlgItemMessage(hDlg, nIDDlgItem, BM_GETCHECK, (WPARAM) 0, (LPARAM) 0) == BST_CHECKED) ? true : false)
#define CosineInterp(x) (0.5f - 0.5f*cosf((x) * 3.1415926535898f))
#define InvCosineInterp(x) (acosf(1.0f - 2.0f*(x))/3.1415926535898f)
float   PowCosineInterp(float x, float pow);
float   AdjustRateToFPS(float per_frame_decay_rate_at_fps1, float fps1, float actual_fps);

//int   GetPrivateProfileInt - part of Win32 API
#define GetPrivateProfileBoolW(w,x,y,z) ((bool)(GetPrivateProfileIntW(w,x,y,z) != 0))
#define GetPrivateProfileBOOLW(w,x,y,z) ((BOOL)(GetPrivateProfileIntW(w,x,y,z) != 0))
float   GetPrivateProfileFloatW(wchar_t *szSectionName, wchar_t *szKeyName, float fDefault, wchar_t *szIniFile);
bool    WritePrivateProfileIntW(int d, wchar_t *szKeyName, wchar_t *szIniFile, wchar_t *szSectionName);
bool    WritePrivateProfileFloatW(float f, wchar_t *szKeyName, wchar_t *szIniFile, wchar_t *szSectionName);

extern  _locale_t g_use_C_locale;
extern	char keyMappings[8];

void    RemoveExtension(wchar_t *str);
void    RemoveSingleAmpersands(wchar_t *str);
void    TextToGuid(char *str, GUID *pGUID);
void    GuidToText(GUID *pGUID, char *str, int nStrLen);
void    MissingDirectX(HWND hwnd);
bool    CheckForMMX();
bool    CheckForSSE();
void    GetDesktopFolder(char *szDesktopFolder); // should be MAX_PATH len.

#include <shlobj.h>
#include <list>

BOOL    DoExplorerMenu (HWND hwnd, LPCTSTR pszPath,   POINT point);
BOOL    DoExplorerMenu (HWND hwnd, LPITEMIDLIST pidl, POINT point);
UINT    GetItemCount (LPITEMIDLIST pidl);
LPITEMIDLIST GetNextItem (LPITEMIDLIST pidl);
LPITEMIDLIST DuplicateItem (LPMALLOC pMalloc, LPITEMIDLIST pidl);
void    FindDesktopWindows(HWND *desktop_progman, HWND *desktopview_wnd, HWND *listview_wnd);
void    ExecutePidl(LPITEMIDLIST pidl, char *szPathAndFile, char *szWorkingDirectory, HWND hWnd);
int     GetDesktopIconSize();

// handy functions for populating Combo Boxes:
inline void AddItem(HWND ctrl, const wchar_t* text, DWORD itemdata) {
	LRESULT nPos = SendMessageW( ctrl, CB_ADDSTRING, 0, (LPARAM)text);
	SendMessage( ctrl, CB_SETITEMDATA, nPos, itemdata);
}
inline void SelectItemByPos(HWND ctrl, int pos) {
    SendMessage(ctrl, CB_SETCURSEL, pos, 0);
}
int SelectItemByValue(HWND ctrl, DWORD value);
bool ReadCBValue(HWND hwnd, DWORD ctrl_id, int* pRetValue);

void* GetTextResource(UINT id, int no_fallback);

intptr_t myOpenURL(HWND hwnd, wchar_t *loc);

#endif