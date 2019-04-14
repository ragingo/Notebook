#pragma once
#include "stdafx.h"
#include "Player.h"


typedef CWinTraits<WS_VISIBLE | WS_OVERLAPPEDWINDOW> MainWindowTraits;

class MainWindow :
	public CWindowImpl<MainWindow, CWindow, MainWindowTraits>,
	public CComObjectRootEx<CComMultiThreadModel>
{
	BEGIN_COM_MAP(MainWindow)
	END_COM_MAP()

	BEGIN_MSG_MAP(MainWindow)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_SIZE, OnSizeChanged)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_APP_MEDIA_EVENT, OnMediaEventChanged)
	END_MSG_MAP()

private:
	CComObject<Player> *m_Player;

	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnSizeChanged(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnMediaEventChanged(UINT, WPARAM, LPARAM lParam, BOOL&);


public:
	MainWindow();
	~MainWindow();
};

