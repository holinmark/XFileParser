#pragma once

#ifndef SFILEIO_H
#define SFILEIO_H

#include <Windows.h>
#include <string>
#include <any>
#include "sErrorMessageException.h"
#include "sFunctionCallHistory.h"

namespace ns_HoLin
{
	class cFileInput
	{
		PHANDLE hfile = nullptr;
		DWORD page_size_in_bytes = 0;
		char *file_buffer = NULL;
		DWORD bytes_read_from_file = 0;
		DWORD index_of_next_char_to_read = 0;
		std::string prevpage;
		char ch;
		BOOL endoffile;

	public:
		cFileInput();
		cFileInput(cFileInput&&) = delete;
		cFileInput(const cFileInput&) = delete;
		~cFileInput();
		void Cleanup();
		cFileInput& operator=(cFileInput&&) = delete;
		cFileInput& operator=(const cFileInput&) = delete;
		explicit operator bool() const;
		BOOL MoveBufferIndex(BOOL);
		char GetCurrentCharToProcess() { return ch; } // Doesn't move the index to the buffer
		void GetBytesFromFile(BYTE*, DWORD, INT_PTR = 0, const char* = nullptr);
		void DeleteBuffer();
		BOOL GetEndOfFileStatus() { return endoffile; }
		void SetEndOfFileStatus(BOOL b) { endoffile = b; }
		void SetFileHandle(PHANDLE pfile) { hfile = pfile; }
		DWORD GetPageSize() { return page_size_in_bytes; }
		void GetOffendingLine(std::string&, std::string&);
	};
}

#endif
