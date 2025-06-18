// log.cpp

#include "log.h"
#include <string.h> // For strnlen_s, wcsnlen_s, strncpy_s, wcsncpy_s (Windows), strlen, strcpy (Linux)
#include <stdio.h>  // Already included via log.h but good for explicitness (vprintf, etc.)

#define LOG_SIZE 512

#ifdef _WIN32
//LPCTSTR UNICODE _UNICODE // This seems like a comment, not active code

//--------------------------------------------------

//LPCSTR
void LOGA(LPCSTR format, ...) {
	char output_buff[LOG_SIZE];

	char err[20] = ""; //"Warn: ";
	int sl = strnlen_s(err, 20);
	strncpy_s(output_buff, err, sl);

	va_list args_list;
	va_start(args_list, format);
	vsprintf_s(&output_buff[sl], sizeof(output_buff) / sizeof(output_buff[0]) - sl, format, args_list);
	va_end(args_list);

	OutputDebugStringA(output_buff);
    printf("%s\n", output_buff); // Also print to console for Windows command line apps
}

//LPCWSTR
void LOG(LPCWSTR format, ...) {
	wchar_t output_buff[LOG_SIZE];

	wchar_t err[20] = L""; //L"Warn: ";
	int sl = wcsnlen_s(err, 20);
	wcsncpy_s(output_buff, err, sl);

	va_list args_list;
	va_start(args_list, format);
	vswprintf_s(&output_buff[sl], sizeof(output_buff) / sizeof(output_buff[0]) - sl, format, args_list);
	va_end(args_list);

	OutputDebugStringW(output_buff);
    wprintf(L"%s\n", output_buff); // Also print to console
}

//--------------------------------------------------

//LPCSTR
void ERRA(LPCSTR format, ...) {
	char output_buff[LOG_SIZE];

	char err[20] = "Error: ";
	int sl = strnlen_s(err, 20);
	strncpy_s(output_buff, err, sl);

	va_list args_list;
	va_start(args_list, format);
	vsprintf_s(&output_buff[sl], sizeof(output_buff) / sizeof(output_buff[0]) - sl, format, args_list);
	va_end(args_list);

	OutputDebugStringA(output_buff);
    fprintf(stderr, "%s\n", output_buff); // Also print to stderr for Windows
}

//LPCWSTR
void ERR(LPCWSTR format, ...) {
	wchar_t output_buff[LOG_SIZE];

	wchar_t err[20] = L"Error: ";
	int sl = wcsnlen_s(err, 20);
	wcsncpy_s(output_buff, err, sl);

	va_list args_list;
	va_start(args_list, format);
	vswprintf_s(&output_buff[sl], sizeof(output_buff) / sizeof(output_buff[0]) - sl, format, args_list);
	va_end(args_list);

	OutputDebugStringW(output_buff);
    fwprintf(stderr, L"%s\n", output_buff); // Also print to stderr
}

//--------------------------------------------------

#else // Non-Windows (Linux, etc.)

// For non-Windows, LOG and ERR take const char*
void LOG(const char* format, ...) {
    char output_buff[LOG_SIZE];
    va_list args_list;
    va_start(args_list, format);
    // vsnprintf is safer than vsprintf
    vsnprintf(output_buff, LOG_SIZE, format, args_list);
    va_end(args_list);
    printf("%s\n", output_buff);
}

void ERR(const char* format, ...) {
    char output_buff[LOG_SIZE];
    char prefix[20] = "Error: ";
    size_t prefix_len = strlen(prefix);
    strcpy(output_buff, prefix); // Use strcpy_s or strncpy for safety if available and desired

    va_list args_list;
    va_start(args_list, format);
    vsnprintf(output_buff + prefix_len, LOG_SIZE - prefix_len, format, args_list);
    va_end(args_list);
    fprintf(stderr, "%s\n", output_buff);
}

// If LOGA and ERRA were made public in log.h for non-Windows, their implementations would go here.
// For now, they are Windows-only.

#endif // _WIN32
