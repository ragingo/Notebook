// c++11_test2.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

static const LPWSTR VIDEO_FILE_NAME = L"C:\\Users\\Public\\Videos\\Sample Videos\\Wildlife.wmv";

static void Uninitialize()
{
	MFShutdown();
	CoUninitialize();

	std::cout << "Uninitialized." << std::endl;
}

ref class Test1
{
public:
		System::Windows::Forms::Form^ form;
		Test1()
		{
			form = gcnew System::Windows::Forms::Form();
			form->Width = 300;
			form->Height = 300;
		}

};

int _tmain(int argc, _TCHAR* argv[])
{
	CoInitialize(NULL);
	std::atexit(Uninitialize);

	HRESULT hr;

	hr = MFStartup(MF_VERSION);

	CComPtr<IMFMediaSession> pMediaSession = NULL;
	MFCreateMediaSession(NULL, &pMediaSession);
	pMediaSession->Close();
	pMediaSession->BeginGetEvent(NULL, NULL);

	CComPtr<IMFSourceResolver> pSourceResolver = NULL;
	MFCreateSourceResolver(&pSourceResolver);

	MF_OBJECT_TYPE objectType;
	CComPtr<IUnknown> pSource = NULL;
	CComPtr<IMFMediaSource> pMediaSource = NULL;
	pSourceResolver->CreateObjectFromURL(L"C:\\Users\\Public\\Videos\\Sample Videos\\Wildlife.wmv", MF_RESOLUTION_MEDIASOURCE, NULL, &objectType, &pSource);
	pSource->QueryInterface(IID_PPV_ARGS(&pMediaSource));

	CComPtr<IMFPresentationDescriptor> desc;
	DWORD streamCount;
	CComPtr<IMFTopology> topology;
	MFCreateTopology(&topology);

	pMediaSource->CreatePresentationDescriptor(&desc);
	desc->GetStreamDescriptorCount(&streamCount);

	Test1^ t = gcnew Test1();
	t->form->Show(nullptr);

	HWND hWnd = (HWND)(int)(t->form->Handle.ToInt32());


	for(DWORD i=0;i<streamCount; i++)
	{
		CComPtr<IMFStreamDescriptor> stream_desc;
		CComPtr<IMFActivate> sink_activate;
		CComPtr<IMFTopologyNode> src_node;
		CComPtr<IMFTopologyNode> output_node;
		BOOL selected=FALSE;
		hr=desc->GetStreamDescriptorByIndex(i, &selected, &stream_desc);
		if(selected) {
			hr=MFCreateVideoRendererActivate(hWnd, &sink_activate);
			if(SUCCEEDED(hr)) {
				hr=MFCreateTopologyNode(MF_TOPOLOGY_SOURCESTREAM_NODE, &src_node);
				hr=src_node->SetUnknown(MF_TOPONODE_SOURCE, pMediaSource);
				hr=src_node->SetUnknown(MF_TOPONODE_PRESENTATION_DESCRIPTOR, desc);
				hr=src_node->SetUnknown(MF_TOPONODE_STREAM_DESCRIPTOR, stream_desc);
				hr=topology->AddNode(src_node);

				hr=MFCreateTopologyNode(MF_TOPOLOGY_OUTPUT_NODE, &output_node);
				hr=output_node->SetObject(sink_activate);
				hr=output_node->SetUINT32(MF_TOPONODE_STREAMID, 0);
				hr=topology->AddNode(output_node);

				src_node->ConnectOutput(0, output_node, 0);

			}
		}
	}

	pMediaSession->SetTopology(0, topology);


	CComPtr<IMFVideoDisplayControl> pVideo = NULL;
	MFGetService(pMediaSession, MR_VIDEO_RENDER_SERVICE, __uuidof(IMFVideoDisplayControl), (void**)(&pVideo));
	pVideo->SetVideoWindow(hWnd);
	pVideo->RepaintVideo();

	PROPVARIANT var;
	PropVariantInit(&var);
	var.vt = VT_EMPTY;
	pMediaSession->Start(NULL,  &var);



	getch();

	return 0;
}

