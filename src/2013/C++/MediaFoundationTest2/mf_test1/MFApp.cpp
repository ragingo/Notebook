#include "StdAfx.h"
#include "MFApp.h"

MFApp::MFApp()
{
}


MFApp::~MFApp()
{
}

HRESULT MFApp::PreMessageLoop(_In_ int nCmdShow) throw()
{
	CComObject<MainWindow>::CreateInstance(&m_MainWindow);
	m_MainWindow->AddRef();
	m_MainWindow->Create(GetDesktopWindow());
	m_MainWindow->ShowWindow(nCmdShow);
	m_MainWindow->UpdateWindow();
	return S_OK;
}

HRESULT MFApp::PostMessageLoop() throw()
{
	m_MainWindow->Release();
	return S_OK;
}
