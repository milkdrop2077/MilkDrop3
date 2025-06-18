#ifndef VIS_MILK2_UTILITY_H_LINUX_SHIM
#define VIS_MILK2_UTILITY_H_LINUX_SHIM

// This is a Linux-specific shim for utility.h to allow compilation
// when the original utility.h cannot be modified by the tools.
// It declares only functions that have stubs/implementations in
// linux_utility_stubs.cpp and are needed by other vis_milk2 components.

#include <wchar.h>  // For wchar_t
#include <stddef.h> // For size_t, NULL
#include <stdint.h> // For intptr_t

// Basic type compatibility
typedef int BOOL;
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
typedef unsigned long DWORD;
typedef unsigned int UINT;
typedef void* HWND;
typedef wchar_t WCHAR; // From winnt.h
typedef WCHAR *LPWSTR;
typedef const WCHAR *LPCWSTR;
typedef char CHAR;
typedef CHAR *LPSTR;
typedef const CHAR *LPCSTR;


// Define GUID structure for Linux
#ifndef GUID_DEFINED
#define GUID_DEFINED
typedef struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
} GUID;
#endif // GUID_DEFINED

// Define _locale_t for Linux
#ifndef _LOCALE_T_DEFINED
// This is a placeholder. Actual locale functionality requires more.
typedef void* _locale_t;
#define _LOCALE_T_DEFINED
#endif // _LOCALE_T_DEFINED
extern _locale_t g_use_C_locale; // Declaration for g_use_C_locale

extern char keyMappings[8]; // Declaration for keyMappings

#ifdef __cplusplus
extern "C" {
#endif

// Math/Utility functions
float   CosineInterp(float x);
float   InvCosineInterp(float x);
float   PowCosineInterp(float x, float pow);
float   AdjustRateToFPS(float per_frame_decay_rate_at_fps1, float fps1, float actual_fps);

// INI file function declarations
int GetPrivateProfileIntW(LPCWSTR lpAppName, LPCWSTR lpKeyName, int nDefault, LPCWSTR lpFileName);
DWORD GetPrivateProfileStringW(LPCWSTR lpAppName, LPCWSTR lpKeyName, LPCWSTR lpDefault, LPWSTR lpReturnedString, DWORD nSize, LPCWSTR lpFileName);
BOOL WritePrivateProfileStringW(LPCWSTR lpAppName, LPCWSTR lpKeyName, LPCWSTR lpString, LPCWSTR lpFileName);
float   GetPrivateProfileFloatW(wchar_t *szSectionName, wchar_t *szKeyName, float fDefault, wchar_t *szIniFile);
bool    WritePrivateProfileIntW(int d, wchar_t *szKeyName, wchar_t *szIniFile, wchar_t *szSectionName); // Section name at end
bool    WritePrivateProfileFloatW(float f, wchar_t *szKeyName, wchar_t *szIniFile, wchar_t *szSectionName); // Section name at end

#define GetPrivateProfileBoolW(w,x,y,z) ((bool)(GetPrivateProfileIntW(w,x,y,z) != 0))
#define GetPrivateProfileBOOLW(w,x,y,z) ((BOOL)(GetPrivateProfileIntW(w,x,y,z) != 0))


// String manipulation
void    RemoveExtension(wchar_t *str);
void    RemoveSingleAmpersands(wchar_t *str);

// GUID functions
void    TextToGuid(const char *str, GUID *pGUID); // Changed to const char*
void    GuidToText(const GUID *pGUID, char *str, int nStrLen); // Changed to const GUID*
extern const GUID GUID_NULL; // Declaration for GUID_NULL defined in linux_utility_stubs.cpp

// CPU feature checks
bool    CheckForMMX();
bool    CheckForSSE();

// URL Opener
intptr_t myOpenURL(HWND hwnd, const wchar_t *loc); // Changed to const wchar_t*

// Resource loading stub
void* GetTextResource(UINT id, int no_fallback);

// DirectX related stubs (if their declarations are needed by common code paths)
void    MissingDirectX(HWND hwnd);
// void    DownloadDirectX(HWND hwnd); // Not typically called by other vis_milk2 code directly

#ifdef __cplusplus
}
#endif

#endif // VIS_MILK2_UTILITY_H_LINUX_SHIM
