// linux_utility_stubs.cpp
// Provides implementations for functions declared in utility.h for the Linux platform.

#ifndef _WIN32 // Ensure this whole file is only compiled for non-Windows

#include "utility.h" // To get function declarations and common types like GUID (if defined there for Linux)
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cwchar>
#include <cstring> // For memset, wcslen, wcsncpy
#include <map>     // For INI stubs
#include <algorithm> // For std::replace

// Define types if not already available via utility.h's Linux path
// These are basic fallbacks. A more robust solution would use a shared config.h or ensure utility.h handles this.
#ifndef GUID_DEFINED
#define GUID_DEFINED
typedef struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
} GUID;
#endif

#ifndef _LOCALE_T_DEFINED
typedef void* _locale_t; // Basic placeholder
#define _LOCALE_T_DEFINED
#endif
_locale_t g_use_C_locale = (_locale_t)0; // Placeholder

// Definition for GUID_NULL, expected to be declared as extern in the shim utility.h
const GUID GUID_NULL = { 0x00000000, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };

// Ini file stubs (returning defaults or doing minimal operations)
// Helper to convert wstring to string for file operations if needed
std::string LinuxWstringToString(const std::wstring& wstr) {
    // This is a simplified conversion. Production code should handle UTF-8 properly.
    std::string str(wstr.length(), ' ');
    std::transform(wstr.begin(), wstr.end(), str.begin(), [](wchar_t wc){ return (char)wc; });
    return str;
}

std::wstring LinuxStringToWstring(const std::string& str) {
    std::wstring wstr(str.length(), L' ');
    std::transform(str.begin(), str.end(), wstr.begin(), [](char c){ return (wchar_t)c; });
    return wstr;
}


// Very simplified INI handling for stubs
static std::map<std::wstring, std::map<std::wstring, std::wstring>> linux_ini_data;
static std::wstring linux_ini_current_file;

void LoadIniFileLinux(const wchar_t* fileName) {
    if (fileName == nullptr || (linux_ini_current_file == fileName && !linux_ini_data.empty())) {
        return;
    }
    linux_ini_data.clear();
    linux_ini_current_file = fileName;
    std::wifstream wif(LinuxWstringToString(fileName).c_str());
    if (!wif.is_open()) {
        return;
    }
    std::wstring line, currentSection;
    while (std::getline(wif, line)) {
        if (line.empty() || line[0] == L';' || line[0] == L'#') continue;
        if (line[0] == L'[' && line.back() == L']') {
            currentSection = line.substr(1, line.length() - 2);
        } else if (!currentSection.empty()) {
            size_t eqPos = line.find(L'=');
            if (eqPos != std::wstring::npos) {
                linux_ini_data[currentSection][line.substr(0, eqPos)] = line.substr(eqPos + 1);
            }
        }
    }
}

void SaveIniFileLinux(const wchar_t* fileName) {
    if (fileName == nullptr) return;
    std::wofstream wof(LinuxWstringToString(fileName).c_str());
    if (!wof.is_open()) {
        return;
    }
    for (const auto& section : linux_ini_data) {
        wof << L"[" << section.first << L"]" << std::endl;
        for (const auto& entry : section.second) {
            wof << entry.first << L"=" << entry.second << std::endl;
        }
    }
}


DWORD GetPrivateProfileStringW(const wchar_t* lpAppName, const wchar_t* lpKeyName, const wchar_t* lpDefault,
                               wchar_t* lpReturnedString, DWORD nSize, const wchar_t* lpFileName) {
    LoadIniFileLinux(lpFileName);
    if (lpAppName && lpKeyName && lpReturnedString && nSize > 0) {
        auto it_app = linux_ini_data.find(lpAppName);
        if (it_app != linux_ini_data.end()) {
            auto it_key = it_app->second.find(lpKeyName);
            if (it_key != it_app->second.end()) {
                wcsncpy(lpReturnedString, it_key->second.c_str(), nSize -1);
                lpReturnedString[nSize-1] = L'\0';
                return wcslen(lpReturnedString);
            }
        }
        if (lpDefault) {
            wcsncpy(lpReturnedString, lpDefault, nSize -1);
            lpReturnedString[nSize-1] = L'\0';
            return wcslen(lpReturnedString);
        }
    }
    if (nSize > 0 && lpReturnedString) lpReturnedString[0] = L'\0';
    return 0;
}

BOOL WritePrivateProfileStringW(const wchar_t* lpAppName, const wchar_t* lpKeyName, const wchar_t* lpString,
                                const wchar_t* lpFileName) {
    if (!lpAppName || !lpKeyName || !lpFileName) return FALSE;
    LoadIniFileLinux(lpFileName); // Load existing or prepare cache
    linux_ini_data[std::wstring(lpAppName)][std::wstring(lpKeyName)] = (lpString ? std::wstring(lpString) : std::wstring());
    SaveIniFileLinux(lpFileName);
    return TRUE;
}

int GetPrivateProfileIntW(const wchar_t* szSectionName, const wchar_t* szKeyName, int nDefault, const wchar_t* szIniFile) {
    wchar_t default_str[32];
    swprintf(default_str, 32, L"%d", nDefault);
    wchar_t value_str[256];
    GetPrivateProfileStringW(szSectionName, szKeyName, default_str, value_str, 256, szIniFile);
    int val = nDefault;
    if (swscanf(value_str, L"%d", &val) == 1) {
        return val;
    }
    return nDefault;
}

float GetPrivateProfileFloatW(const wchar_t* szSectionName, const wchar_t* szKeyName, float fDefault, const wchar_t* szIniFile) {
    wchar_t default_str[64];
    swprintf(default_str, 64, L"%f", fDefault);
    wchar_t value_str[256];
    GetPrivateProfileStringW(szSectionName, szKeyName, default_str, value_str, 256, szIniFile);
    float val = fDefault;
    // Use wcstof for robust parsing if available, or standard swscanf
    if (swscanf(value_str, L"%f", &val) == 1) {
         return val;
    }
    return fDefault;
}

bool WritePrivateProfileIntW(int d, const wchar_t* szKeyName, const wchar_t* szIniFile, const wchar_t* szSectionName) {
    wchar_t value_str[32];
    swprintf(value_str, 32, L"%d", d);
    return WritePrivateProfileStringW(szSectionName, szKeyName, value_str, szIniFile);
}

bool WritePrivateProfileFloatW(float f, const wchar_t* szKeyName, const wchar_t* szIniFile, const wchar_t* szSectionName) {
    wchar_t value_str[64];
    swprintf(value_str, 64, L"%f", f);
    return WritePrivateProfileStringW(szSectionName, szKeyName, value_str, szIniFile);
}

intptr_t myOpenURL(HWND hwnd, const wchar_t *loc) {
    if (!loc) return 0; // Error
    std::string url = LinuxWstringToString(std::wstring(loc));
    // Basic sanitization for quotes to prevent command injection with system()
    std::string safe_url;
    for (char c : url) {
        if (c == '"') safe_url += "\\\"";
        else safe_url += c;
    }
    std::string cmd = "xdg-open \"" + safe_url + "\"";
    int ret = system(cmd.c_str());
    if (ret == 0) return 33; // Success (mimicking ShellExecute success > 32)
    perror(("xdg-open failed for: " + url).c_str());
    return 0; // Failure
}

void RemoveExtension(wchar_t *str) {
    if (!str) return;
    wchar_t *p = wcsrchr(str, L'.');
    if (p) *p = L'\0';
}

void RemoveSingleAmpersands(wchar_t *str) {
    // Stub or simple implementation if needed for menu display.
    // This function was for Windows GDI specific ampersand handling.
    // If RenderStringOpenGL doesn't need this, it can be empty.
    // For now, simple pass-through or basic removal if absolutely needed by shared logic.
}

bool CheckForMMX() { return true; } // Assume modern CPUs have MMX
bool CheckForSSE() { return true; } // Assume modern CPUs have SSE

void TextToGuid(const char *str, GUID *pGUID) {
    if (pGUID) memset(pGUID, 0, sizeof(GUID));
    if (!str || !pGUID) return;
    // Simplified: sscanf might not be ideal for all GUID string formats.
    // A more robust parser might be needed depending on string format.
    unsigned long d1; unsigned int d2, d3, d4[8];
    int result = sscanf(str, "%8lx-%4x-%4x-%2x%2x-%2x%2x%2x%2x%2x%2x",
        &d1, &d2, &d3, &d4[0], &d4[1], &d4[2], &d4[3], &d4[4], &d4[5], &d4[6], &d4[7]);
    if (result == 11) {
        pGUID->Data1 = d1; pGUID->Data2 = (unsigned short)d2; pGUID->Data3 = (unsigned short)d3;
        for(int i=0; i<8; ++i) pGUID->Data4[i] = (unsigned char)d4[i];
    }
}

void GuidToText(GUID *pGUID, char *str, int nStrLen) {
    if (str && nStrLen > 0) str[0] = '\0';
    if (!pGUID || !str || nStrLen <= 0) return;
    snprintf(str, nStrLen, "%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
        pGUID->Data1, pGUID->Data2, pGUID->Data3,
        pGUID->Data4[0], pGUID->Data4[1], pGUID->Data4[2], pGUID->Data4[3],
        pGUID->Data4[4], pGUID->Data4[5], pGUID->Data4[6], pGUID->Data4[7]);
}

void* GetTextResource(unsigned int id, int no_fallback) {
    return nullptr; // No PE resources on Linux
}

// Stubs for other functions declared in utility.h that were Windows-specific
void MissingDirectX(HWND hwnd) { /* No-op on Linux */ }
void GetDesktopFolder(char *szDesktopFolder) { if(szDesktopFolder) szDesktopFolder[0] = '\0'; }
// ... any other stubs needed based on utility.h's non-conditional declarations ...

#endif // !_WIN32
