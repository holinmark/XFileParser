#include "stdafx.h"

namespace ns_HoLin
{
	void WriteToFile(HANDLE hfile, const wchar_t *format, ...)
	{
		if (hfile == NULL)
			return;
		
		va_list arg;
		const DWORD wblen = 1024;
		wchar_t *pbuff = (wchar_t*)new wchar_t[wblen];
		DWORD bytes, byteswritten;

		if (pbuff == NULL)
			return;
		__try {
			va_start(arg, format);
			bytes = _vswprintf_p(pbuff, wblen, format, arg) * sizeof(wchar_t);
			if (hfile)
				WriteFile(hfile, (LPCVOID)pbuff, bytes, &byteswritten, NULL);
			va_end(arg);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			bytes = swprintf_s(pbuff, wblen, L"%s\r\n", L"Error exception thrown");
			if (hfile)
				WriteFile(hfile, (LPCVOID)pbuff, bytes, &byteswritten, NULL);
		}
		delete[] pbuff;
		pbuff = NULL;
	}

	void WriteToConsoleW(const wchar_t *format, ...)
	{
		HANDLE hstdoutput = GetStdHandle(STD_OUTPUT_HANDLE);
		
		if (hstdoutput == INVALID_HANDLE_VALUE)
			return;
		
		va_list arg;
		const DWORD wblen = 1024;
		wchar_t *pbuff = (wchar_t*)new wchar_t[wblen];
		DWORD bytes, byteswritten;

		if (pbuff == NULL)
			return;
		__try {
			va_start(arg, format);
			bytes = _vswprintf_p(pbuff, wblen, format, arg);
			WriteConsole(hstdoutput, pbuff, bytes, &byteswritten, NULL);
			va_end(arg);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			bytes = swprintf_s(pbuff, wblen, L"%s\r\n", L"Error exception thrown");
			WriteConsole(hstdoutput, (LPCVOID)pbuff, bytes, &byteswritten, NULL);
		}
		delete[] pbuff;
		pbuff = NULL;
	}

	void WriteToConsoleA(const char *format, ...)
	{
		HANDLE hstdoutput = GetStdHandle(STD_OUTPUT_HANDLE);
		
		if (hstdoutput == INVALID_HANDLE_VALUE)
			return;
		
		va_list arg;
		const DWORD blen = 1024;
		char *pbuff = (char*)new char[blen];
		DWORD bytes, byteswritten;

		if (pbuff == NULL)
			return;
		__try {
			va_start(arg, format);
			bytes = _vsprintf_p(pbuff, blen, format, arg);
			WriteConsoleA(hstdoutput, pbuff, bytes, &byteswritten, NULL);
			va_end(arg);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			bytes = sprintf_s(pbuff, blen, "%s\r\n", "Error exception thrown");
			WriteConsoleA(hstdoutput, (LPCVOID)pbuff, bytes, &byteswritten, NULL);
		}
		delete[] pbuff;
		pbuff = NULL;
	}

	void DisplayToConsole(wchar_t *pbuff, const size_t blen, const wchar_t *format, ...)
	{
#ifndef FULLSCREENMODE
		va_list arg;
		static DWORD bytestowrite, byteswritten;
		CONSOLE_SCREEN_BUFFER_INFO console;
		HANDLE hstdoutput = GetStdHandle(STD_OUTPUT_HANDLE);
		
		if (hstdoutput == INVALID_HANDLE_VALUE)
			return;
		GetConsoleScreenBufferInfo(hstdoutput, &console);
		__try {
			va_start(arg, format);
			bytestowrite = _vswprintf_p(pbuff, blen, format, arg);
			WriteConsole(hstdoutput, pbuff, bytestowrite, &byteswritten, NULL);
			va_end(arg);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			bytestowrite = swprintf_s(pbuff, blen, L"%s\r\n", L"Error exception handler");
			WriteConsole(hstdoutput, pbuff, bytestowrite, &byteswritten, NULL);
		}
		SetConsoleCursorPosition(hstdoutput, console.dwCursorPosition);
#endif
	}

	BOOL OutputError(HRESULT hres, const wchar_t *perrmessage, unsigned long line, const wchar_t *pfilename)
	{
		wchar_t *perrstring = NULL;
		const DWORD blen = 2048;
		wchar_t *pbuff = (wchar_t*)new wchar_t[blen];

		if (pbuff == NULL)
			return FALSE;
		ns_HoLin::WriteToConsole(L"%u\r\n", HRESULT_CODE(hres));
		if (FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, (DWORD)hres, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&perrstring, 0, NULL) != 0) {
			if (perrstring) {

				swprintf_s(
					pbuff,
					blen,
					L"%u %s\r\n%s : %lu\r\n%s : %s\r\n%s\r\n",
					HRESULT_CODE(hres),
					perrmessage,
					L"Line",
					line,
					L"File",
					pfilename,
					perrstring
				);

				ns_HoLin::WriteToConsole(L"%s\r\n", pbuff);
				LocalFree(perrstring);
				perrstring = NULL;
			}
		}
		else {
			ns_HoLin::WriteToConsole(L"%s %u\r\n%s %d\r\n", L"OutputError Error", GetLastError(), __FILEW__, __LINE__);
		}
		delete[] pbuff;
		pbuff = NULL;
		return FALSE;
	}
	
	BOOL OutputError(const wchar_t *pstrerror, unsigned long linenumber, const wchar_t *filename)
	{
		const size_t blen = 2048;
		wchar_t *pbuff = (wchar_t*)new wchar_t[blen];

		if (pbuff == NULL)
			return FALSE;
		swprintf_s(pbuff, blen, L"%s\r\n%s %lu\r\n%s\r\n", pstrerror, L"Line number", linenumber, filename);
		ns_HoLin::WriteToConsole(L"%s", pbuff);
		delete[] pbuff;
		pbuff = NULL;
		return FALSE;
	}

	BOOL OutputError(wchar_t *buff, std::size_t blen, const wchar_t *format, ...)
	{
		if (buff == NULL) {
			if (blen > 0)
				buff = new wchar_t[blen];
			if (buff == NULL)
				return FALSE;
		}

		HANDLE hstdoutput = GetStdHandle(STD_OUTPUT_HANDLE);
		va_list arg;
		DWORD bytes = 0, byteswritten = 0;

		if (hstdoutput == INVALID_HANDLE_VALUE)
			return FALSE;
		__try {
			va_start(arg, format);
			bytes = _vswprintf_p(buff, blen, format, arg);
			WriteConsole(hstdoutput, (LPVOID)buff, bytes, &byteswritten, NULL);
			va_end(arg);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			bytes = swprintf_s(buff, blen, L"%s\r\n", L"Error exception thrown");
			WriteConsole(hstdoutput, (LPVOID)buff, bytes, &byteswritten, NULL);
		}
		delete[] buff;
		buff = NULL;
		return FALSE;
	}

	BOOL OutputError(DWORD error, const wchar_t *perrmessage, unsigned long line, const wchar_t *file_name)
	{
		return ns_HoLin::OutputError(HRESULT_FROM_WIN32(error), perrmessage, line, file_name);
	}
}
