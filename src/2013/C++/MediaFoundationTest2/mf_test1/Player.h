#pragma once
#include "stdafx.h"

class Player : 
	public CComObjectRootEx<CComMultiThreadModel>
{
	BEGIN_COM_MAP(Player)
	END_COM_MAP()
	DECLARE_PROTECT_FINAL_CONSTRUCT();

private:
	HANDLE m_CloseEvent;
	HWND m_hWnd;
	CComPtr<IMFMediaSession> m_Session;
	CComQIPtr<IMFMediaSource> m_Source;
	CComPtr<IMFTopology> m_Topology;
	CComPtr<IMFVideoDisplayControl> m_VideoDisplayControl;

	AsyncCallback<Player> m_OnGetEvent;
	HRESULT OnGetEvent(IMFAsyncResult *pAsyncResult);
	HRESULT ProcessMediaEvent(const CComPtr<IMFMediaEvent>& mediaEvent, const MediaEventType& mediaEventType);

	HRESULT OnTopologyReady();
	HRESULT OnSessionClosed();

	HRESULT CreateSession();

public:
	Player();
	~Player();

	HRESULT Initialize();
	HRESULT Uninitialize();
	HRESULT OpenFromURL(const LPTSTR lpszUrl);
	HRESULT CreateMediaActivate(const CComPtr<IMFStreamDescriptor> &streamDescriptor, CComPtr<IMFActivate> &activate);
	void AddSourceNode(CComPtr<IMFPresentationDescriptor> pres_desc, CComPtr<IMFStreamDescriptor> stream_desc, CComPtr<IMFTopologyNode> &node);
	void AddOutputNode(CComPtr<IMFActivate> activate, CComPtr<IMFTopologyNode> &node);
	void SetVideoWindow(const HWND& hWnd);
	void SetVideoWindowPosition(const RECT& rect);
};

