
#include "header.h"

// http://somethingforthepain.blog22.fc2.com/blog-entry-49.html
int main()
{
	const wchar_t *input_filename  = L"c:\\windows\\clock.avi";
	const wchar_t *output_filename = L"d:\\test.avi";

	CoInitialize(NULL);
	{
		HRESULT hr;
		CAtlArray<CComPtr<IMoniker>> monikers;
		MonikerEnumerator(CLSID_VideoCompressorCategory, monikers);

		CComPtr<IBaseFilter>   pSource, pSplitter, pCompressor, pMux, pWriter;
		CComPtr<IGraphBuilder> pGraphBuilder;
		CComPtr<IMediaControl> pMediaControl;

		hr = pGraphBuilder.CoCreateInstance(CLSID_FilterGraph);
		hr = pGraphBuilder->AddSourceFilter(input_filename, L"Source", &pSource);
		hr = pGraphBuilder->QueryInterface<IMediaControl>(&pMediaControl);

		hr = pSplitter.CoCreateInstance(CLSID_AviSplitter);
		hr = pMux.CoCreateInstance(CLSID_AviDest);
		hr = pWriter.CoCreateInstance(CLSID_FileWriter);
		hr = monikers[monikers.GetCount()-1]->BindToObject(NULL, NULL, IID_IBaseFilter, reinterpret_cast<void**>(&pCompressor));

		hr = pGraphBuilder->AddFilter(pSource, L"SourceFilter");
		hr = pGraphBuilder->AddFilter(pSplitter, L"Splitter");
		hr = pGraphBuilder->AddFilter(pMux, L"Mux");
		hr = pGraphBuilder->AddFilter(pWriter, L"Writer");
		hr = pGraphBuilder->AddFilter(pCompressor, L"Compressor");

		CComPtr<ICaptureGraphBuilder2> pCaptureGraphBuilder2;
		hr = pCaptureGraphBuilder2.CoCreateInstance(CLSID_CaptureGraphBuilder2);
		hr = pCaptureGraphBuilder2->SetFiltergraph(pGraphBuilder);
		//pCaptureGraphBuilder2->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, pSource, pCompressor, pMux);
		hr = pCaptureGraphBuilder2->RenderStream(NULL, NULL, pSource, NULL, pSplitter);
		//hr = pCaptureGraphBuilder2->RenderStream(NULL, &MEDIATYPE_Video, pSplitter, pCompressor, pMux);
		hr = pCaptureGraphBuilder2->RenderStream(NULL, &MEDIATYPE_Audio, pSplitter, NULL, pMux);
		hr = pCaptureGraphBuilder2->RenderStream(NULL, NULL, pMux, NULL, pWriter);

		hr = pMediaControl->Run();
		MessageBox(0,0,0,0);
		hr = pMediaControl->Stop();
	}
	CoUninitialize();

	return 0;
}