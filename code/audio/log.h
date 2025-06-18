// log.h

#if defined(_WIN32)
#include <windows.h> // For LPCWSTR, OutputDebugStringW, etc.
#include <stdio.h>   // For vsprintf_s, vswprintf_s (used in log.cpp for Windows)

// Windows-specific declarations (wide strings)
void LOG(LPCWSTR format, ...);
void ERR(LPCWSTR format, ...);

// Windows-specific declarations (narrow strings - if these are meant to be public)
// void LOGA(LPCSTR format, ...);
// void ERRA(LPCSTR format, ...);

#else // Non-Windows platforms

#include <stdio.h>   // For printf, fprintf, va_list, vsnprintf
#include <stdarg.h>  // For va_list, va_start, va_end

// Non-Windows declarations (narrow strings)
// We are simplifying to use narrow strings for LOG/ERR on Linux.
// If wide string support is needed, it would require wprintf and char conversion.
void LOG(const char* format, ...);
void ERR(const char* format, ...);

// Define LPCWSTR and other Windows types if they are used in common code sections
// that are NOT platform-specific. For now, assuming they are confined to _WIN32 blocks.
// typedef const char* LPCSTR; // Example, if needed

#endif // _WIN32 / Non-Windows

// Generic helper macros (optional, could be used if function signatures were identical)
// #define GENERIC_LOG(format, ...) ActualLogFunction(format, ##__VA_ARGS__)
// #define GENERIC_ERR(format, ...) ActualErrFunction("Error: " format, ##__VA_ARGS__)
