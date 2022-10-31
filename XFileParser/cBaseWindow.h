#pragma once

#ifndef CBASEWINDOW_H
#define CBASEWINDOW_H

#include <Windows.h>

namespace ns_HoLin
{
	template <typename Derived>
	class cBaseWindow
	{
	public:
		cBaseWindow() {}
		static LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
		{
			Derived *p_derived = nullptr;

			if (msg == WM_NCCREATE) {
				if (lp) {
					LPCREATESTRUCT p_cs = reinterpret_cast<LPCREATESTRUCT>(lp);

					if (p_cs->lpCreateParams) {
						p_derived = reinterpret_cast<Derived*>(p_cs->lpCreateParams);
						if (SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)p_cs->lpCreateParams) == 0) {
							if (GetLastError() == 0) {
								p_derived->SetWindowHandle(hWnd);
								SetWindowText(hWnd, p_cs->lpszName);
								return (LRESULT)TRUE;
							}
						}
					}
				}
				return (LRESULT)FALSE;
			}
			else if (msg == WM_CREATE) {
				p_derived = reinterpret_cast<Derived*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
				if (p_derived) {
					if (p_derived->CreateInterface(wp, lp)) {
						return (LRESULT)0;
					}
				}
				return(LRESULT)-1;
			}
			if (!p_derived) {
				p_derived = reinterpret_cast<Derived*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
			}
			if (p_derived) {
				return p_derived->MessageHandler(msg, wp, lp);
			}
			return DefWindowProc(hWnd, msg, wp, lp);
		}

	protected:
		virtual void SetWindowHandle(HWND) = 0;
		virtual PCWSTR ClassName() const = 0;
		virtual BOOL CreateInterface(WPARAM, LPARAM) = 0;
		virtual LRESULT MessageHandler(UINT, WPARAM, LPARAM) = 0;
	};
}

#endif
