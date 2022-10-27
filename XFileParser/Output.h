#pragma once

#include <windows.h>
#include <memory>
#include <stdio.h>
#include <stdarg.h>

#ifdef _UNICODE
#define WriteToConsole			WriteToConsoleW
#define DisplayToConsole		DisplayToConsoleW
#else
#define WriteToConsole			WriteToConsoleA
#define DisplayToConsole		DisplayToConsoleA
#endif

namespace ns_HoLin
{
	void WriteToFile(HANDLE, const wchar_t *, ...);
	void WriteToConsoleW(const wchar_t*, ...);
	void WriteToConsoleA(const char*, ...);
	void DisplayToConsoleW(wchar_t*, const size_t, const wchar_t*, ...);
	void DisplayToConsoleA(wchar_t*, const size_t, const wchar_t*, ...);
	BOOL OutputError(HRESULT, const wchar_t*, unsigned long, const wchar_t*);
	BOOL OutputError(const wchar_t*, unsigned long, const wchar_t*);
	BOOL OutputError(wchar_t*, std::size_t, const wchar_t*, ...);
	BOOL OutputError(DWORD, const wchar_t*, unsigned long, const wchar_t*);
}
