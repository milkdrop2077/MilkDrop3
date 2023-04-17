#pragma once

#include <windows.h>

extern wchar_t* wasabiApiLangString(int id, wchar_t* buffer, int len);
extern wchar_t* wasabiApiLangString(int id);
extern HMENU wasabiApiLoadMenu(int id);
extern HWND wasabiApiCreateDialogParam(int templateName, HWND parent, DLGPROC proc, LPARAM initParam);