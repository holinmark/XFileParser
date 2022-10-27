#include "stdafx.h"

namespace ns_HoLin
{
	cApplication::cApplication()
	{
		m_hWnd = nullptr;
		client_width = client_height = 0;
	}

	cApplication::~cApplication()
	{
		this->Cleanup();
	}

	void cApplication::Cleanup(HANDLE hfile)
	{
	}
	
	BOOL cApplication::CreateInterface(LPARAM lp)
	{
		LPCREATESTRUCT p_cs = (LPCREATESTRUCT)lp;
		RECT r;
		int xpos = 0, ypos = 0;
		int width = 300, height = 50;
		int height_seperator = 5;
		
		if (p_cs) {
			if (GetClientRect(this->m_hWnd, &r)) {
				xpos = static_cast<int>(((r.right - r.left) - width) / 2);
				ypos = static_cast<int>(((r.bottom - r.top) - height) / 5);
				
				HWND h_button = CreateWindowEx(
					0,
					L"BUTTON",
					L"Select file",
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					xpos,
					ypos,
					width,
					height,
					this->m_hWnd,
					(HMENU)MAKEINTRESOURCE(ID_BUTTONSELECTFILE),
					p_cs->hInstance,
					nullptr
				);
				
				if (!h_button) {
					return FALSE;
				}
				
				int checkbox_width = 150, checkbox_height = 50;
				
				HWND h_check = CreateWindowEx(
					0,
					L"BUTTON",
					L"Show Headers",
					WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
					xpos,
					(int)(ypos + height + height_seperator),
					checkbox_width,
					checkbox_height,
					this->m_hWnd,
					(HMENU)MAKEINTRESOURCE(ID_BUTTONSHOWHEADER),
					p_cs->hInstance,
					nullptr
				);
				
				if (!h_check) {
					return FALSE;
				}
				return TRUE;
			}
		}
		return FALSE;
	}
	
	int cApplication::Run(HINSTANCE hInstance, int width, int height, int nCmdShow)
	{
		int ret = 0;
		HANDLE hfile = INVALID_HANDLE_VALUE;

		if (AllocConsole()) {
			if (this->Create(hInstance, (LPCWSTR)L"Windows Template", WS_OVERLAPPEDWINDOW, width, height, nCmdShow)) {
				ShowWindow(this->m_hWnd, nCmdShow);
				UpdateWindow(this->m_hWnd);
				ret = this->MessageLoop();
			}
			hfile = CreateFile(L"Cleanup.txt", GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
			this->Cleanup(hfile);
			CloseHandle(hfile);
			hfile = INVALID_HANDLE_VALUE;
			FreeConsole();
		}
		return ret;
	}

	int cApplication::MessageLoop()
	{
		MSG msg;

		while (TRUE) {
			if (GetMessage(&msg, nullptr, 0, 0) > 0) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else {
				break;
			}
		}
		UnregisterClass(this->ClassName(), nullptr);
		return (int)msg.wParam;
	}

	LRESULT cApplication::MessageHandler(UINT msg, WPARAM wP, LPARAM lP)
	{
		switch (msg) {
		case WM_COMMAND:
			switch (LOWORD(wP)) {
			case ID_BUTTONSELECTFILE:
				return this->GetFileName();
			case ID_BUTTONSHOWHEADER:
				SetFocus(this->m_hWnd);
				return 0;
			}
			break;
		case WM_KEYUP:
			return this->KeyPressed(wP, lP);
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(this->m_hWnd, &ps);
			// TODO: Add any drawing code that uses hdc here...
			EndPaint(this->m_hWnd, &ps);
		}
		return 0;
		case WM_CLOSE:
			DestroyWindow(this->m_hWnd);
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}
		return DefWindowProc(this->m_hWnd, msg, wP, lP);
	}
	
	HRESULT cApplication::DirectoryDialog(std::wstring &file_name)
	{
		// CoCreate the File Open Dialog object.
		Microsoft::WRL::ComPtr<IFileOpenDialog> fd;
		Microsoft::WRL::ComPtr<IShellItem> siResult;
		HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&fd));
		DWORD dwFlags = 0;
		PWSTR pszFilePath = NULL;
		
		if (FAILED(hr)) {
			return hr;
		}
		// Before setting, always get the options first in order 
		// not to override existing options.
		hr = fd->GetOptions(&dwFlags);
		if (FAILED(hr)) {
			return hr;
		}
		// In this case, get shell items only for file system items.
		hr = fd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
		if (FAILED(hr)) {
			return hr;
		}
		// Set the default extension to be ".doc" file.
		hr = fd->SetDefaultExtension(L"*.*");
		if (FAILED(hr)) {
			return hr;
		}
		// Show the dialog
		hr = fd->Show(NULL);
		if (FAILED(hr)) {
			return hr;
		}
		// Obtain the result once the user clicks 
		// the 'Open' button.
		// The result is an IShellItem object.
		hr = fd->GetResult(&siResult);
		if (SUCCEEDED(hr)) {
			// We are just going to print out the 
			// name of the file for sample sake.
			hr = siResult->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
			if (SUCCEEDED(hr)) {
				file_name = pszFilePath;
				ns_HoLin::WriteToConsole(L"%s %s\r\n", L"File selected", pszFilePath);
				CoTaskMemFree(pszFilePath);
				return (HRESULT)S_OK;
			}
		}
		return hr;
	}
	
	int cApplication::GetFileName(BOOL boverride, BOOL show_headers)
	{
		std::wstring file_name;
		HANDLE hfile = INVALID_HANDLE_VALUE;
		ns_HoLin::cXFile xfile;
		
		if (SUCCEEDED(this->DirectoryDialog(file_name))) {
			xfile.GetBinaryData()->needed_struct_file.output_header_to_file = show_headers;
			if (xfile.ReadXFile(file_name.c_str(), FALSE, boverride)) {
				if (xfile.GetXFileType() == TEXT_FILE) {
					MessageBox(nullptr, L"Text file.", L"File type.", MB_OK);
				}
				else if (xfile.GetXFileType() == BINARY_FILE) {
					MessageBox(nullptr, L"Binary file.", L"File type.", MB_OK);
				}
			}
			else {
				MessageBox(nullptr, L"Error reading file.", L"Error!", MB_OK);
			}
		}
		xfile.GetBinaryData()->needed_struct_file.output_header_to_file = FALSE;
		return 0;
	}
	
	int cApplication::KeyPressed(WPARAM wp, LPARAM lp)
	{
		switch (wp) {
		case 0x4F: // O
			SetFocus(this->m_hWnd);
			if (SendMessage(GetDlgItem(this->m_hWnd, ID_BUTTONSHOWHEADER), BM_GETCHECK, 0, 0) == BST_CHECKED) {
				return this->GetFileName(TRUE, TRUE);
			}
			return this->GetFileName(TRUE);
		}
		return 0;
	}
	
	ATOM cApplication::MyRegisterClass(HINSTANCE hInstance)
	{
		WNDCLASSEXW wcex;

		memset((void*)&wcex, 0, sizeof(wcex));
		wcex.cbSize = sizeof(wcex);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = cApplication::WindowProcedure;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSTEMPLATE));
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = (LPCTSTR)this->ClassName();
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
		return RegisterClassExW(&wcex);
	}

	BOOL cApplication::Create(HINSTANCE hInstance, LPCWSTR szTitle, DWORD style, int width, int height, int nCmdShow)
	{
		if (!MyRegisterClass(hInstance)) {
			return FALSE;
		}

		RECT r;
		int screenwidth = 0, screenheight = 0;
		int xpos = 0, ypos = 0;

		screenwidth = GetSystemMetrics(SM_CXSCREEN);
		screenheight = GetSystemMetrics(SM_CYSCREEN);
		if (!SetRect(&r, 0, 0, width, height)) {
			return FALSE;
		}
		if (!AdjustWindowRect(&r, style, FALSE)) {
			return FALSE;
		}
		client_width = r.right - r.left;
		client_height = r.bottom - r.top;
		xpos = (int)((screenwidth - client_width) / 2);
		ypos = (int)((screenheight - client_height) / 2);

		HWND hWnd = CreateWindowEx(
			0,
			(LPCWSTR)this->ClassName(),
			(LPCWSTR)L"Windows Program",
			style,
			xpos,
			ypos,
			client_width,
			client_height,
			nullptr,
			nullptr,
			hInstance,
			(LPVOID)this);

		if (!hWnd) {
			UnregisterClass(this->ClassName(), nullptr);
			return FALSE;
		}
		return TRUE;
	}
}
