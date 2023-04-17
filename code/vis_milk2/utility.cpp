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

#include "utility.h"
#include <math.h>
#include <locale.h>
#include <windows.h>
#ifdef _DEBUG
    #define D3D_DEBUG_INFO  // declare this before including d3d9.h
#endif
#include <d3d9.h>
#include "resource.h"
#include "wasabi.h"
#include <shellapi.h>

extern HINSTANCE api_orig_hinstance;

intptr_t myOpenURL(HWND hwnd, wchar_t *loc)
{
	/* TODO FIXME */
    return 0;
}

float PowCosineInterp(float x, float pow)
{
    // input (x) & output should be in range 0..1.
    // pow > 0: tends to push things toward 0 and 1
    // pow < 0: tends to push things toward 0.5.

    if (x<0)
        return 0;
    if (x>1)
        return 1;

    int bneg = (pow < 0) ? 1 : 0;
    if (bneg)
        pow = -pow;

    if (pow>1000) pow=1000;

    int its = (int)pow;
    for (int i=0; i<its; i++)
    {
        if (bneg)
            x = InvCosineInterp(x);
        else
            x = CosineInterp(x);
    }
    float x2 = (bneg) ? InvCosineInterp(x) : CosineInterp(x);
    float dx = pow - its;
    return ((1-dx)*x + (dx)*x2);
}

float AdjustRateToFPS(float per_frame_decay_rate_at_fps1, float fps1, float actual_fps)
{
    // returns the equivalent per-frame decay rate at actual_fps

    // basically, do all your testing at fps1 and get a good decay rate;
    // then, in the real application, adjust that rate by the actual fps each time you use it.

    float per_second_decay_rate_at_fps1 = powf(per_frame_decay_rate_at_fps1, fps1);
    float per_frame_decay_rate_at_fps2 = powf(per_second_decay_rate_at_fps1, 1.0f/actual_fps);

    return per_frame_decay_rate_at_fps2;
}

float GetPrivateProfileFloatW(wchar_t *szSectionName, wchar_t *szKeyName, float fDefault, wchar_t *szIniFile)
{
    wchar_t string[64];
    wchar_t szDefault[64];
    float ret = fDefault;

    _swprintf_l(szDefault, L"%f", g_use_C_locale, fDefault);

    if (GetPrivateProfileStringW(szSectionName, szKeyName, szDefault, string, 64, szIniFile) > 0)
    {
        _swscanf_l(string, L"%f", g_use_C_locale, &ret);
    }
    return ret;
}

bool WritePrivateProfileFloatW(float f, wchar_t *szKeyName, wchar_t *szIniFile, wchar_t *szSectionName)
{
    wchar_t szValue[32];
    _swprintf_l(szValue, L"%f", g_use_C_locale, f);
    return (WritePrivateProfileStringW(szSectionName, szKeyName, szValue, szIniFile) != 0);
}

bool WritePrivateProfileIntW(int d, wchar_t *szKeyName, wchar_t *szIniFile, wchar_t *szSectionName)
{
    wchar_t szValue[32];
    swprintf(szValue, L"%d", d);
    return (WritePrivateProfileStringW(szSectionName, szKeyName, szValue, szIniFile) != 0);
}

void RemoveExtension(wchar_t *str)
{
    wchar_t *p = wcsrchr(str, L'.');
    if (p) *p = 0;
}

static void ShiftDown(wchar_t *str)
{
	while (*str)
	{
		str[0] = str[1];
		str++;
	}
}

void RemoveSingleAmpersands(wchar_t *str)
{
	while (*str)
	{
		if (str[0] == L'&')
		{
			if (str[1] == L'&') // two in a row: replace with single ampersand, move on
				str++;

			ShiftDown(str);
		}
		else
			str = CharNextW(str);
	}
}

void TextToGuid(char *str, GUID *pGUID)
{
    if (!str) return;
    if (!pGUID) return;

    DWORD d[11];

    sscanf(str, "%X %X %X %X %X %X %X %X %X %X %X",
        &d[0], &d[1], &d[2], &d[3], &d[4], &d[5], &d[6], &d[7], &d[8], &d[9], &d[10]);

    pGUID->Data1 = (DWORD)d[0];
    pGUID->Data2 = (WORD)d[1];
    pGUID->Data3 = (WORD)d[2];
    pGUID->Data4[0] = (BYTE)d[3];
    pGUID->Data4[1] = (BYTE)d[4];
    pGUID->Data4[2] = (BYTE)d[5];
    pGUID->Data4[3] = (BYTE)d[6];
    pGUID->Data4[4] = (BYTE)d[7];
    pGUID->Data4[5] = (BYTE)d[8];
    pGUID->Data4[6] = (BYTE)d[9];
    pGUID->Data4[7] = (BYTE)d[10];
}

void GuidToText(GUID *pGUID, char *str, int nStrLen)
{
    // note: nStrLen should be set to sizeof(str).
    if (!str) return;
    if (!nStrLen) return;
    str[0] = 0;
    if (!pGUID) return;

    DWORD d[11];
    d[0]  = (DWORD)pGUID->Data1;
    d[1]  = (DWORD)pGUID->Data2;
    d[2]  = (DWORD)pGUID->Data3;
    d[3]  = (DWORD)pGUID->Data4[0];
    d[4]  = (DWORD)pGUID->Data4[1];
    d[5]  = (DWORD)pGUID->Data4[2];
    d[6]  = (DWORD)pGUID->Data4[3];
    d[7]  = (DWORD)pGUID->Data4[4];
    d[8]  = (DWORD)pGUID->Data4[5];
    d[9]  = (DWORD)pGUID->Data4[6];
    d[10] = (DWORD)pGUID->Data4[7];

    sprintf(str, "%08X %04X %04X %02X %02X %02X %02X %02X %02X %02X %02X",
        d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7], d[8], d[9], d[10]);
}

/*
int GetPentiumTimeRaw(unsigned __int64 *cpu_timestamp)
{
    // returns 0 on failure, 1 on success
    // warning: watch out for wraparound!

    // note: it's probably better to use QueryPerformanceFrequency
    // and QueryPerformanceCounter()!

    // get high-precision time:
    __try
    {
        unsigned __int64 *dest = (unsigned __int64 *)cpu_timestamp;
        __asm
        {
            _emit 0xf        // these two bytes form the 'rdtsc' asm instruction,
            _emit 0x31       //  available on Pentium I and later.
            mov esi, dest
            mov [esi  ], eax    // lower 32 bits of tsc
            mov [esi+4], edx    // upper 32 bits of tsc
        }
        return 1;
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
        return 0;
    }

    return 0;
}

double GetPentiumTimeAsDouble(unsigned __int64 frequency)
{
    // returns < 0 on failure; otherwise, returns current cpu time, in seconds.
    // warning: watch out for wraparound!

    // note: it's probably better to use QueryPerformanceFrequency
    // and QueryPerformanceCounter()!

    if (frequency==0)
        return -1.0;

    // get high-precision time:
    __try
    {
        unsigned __int64 high_perf_time;
        unsigned __int64 *dest = &high_perf_time;
        __asm
        {
            _emit 0xf        // these two bytes form the 'rdtsc' asm instruction,
            _emit 0x31       //  available on Pentium I and later.
            mov esi, dest
            mov [esi  ], eax    // lower 32 bits of tsc
            mov [esi+4], edx    // upper 32 bits of tsc
        }
        __int64 time_s     = (__int64)(high_perf_time / frequency);  // unsigned->sign conversion should be safe here
        __int64 time_fract = (__int64)(high_perf_time % frequency);  // unsigned->sign conversion should be safe here
        // note: here, we wrap the timer more frequently (once per week)
        // than it otherwise would (VERY RARELY - once every 585 years on
        // a 1 GHz), to alleviate floating-point precision errors that start
        // to occur when you get to very high counter values.
        double ret = (time_s % (60*60*24*7)) + (double)time_fract/(double)((__int64)frequency);
        return ret;
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
        return -1.0;
    }

    return -1.0;
}
*/

void DownloadDirectX(HWND hwnd)
{
    wchar_t szUrl[] = L"http://www.microsoft.com/download/details.aspx?id=35";
    intptr_t ret = myOpenURL(NULL, szUrl);
    if (ret <= 32)
    {
        wchar_t buf[1024];
        switch(ret)
        {
        case SE_ERR_FNF:
        case SE_ERR_PNF:
            swprintf(buf, wasabiApiLangString(IDS_URL_COULD_NOT_OPEN), szUrl);
            break;
        case SE_ERR_ACCESSDENIED:
        case SE_ERR_SHARE:
            swprintf(buf, wasabiApiLangString(IDS_ACCESS_TO_URL_WAS_DENIED), szUrl);
            break;
        case SE_ERR_NOASSOC:
            swprintf(buf, wasabiApiLangString(IDS_ACCESS_TO_URL_FAILED_DUE_TO_NO_ASSOC), szUrl);
            break;
        default:
            swprintf(buf, wasabiApiLangString(IDS_ACCESS_TO_URL_FAILED_CODE_X), szUrl, ret);
            break;
        }
        MessageBoxW(hwnd, buf, wasabiApiLangString(IDS_ERROR_OPENING_URL),
					MB_OK|MB_SETFOREGROUND|MB_TOPMOST|MB_TASKMODAL);
    }
}

void MissingDirectX(HWND hwnd)
{
    // DIRECTX MISSING OR CORRUPT -> PROMPT TO GO TO WEB.
	wchar_t title[128];
    int ret = MessageBoxW(hwnd,
        #ifndef D3D_SDK_VERSION
            --- error; you need to #include <d3d9.h> ---
        #endif
        #if (D3D_SDK_VERSION==120)
            // plugin was *built* using the DirectX 9.0 sdk, therefore,
            // the dx9.0 runtime is missing or corrupt
            "Failed to initialize DirectX 9.0 or later.\n"
            "Milkdrop requires d3dx9_31.dll to be installed.\n"
            "\n"
            "Would you like to be taken to:\n"
			"http://www.microsoft.com/download/details.aspx?id=35,\n"
            "where you can update DirectX 9.0?\n"
            XXXXXXX
        #else
            // plugin was *built* using some other version of the DirectX9 sdk, such as
            // 9.1b; therefore, we don't know exactly what version to tell them they need
            // to install; so we ask them to go get the *latest* version.
            wasabiApiLangString(IDS_DIRECTX_MISSING_OR_CORRUPT_TEXT)
        #endif
        ,
		wasabiApiLangString(IDS_DIRECTX_MISSING_OR_CORRUPT, title, 128),
		MB_YESNO|MB_SETFOREGROUND|MB_TOPMOST|MB_TASKMODAL);

    if (ret==IDYES)
        DownloadDirectX(hwnd);
}

bool CheckForMMX()
{
    DWORD bMMX = 0;
    DWORD *pbMMX = &bMMX;
    __try {
        __asm {
            mov eax, 1
            cpuid
            mov edi, pbMMX
            mov dword ptr [edi], edx
        }
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
        bMMX = 0;
    }

    if (bMMX & 0x00800000)  // check bit 23
		return true;

	return false;
}

bool CheckForSSE()
{
#ifdef _WIN64
	return true; // All x64 processors support SSE
#else
    /*
    The SSE instruction set was introduced with the Pentium III and features:
        * Additional MMX instructions such as min/max
        * Prefetch and write-through instructions for optimizing data movement
            from and to the L2/L3 caches and main memory
        * 8 New 128 bit XMM registers (xmm0..xmm7) and corresponding 32 bit floating point
            (single precision) instructions
    */

	DWORD bSSE = 0;
	DWORD *pbSSE = &bSSE;
    __try {
	    __asm
	    {
		    mov eax, 1
		    cpuid
            mov edi, pbSSE
            mov dword ptr [edi], edx
	    }
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
        bSSE = 0;
    }

	if (bSSE & 0x02000000)  // check bit 25
		return true;

	return false;
#endif
}

void GetDesktopFolder(char *szDesktopFolder) // should be MAX_PATH len.
{
    // returns the path to the desktop folder, WITHOUT a trailing backslash.
    szDesktopFolder[0] = 0;
    ITEMIDLIST pidl;
    ZeroMemory(&pidl, sizeof(pidl));
    if (!SHGetPathFromIDList(&pidl, szDesktopFolder))
        szDesktopFolder[0] = 0;
}

void ExecutePidl(LPITEMIDLIST pidl, char *szPathAndFile, char *szWorkingDirectory, HWND hWnd)
{
    // This function was based on code by Jeff Prosise.

    // Note: for some reason, ShellExecuteEx fails when executing
    // *shortcuts* (.lnk files) from the desktop, using their PIDLs.
    // So, if that fails, we try again w/the plain old text filename
    // (szPathAndFile).

    char szVerb[] = "open";
    char szFilename2[MAX_PATH];

    sprintf(szFilename2, "%s.lnk", szPathAndFile);

    // -without the "no-verb" pass,
    //   certain icons still don't work (like shortcuts
    //   to IE, VTune...)
    // -without the "context menu" pass,
    //   certain others STILL don't work (Netscape...)
    // -without the 'ntry' pass, shortcuts (to folders/files)
    //   don't work
    for (int verb_pass=0; verb_pass<2; verb_pass++)
    {
        for (int ntry=0; ntry<3; ntry++)
        {
            for (int context_pass=0; context_pass<2; context_pass++)
            {
                SHELLEXECUTEINFO sei = { sizeof(sei) };
                sei.hwnd = hWnd;
                sei.fMask = SEE_MASK_FLAG_NO_UI;
                if (context_pass==1)
                    sei.fMask |= SEE_MASK_INVOKEIDLIST;
                sei.lpVerb = (verb_pass) ? NULL : szVerb;
                sei.lpDirectory = szWorkingDirectory;
                sei.nShow = SW_SHOWNORMAL;

                if (ntry==0)
                {
                    // this case works for most non-shortcuts
                    sei.fMask |= SEE_MASK_IDLIST;
                    sei.lpIDList = pidl;
                }
                else if (ntry==1)
                {
                    // this case is required for *shortcuts to folders* to work
                    sei.lpFile = szPathAndFile;
                }
                else if (ntry==2)
                {
                    // this case is required for *shortcuts to files* to work
                    sei.lpFile = szFilename2;
                }

                if (ShellExecuteEx(&sei))
                    return;
            }
        }
    }
}

WNDPROC        g_pOldWndProc;
LPCONTEXTMENU2 g_pIContext2or3;

LRESULT CALLBACK HookWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
   //UINT uItem;
   //TCHAR szBuf[MAX_PATH];

   switch (msg)
   {
   case WM_DRAWITEM:
   case WM_MEASUREITEM:
      if(wp) break; // not menu related
   case WM_INITMENUPOPUP:
      g_pIContext2or3->HandleMenuMsg(msg, wp, lp);
      return (msg==WM_INITMENUPOPUP ? 0 : TRUE); // handled

   /*case WM_MENUSELECT:
      // if this is a shell item, get its descriptive text
      uItem = (UINT) LOWORD(wp);
      if(0 == (MF_POPUP & HIWORD(wp)) && uItem >= 1 && uItem <= 0x7fff)
      {
         g_pIContext2or3->GetCommandString(uItem-1, GCS_HELPTEXT,
            NULL, szBuf, sizeof(szBuf)/sizeof(szBuf[0]) );

         // set the status bar text
         ((CFrameWnd*)(AfxGetApp()->m_pMainWnd))->SetMessageText(szBuf);
         return 0;
      }
      break;*/

	default:
		break;
	}

	// for all untreated messages, call the original wndproc
	return ::CallWindowProc(g_pOldWndProc, hWnd, msg, wp, lp);
}

BOOL DoExplorerMenu (HWND hwnd, LPITEMIDLIST pidlMain, POINT point)
{
    LPMALLOC pMalloc;
    LPSHELLFOLDER psfFolder, psfNextFolder;
    LPITEMIDLIST pidlItem, pidlNextItem, *ppidl;
    LPCONTEXTMENU pContextMenu;
    CMINVOKECOMMANDINFO ici;
    UINT nCount, nCmd;
    BOOL bResult;
    HMENU hMenu;

    //
    // Get pointers to the shell's IMalloc interface and the desktop's
    // IShellFolder interface.
    //
    bResult = FALSE;

    if (!SUCCEEDED (SHGetMalloc (&pMalloc)))
		return bResult;

    if (!SUCCEEDED (SHGetDesktopFolder (&psfFolder))) {
        pMalloc->Release();
        return bResult;
    }

    if (nCount = GetItemCount (pidlMain)) // nCount must be > 0
    {
        //
        // Initialize psfFolder with a pointer to the IShellFolder
        // interface of the folder that contains the item whose context
        // menu we're after, and initialize pidlItem with a pointer to
        // the item's item ID. If nCount > 1, this requires us to walk
        // the list of item IDs stored in pidlMain and bind to each
        // subfolder referenced in the list.
        //
        pidlItem = pidlMain;

        while (--nCount) {
            //
            // Create a 1-item item ID list for the next item in pidlMain.
            //
            pidlNextItem = DuplicateItem (pMalloc, pidlItem);
            if (pidlNextItem == NULL) {
                psfFolder->Release();
                pMalloc->Release();
                return bResult;
            }

            //
            // Bind to the folder specified in the new item ID list.
            //
            if (!SUCCEEDED (psfFolder->BindToObject(pidlNextItem, NULL, IID_IShellFolder, (void**)&psfNextFolder)))  // modified by RG
            {
                pMalloc->Free(pidlNextItem);
                psfFolder->Release();
                pMalloc->Release();
                return bResult;
            }

            //
            // Release the IShellFolder pointer to the parent folder
            // and set psfFolder equal to the IShellFolder pointer for
            // the current folder.
            //
            psfFolder->Release();
            psfFolder = psfNextFolder;

            //
            // Release the storage for the 1-item item ID list we created
            // just a moment ago and initialize pidlItem so that it points
            // to the next item in pidlMain.
            //
            pMalloc->Free(pidlNextItem);
            pidlItem = GetNextItem (pidlItem);
        }

        //
        // Get a pointer to the item's IContextMenu interface and call
        // IContextMenu::QueryContextMenu to initialize a context menu.
        //
        ppidl = &pidlItem;
        if (SUCCEEDED (psfFolder->GetUIObjectOf(hwnd, 1, (LPCITEMIDLIST*)ppidl, IID_IContextMenu, NULL, (void**)&pContextMenu)))   // modified by RG
        {
            // try to see if we can upgrade to an IContextMenu3
            // or IContextMenu2 interface pointer:
            int level = 1;
            void *pCM = NULL;
            if (pContextMenu->QueryInterface(IID_IContextMenu3, &pCM) == NOERROR)
            {
                pContextMenu->Release();
                pContextMenu = (LPCONTEXTMENU)pCM;
                level = 3;
            }
            else if (pContextMenu->QueryInterface(IID_IContextMenu2, &pCM) == NOERROR)
            {
                pContextMenu->Release();
                pContextMenu = (LPCONTEXTMENU)pCM;
                level = 2;
            }

            hMenu = CreatePopupMenu ();
            if (SUCCEEDED (pContextMenu->QueryContextMenu(hMenu, 0, 1, 0x7FFF, CMF_EXPLORE)))
            {
                ClientToScreen (hwnd, &point);

                // install the subclassing "hook", for versions 2 or 3
                if (level >= 2)
                {
                    g_pOldWndProc   = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (DWORD_PTR)HookWndProc);
                    g_pIContext2or3 = (LPCONTEXTMENU2)pContextMenu; // cast ok for ICMv3
                }
                else
                {
                    g_pOldWndProc   = NULL;
                    g_pIContext2or3 = NULL;
                }

                //
                // Display the context menu.
                //
                nCmd = TrackPopupMenu (hMenu, TPM_LEFTALIGN |
                    TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_RETURNCMD,
                    point.x, point.y, 0, hwnd, NULL);

                // restore old wndProc
                if (g_pOldWndProc)
                {
                    SetWindowLongPtr(hwnd, GWL_WNDPROC, (LONG_PTR)g_pOldWndProc);
                }

                //
                // If a command was selected from the menu, execute it.
                //
                if (nCmd >= 1 && nCmd <= 0x7fff)
                {
                    ZeroMemory(&ici, sizeof(ici));
                    ici.cbSize          = sizeof (CMINVOKECOMMANDINFO);
                    //ici.fMask           = 0;
                    ici.hwnd            = hwnd;
                    ici.lpVerb          = MAKEINTRESOURCE (nCmd - 1);
                    //ici.lpParameters    = NULL;
                    //ici.lpDirectory     = NULL;
                    ici.nShow           = SW_SHOWNORMAL;
                    //ici.dwHotKey        = 0;
                    //ici.hIcon           = NULL;

                    if (SUCCEEDED ( pContextMenu->InvokeCommand (&ici)))
                        bResult = TRUE;
                }
                /*else if (nCmd)
                {
                    PostMessage(hwnd, WM_COMMAND, nCmd, NULL); // our command
                }*/
            }
            DestroyMenu (hMenu);
            pContextMenu->Release();
        }
    }

    //
    // Clean up and return.
    //
    psfFolder->Release();
    pMalloc->Release();

    return bResult;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  Note: a special thanks goes out to Jeff Prosise for writing & publishing
//        the following code!
//
//  FUNCTION:       GetItemCount
//
//  DESCRIPTION:    Computes the number of item IDs in an item ID list.
//
//  INPUT:          pidl = Pointer to an item ID list.
//
//  RETURNS:        Number of item IDs in the list.
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

UINT GetItemCount (LPITEMIDLIST pidl)
{
    USHORT nLen;
    UINT nCount;

    nCount = 0;
    while ((nLen = pidl->mkid.cb) != 0) {
        pidl = GetNextItem (pidl);
        nCount++;
    }
    return nCount;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  Note: a special thanks goes out to Jeff Prosise for writing & publishing
//        the following code!
//
//  FUNCTION:       GetNextItem
//
//  DESCRIPTION:    Finds the next item in an item ID list.
//
//  INPUT:          pidl = Pointer to an item ID list.
//
//  RETURNS:        Pointer to the next item.
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

LPITEMIDLIST GetNextItem (LPITEMIDLIST pidl)
{
    USHORT nLen;

    if ((nLen = pidl->mkid.cb) == 0)
        return NULL;

    return (LPITEMIDLIST) (((LPBYTE) pidl) + nLen);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  Note: a special thanks goes out to Jeff Prosise for writing & publishing
//        the following code!
//
//  FUNCTION:       DuplicateItem
//
//  DESCRIPTION:    Makes a copy of the next item in an item ID list.
//
//  INPUT:          pMalloc = Pointer to an IMalloc interface.
//                  pidl    = Pointer to an item ID list.
//
//  RETURNS:        Pointer to an ITEMIDLIST containing the copied item ID.
//
//  NOTES:          It is the caller's responsibility to free the memory
//                  allocated by this function when the item ID is no longer
//                  needed. Example:
//
//                    pidlItem = DuplicateItem (pMalloc, pidl);
//                      .
//                      .
//                      .
//                    pMalloc->lpVtbl->Free (pMalloc, pidlItem);
//
//                  Failure to free the ITEMIDLIST will result in memory
//                  leaks.
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

LPITEMIDLIST DuplicateItem (LPMALLOC pMalloc, LPITEMIDLIST pidl)
{
    USHORT nLen;
    LPITEMIDLIST pidlNew;

    nLen = pidl->mkid.cb;
    if (nLen == 0)
        return NULL;

    pidlNew = (LPITEMIDLIST) pMalloc->Alloc (
        nLen + sizeof (USHORT));
    if (pidlNew == NULL)
        return NULL;

    CopyMemory (pidlNew, pidl, nLen);
    *((USHORT*) (((LPBYTE) pidlNew) + nLen)) = 0;

    return pidlNew;
}

//----------------------------------------------------------------------
// A special thanks goes out to Jeroen-bart Engelen (Yeep) for providing
// his source code for getting the position & label information for all
// the icons on the desktop, as found below.  See his article at
// http://www.digiwar.com/scripts/renderpage.php?section=2&subsection=2
//----------------------------------------------------------------------

void FindDesktopWindows(HWND *desktop_progman, HWND *desktopview_wnd, HWND *listview_wnd)
{
    *desktop_progman = NULL;
	*desktopview_wnd = NULL;
	*listview_wnd = NULL;

	*desktop_progman = FindWindow(NULL, ("Program Manager"));
	if(*desktop_progman == NULL)
	{
		//MessageBox(NULL, "Unable to get the handle to the Program Manager.", "Fatal error", MB_OK|MB_ICONERROR);
		return;
	}

	*desktopview_wnd = FindWindowEx(*desktop_progman, NULL, "SHELLDLL_DefView", NULL);
	if(*desktopview_wnd == NULL)
	{
		//MessageBox(NULL, "Unable to get the handle to the desktopview.", "Fatal error", MB_OK|MB_ICONERROR);
		return;
	}

	// Thanks ef_ef_ef@yahoo.com for pointing out this works in NT 4 and not the way I did it originally.
	*listview_wnd = FindWindowEx(*desktopview_wnd, NULL, "SysListView32", NULL);
	if(*listview_wnd == NULL)
	{
		//MessageBox(NULL, "Unable to get the handle to the folderview.", "Fatal error", MB_OK|MB_ICONERROR);
		return;
	}
}

//----------------------------------------------------------------------

int GetDesktopIconSize()
{
    int ret = 32;

    // reads the key: HKEY_CURRENT_USER\Control Panel, Desktop\WindowMetrics\Shell Icon Size
    unsigned char buf[64];
    unsigned long len = sizeof(buf);
    DWORD type;
    HKEY key;

    if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, "Control Panel\\Desktop\\WindowMetrics", 0, KEY_READ, &key))
    {
        if (ERROR_SUCCESS == RegQueryValueEx(key, "Shell Icon Size", NULL, &type, (unsigned char*)buf, &len) &&
            type == REG_SZ)
        {
            int x = _atoi_l((char*)buf, g_use_C_locale);
            if (x>0 && x<=128)
                ret = x;
        }

        RegCloseKey(key);
    }

    return ret;
}

//----------------------------------------------------------------------

// handy functions for populating Combo Boxes:
int SelectItemByValue(HWND ctrl, DWORD value)
{
    int count = SendMessage(ctrl, CB_GETCOUNT, 0, 0);
	for (int i=0; i<count; i++)
	{
		DWORD value_i = SendMessage( ctrl, CB_GETITEMDATA, i, 0);
		if (value_i == value)
        {
			SendMessage( ctrl, CB_SETCURSEL, i, 0);
            return i;
        }
	}
    return -1;
}

bool ReadCBValue(HWND hwnd, DWORD ctrl_id, int* pRetValue)
{
    if (!pRetValue)
        return false;
    HWND ctrl = GetDlgItem( hwnd, ctrl_id );
	int t = SendMessage( ctrl, CB_GETCURSEL, 0, 0);
	if (t == CB_ERR)
        return false;
    *pRetValue = (int)SendMessage( ctrl, CB_GETITEMDATA, t, 0);
    return true;
}

void* GetTextResource(UINT id, int no_fallback){
	void* data = 0;
	HINSTANCE hinst = api_orig_hinstance;
	HRSRC rsrc = FindResource(hinst,MAKEINTRESOURCE(id),"TEXT");
	if(!rsrc && !no_fallback) rsrc = FindResource((hinst = api_orig_hinstance),MAKEINTRESOURCE(id),"TEXT");
	if(rsrc){
	HGLOBAL resourceHandle = LoadResource(hinst,rsrc);
		data = LockResource(resourceHandle);
	}
	return data;
}