#pragma once
#include "StdAfx.h"
#include "Player.h"
#include "util.h"


Player::Player() : 
	m_CloseEvent(NULL),
	m_Session(NULL),
	m_hWnd(NULL),
	m_Topology(NULL),
	m_VideoDisplayControl(NULL),
	m_OnGetEvent(this, &Player::OnGetEvent)
{
}

Player::~Player()
{
}

HRESULT Player::OnGetEvent(IMFAsyncResult *pAsyncResult)
{
	MediaEventType type;
	CComPtr<IMFMediaEvent> event;

	HRESULT hr = m_Session->EndGetEvent(pAsyncResult, &event);
	if (FAILED(hr))
	{
		ATLTRACE2(_T("Player::OnGetEvent failed. (IMFMediaSession::EndGetEvent)\r\n"));
		return hr;
	}

	hr = event->GetType(&type);
	if (FAILED(hr))
	{
		ATLTRACE2(_T("Player::OnGetEvent failed. (IMFMediaEvent::GetType)\r\n"));
		return hr;
	}

	ProcessMediaEvent(event, type);

	return S_OK;
}

HRESULT Player::ProcessMediaEvent(const CComPtr<IMFMediaEvent>& mediaEvent, const MediaEventType& mediaEventType)
{
	ATLTRACE2(_T("Player::ProcessMediaEvent. (%s) \r\n"), GetMediaEventTypeName(mediaEventType));

	if (mediaEventType != MESessionClosed)
	{
		HRESULT hr = m_Session->BeginGetEvent(&m_OnGetEvent, NULL);
		if (FAILED(hr))
		{
			ATLTRACE2(_T("Player::ProcessMediaEvent failed.\r\n"));
			return hr;
		}
	}

	switch (mediaEventType)
	{
	case MESessionClosed:
		OnSessionClosed();
		break;

	case MESessionTopologyStatus:
		{
			MF_TOPOSTATUS status;
			mediaEvent->GetUINT32(MF_EVENT_TOPOLOGY_STATUS, reinterpret_cast<UINT32*>(&status));

			switch (status)
			{
			case MF_TOPOSTATUS_READY:
				OnTopologyReady();
				break;
			}
		}
		break;
	}

	::PostMessage(m_hWnd, WM_APP_MEDIA_EVENT, (WPARAM)NULL, (LPARAM)mediaEventType);

	return S_OK;
}

HRESULT Player::OnTopologyReady()
{
	PROPVARIANT var;
	PropVariantInit(&var);
	var.vt = VT_EMPTY;

	HRESULT hr = m_Session->Start(NULL, &var);
	PropVariantClear(&var);

	if (FAILED(hr))
	{
		ATLTRACE2(_T("Player::ProcessMediaEvent failed. (IMFMediaSession::Start)\r\n"));
		return hr;
	}

	MFGetService(m_Session, MR_VIDEO_RENDER_SERVICE, IID_IMFVideoDisplayControl, reinterpret_cast<void**>(&m_VideoDisplayControl));
	m_VideoDisplayControl->SetVideoWindow(m_hWnd);
	RECT rect;
	GetClientRect(m_hWnd, &rect);
	m_VideoDisplayControl->SetVideoPosition(NULL,  &rect);
	m_VideoDisplayControl->SetRenderingPrefs(MFVideoRenderPrefs_AllowBatching | MFVideoRenderPrefs_AllowScaling);

	return S_OK;
}

HRESULT Player::OnSessionClosed()
{
	SetEvent(m_CloseEvent);
	return S_OK;
}

HRESULT Player::Initialize()
{
	m_CloseEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (m_CloseEvent == NULL)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	HRESULT hr = MFStartup(MF_VERSION);
	if (FAILED(hr))
	{
		return hr;
	}

	ATLTRACE2(_T("Player::Initialized.\r\n"));

	return S_OK;
}

HRESULT Player::Uninitialize()
{
	m_Session->Close();
	MFShutdown();
	CloseHandle(m_CloseEvent);

	ATLTRACE2(_T("Player::Uninitialized.\r\n"));

	return S_OK;
}

HRESULT Player::CreateSession()
{
	if (m_Session != NULL)
	{
		return S_OK;
	}

	HRESULT hr = MFCreateMediaSession(NULL, &m_Session);
	if (FAILED(hr))
	{
		ATLTRACE2(_T("Player::CreateSession failed. (MFCreateMediaSession)\r\n"));
		return hr;
	}

	return S_OK;
}

HRESULT Player::OpenFromURL(const LPTSTR lpszUrl)
{
	HRESULT hr = CreateSession();
	if (FAILED(hr))
	{
		return hr;
	}

	hr = m_Session->BeginGetEvent(&m_OnGetEvent, NULL);

	CComPtr<IMFSourceResolver> resolver;
	hr = MFCreateSourceResolver(&resolver);
	if (FAILED(hr))
	{
		ATLTRACE2(_T("Player::OpenFromURL failed. (MFCreateSourceResolver)\r\n"));
		return hr;
	}

	CComPtr<IUnknown> source;
	MF_OBJECT_TYPE type;
	LPWSTR url;
#ifdef UNICODE
	url = lpszUrl;
#else
	url = ATL::CA2W(lpszUrl);
#endif
	hr = resolver->CreateObjectFromURL(url, MF_RESOLUTION_MEDIASOURCE, NULL, &type, &source);
	if (FAILED(hr))
	{
		ATLTRACE2(_T("Player::OpenFromURL failed. (IMFSourceResolver::CreateObjectFromURL)\r\n"));
		return hr;
	}

	m_Source = source;

	hr = MFCreateTopology(&m_Topology);
	if (FAILED(hr))
	{
		return hr;
	}

	CComPtr<IMFPresentationDescriptor> presentationDescriptor;
	hr = m_Source->CreatePresentationDescriptor(&presentationDescriptor);
	if (FAILED(hr))
	{
		ATLTRACE2(_T("Player::OpenFromURL failed. (IMFMEdiaSource::CreatePresentationDescriptor)\r\n"));
		return hr;
	}

	DWORD streamCount;
	hr = presentationDescriptor->GetStreamDescriptorCount(&streamCount);
	if (FAILED(hr))
	{
		ATLTRACE2(_T("Player::OpenFromURL failed. (IMFPresentationDescriptor::GetStreamDescriptorCount)\r\n"));
		return hr;
	}

	for (DWORD i=0; i<streamCount; i++)
	{
		BOOL selected;
		CComPtr<IMFStreamDescriptor> streamDescriptor;
		hr = presentationDescriptor->GetStreamDescriptorByIndex(i, &selected, &streamDescriptor);
		if (FAILED(hr))
		{
			ATLTRACE2(_T("Player::OpenFromURL failed. (IMFPresentationDescriptor::GetStreamDescriptorByIndex)\r\n"));
			return hr;
		}

		if (!selected)
		{
			continue;
		}

		CComPtr<IMFActivate> activate;
		hr = CreateMediaActivate(streamDescriptor, activate);
		if (FAILED(hr))
		{
			ATLTRACE2(_T("Player::OpenFromURL failed. (CreateMediaActivate)\r\n"));
			return hr;
		}

		CComPtr<IMFTopologyNode> sourceNode;
		CComPtr<IMFTopologyNode> outputNode;
		AddSourceNode(presentationDescriptor, streamDescriptor, sourceNode);
		AddOutputNode(activate, outputNode);
		hr = sourceNode->ConnectOutput(0, outputNode, 0);
		if (FAILED(hr))
		{
			ATLTRACE2(_T("Player::OpenFromURL failed. (IMFTopologyNode::ConnectOutput)\r\n"));
			return hr;
		}
	}

	hr = m_Session->SetTopology(0, m_Topology);
	if (FAILED(hr))
	{
		ATLTRACE2(_T("Player::OpenFromURL failed. (IMFMediaSession::SetTopology)\r\n"));
		return hr;
	}

	return S_OK;
}

HRESULT Player::CreateMediaActivate(const CComPtr<IMFStreamDescriptor> &streamDescriptor, CComPtr<IMFActivate> &activate)
{
	CComPtr<IMFMediaTypeHandler> handler;
	HRESULT hr = streamDescriptor->GetMediaTypeHandler(&handler);
	if (FAILED(hr))
	{
		ATLTRACE2(_T("CPlayer::CreateMediaActivate failed. (IMFStreamDescriptor::GetMediaTypeHandler)\r\n"));
		return hr;
	}

	GUID majorType;
	hr = handler->GetMajorType(&majorType);
	if (FAILED(hr))
	{
		ATLTRACE2(_T("CPlayer::CreateMediaActivate failed. (IMFMediaTypeHandler::GetMajorType)\r\n"));
		return hr;
	}

	if (majorType == MFMediaType_Audio)
	{
		hr = MFCreateAudioRendererActivate(&activate);
		if (FAILED(hr))
		{
			ATLTRACE2(_T("CPlayer::CreateMediaActivate failed. (MFCreateAudioRendererActivate)\r\n"));
			return hr;
		}
	}
	else if (majorType == MFMediaType_Video)
	{
		hr = MFCreateVideoRendererActivate(m_hWnd, &activate);
		if (FAILED(hr))
		{
			ATLTRACE2(_T("CPlayer::CreateMediaActivate failed. (MFCreateVideoRendererActivate)\r\n"));
			return hr;
		}
	}

	return S_OK;
}

void Player::AddSourceNode(
	CComPtr<IMFPresentationDescriptor> pres_desc,
	CComPtr<IMFStreamDescriptor> stream_desc,
	CComPtr<IMFTopologyNode> &node)
{
	HRESULT hr;
	hr=MFCreateTopologyNode(MF_TOPOLOGY_SOURCESTREAM_NODE, &node);
	hr=node->SetUnknown(MF_TOPONODE_SOURCE, m_Source);
	hr=node->SetUnknown(MF_TOPONODE_PRESENTATION_DESCRIPTOR, pres_desc);
	hr=node->SetUnknown(MF_TOPONODE_STREAM_DESCRIPTOR, stream_desc);
	hr=m_Topology->AddNode(node);
}

void Player::AddOutputNode(
	CComPtr<IMFActivate> activate,
	CComPtr<IMFTopologyNode> &node)
{
	HRESULT hr;
	hr=MFCreateTopologyNode(MF_TOPOLOGY_OUTPUT_NODE, &node);
	hr=node->SetObject(activate);
	hr=node->SetUINT32(MF_TOPONODE_STREAMID, 0);
	hr=m_Topology->AddNode(node);
}

void Player::SetVideoWindow(const HWND& hWnd)
{
	m_hWnd = hWnd;
}

void Player::SetVideoWindowPosition(const RECT& rect)
{
	if (m_VideoDisplayControl != NULL)
	{
		m_VideoDisplayControl->SetVideoPosition(NULL, const_cast<LPRECT>(&rect));
	}
}