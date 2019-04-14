#include "StdAfx.h"
#include "DirectShow.h"

using namespace std;
using namespace Hanamichi::DirectShow;

//------------------------------------------------------------------------------
// �t�@�C���� : DirectShow.cpp
// �T      �v : DirectShow.h ���̃N���X�̎���
//
// Copyright (c) sep 2007, Hanamichi Corporation.  All rights reserved.
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// �N���X�� : MediaFileControl
// �T    �v : MediaFileControl�N���X�̎���
//------------------------------------------------------------------------------


// ����������
MediaFileControl::MediaFileControl(HWND ownerWindow)
{
	rate				= 1.0;
	volume				= -5000;
	mute				= false;
	play				= false;
	currentPos			= 0.0;
	playTotalTime		= 0;
	this->ownerWindow	= ownerWindow;
	pBaseFilter			= NULL;
	pBasicAudio			= NULL;
	pBasicVideo			= NULL;
	pGraphBuilder		= NULL;
	pMediaControl		= NULL;
	pMediaEventEx		= NULL;
	pMediaSeeking		= NULL;
	pVideoWindow		= NULL;
	pVideoFrameStep		= NULL;
	pSampleGrabber		= NULL;
}

// �I�u�W�F�N�g�j������
MediaFileControl::~MediaFileControl()
{
	SafeRelease(pBasicAudio);
	SafeRelease(pBasicVideo);
	SafeRelease(pGraphBuilder);
	SafeRelease(pMediaControl);
	SafeRelease(pMediaEventEx);
	SafeRelease(pMediaSeeking);
	SafeRelease(pVideoFrameStep);
	SafeRelease(pVideoWindow);
	SafeRelease(pSampleGrabber);
}

// �����ݒ�
bool MediaFileControl::Initialize()
{
	if(!initialize())
	{
		return false;
	}

	ZeroMemory(&mediaType, sizeof(AM_MEDIA_TYPE)); // ��ɍ\���̂�j���������Ȃ���΂Ȃ�Ȃ�
	setMediaType();

	return true;
}

bool MediaFileControl::initialize()
{
	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void**)&pGraphBuilder);
	CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC, IID_IBaseFilter, (void**)&pBaseFilter);
	
	if(pBaseFilter)
	{
		pBaseFilter->QueryInterface<ISampleGrabber>(&pSampleGrabber);
	}
	else
	{
		return false;
	}

	if(pSampleGrabber)
	{
		pSampleGrabber->SetMediaType(&mediaType);
	}
	else
	{
		return false;
	}

	if(pGraphBuilder)
	{
		pGraphBuilder->AddFilter(pBaseFilter, L"Sample Grabber");
		pGraphBuilder->QueryInterface<IBasicAudio>(&pBasicAudio);
		pGraphBuilder->QueryInterface<IBasicVideo2>(&pBasicVideo);
		pGraphBuilder->QueryInterface<IMediaControl>(&pMediaControl);
		pGraphBuilder->QueryInterface<IMediaEventEx>(&pMediaEventEx);
		pGraphBuilder->QueryInterface<IMediaSeeking>(&pMediaSeeking);
		pGraphBuilder->QueryInterface<IVideoWindow>(&pVideoWindow);
		pGraphBuilder->QueryInterface<IVideoFrameStep>(&pVideoFrameStep);
	}
	else
	{
		return false;
	}

	return true;
}


// ���f�B�A�^�C�v�ݒ�(����̉��ǑΏۥ�����f�B�A�I�[�v����ɍœK�ȃ��f�B�A�^�C�v��ݒ�)
void MediaFileControl::setMediaType()
{
	mediaType.majortype		= MEDIATYPE_Video;
	mediaType.subtype		= MEDIASUBTYPE_RGB24;
	mediaType.formattype	= FORMAT_VideoInfo;
}

// �Đ�
bool MediaFileControl::Play()
{
	if(pMediaControl)
	{
		pMediaControl->Run();
		play = true;
	}
	else
	{
		return false;
	}
	return true;
}

// ��~
bool MediaFileControl::Stop()
{
	if(pMediaControl)
	{
		pMediaControl->Stop();
		play = false;
	}
	else
	{
		return false;
	}
	return true;
}

// �ꎞ��~
bool MediaFileControl::Pause()
{
	if(pMediaControl)
	{
		pMediaControl->Pause();
		play = false;
	}
	else
	{
		return false;
	}
	return true;
}

// �ĊJ
bool MediaFileControl::Resume()
{
	if(pMediaControl)
	{
		pMediaControl->Run();
		play = true;
	}
	else
	{
		return false;
	}
	return true;
}

// ���x�擾
void MediaFileControl::GetSpeed(double *pRate)
{
	if(pMediaSeeking)
	{
		pMediaSeeking->GetRate(pRate);
	}
	else
	{
		*pRate = 0.0;
	}
}

// ���x�ݒ�
void MediaFileControl::SetSpeed(double rate)
{
	if(pMediaSeeking)
	{
		this->rate = rate;
		pMediaSeeking->SetRate(this->rate);
	}
	else
	{
		return;
	}
}

// ���ʎ擾
void MediaFileControl::GetVolume(long *pVolume)
{
	if(pBasicAudio)
	{
		pBasicAudio->get_Volume(pVolume);
	}
	else
	{
		*pVolume = -5000;
	}
}

// ���ʐݒ�
void MediaFileControl::SetVolume(long volume)
{
	if(pBasicAudio)
	{
		this->volume = volume;
		pBasicAudio->put_Volume(this->volume);
	}
	else
	{
		return;
	}
}

// �r�f�I�̕��E�������擾
bool MediaFileControl::GetVideoSize(long *pWidth, long *pHeight)
{
	if(pBasicVideo)
	{
		pBasicVideo->get_VideoWidth(pWidth);
		pBasicVideo->get_VideoHeight(pHeight);
	}
	else
	{
		return false;
	}

	return true;
}

// ������
bool MediaFileControl::SetMute(void)
{
	if(pBasicAudio)
	{
		pBasicAudio->put_Volume(0L);
	}
	else
	{
		return false;
	}

	return true;
}

// ��������
bool MediaFileControl::ResetMute(void)
{
	if(pBasicAudio)
	{
		pBasicAudio->put_Volume(this->volume);
	}
	else
	{
		return false;
	}

	return true;
}

// �V�[�N�\���`�F�b�N
bool MediaFileControl::canMediaSeek(void)
{
	if(pMediaSeeking)
	{
		DWORD check = AM_SEEKING_CanSeekAbsolute | AM_SEEKING_CanGetDuration;
		if(pMediaSeeking->CheckCapabilities(&check) == S_OK)
		{
			// ����
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool MediaFileControl::GetDuration(LONGLONG *pDuration)
{
	if(canMediaSeek())
	{
		pMediaSeeking->GetDuration(pDuration);
		playTotalTime = *pDuration;
	}
	else
	{
		return false;
	}

	return true;
}

// VideoWindow�쐬
bool MediaFileControl::createVideoWindow(HWND ownerWindow)
{
	if(pVideoWindow)
	{
		// �I�[�i�[�E�B���h�E�����O�w�肵�Ă���ꍇ
		if(ownerWindow)
		{
			pVideoWindow->put_Owner((OAHWND)ownerWindow);
			pVideoWindow->put_WindowStyle(WS_CHILD);
			pVideoWindow->SetWindowForeground(OAFALSE);		// VideoWindow�Ƀt�H�[�J�X��^���Ȃ�
		}
		else
		{
			pVideoWindow->put_WindowStyle(WS_OVERLAPPEDWINDOW);
			pVideoWindow->put_Caption((BSTR)fileNameW.c_str());
			pVideoWindow->SetWindowForeground(OATRUE);		// VideoWindow�Ƀt�H�[�J�X��^����
		}
	}
	else
	{
		return false;
	}

	return true;
}

// VideoWindow�j��
bool MediaFileControl::destroyVideoWindow()
{
	return false;
}

// �t���X�N���[����
bool MediaFileControl::SetFullScreen()
{
	if(pVideoWindow)
	{
		pVideoWindow->put_FullScreenMode(OATRUE);
	}
	else
	{
		return false;
	}

	return true;
}

// �t���X�N���[������
bool MediaFileControl::ResetFullScreen()
{
	if(pVideoWindow)
	{
		pVideoWindow->put_FullScreenMode(OAFALSE);
	}
	else
	{
		return false;
	}

	return true;
}

// VideoWindow�T�C�Y�擾
void MediaFileControl::GetVideoWindowPosition(long *pLeft, long *pTop, long *pWidth, long *pHeight)
{
	if(pVideoWindow)
	{
		pVideoWindow->GetWindowPosition(pLeft, pTop, pWidth, pHeight);
	}
}

// VideoWindow�T�C�Y�ݒ�
// �I�[�i�[�E�B���h�E�̒����ɔz�u �Fleft, top �s�v
// �X�N���[���̒����ɔz�u         �Fleft, top �s�v
void MediaFileControl::SetVideoWindowPosition(long left, long top, long width, long height, bool centerPos)
{
	if(pVideoWindow)
	{
		if(centerPos)
		{
			if(ownerWindow)
			{
				RECT r;
				GetClientRect(ownerWindow, &r);
				left  = ((r.right  - r.left) - width)  / 2;
				top   = ((r.bottom - r.top)  - height) / 2;
				// �I�[�i�[�E�B���h�E�̒����ɔz�u
				pVideoWindow->SetWindowPosition(left, top, width, height);
			}
			else
			{
				// �X�N���[���̒����ɔz�u
				pVideoWindow->SetWindowPosition(left, top, width, height);
			}
		}
		else
		{
			pVideoWindow->SetWindowPosition(left, top, width, height);
		}
	}
}

// VideoWindow�\��
bool MediaFileControl::ShowVideoWindow(HWND ownerWindow)
{
	if(pVideoWindow)
	{
		createVideoWindow(ownerWindow);
		pVideoWindow->put_Visible(OATRUE);
	}
	else
	{
		return false;
	}

	return true;
}

// VideoWindow��\��
bool MediaFileControl::HideVideoWindow()
{
	if(pVideoWindow)
	{
		pVideoWindow->put_Visible(OAFALSE);
	}
	else
	{
		return false;
	}

	return true;
}

// OPENFILENAME.lpstrFile������擾�����t�@�C������n��
bool MediaFileControl::OnFileOpen(char *fileName, int buffSize)
{
	if(!fileName)
	{
		return false;
	}

	fileNameA.clear();
	fileNameW.clear();
	fileNameA.reserve(buffSize);
	fileNameW.reserve(buffSize);
	fileNameA.append(fileName, buffSize);
	fileNameW.append(_com_util::ConvertStringToBSTR(fileName), buffSize);

	if(pGraphBuilder)
	{
		try
		{
			pGraphBuilder->RenderFile((BSTR)fileNameW.c_str(), NULL);

			if(pSampleGrabber)
			{
				pSampleGrabber->GetConnectedMediaType(&mediaType);
				pVideoInfo = (VIDEOINFOHEADER*)mediaType.pbFormat;
				pSampleGrabber->SetBufferSamples(TRUE);
			}
		}
		catch(...)
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}



//------------------------------------------------------------------------------
// �N���X�� : DvdControl
// �T    �v : DvdControl�N���X�̎���
//------------------------------------------------------------------------------


// ����������
DvdControl::DvdControl(HWND ownerWindow, char *discPath, int buffSize)
{
	rate				= 1.0;
	volume				= -5000;
	mute				= false;
	play				= false;
	currentPos			= 0.0;
	playTotalTime		= 0;
	this->ownerWindow	= ownerWindow;
	pBasicAudio			= NULL;
	pBasicVideo			= NULL;
	pGraphBuilder		= NULL;
	pMediaControl		= NULL;
	pMediaEventEx		= NULL;
	pMediaSeeking		= NULL;
	pVideoWindow		= NULL;
	pVideoFrameStep		= NULL;
	pDvdControl			= NULL;
	pDvdCmd				= NULL;
	pDvdGraphBuilder	= NULL;
	pDvdInfo;
	discPathA.clear();
	discPathW.clear();
	discPathA.reserve(buffSize);
	discPathW.reserve(buffSize);
	discPathA.append(discPath, buffSize);
	discPathW.append(_com_util::ConvertStringToBSTR(discPath), buffSize);
}

// �I�u�W�F�N�g�j������
DvdControl::~DvdControl()
{
	SafeRelease(pBasicAudio);
	SafeRelease(pBasicVideo);
	SafeRelease(pGraphBuilder);
	SafeRelease(pMediaControl);
	SafeRelease(pMediaEventEx);
	SafeRelease(pMediaSeeking);
	SafeRelease(pVideoFrameStep);
	SafeRelease(pVideoWindow);
	SafeRelease(pDvdControl);
	SafeRelease(pDvdCmd);
	SafeRelease(pDvdGraphBuilder);
	SafeRelease(pDvdInfo);
}

// �����ݒ�
bool DvdControl::Initialize()
{
	if(!initialize())
	{
		return false;
	}

	return true;
}

bool DvdControl::initialize()
{
	CoCreateInstance(CLSID_DvdGraphBuilder, NULL, CLSCTX_INPROC_SERVER, IID_IDvdGraphBuilder, (void**)&pDvdGraphBuilder);
	//CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void**)&pGraphBuilder);
	
	if(pDvdGraphBuilder)
	{
		if(!onAccessDvdDrive(discPathW.c_str()))
		{
			return false;
		}

		pDvdGraphBuilder->GetDvdInterface(IID_IDvdInfo2, (void**)&pDvdInfo);
		pDvdGraphBuilder->GetDvdInterface(IID_IDvdControl2, (void**)&pDvdControl);
		pDvdGraphBuilder->GetDvdInterface(IID_IDvdCmd, (void**)&pDvdCmd);
		//pDvdGraphBuilder->GetDvdInterface(IID_IVideoWindow, (void**)&pVideoWindow);

		pDvdGraphBuilder->GetFiltergraph(&pGraphBuilder);
	}
	else
	{
		return false;
	}

	if(pGraphBuilder)
	{
		pGraphBuilder->QueryInterface<IBasicAudio>(&pBasicAudio);
		pGraphBuilder->QueryInterface<IBasicVideo2>(&pBasicVideo);
		pGraphBuilder->QueryInterface<IMediaControl>(&pMediaControl);
		pGraphBuilder->QueryInterface<IMediaEventEx>(&pMediaEventEx);
		pGraphBuilder->QueryInterface<IMediaSeeking>(&pMediaSeeking);
		pGraphBuilder->QueryInterface<IVideoWindow>(&pVideoWindow);
		pGraphBuilder->QueryInterface<IVideoFrameStep>(&pVideoFrameStep);
	}

	return true;
}


// �Đ�
bool DvdControl::Play()
{
	if(pMediaControl)
	{
		pMediaControl->Run();
		play = true;
	}
	else
	{
		return false;
	}
	return true;
}

// ��~
bool DvdControl::Stop()
{
	if(pMediaControl)
	{
		pMediaControl->Stop();
		play = false;
	}
	else
	{
		return false;
	}
	return true;
}

// �ꎞ��~
bool DvdControl::Pause()
{
	if(pMediaControl)
	{
		pMediaControl->Pause();
		play = false;
	}
	else
	{
		return false;
	}
	return true;
}

// �ĊJ
bool DvdControl::Resume()
{
	if(pMediaControl)
	{
		pMediaControl->Run();
		play = true;
	}
	else
	{
		return false;
	}
	return true;
}

// ���x�擾
void DvdControl::GetSpeed(double *pRate)
{
	if(pMediaSeeking)
	{
		pMediaSeeking->GetRate(pRate);
	}
	else
	{
		*pRate = 0.0;
	}
}

// ���x�ݒ�
void DvdControl::SetSpeed(double rate)
{
	if(pMediaSeeking)
	{
		this->rate = rate;
		pMediaSeeking->SetRate(this->rate);
	}
	else
	{
		return;
	}
}

// ���ʎ擾
void DvdControl::GetVolume(long *pVolume)
{
	if(pBasicAudio)
	{
		pBasicAudio->get_Volume(pVolume);
	}
	else
	{
		*pVolume = -5000;
	}
}

// ���ʐݒ�
void DvdControl::SetVolume(long volume)
{
	if(pBasicAudio)
	{
		this->volume = volume;
		pBasicAudio->put_Volume(this->volume);
	}
	else
	{
		return;
	}
}

// �r�f�I�̕��E�������擾
bool DvdControl::GetVideoSize(long *pWidth, long *pHeight)
{
	if(pBasicVideo)
	{
		pBasicVideo->get_VideoWidth(pWidth);
		pBasicVideo->get_VideoHeight(pHeight);
	}
	else
	{
		return false;
	}

	return true;
}

// ������
bool DvdControl::SetMute(void)
{
	if(pBasicAudio)
	{
		pBasicAudio->put_Volume(0L);
	}
	else
	{
		return false;
	}

	return true;
}

// ��������
bool DvdControl::ResetMute(void)
{
	if(pBasicAudio)
	{
		pBasicAudio->put_Volume(this->volume);
	}
	else
	{
		return false;
	}

	return true;
}

// �V�[�N�\���`�F�b�N
bool DvdControl::canMediaSeek(void)
{
	if(pMediaSeeking)
	{
		DWORD check = AM_SEEKING_CanSeekAbsolute | AM_SEEKING_CanGetDuration;
		if(pMediaSeeking->CheckCapabilities(&check) == S_OK)
		{
			// ����
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool DvdControl::GetDuration(LONGLONG *pDuration)
{
	if(canMediaSeek())
	{
		pMediaSeeking->GetDuration(pDuration);
		playTotalTime = *pDuration;
	}
	else
	{
		return false;
	}

	return true;
}

// VideoWindow�쐬
bool DvdControl::createVideoWindow(HWND ownerWindow)
{
	if(pVideoWindow)
	{
		// �I�[�i�[�E�B���h�E�����O�w�肵�Ă���ꍇ
		if(ownerWindow)
		{
			pVideoWindow->put_Owner((OAHWND)ownerWindow);
			pVideoWindow->put_WindowStyle(WS_CHILD);
			pVideoWindow->SetWindowForeground(OAFALSE);		// VideoWindow�Ƀt�H�[�J�X��^���Ȃ�
		}
		else
		{
			pVideoWindow->put_WindowStyle(WS_OVERLAPPEDWINDOW);
			pVideoWindow->put_Caption((BSTR)discPathW.c_str());
			pVideoWindow->SetWindowForeground(OATRUE);		// VideoWindow�Ƀt�H�[�J�X��^����
		}
	}
	else
	{
		return false;
	}

	return true;
}

// VideoWindow�j��
bool DvdControl::destroyVideoWindow()
{
	return false;
}

// �t���X�N���[����
bool DvdControl::SetFullScreen()
{
	if(pVideoWindow)
	{
		pVideoWindow->put_FullScreenMode(OATRUE);
	}
	else
	{
		return false;
	}

	return true;
}

// �t���X�N���[������
bool DvdControl::ResetFullScreen()
{
	if(pVideoWindow)
	{
		pVideoWindow->put_FullScreenMode(OAFALSE);
	}
	else
	{
		return false;
	}

	return true;
}

// VideoWindow�T�C�Y�擾
void DvdControl::GetVideoWindowPosition(long *pLeft, long *pTop, long *pWidth, long *pHeight)
{
	if(pVideoWindow)
	{
		pVideoWindow->GetWindowPosition(pLeft, pTop, pWidth, pHeight);
	}
}

// VideoWindow�T�C�Y�ݒ�
// �I�[�i�[�E�B���h�E�̒����ɔz�u �Fleft, top �s�v
// �X�N���[���̒����ɔz�u         �Fleft, top �s�v
void DvdControl::SetVideoWindowPosition(long left, long top, long width, long height, bool centerPos)
{
	if(pVideoWindow)
	{
		if(centerPos)
		{
			if(ownerWindow)
			{
				RECT r;
				GetClientRect(ownerWindow, &r);
				left  = ((r.right  - r.left) - width)  / 2;
				top   = ((r.bottom - r.top)  - height) / 2;
				// �I�[�i�[�E�B���h�E�̒����ɔz�u
				pVideoWindow->SetWindowPosition(left, top, width, height);
			}
			else
			{
				// �X�N���[���̒����ɔz�u
				pVideoWindow->SetWindowPosition(left, top, width, height);
			}
		}
		else
		{
			pVideoWindow->SetWindowPosition(left, top, width, height);
		}
	}
}

// VideoWindow�\��
bool DvdControl::ShowVideoWindow(HWND ownerWindow)
{
	if(pVideoWindow)
	{
		createVideoWindow(ownerWindow);
		pVideoWindow->put_Visible(OATRUE);
	}
	else
	{
		return false;
	}

	return true;
}

// VideoWindow��\��
bool DvdControl::HideVideoWindow()
{
	if(pVideoWindow)
	{
		pVideoWindow->put_Visible(OAFALSE);
	}
	else
	{
		return false;
	}

	return true;
}

// OPENFILENAME.lpstrFile������擾�����t�@�C������n��
bool DvdControl::onAccessDvdDrive(const wchar_t *discPath)
{
	if(!discPath)
	{
		return false;
	}

	if(pDvdGraphBuilder)
	{
		AM_DVD_RENDERSTATUS	 status;

		pDvdGraphBuilder->RenderDvdVideoVolume(discPath, AM_DVD_VMR9_ONLY /*AM_DVD_HWDEC_PREFER*/, &status);
	}
	else
	{
		return false;
	}

	return true;
}


//------------------------------------------------------------------------------
// �N���X�� : DeviceControl
// �T    �v : DeviceControl�N���X�̎���
//------------------------------------------------------------------------------

// ����������
DeviceControl::DeviceControl(HWND ownerWindow)
{
	this->ownerWindow	= ownerWindow;
	pCaptureGraphBuilder2 = NULL;
	pCaptureFilter		= NULL;
	pCreateDevEnum		= NULL;
	pGrabFilter			= NULL;
	pEnumMoniker		= NULL;
	pBasicAudio			= NULL;
	pBasicVideo			= NULL;
	pGraphBuilder		= NULL;
	pMediaControl		= NULL;
	pMediaEventEx		= NULL;
	pMoniker			= NULL;
	pVideoWindow		= NULL;
	pVideoFrameStep		= NULL;
	pSampleGrabber		= NULL;
}

// �I�u�W�F�N�g�j������
DeviceControl::~DeviceControl()
{
	SafeRelease(pBasicAudio);
	SafeRelease(pBasicVideo);
	SafeRelease(pGraphBuilder);
	SafeRelease(pMediaControl);
	SafeRelease(pMediaEventEx);
	SafeRelease(pVideoFrameStep);
	SafeRelease(pVideoWindow);
	SafeRelease(pSampleGrabber);
}

// �����ݒ�
bool DeviceControl::Initialize()
{
	if(!initialize())
	{
		return false;
	}

	return true;
}

bool DeviceControl::initialize()
{
	CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC, IID_ICreateDevEnum, (void**)&pCreateDevEnum);

	pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnumMoniker, 0);

	if(!pEnumMoniker)
	{
		return false;
	}

	ULONG fetched = 0;
	pEnumMoniker->Reset();
	pEnumMoniker->Next(1, &pMoniker, &fetched);

	if(!pMoniker)
	{
		return false;
	}

	pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter, (void**)&pCaptureFilter);
	SafeRelease(pMoniker);
	SafeRelease(pEnumMoniker);
	SafeRelease(pCreateDevEnum);

	if(!pCaptureFilter)
	{
		return false;
	}

	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void**)&pGraphBuilder);
	CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC, IID_IBaseFilter, (void**)&pGrabFilter);
	CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC, IID_ICaptureGraphBuilder2, (void**)&pCaptureGraphBuilder2);

	if(!pCaptureGraphBuilder2)
	{
		return false;
	}

	pGraphBuilder->AddFilter(pCaptureFilter, L"Video Capture");

	ZeroMemory(&mediaType, sizeof(AM_MEDIA_TYPE));
	mediaType.majortype		= MEDIATYPE_Video;
	mediaType.subtype		= MEDIASUBTYPE_RGB24;
	mediaType.formattype	= FORMAT_VideoInfo;

	if(!pGrabFilter)
	{
		return false;
	}
	pGrabFilter->QueryInterface<ISampleGrabber>(&pSampleGrabber);

	if(!pSampleGrabber)
	{
		return false;
	}
	pSampleGrabber->SetMediaType(&mediaType);

	if(!pCaptureGraphBuilder2)
	{
		return false;
	}
	pCaptureGraphBuilder2->SetFiltergraph(pGraphBuilder);
	//pCaptureGraphBuilder2->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, pCaptureFilter, NULL, pGrabFilter);
	pCaptureGraphBuilder2->RenderStream(&PIN_CATEGORY_PREVIEW, NULL, pCaptureFilter, NULL, NULL);

	if(pGraphBuilder)
	{
		pGraphBuilder->AddFilter(pGrabFilter, L"Sample Grabber");
		pGraphBuilder->QueryInterface<IBasicAudio>(&pBasicAudio);
		pGraphBuilder->QueryInterface<IBasicVideo2>(&pBasicVideo);
		pGraphBuilder->QueryInterface<IMediaControl>(&pMediaControl);
		pGraphBuilder->QueryInterface<IMediaEventEx>(&pMediaEventEx);
		pGraphBuilder->QueryInterface<IVideoWindow>(&pVideoWindow);
		pGraphBuilder->QueryInterface<IVideoFrameStep>(&pVideoFrameStep);
	}
	else
	{
		return false;
	}

	return true;
}

// VideoWindow�\��
bool DeviceControl::ShowVideoWindow(HWND ownerWindow)
{
	if(pVideoWindow)
	{
		createVideoWindow(ownerWindow);
		pVideoWindow->put_Visible(OATRUE);
	}
	else
	{
		return false;
	}

	return true;
}

// VideoWindow�쐬
bool DeviceControl::createVideoWindow(HWND ownerWindow)
{
	if(pVideoWindow)
	{
		// �I�[�i�[�E�B���h�E�����O�w�肵�Ă���ꍇ
		if(ownerWindow)
		{
			pVideoWindow->put_Owner((OAHWND)ownerWindow);
			pVideoWindow->put_WindowStyle(WS_CHILD);
			pVideoWindow->SetWindowForeground(OAFALSE);		// VideoWindow�Ƀt�H�[�J�X��^���Ȃ�
		}
		else
		{
			pVideoWindow->put_WindowStyle(WS_OVERLAPPEDWINDOW);
			//pVideoWindow->put_Caption("");
			pVideoWindow->SetWindowForeground(OATRUE);		// VideoWindow�Ƀt�H�[�J�X��^����
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool DeviceControl::StartCapture()
{
	if(!pMediaControl)
	{
		return false;
	}
	pMediaControl->Run();
	return true;
}

