#pragma once
#include "StdAfx.h"
#include "MainWindow.h"

MainWindow::MainWindow()
{
	CComObject<Player>::CreateInstance(&m_Player);
}

MainWindow::~MainWindow()
{
}

LRESULT MainWindow::OnCreate(UINT, WPARAM, LPARAM, BOOL&)
{
	HRESULT hr = m_Player->Initialize();
	if (FAILED(hr))
	{
		AtlThrow(hr);
	}

	hr = m_Player->OpenFromURL(_T("C:\\Users\\Public\\Videos\\Sample Videos\\Wildlife.wmv"));
	if (FAILED(hr))
	{
		AtlThrow(hr);
	}

	ResizeClient(300, 300);
	SetWindowText(_T("Media Foundation サンプル1"));

	m_Player->SetVideoWindow(m_hWnd);

	return S_OK;
}

LRESULT MainWindow::OnDestroy(UINT, WPARAM, LPARAM, BOOL&)
{
	m_Player->Uninitialize();
	PostQuitMessage(0);
	return S_OK;
}

LRESULT MainWindow::OnSizeChanged(UINT, WPARAM, LPARAM, BOOL&)
{
	RECT r;
	GetClientRect(&r);
	m_Player->SetVideoWindowPosition(r);
	return DefWindowProc();
}

LRESULT MainWindow::OnPaint(UINT, WPARAM, LPARAM, BOOL&)
{
	return DefWindowProc();
}

LRESULT MainWindow::OnMediaEventChanged(UINT, WPARAM, LPARAM lParam, BOOL&)
{
	return S_OK;
}
