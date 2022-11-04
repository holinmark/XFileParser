#pragma once

#ifndef CAPPLICATION_H
#define CAPPLICATION_H

#define ID_BUTTONSELECTFILE				WM_APP + 1
#define ID_BUTTONSHOWHEADER				ID_BUTTONSELECTFILE + 1
#define READMESHFILEFINISHED			ID_BUTTONSHOWHEADER + 1

#include <Windows.h>
#include <DirectXMath.h>
#include <WRL\client.h>
#include <Shobjidl.h>
#include <string>
#include <fstream>
#include <cwchar>
#include <future>
#include <thread>

#ifdef FUNCTIONCALLSTACK
#include "Output.h"
#endif

#include "cBaseWindow.h"
#include "sMaterialList.h"
#include "sFrameList.h"
#include "sMeshList.h"
#include "cTextXFileParser.h"
#include "cBinaryXFileParser.h"
#include "cXFile.h"

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
		BOOL CreateInterface(WPARAM, LPARAM);
		int Run(HINSTANCE, int, int, int);
		LRESULT MessageHandler(UINT, WPARAM, LPARAM);

	protected:
		HWND m_hWnd;
		int client_width, client_height;
		std::thread worker_thread;
		BOOL b_show_headers;

		int MessageLoop();
		HRESULT DirectoryDialog(std::wstring&);
		int GetFileName(HWND, BOOL = FALSE, BOOL = FALSE);
		int KeyPressed(WPARAM, LPARAM);
		BOOL ReadMeshFileWorkFunction(std::wstring, ULONGLONG, DWORD, BOOL = FALSE);
		int ThreadCleanup(WPARAM, LPARAM);
		ATOM MyRegisterClass(HINSTANCE);
		BOOL Create(HINSTANCE, LPCWSTR, DWORD, int, int, int);
		PCWSTR ClassName() const { return L"format"; }
	};

	inline ns_HoLin::cApplication* GetAppState(HWND hwnd)
	{
		return reinterpret_cast<ns_HoLin::cApplication*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}
	void PrintMatrix(DirectX::XMFLOAT4X4);
	void PrintMaterials(std::vector<ns_HoLin::sMaterial>&, ns_HoLin::sMesh*, const char*);
	void PrintMesh(ns_HoLin::sMeshList*);
	void PrintFrames(ns_HoLin::sSequenceOfFrames*);
	void PrintAnimationSet(ns_HoLin::sAnimationSetList*);
	void PrintDuplicates(ns_HoLin::sMesh*);
	void PrintData(ns_HoLin::cTextXFileParser*);
}

#endif
