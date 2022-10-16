#include "stdafx.h"

namespace ns_HoLin
{
	cXFile::cXFile()
	{
		file_type = TEXT_FILE;
		floatsize = 32;
		hfile = nullptr;
	}

	cXFile::~cXFile()
	{
		Cleanup();
	}

	void cXFile::Cleanup()
	{
		file_type = TEXT_FILE;
		floatsize = 32;
		if (hfile) {
			CloseHandle(hfile);
			hfile = nullptr;
		}
		text.xfiledata.Cleanup();
	}
	
	BOOL cXFile::ReadCommandLineArgumentsThenParse(DWORD argv, const wchar_t **argc)
	{
		if (hfile)
			return FALSE;

		Cleanup();
		text.xfiledata.Cleanup();

		std::array<std::wstring, 4> state_strings{ // Do not reposition strings
			std::wstring(L"-trackoutput"),
			std::wstring(L"-f"),
			std::wstring(L"-showheader"),
			std::wstring(L"-o")
		};
		std::unordered_map<std::wstring, DWORD> map_options{
			std::make_pair(state_strings[0], 0),
			std::make_pair(state_strings[1], 1),
			std::make_pair(state_strings[2], 2),
			std::make_pair(state_strings[3], 3)
		};
		std::array<std::any, state_strings.size()> arguments_options;
		BOOL btrack = FALSE, override_binary_error = FALSE;

		for (DWORD i = 1; i < argv; ++i) {
			if (state_strings[0] == std::wstring(argc[i])) {
				arguments_options[map_options[state_strings[0]]] = TRUE;
			}
			else if (state_strings[1] == std::wstring(argc[i])) {
				if (argv <= (i + 1)) {
					std::wstring s = L"Error -f option, no file name entered, exiting.";
					
					MessageBoxW(nullptr, s.c_str(), L"Error.", MB_OK);
					return FALSE;
				}
				arguments_options[map_options[state_strings[1]]] = std::wstring((const wchar_t*)argc[++i]);
			}
			else if (state_strings[2] == std::wstring(argc[i])) {
				binary.needed_struct_file.output_header_to_file = TRUE;
			}
			else if (state_strings[3] == std::wstring(argc[i])) {
				override_binary_error = TRUE;
			}
			else {
				std::wstring s = L"Quitting unknown command \'";
				
				s += (const wchar_t*)argc[i];
				s += L"\'";
				MessageBoxW(nullptr, s.c_str(), L"Error.", MB_OK);
				return FALSE;
			}
		}
		if (arguments_options[map_options[state_strings[1]]].has_value()) {
			if (!this->OpenMeshFile(std::any_cast<std::wstring>(arguments_options[map_options[state_strings[1]]]).c_str()))
				return FALSE;
		}
		if (!hfile) {
			OpenFileWithMeshFileName();
		}
		if (hfile) {
			try {
				if (arguments_options[map_options[state_strings[0]]].has_value())
					btrack = std::any_cast<BOOL>(arguments_options[map_options[state_strings[0]]]);
				if (this->ParseFile(btrack, override_binary_error)) {
					MessageBoxW(nullptr, L"File parsed successfully.\n", L"OK", MB_OK);
					CloseHandle(hfile);
					hfile = nullptr;
					return TRUE;
				}
				else {
					MessageBoxW(nullptr, L"Error parsing file.", L"Error", MB_OK);
				}
			}
			catch (ns_HoLin::sErrorMessageException serror) {
				std::cerr << serror.what();
			}
			catch (std::exception e) {
				std::cerr << e.what() << '\n';
			}
			CloseHandle(hfile);
			hfile = nullptr;
		}
		return FALSE;
	}

	BOOL cXFile::ReadXFile(const wchar_t *file_name)
	{
		BOOL success = FALSE;
		
		if (hfile)
			return success;
		Cleanup();
		text.xfiledata.Cleanup();
		if (file_name) {
			if (this->OpenMeshFile(file_name)) {
				success = this->ParseFile(FALSE);
				CloseHandle(hfile);
				hfile = nullptr;
			}
			else {
				MessageBoxW(nullptr, L"Could not open mesh file.", L"Error", MB_OK);
			}
		}
		else {
			MessageBoxW(nullptr, L"No file name entered.", L"Error", MB_OK);
		}
		return success;
	}

	void cXFile::OpenFileWithMeshFileName()
	{
		std::wifstream fin;
		wchar_t buff[MAX_PATH];
		DWORD bytes_read = 0;

		buff[0] = L'\0';
		fin.open(L"file.txt");
		if (fin.is_open()) {
			fin.getline(buff, MAX_PATH);
			if (std::wcslen(buff) > 0) {
				OpenMeshFile(buff);
			}
			else {
				MessageBoxW(nullptr, L"\nError \'file.txt\' empty.\n", L"Error.", MB_OK);
			}
			fin.close();
		}
		else {
			MessageBoxW(nullptr, L"\nError opening \'file.txt\'.", L"Error loading file.", MB_OK);
		}
	}

	bool cXFile::OpenMeshFile(const wchar_t *file_name)
	{
		this->hfile = CreateFile(file_name, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (this->hfile != INVALID_HANDLE_VALUE)
			return true;
		std::wstring s(L"Error could not locate ");
		s.append(file_name);
		MessageBoxW(nullptr, s.c_str(), L"File error.", MB_OK);
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
						MessageBoxW(nullptr, L"Error not an x file.", L"Error", MB_OK);
						return FALSE;
					}
				}
				if (memcpy_s((void*)&data, sizeof(long), (const void*)&headerbuffer.version, sizeof(long)) == 0) {
					switch (data) {
					case XOFFILE_FORMAT_VERSION:
						break;
					default:
						MessageBoxW(nullptr, L"Error unknown version.\r\nOnly version 0303 supported.", L"Error", MB_OK);
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
							MessageBoxW(nullptr, L"Binary version not supported.", L"Error", MB_OK);
							return FALSE;
						}
						break;
					case XOFFILE_FORMAT_COMPRESSED:
						file_type = ZIP_FILE;
						MessageBoxW(nullptr, L"Can't read compressed file.", L"Error", MB_OK);
						return FALSE;
					default:
						MessageBoxW(nullptr, L"Header error, unknown file format.", L"Error", MB_OK);
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
						MessageBoxW(nullptr, L"64 bit float not supported at this time.", L"Error", MB_OK);
						return FALSE;
					default:
						MessageBoxW(nullptr, L"Unknown float size.", L"Error", MB_OK);
						return FALSE;
					}
				}
			}
			else {
				MessageBoxW(nullptr, L"Unable to read X file header.", L"Error", MB_OK);
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
			}
		}
		return FALSE;
	}
}
