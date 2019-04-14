// Hanamichi.DirectShow.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//char *str = _com_util::ConvertBSTRToString(L"123ABC\n\n");
	//printf_s("%s", str);

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	CoInitialize(NULL);
	Hanamichi::DirectShow::DeviceControl2 *mc = new Hanamichi::DirectShow::DeviceControl2();
	//Hanamichi::DirectShow::DeviceControl *mc = new Hanamichi::DirectShow::DeviceControl(NULL);
	//Hanamichi::DirectShow::MediaFileControl *mc = new Hanamichi::DirectShow::MediaFileControl(NULL);
	//Hanamichi::DirectShow::DvdControl *mc = new Hanamichi::DirectShow::DvdControl(NULL, "E:\\", 10);
// 実験用処理

	mc->OpenDevice();
	mc->ShowVideoWindow(NULL, true);
	mc->BeginCapture();
	//mc->Initialize();
	//mc->OnFileOpen("C:\\Users\\Gateway_WinVista\\Desktop\\mongol_4.avi", lstrlenA("C:\\Users\\Gateway_WinVista\\Desktop\\mongol_4.avi"));
	//mc->ShowVideoWindow(NULL);
	//mc->StartCapture();
	//mc->Play();
	getchar();
	//mc->Stop();

	delete mc;
	//CoUninitialize();

/*
IGraphBuilder *pGraphBuilder;
 ICaptureGraphBuilder2 *pCaptureGraphBuilder2;
 IMediaControl *pMediaControl;

 // 補足情報）英単語の日本語訳
 //   Enumerate : 列挙、一覧
 //   Moniker   : あだ名
 ICreateDevEnum *pCreateDevEnum = NULL;
 IEnumMoniker *pEnumMoniker = NULL;
 IMoniker *pMoniker = NULL;

 ULONG nFetched = 0;
 IBaseFilter *pDeviceFilter;

 // COMを初期化
 CoInitialize(NULL);

 // FilterGraphを生成
 CoCreateInstance(CLSID_FilterGraph,
	NULL,
	CLSCTX_INPROC,
	IID_IGraphBuilder,
	(LPVOID *)&pGraphBuilder);

 // CaptureGraphBuilder2というキャプチャ用GraphBuilderを生成する
 CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC, 
   IID_ICaptureGraphBuilder2, 
   (LPVOID *)&pCaptureGraphBuilder2);

 // FilterGraphをセットする
 pCaptureGraphBuilder2->SetFiltergraph(pGraphBuilder);

 // MediaControlインターフェース取得
 pGraphBuilder->QueryInterface(IID_IMediaControl,
	(LPVOID *)&pMediaControl);

 // デバイスを列挙するためのCreateDevEnumを生成
 CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, 
   IID_ICreateDevEnum, (PVOID *)&pCreateDevEnum);
    
 // VideoInputDeviceを列挙するためのEnumMonikerを生成 
 pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,
   &pEnumMoniker, 0);
 if (pEnumMoniker == NULL) {
   // 接続された映像入力デバイスが一つも無い場合にはこのif文に入ります
   printf("no device\n");
   return 0;
 }

 // EnumMonikerをResetする
 // Resetすると、先頭から数えなおします
 pEnumMoniker->Reset();

 // 最初のMonikerを取得
 pEnumMoniker->Next(1, &pMoniker, &nFetched);

 // MonkierをFilterにBindする
 pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void**)&pDeviceFilter );

 // FilterGraphにデバイスフィルタを追加する
 pGraphBuilder->AddFilter(pDeviceFilter, L"Device Filter");

 // この時点でMonker系、Enumerate系は用済み
 pMoniker->Release();
 pEnumMoniker->Release();
 pCreateDevEnum->Release();

 // Graphを生成する
 pCaptureGraphBuilder2->RenderStream(&PIN_CATEGORY_PREVIEW,
   NULL, pDeviceFilter, NULL, NULL);

 // 再生開始
 pMediaControl->Run();

 // 再生中にプログラムが終わってしまわないように
 MessageBoxA(NULL,
	"Block Execution",
	"Block",
	MB_OK);

 // 資源を解放
 pMediaControl->Release();
 pCaptureGraphBuilder2->Release();
 pGraphBuilder->Release();*/

 // COM終了
 CoUninitialize();
	return 0;
}

