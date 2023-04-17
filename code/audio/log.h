// log.h

#include <stdio.h>
#include <windows.h>

//#define LOG(format, ...) wprintf(format L"\n", __VA_ARGS__)
//#define ERR(format, ...) LOG(L"Error: " format, __VA_ARGS__)

void LOG(LPCWSTR format, ...);
void ERR(LPCWSTR format, ...);
