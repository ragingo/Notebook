#pragma once


#define WM_GRAPH_NOTIFY  (WM_APP + 1)	// メディア再生終了通知用


namespace Hanamichi
{
	namespace DirectShow
	{
		class Object
		{
		protected:
			virtual void SafeRelease(IUnknown* pObject)
			{
				if(pObject)
				{
					pObject->Release();
					pObject = NULL;
				}
			}
		};

		// マルチメディア制御インタフェイス
		class IMultiMediaControl
		{
		public:
			virtual bool Play() = 0;
			virtual bool Stop() = 0;
			virtual bool Pause() = 0;
			virtual bool Resume() = 0;
			virtual void SetSpeed(double rate) = 0;
			virtual void GetSpeed(double *pRate) = 0;
			virtual void SetVolume(long volume) = 0;
			virtual void GetVolume(long *pVolume) = 0;
			virtual bool SetMute() = 0;
			virtual bool ResetMute() = 0;
			virtual bool GetDuration(LONGLONG *pDuration) = 0;

		};

		// ビデオウィンドウ制御インタフェイス
		class IVideoWindowControl
		{
		public:
			virtual bool SetFullScreen() = 0;
			virtual bool ResetFullScreen() = 0;
			virtual void SetVideoWindowPosition(long left, long top, long width, long height, bool centerPos) = 0;
			virtual void GetVideoWindowPosition(long *pLeft, long *pTop, long *pWidth, long *pHeight) = 0;
			virtual bool ShowVideoWindow(HWND ownerWindow) = 0;
			virtual bool HideVideoWindow() = 0;
		};

		// メディアファイル制御クラス
		class MediaFileControl : public IMultiMediaControl, IVideoWindowControl, Object
		{
		private:
			double			rate;			// 速度
			long			volume;			// 音量
			bool			mute;			// 無音
			bool			play;			// 再生状況
			double			currentPos;		// 現在位置
			__int64			playTotalTime;	// メディア再生時間
			std::string		fileNameA;		// ファイル名 ASCII
			std::wstring	fileNameW;		// ファイル名 Unicode
			HWND			ownerWindow;	// VideoWindowのオーナーウィンドウハンドル
			IBaseFilter		*pBaseFilter;
			IBasicAudio     *pBasicAudio;
			IBasicVideo2    *pBasicVideo;
			IGraphBuilder   *pGraphBuilder;
			IMediaControl   *pMediaControl;
			IMediaEventEx   *pMediaEventEx;
			IMediaSeeking	*pMediaSeeking;
			IVideoFrameStep *pVideoFrameStep;
			IVideoWindow    *pVideoWindow;
			ISampleGrabber  *pSampleGrabber;
			VIDEOINFOHEADER	*pVideoInfo;
			AM_MEDIA_TYPE	mediaType;
			
			void setMediaType();
			bool initialize();
			
			bool createVideoWindow(HWND ownerWindow);
			bool destroyVideoWindow();
			bool canMediaSeek();

		public:
			MediaFileControl(HWND ownerWindow);
			~MediaFileControl();

			bool Initialize();
			bool Play();
			bool Stop();
			bool Pause();
			bool Resume();
			void SetSpeed(double rate);
			void GetSpeed(double *pRate);
			void SetVolume(long volume);
			void GetVolume(long *pVolume);
			bool SetMute();
			bool ResetMute();
			bool GetVideoSize(long *pWidth, long *pHeight);
			bool GetDuration(LONGLONG *pDuration);

			bool SetFullScreen();
			bool ResetFullScreen();
			void SetVideoWindowPosition(long left, long top, long width, long height, bool centerPos);
			void GetVideoWindowPosition(long *pLeft, long *pTop, long *pWidth, long *pHeight);
			bool ShowVideoWindow(HWND ownerWindow);
			bool HideVideoWindow();

			bool OnFileOpen(char *fileName, int buffSize);
		};

		// DVD制御クラス
		class DvdControl : public IMultiMediaControl, IVideoWindowControl, Object
		{
		private:
			double				rate;				// 速度
			long				volume;				// 音量
			bool				mute;				// 無音
			bool				play;				// 再生状況
			double				currentPos;			// 現在位置
			__int64				playTotalTime;		// メディア再生時間
			std::string			discPathA;			// ディスク読込対象ドライブのパス ASCII
			std::wstring		discPathW;			// ディスク読込対象ドライブのパス Unicode
			HWND				ownerWindow;		// VideoWindowのオーナーウィンドウハンドル
			IBasicAudio			*pBasicAudio;
			IBasicVideo2		*pBasicVideo;
			IDvdControl2		*pDvdControl;
			IDvdCmd				*pDvdCmd;
			IDvdGraphBuilder	*pDvdGraphBuilder;
			IDvdInfo2			*pDvdInfo;
			IGraphBuilder   	*pGraphBuilder;
			IMediaControl   	*pMediaControl;
			IMediaEventEx   	*pMediaEventEx;
			IMediaSeeking		*pMediaSeeking;
			IVideoFrameStep		*pVideoFrameStep;
			IVideoWindow    	*pVideoWindow;
			
			bool initialize();
			
			bool createVideoWindow(HWND ownerWindow);
			bool destroyVideoWindow();
			bool canMediaSeek();
			bool onAccessDvdDrive(const wchar_t *discPath);

		public:
			DvdControl(HWND ownerWindow, char *discPath, int buffSize);
			~DvdControl();

			bool Initialize();
			bool Play();
			bool Stop();
			bool Pause();
			bool Resume();
			void SetSpeed(double rate);
			void GetSpeed(double *pRate);
			void SetVolume(long volume);
			void GetVolume(long *pVolume);
			bool SetMute();
			bool ResetMute();
			bool GetVideoSize(long *pWidth, long *pHeight);
			bool GetDuration(LONGLONG *pDuration);

			bool SetFullScreen();
			bool ResetFullScreen();
			void SetVideoWindowPosition(long left, long top, long width, long height, bool centerPos);
			void GetVideoWindowPosition(long *pLeft, long *pTop, long *pWidth, long *pHeight);
			bool ShowVideoWindow(HWND ownerWindow);
			bool HideVideoWindow();

		};

		class DeviceControl : public Object
		{
		private:
			double				rate;				// 速度
			long				volume;				// 音量
			HWND				ownerWindow;		// VideoWindowのオーナーウィンドウハンドル
			IBaseFilter			*pCaptureFilter;
			IBaseFilter			*pGrabFilter;
			IBasicAudio			*pBasicAudio;
			IBasicVideo2		*pBasicVideo;
			ICaptureGraphBuilder2 *pCaptureGraphBuilder2;
			ICreateDevEnum		*pCreateDevEnum;
			IEnumMoniker		*pEnumMoniker;
			IGraphBuilder   	*pGraphBuilder;
			IMediaControl   	*pMediaControl;
			IMediaEventEx   	*pMediaEventEx;
			IMoniker			*pMoniker;
			IVideoFrameStep		*pVideoFrameStep;
			IVideoWindow    	*pVideoWindow;
			ISampleGrabber		*pSampleGrabber;
			VIDEOINFOHEADER		*pVideoInfo;
			AM_MEDIA_TYPE		mediaType;
			
			//void setMediaType();
			bool initialize();
			
			bool createVideoWindow(HWND ownerWindow);
			bool destroyVideoWindow();
			bool canMediaSeek();
			bool onAccessDvdDrive(const wchar_t *discPath);

		public:
			DeviceControl(HWND ownerWindow);
			~DeviceControl();

			bool Initialize();
			bool StartCapture();

			//bool SetFullScreen();
			//bool ResetFullScreen();
			//void SetVideoWindowPosition(long left, long top, long width, long height, bool centerPos);
			//void GetVideoWindowPosition(long *pLeft, long *pTop, long *pWidth, long *pHeight);
			bool ShowVideoWindow(HWND ownerWindow);
			//bool HideVideoWindow();
		};

		//class GraghBuilder : virtual public Object
		//{
		//private:
		//	IGraphBuilder *pGraphBuilder;
		//public:
		//	GraghBuilder()
		//	{
		//		pGraphBuilder = NULL;
		//	}
		//	~GraghBuilder()
		//	{
		//		SafeRelease(pGraphBuilder);
		//	}
		//	bool CreateInstance()
		//	{
		//		HRESULT hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void**)&pGraphBuilder);
		//		if(hr == S_OK)
		//		{
		//			return true;
		//		}
		//		else
		//		{
		//			return false;
		//		}
		//	}
		//};

		class DeviceControl2 : virtual public Object
		{
		private:
			IBaseFilter				*pDeviceFilter;
			IBaseFilter				*pGrabFilter;
			ICaptureGraphBuilder2	*pCaptureGraphBuilder2;
			ICreateDevEnum			*pCreateDevEnum;
			IEnumMoniker			*pEnumMoniker;
			IGraphBuilder   		*pGraphBuilder;
			IMediaControl   		*pMediaControl;
			IMediaEventEx   		*pMediaEventEx;
			IMoniker				*pMoniker;
			ISampleGrabber			*pSampleGrabber;
			IVideoWindow    		*pVideoWindow;
			VIDEOINFOHEADER			*pVideoInfo;
			AM_MEDIA_TYPE			mediaType;

//			typedef struct tagDeviceInfo
//			{
//#ifdef UNICODE
//				std::wstring name;
//#else if
//				std::string name;
//#endif
//				IMoniker *pMonikers;
//			} DeviceInfo deviceInfo;

			bool CreateVideoWindow(HWND hWndParent)
			{
				if(hWndParent)
				{
					// オーナーウィンドウを事前指定している場合
					if(hWndParent)
					{
						pVideoWindow->put_Owner((OAHWND)hWndParent);
						pVideoWindow->put_WindowStyle(WS_CHILD);
						pVideoWindow->SetWindowForeground(OAFALSE);		// VideoWindowにフォーカスを与えない
					}
					else
					{
						pVideoWindow->put_WindowStyle(WS_OVERLAPPEDWINDOW);
						//pVideoWindow->put_Caption("");
						pVideoWindow->SetWindowForeground(OATRUE);		// VideoWindowにフォーカスを与える
					}
				}
				else
				{
					return false;
				}

				return true;
			}

		public:
			DeviceControl2()
			{
				pDeviceFilter			= NULL;
				pGrabFilter				= NULL;
				pCaptureGraphBuilder2	= NULL;
				pCreateDevEnum			= NULL;
				pEnumMoniker			= NULL;
				pGraphBuilder			= NULL;
				pMediaControl			= NULL;
				pMediaEventEx			= NULL;
				pMoniker				= NULL;
				pSampleGrabber			= NULL;
				pVideoWindow			= NULL;
				pVideoInfo				= NULL;
				RtlZeroMemory(&mediaType, sizeof(AM_MEDIA_TYPE));
			}
			~DeviceControl2()
			{
				SafeRelease(pDeviceFilter);
				SafeRelease(pGrabFilter);
				SafeRelease(pCaptureGraphBuilder2);
				SafeRelease(pGraphBuilder);
				SafeRelease(pMediaControl);
				SafeRelease(pMediaEventEx);
				SafeRelease(pSampleGrabber);
				SafeRelease(pVideoWindow);
			}

			bool OpenDevice()
			{
				CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void**)&pGraphBuilder);
				CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC, IID_ICaptureGraphBuilder2, (void**)&pCaptureGraphBuilder2);
				if(!pGraphBuilder)
				{
					return false;
				}
				if(!pCaptureGraphBuilder2)
				{
					return false;
				}
				pCaptureGraphBuilder2->SetFiltergraph(pGraphBuilder);
				pGraphBuilder->QueryInterface<IMediaControl>(&pMediaControl);
				pGraphBuilder->QueryInterface<IMediaEventEx>(&pMediaEventEx);
				pGraphBuilder->QueryInterface<IVideoWindow>(&pVideoWindow);


				// Device関係
				CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC, IID_ICreateDevEnum, (void**)&pCreateDevEnum);
				pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnumMoniker, 0);
				if(!pEnumMoniker)
				{
					// デバイスの未接続
					return false;
				}
				ULONG fetched = 0;
				pEnumMoniker->Reset();
				pEnumMoniker->Next(1, &pMoniker, &fetched);
				pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter, (void**)&pDeviceFilter);
				if(!pDeviceFilter)
				{
					return false;
				}
				pGraphBuilder->AddFilter(pDeviceFilter, L"Device Filter");
				SafeRelease(pCreateDevEnum);
				SafeRelease(pEnumMoniker);
				SafeRelease(pMoniker);
				pCaptureGraphBuilder2->RenderStream(&PIN_CATEGORY_PREVIEW, NULL, pDeviceFilter, NULL, NULL);
				
				
				// SampleGrabber関係
				CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC, IID_IBaseFilter, (void**)&pGrabFilter);
				if(!pGrabFilter)
				{
					return false;
				}
				pGraphBuilder->AddFilter(pGrabFilter, L"Sample Grabber");
				ZeroMemory(&mediaType, sizeof(AM_MEDIA_TYPE));
				mediaType.majortype		= MEDIATYPE_Video;
				mediaType.subtype		= MEDIASUBTYPE_RGB24;
				mediaType.formattype	= FORMAT_VideoInfo;
				pGrabFilter->QueryInterface<ISampleGrabber>(&pSampleGrabber);
				if(!pSampleGrabber)
				{
					return false;
				}
				pSampleGrabber->SetMediaType(&mediaType);

				return true;
			}

			//std::vector<IMoniker*>* GetDevices()
			//{
			//	CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC, IID_ICreateDevEnum, (void**)&pCreateDevEnum);

			//	pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnumMoniker, 0);

			//	if(pEnumMoniker)
			//	{
			//		ULONG fetched = 0;
			//		while (pEnumMoniker->Next(1, &pMoniker, &fetched) == S_OK)
			//		{
			//			//IPropertyBag *pPropertyBag;
			//			//wchar_t deviceName[256];

			//			// 以下7行は、デバイス名取得用   これ取得用メソッドを用意した方がいいかも・・・
			//			//pMoniker->BindToStorage(NULL, NULL, IID_IPropertyBag, (void**)&pPropertyBag);
			//			//VARIANT variant;
			//			//FriendlyNameを取得
			//			//variant.vt = VT_BSTR;
			//			//pPropertyBag->Read(L"FriendlyName", &variant, 0);
			//			//WideCharToMultiByte(CP_ACP, 0, var.bstrVal, -1, deviceName, sizeof(deviceName), 0, 0);
			//			//VariantClear(&variant);

			//			devicesInfo->push_back(pMoniker);
			//		}
			//	}

			//	return devicesInfo;
			//}
			bool ShowVideoWindow(HWND hWndParent, bool visible)
			{
				if(pVideoWindow)
				{
					CreateVideoWindow(hWndParent);
					pVideoWindow->put_Visible(visible?OATRUE:OAFALSE);
				}
				else
				{
					return false;
				}

				return true;
			}

			bool BeginCapture()
			{
				if(!pMediaControl)
				{
					return false;
				}
				pMediaControl->Run();
				return true;
			}

			//void EndCapture();
			//void SaveVideo();
			//void SaveImage();
		};
	}
}
