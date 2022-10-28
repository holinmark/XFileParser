// WindowsTemplate.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "WindowsTemplate.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	ns_HoLin::cApplication app;
	
	return app.Run(hInstance, 500, 300, nCmdShow);
}
