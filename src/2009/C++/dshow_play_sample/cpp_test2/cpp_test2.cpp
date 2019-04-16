
#include "stdafx.h"

typedef enum _PlayState
{
	None,
	Play,
	Stop,
	Pause
} PlayState;

class IMedia
{
public:
	virtual void Open(TCHAR*) = 0;
	virtual void Play() = 0;
	virtual void Stop() = 0;
	virtual void Pause() = 0;
};

class WindowBase
{
private:
	WindowBase(const WindowBase& rhs){}
	const WindowBase& operator=(const WindowBase& rhs){}

	CComPtr<IVideoWindow> m_VideoWindow;

protected:
	WindowBase() : m_VideoWindow()
	{
	}
	virtual ~WindowBase()
	{
	}
	void QueryInterfaceFomGraphBuilder(CComPtr<IGraphBuilder>& pSrc)
	{
		if(pSrc != NULL)
		{
			pSrc.QueryInterface<IVideoWindow>(&m_VideoWindow);
		}
	}
public:

};

class Video : WindowBase, IMedia
{
private:
	Video(const Video& rhs){}
	const Video& operator=(const Video& rhs){}

	PlayState m_PlayState;

	CComPtr<IBasicAudio>		m_BasicAudio;
	CComPtr<IBasicVideo2>		m_BasicVideo2;
	CComPtr<IGraphBuilder>		m_GraphBuilder;
	CComPtr<IMediaControl>		m_MediaControl;
	CComPtr<IMediaEventEx>		m_MediaEventEx;
	CComPtr<IVideoFrameStep>	m_VideoFrameStep;

public:
	Video() : 
	  m_PlayState(None),
	  m_BasicAudio(),
	  m_BasicVideo2(),
	  m_GraphBuilder(),
	  m_MediaControl(),
	  m_MediaEventEx(),
	  m_VideoFrameStep()
	{
	}
	~Video()
	{
		if(m_PlayState == ::Play)
		{
			m_MediaControl->Stop();
		}
	}
	void Initialize()
	{
		m_GraphBuilder.CoCreateInstance(CLSID_FilterGraph);
		m_GraphBuilder.QueryInterface<IBasicAudio>(&m_BasicAudio);
		m_GraphBuilder.QueryInterface<IBasicVideo2>(&m_BasicVideo2);
		m_GraphBuilder.QueryInterface<IMediaControl>(&m_MediaControl);
		m_GraphBuilder.QueryInterface<IMediaEventEx>(&m_MediaEventEx);
		m_GraphBuilder.QueryInterface<IVideoFrameStep>(&m_VideoFrameStep);
		this->WindowBase::QueryInterfaceFomGraphBuilder(m_GraphBuilder);
	}
	void Open(TCHAR* lpRenderPath)
	{
		m_GraphBuilder->RenderFile(lpRenderPath, NULL);
	}
	void Play()
	{
		switch(m_PlayState)
		{
		case ::None:
		case ::Stop:
		case ::Pause:
			m_MediaControl->Run();
			m_PlayState = ::Play;
			break;
		}
	}
	void Stop()
	{
		switch(m_PlayState)
		{
		case ::Play:
			m_MediaControl->Stop();
			m_PlayState = ::Stop;
			break;
		}
	}
	void Pause()
	{
		switch(m_PlayState)
		{
		case ::Play:
			m_MediaControl->Pause();
			m_PlayState = ::Pause;
			break;
		}
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	CoInitialize(NULL);

	Video v;
	v.Initialize();
	v.Open(TEXT("C:\\Users\\Public\\Videos\\Sample Videos\\Butterfly.wmv"));
	v.Play();
	MessageBox(0,0,0,0);
	
	CoUninitialize();
	return 0;
}

