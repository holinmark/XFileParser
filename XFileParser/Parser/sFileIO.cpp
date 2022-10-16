#include "stdafx.h"

namespace ns_HoLin
{
	cFileInput::cFileInput()
	{
		SYSTEM_INFO sys_info;
		
		hfile = nullptr;
		page_size_in_bytes = 0;
		file_buffer = NULL;
		bytes_read_from_file = 0;
		index_of_next_char_to_read = 0;
		page_size_in_bytes = 0;
		GetSystemInfo(&sys_info);
		if (sys_info.dwPageSize > 0) {
			page_size_in_bytes = sys_info.dwPageSize * 10;
			file_buffer = new char[page_size_in_bytes];
		}
		ch = ' ';
		endoffile = FALSE;
	}

	cFileInput::~cFileInput()
	{
		hfile = nullptr;
		page_size_in_bytes = 0;
		bytes_read_from_file = 0;
		index_of_next_char_to_read = 0;
		page_size_in_bytes = 0;
		DeleteBuffer();
		ch = ' ';
		endoffile = FALSE;
	}

	void cFileInput::Cleanup()
	{
	}

	cFileInput::operator bool() const
	{
		return (bool)(*hfile != nullptr);
	}

	BOOL cFileInput::MoveBufferIndex(BOOL btrackoutput)
	{
		if (index_of_next_char_to_read == bytes_read_from_file) {
			prevpage = file_buffer;
			index_of_next_char_to_read = 0;
			file_buffer[0] = '\0';
			if (!ReadFile(*hfile, (LPVOID)file_buffer, (DWORD)(page_size_in_bytes - 1), (LPDWORD)&bytes_read_from_file, nullptr)) {
				hfile = nullptr;
				return FALSE;
			}
			if (bytes_read_from_file == 0) {
				hfile = nullptr;
				endoffile = TRUE;
				return FALSE;
			}
			file_buffer[bytes_read_from_file] = '\0';
		}
		ch = file_buffer[index_of_next_char_to_read++];
		return TRUE;
	}

	void cFileInput::GetBytesFromFile(BYTE *buffer, DWORD bytes_to_read, INT_PTR line, const char *file_name)
	{
#ifdef _DEBUG
		ns_HoLin::sFunctionCallHistory f(__func__);
#endif
		if (!ReadFile(*hfile, (LPVOID)buffer, bytes_to_read, &bytes_read_from_file, nullptr)) {
			ns_HoLin::sErrorMessageException *p = new ns_HoLin::sErrorMessageException();
			
#ifdef _DEBUG
			f.PrintHistoryLog();
#endif
			p->error_messages += std::string("Error, reading from source file.\n") + std::to_string(line);
			p->error_messages.append(1, L'\n');
			p->error_messages += std::string(file_name) + std::string("\n\n");
			hfile = nullptr;
			throw(p);
		}
		if (bytes_read_from_file == 0) {
			throw(std::string("Unexpected end of file.\n") + std::to_string(line) + std::string("\n\n"));
		}
		if (bytes_read_from_file != bytes_to_read) {
			hfile = nullptr;
#ifdef _DEBUG
			f.PrintHistoryLog();
#endif
			throw(std::string("Error, unable to read necessary bytes.\n"));
		}
	}

	void cFileInput::GetOffendingLine(std::string &before, std::string &after)
	{
		DWORD index = 0, count = 0;

		for (index = index_of_next_char_to_read; index < GetPageSize(); ++index) {
			if (file_buffer[index] == '\n')
				break;
			else {
				after.append(1, file_buffer[index]);
			}
		}
		for (index = index_of_next_char_to_read; index < GetPageSize(); --index) {
			if (index == 0) {
				before.insert(std::begin(before), file_buffer[index]);
				for (count = (DWORD)(prevpage.size() - 1); count < GetPageSize(); --count) {
					if (prevpage[count] == '\n')
						break;
					before.insert(std::begin(before), prevpage[count]);
				}
				break;
			}
			before.insert(std::begin(before), file_buffer[index]);
			if (file_buffer[index] == '\n')
				break;
		}
	}

	void cFileInput::DeleteBuffer()
	{
		if (file_buffer) {
			delete[] file_buffer;
			file_buffer = nullptr;
		}
	}
}
