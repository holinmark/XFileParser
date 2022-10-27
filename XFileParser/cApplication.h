#pragma once

#ifndef CAPPLICATION_H
#define CAPPLICATION_H

#define ID_BUTTONSELECTFILE				WM_APP + 1
#define ID_BUTTONSHOWHEADER				ID_BUTTONSELECTFILE + 1

#include <Windows.h>
#include <string>
#include <WRL\client.h>
#include <Shobjidl.h>
#include "cBaseWindow.h"
#include "Output.h"

namespace ns_HoLin
{
	class cApplication : public cBaseWindow< cApplication >
	{
	public:
		cApplication();
		~cApplication();
		void Cleanup(HANDLE = nullptr);
		int GetWidth() const { return client_width; }
		int GetHeight() const { return client_height; }
		HWND GetWindowHandle() { return m_hWnd; }
		void SetWindowHandle(HWND h) { m_hWnd = h; }
		BOOL CreateInterface(LPARAM);
		int Run(HINSTANCE, int, int, int);
		LRESULT MessageHandler(UINT, WPARAM, LPARAM);

	protected:
		HWND m_hWnd;
		int client_width, client_height;

		int MessageLoop();
		HRESULT DirectoryDialog(std::wstring&);
		int GetFileName(BOOL = FALSE, BOOL = FALSE);
		int KeyPressed(WPARAM, LPARAM);
		ATOM MyRegisterClass(HINSTANCE);
		BOOL Create(HINSTANCE, LPCWSTR, DWORD, int, int, int);
		PCWSTR ClassName() const { return L"format"; }
	};

	inline ns_HoLin::cApplication* GetAppState(HWND hwnd)
	{
		return reinterpret_cast<ns_HoLin::cApplication*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}
}

#endif
