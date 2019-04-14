#pragma once
#include "stdafx.h"
#include "MainWindow.h"

class MFApp : public CAtlExeModuleT<MFApp>
{
private:
	CComObject<MainWindow> *m_MainWindow;
public:
	MFApp();
	~MFApp();

	HRESULT PreMessageLoop(_In_ int nCmdShow) throw();
	HRESULT PostMessageLoop() throw();
};

