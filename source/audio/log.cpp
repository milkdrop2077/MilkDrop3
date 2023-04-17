// log.cpp

#include "log.h"

#define LOG_SIZE 512

//LPCTSTR UNICODE _UNICODE

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
}

//--------------------------------------------------
