#include "stdafx.h"

namespace ns_HoLin
{
	cXFile::cXFile()
	{
		file_type = UNKNOWN;
		floatsize = 32;
		hfile = nullptr;
	}

	cXFile::~cXFile()
	{
		Cleanup();
	}

	void cXFile::Cleanup()
	{
		file_type = UNKNOWN;
		floatsize = 32;
		if (hfile) {
			CloseHandle(hfile);
			hfile = nullptr;
		}
		text.xfiledata.Cleanup();
	}
	
	BOOL cXFile::ReadXFile(const wchar_t *file_name, BOOL btrack, BOOL boverride)
	{
		BOOL success = FALSE;
		
		if (hfile)
			return success;
		Cleanup();
		text.xfiledata.Cleanup();
		if (file_name) {
			if (this->OpenMeshFile(file_name)) {
				success = this->ParseFile(btrack, boverride);
				CloseHandle(hfile);
				hfile = nullptr;
			}
		}
		return success;
	}

	bool cXFile::OpenMeshFile(const wchar_t *file_name)
	{
		this->hfile = CreateFile(file_name, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (this->hfile != INVALID_HANDLE_VALUE)
			return true;
		ns_HoLin::WriteToConsole(L"%s \'%s\'\r\n", L"Error couldn\'t locate file", file_name);
		this->hfile = nullptr;
		return false;
	}

	BOOL cXFile::ReadHeader(BOOL boverride_binary_error)
	{
		struct sHeader {
			union{
				struct {
					char buffer[17];
				};
				struct {
					long xfileindicator;
					long version;
					long format;
					long floatsize;
					char end;
				};
			};
			bool ReadHeader(HANDLE hfile) {
				memset(this, 0, sizeof(this));
				DWORD bytes_to_read = (DWORD)_ARRAYSIZE(this->buffer) - sizeof(char), bytes_read = 0;
				if (ReadFile(hfile, (LPVOID)this->buffer, bytes_to_read, &bytes_read, nullptr)) {
					if (bytes_read == bytes_to_read)
						return true;
				}
				return false;
			}
		};
		sHeader headerbuffer;
		DWORD bytesread = 0, bytes_to_read = (_ARRAYSIZE(headerbuffer.buffer) * sizeof(char)) - sizeof(char);
		long data = 0;

		memset(&headerbuffer, 0, sizeof(headerbuffer));
		if (ReadFile(hfile, (LPVOID)headerbuffer.buffer, bytes_to_read, &bytesread, nullptr)) {
			if (bytesread == bytes_to_read) {
				if (memcpy_s((void*)&data, sizeof(long), (const void*)&headerbuffer.xfileindicator, sizeof(long)) == 0) {
					switch (data) {
					case XOFFILE_FORMAT_MAGIC:
						break;
					default:
						ns_HoLin::WriteToConsole(L"%s\r\n", L"Error not an x file.");
						return FALSE;
					}
				}
				if (memcpy_s((void*)&data, sizeof(long), (const void*)&headerbuffer.version, sizeof(long)) == 0) {
					switch (data) {
					case XOFFILE_FORMAT_VERSION:
						break;
					default:
						ns_HoLin::WriteToConsole(L"%s\r\n", L"Error unknown version.\r\nOnly version 0303 supported.");
						return FALSE;
					}
				}
				if (memcpy_s((void*)&data, sizeof(long), (const void*)&headerbuffer.format, sizeof(long)) == 0) {
					switch (data) {
					case XOFFILE_FORMAT_TEXT:
						file_type = TEXT_FILE;
						break;
					case XOFFILE_FORMAT_BINARY:
						file_type = BINARY_FILE;
						if (!boverride_binary_error) {
							ns_HoLin::WriteToConsole(L"%s\r\n", L"Error binary version not supported.");
							return FALSE;
						}
						break;
					case XOFFILE_FORMAT_COMPRESSED:
						file_type = ZIP_FILE;
						ns_HoLin::WriteToConsole(L"%s\r\n", "Error can't read compressed file.");
						return FALSE;
					default:
						ns_HoLin::WriteToConsole(L"%s\r\n", L"Header error, unknown file format.");
						return FALSE;
					}
				}
				if (memcpy_s((void*)&data, sizeof(long), (const void*)&headerbuffer.floatsize, sizeof(long)) == 0) {
					switch (data) {
					case XOFFILE_FORMAT_FLOAT_BITS_32:
						floatsize = 32;
						break;
					case XOFFILE_FORMAT_FLOAT_BITS_64:
						floatsize = 64;
						ns_HoLin::WriteToConsole(L"%s\r\n", L"Error 64 bit float not supported at this time.");
						return FALSE;
					default:
						ns_HoLin::WriteToConsole(L"%s\r\n", L"Error unknown float size.");
						return FALSE;
					}
				}
			}
			else {
				ns_HoLin::WriteToConsole(L"%s\r\n", L"Error unable to read X file header.");
				return FALSE;
			}
		}
		return TRUE;
	}

	BOOL cXFile::ParseFile(BOOL btrack, BOOL boverride_binary_error)
	{
		if (ReadHeader(boverride_binary_error)) {
			if (file_type == TEXT_FILE) {
				return text.ParseFile(&hfile, btrack);
			}
			else if (file_type == BINARY_FILE) {
				if (boverride_binary_error) {
					return binary.ParseFile(&hfile);
				}
				else {
				}
			}
			else if (file_type == ZIP_FILE) {
				return FALSE;
			}
		}
		return FALSE;
	}
}
