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
				ns_HoLin::WriteToConsole(TEXT("%s %s\r\n"), TEXT("File selected"), pszFilePath);
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
					ns_HoLin::WriteToConsole(TEXT("%s\r\n"), TEXT("Text file."));
					PrintMesh(&(xfile.GetTextData()->xfiledata.smeshlist));
				}
				else if (xfile.GetXFileType() == BINARY_FILE) {
					ns_HoLin::WriteToConsole(TEXT("%s\r\n"), TEXT("Binary file."));
				}
			}
			else {
				ns_HoLin::WriteToConsole(TEXT("%s \'%s\' %s\r\n"), TEXT("Error reading"), file_name.c_str(), TEXT("file."));
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

	void PrintMatrix(DirectX::XMFLOAT4X4 matrix)
	{
		for (std::size_t row = 0; row < 4; ++row) {
			for (std::size_t col = 0; col < 4; ++col) {
				ns_HoLin::WriteToConsole(TEXT("%f, "), matrix.m[row][col]);
			}
			ns_HoLin::WriteToConsole(TEXT("%s"), "\r\n");
		}
		ns_HoLin::WriteToConsole(TEXT("%s"), "\r\n\r\n");
	}
	
	void PrintMaterials(std::vector<ns_HoLin::sMaterial> &materials, ns_HoLin::sMesh *p_mesh, const char *pstr)
	{
		if (materials.size() > 0) {
			std::clog << pstr << '\n';
			for (std::size_t i = 0; i < materials.size(); ++i) {
				ns_HoLin::WriteToConsoleA("\t%s %s\r\n", "Material name: ", materials[i].name.c_str());

				ns_HoLin::WriteToConsoleA("\t%s %f, %f, %f, %f\r\n",
					"Face color ",
					materials[i].facecolor.x, materials[i].facecolor.y, materials[i].facecolor.z, materials[i].facecolor.w);

				ns_HoLin::WriteToConsoleA("\t%s %f\r\n", "Power", materials[i].power);
				
				ns_HoLin::WriteToConsoleA("\t%s %f, %f, %f\r\n",
					"Specular", materials[i].specularcolor.x, materials[i].specularcolor.y, materials[i].specularcolor.z);
					
				ns_HoLin::WriteToConsoleA("\t%s %f, %f, %f\r\n",
					"Emmisive", materials[i].emissivecolor.x, materials[i].emissivecolor.y, materials[i].emissivecolor.z);
					
				ns_HoLin::WriteToConsoleA("\t%s %s\r\n\r\n", "File name: ", materials[i].filename.c_str());
			}
		}
		ns_HoLin::WriteToConsole(TEXT("%s"), TEXT("\r\n"));
		while (p_mesh) {
			ns_HoLin::WriteToConsoleA("%s %s\r\n", "Mesh name:", p_mesh->name.c_str());
			if (p_mesh->p_extra) {
				for (std::size_t i = 0; i < p_mesh->p_extra->smeshmateriallist.size(); ++i) {
					
					ns_HoLin::WriteToConsoleA("\t%s %s\r\n",
						"Material name:", p_mesh->p_extra->smeshmateriallist[i].name.c_str());
					
					ns_HoLin::WriteToConsole(TEXT("\t%s %f, %f, %f, %f\r\n"),
						TEXT("Face color"),
						p_mesh->p_extra->smeshmateriallist[i].facecolor.x,
						p_mesh->p_extra->smeshmateriallist[i].facecolor.y,
						p_mesh->p_extra->smeshmateriallist[i].facecolor.z,
						p_mesh->p_extra->smeshmateriallist[i].facecolor.w);
						
					ns_HoLin::WriteToConsole(TEXT("\t%s %f\r\n"), TEXT("Power"), p_mesh->p_extra->smeshmateriallist[i].power);
					
					ns_HoLin::WriteToConsole(TEXT("\t%s %f, %f, %f\r\n"), TEXT("Specular"),
						p_mesh->p_extra->smeshmateriallist[i].specularcolor.x,
						p_mesh->p_extra->smeshmateriallist[i].specularcolor.y,
						p_mesh->p_extra->smeshmateriallist[i].specularcolor.z);
						
					ns_HoLin::WriteToConsole(TEXT("\t%s %f, %f, %f\r\n"),
						TEXT("\tEmmisive"),
						p_mesh->p_extra->smeshmateriallist[i].emissivecolor.x,
						p_mesh->p_extra->smeshmateriallist[i].emissivecolor.y,
						p_mesh->p_extra->smeshmateriallist[i].emissivecolor.z);
						
					ns_HoLin::WriteToConsoleA("\t%s %s\r\n",
						"File name:",
						p_mesh->p_extra->smeshmateriallist[i].filename.c_str());
				}
			}
			p_mesh = p_mesh->pnextmesh;
		}
	}
	
	void PrintMesh(ns_HoLin::sMeshList *p_list_of_meshes)
	{
		ns_HoLin::sMesh *pmesh = p_list_of_meshes->pfirstmesh;
		
		if (pmesh) {
			ns_HoLin::WriteToConsole(TEXT("%s %zu\r\n"), TEXT("Number of meshes"), p_list_of_meshes->number_of_meshes);
			while (pmesh) {
				ns_HoLin::WriteToConsoleA("\t%s %s\r\n", "Mesh", pmesh->name.c_str());
				if (pmesh->p_extra) {
					if (pmesh->p_extra->p_skininfo) {
						for (std::size_t i=0; i<pmesh->p_extra->p_skininfo->p_skin_weights.size(); ++i) {
							
							if (pmesh->p_extra->p_skininfo->p_skin_weights[i].p_weights.size() == pmesh->p_extra->p_skininfo->p_skin_weights[i].p_vertexIndices.size()) {
								
								for (std::size_t j=0; j<pmesh->p_extra->p_skininfo->p_skin_weights[i].p_weights.size(); ++j) {
									/*
									ns_HoLin::WriteToConsole(TEXT("%u%s%f\r\n"),
										pmesh->p_extra->p_skininfo->p_skin_weights[i].p_vertexIndices[j],
										TEXT(" : "),
										pmesh->p_extra->p_skininfo->p_skin_weights[i].p_weights[j]);
										*/
								}
							}
						}
					}
				}
				pmesh = pmesh->pnextmesh;
			}
		}
		else {
			ns_HoLin::WriteToConsole(TEXT("%s\r\n\r\n"), TEXT("No mesh."));
		}
	}
	
	void PrintFrames(ns_HoLin::sSequenceOfFrames *pframeseq)
	{
		if (pframeseq != nullptr) {
			std::size_t index = 1;
			ns_HoLin::sFrame *pframe;
			
			ns_HoLin::WriteToConsole(TEXT("\r\n\r\n%s :\r\n"), TEXT("Frames"));
			while (pframeseq) {
				ns_HoLin::WriteToConsole(TEXT("%s\r\n"), TEXT("Keys in frame sequences."));
				for (auto n : pframeseq->framenames) {
					ns_HoLin::WriteToConsoleA("%s, ", n.c_str());
				}
				ns_HoLin::WriteToConsole(TEXT("%s"), TEXT("\r\n"));
				pframe = pframeseq->pfirstframe;
				while (pframe) {
					ns_HoLin::WriteToConsoleA("\t%s %s\r\n", "Frame : ", pframe->name.c_str());
					if (pframe->parent_name.length() > 0)
						ns_HoLin::WriteToConsoleA("\t\t%s %s\r\n", "Parent : ", pframe->parent_name.c_str());
					else
						ns_HoLin::WriteToConsole(TEXT("\t\t%s\r\n"), TEXT("No parent"));
					if (!pframeseq->parent_children[pframe->name].empty()) {
						
						ns_HoLin::WriteToConsole(
							TEXT("\t\t%s %zu\r\n\t\t"),
							TEXT("Children"),
							pframeseq->parent_children[pframe->name].size());
							
						for (auto c : pframeseq->parent_children[pframe->name]) {
							ns_HoLin::WriteToConsoleA("%s, ", c.c_str());
						}
						ns_HoLin::WriteToConsole(TEXT("%s"), TEXT("\r\n"));
					}
					if (pframe->mesh.size() > 0) {
						ns_HoLin::WriteToConsole(TEXT("%s\r\n"), TEXT("Meshes"));
						for (auto m : pframe->mesh) {
							ns_HoLin::WriteToConsoleA("%s, ", m.c_str());
						}
						ns_HoLin::WriteToConsole(TEXT("%s"), TEXT("\r\n"));
					}
					pframe = pframe->pnextframe;
				}
				ns_HoLin::WriteToConsole(TEXT("%s"), TEXT("\r\n"));
				pframeseq = pframeseq->pnextseq;
			}
		}
		else {
			ns_HoLin::WriteToConsole(TEXT("%s\r\n\r\n"), TEXT("No frames."));
		}
	}
	
	void PrintAnimationSet(ns_HoLin::sAnimationSetList *psetlist)
	{
		ns_HoLin::WriteToConsole(TEXT("%s\r\n"), TEXT("\nAnimation data :"));
		if (psetlist) {
			ns_HoLin::sAnimationSet *p_anim_set = psetlist->pfirst_set;
			ns_HoLin::sAnimation *p_anim = nullptr;
			ns_HoLin::sAnimation_Data *p = nullptr;
			std::size_t number_of_animations = 0;
	
			p_anim = psetlist->pfirst_animation;
			while (p_anim) {
				ns_HoLin::WriteToConsoleA("\t%s %s\r\n", "Animation", p_anim->name.c_str());
				p_anim = p_anim->pnextanimation;
			}
			p_anim_set = psetlist->pfirst_set;
			while (p_anim_set) {
				ns_HoLin::WriteToConsoleA("\t%s %s\r\n", "Animation set", p_anim_set->name.c_str());
				p_anim = p_anim_set->pfirstanimation;
				while (p_anim) {
					number_of_animations++;
					ns_HoLin::WriteToConsoleA("\t\t%s %s\r\n", "Animation", p_anim->name.c_str());
					p = p_anim->pfirst_data;
					while (p) {
						ns_HoLin::WriteToConsole(TEXT("\t\t\t%s %u\r\n"), TEXT("Transform type"), p->type_of_transform);
						for (DWORD i = 0; i < p->transformation_data.size(); ++i) {
							ns_HoLin::WriteToConsole(TEXT("%u : "), p->transformation_data[i].time);
							for (DWORD j = 0; j < p->transformation_data[i].tfkeys.size(); ++j) {
								ns_HoLin::WriteToConsole(TEXT("%f, "), p->transformation_data[i].tfkeys[j]);
							}
							ns_HoLin::WriteToConsole(TEXT("%s"), TEXT("\r\n"));
						}
						ns_HoLin::WriteToConsole(TEXT("%s"), TEXT("\r\n"));
						p = p->pnextanimation_data;
					}
					p_anim = p_anim->pnextanimation;
				}
				p_anim_set = p_anim_set->pnext_set;
			}
			ns_HoLin::WriteToConsole(TEXT("\r\n%s %zu\r\n"), TEXT("Number of animations"), number_of_animations);
		}
	}
	
	void PrintDuplicates(ns_HoLin::sMesh *p_mesh)
	{
		while (p_mesh) {
			ns_HoLin::WriteToConsoleA("%s\r\n", p_mesh->name.c_str());
			if (p_mesh->p_extra) {
				for (auto i : p_mesh->p_extra->sduplicates.Indices) {
					ns_HoLin::WriteToConsole(TEXT("%u, "), i);
				}
				ns_HoLin::WriteToConsole(
					TEXT("\r\n%zu %zu\r\n\r\n"),
					p_mesh->p_extra->sduplicates.nIndices, p_mesh->p_extra->sduplicates.nOriginalVertices);
			}
			p_mesh = p_mesh->pnextmesh;
		}
	}
	
	void PrintData(ns_HoLin::cTextXFileParser *p_xfile)
	{
		ns_HoLin::sAnimationSetList *pset = &p_xfile->xfiledata.sanimationsetlist;
	
		PrintMesh(&p_xfile->xfiledata.smeshlist);
		PrintFrames(p_xfile->xfiledata.sframeslist.pfirstseq);
		PrintMaterials(p_xfile->xfiledata.smateriallist, p_xfile->xfiledata.smeshlist.pfirstmesh, "Global materials");
		//PrintAnimationSet(&p_xfile->xfiledata.sanimationsetlist);
		//PrintDuplicates(p_xfile->xfiledata.smeshlist.pfirstmesh);
	}
}
