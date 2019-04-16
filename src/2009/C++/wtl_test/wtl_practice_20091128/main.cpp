
#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "header.h"
#include "CMainFrame.h"

CAppModule _Module;

// �G���g���|�C���g
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpstrCmdLine*/, int nCmdShow)
{
#ifdef DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	DefWindowProc(NULL, 0, 0, 0);
	_Module.Init(NULL, hInstance);

	CMessageLoop msgLoop;
	_Module.AddMessageLoop(&msgLoop);

	// �����ŃE�B���h�E�쐬���\��
	CMainFrame wnd;
	wnd.CreateEx();
	wnd.ShowWindow(nCmdShow);

	int ret = msgLoop.Run();

	_Module.RemoveMessageLoop();
	_Module.Term();
	CoUninitialize();

	return ret;
}