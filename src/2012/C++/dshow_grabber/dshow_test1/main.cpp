
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <atlbase.h>
#include <atlstr.h>
#include <atlcoll.h>

#pragma comment(lib, "strmiids.lib")
#include <dshow.h>
#pragma warning(disable:4192)
#import "qedit.dll" raw_interfaces_only named_guids rename("GetUserName","GetUserName2")

EXTERN_C const CLSID CLSID_SampleGrabber;

void SaveImage(CAtlString saveFileName, long* buffer, long bufferLength, BITMAPINFOHEADER infoHeader)
{
	BITMAPFILEHEADER fileHeader;
	ZeroMemory(&fileHeader, sizeof(BITMAPFILEHEADER));
	fileHeader.bfType    = ('M' << 8) | 'B';
	fileHeader.bfSize    = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bufferLength;
	fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	std::ofstream output(saveFileName, std::ios::out | std::ios::binary);
	output.write((char*)&fileHeader, sizeof(BITMAPFILEHEADER));
	output.write((char*)&infoHeader, sizeof(BITMAPINFOHEADER));
	output.write((char*)buffer, bufferLength);
	output.close();
}

int _tmain(int argc, TCHAR** argv)
{
	const CAtlStringW filename = "C:\\Users\\Public\\Videos\\Sample Videos\\Wildlife.wmv";
	const CAtlString saveDir = "d:\\samples";

	CoInitialize(NULL);
	{
		CComPtr<IGraphBuilder> graph;
		graph.CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC);
		CComPtr<DexterLib::ISampleGrabber> grabber;
		grabber.CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC);
		grabber->SetBufferSamples(TRUE);
		grabber->SetOneShot(FALSE);

		CComQIPtr<IBaseFilter> filter(grabber);
		CComQIPtr<IMediaControl> control(graph);
		CComQIPtr<IMediaSeeking> seeeking(graph);
		DexterLib::_AMMediaType mediaType;
		mediaType.majortype = MEDIATYPE_Video;
		mediaType.subtype = MEDIASUBTYPE_RGB24;
		mediaType.formattype = FORMAT_VideoInfo;
		grabber->SetMediaType(&mediaType);
		graph->AddFilter(filter, L"Capture Filter");
		graph->RenderFile(filename, NULL);

		control->Run();

		LONGLONG duration = 0;
		LONGLONG currentPos = 0;
		seeeking->GetDuration(&duration);
		do {
			seeeking->GetCurrentPosition(&currentPos);
			if(currentPos % 10 == 0)
			{
				long size = 0;
				grabber->GetCurrentBuffer(&size, NULL);
				long *buf = new long[size];
				grabber->GetCurrentBuffer(&size, buf);
				grabber->GetConnectedMediaType(&mediaType);
				CAtlString name;
				name.Format(TEXT("%s\\test%d.bmp"), saveDir, currentPos);
				VIDEOINFOHEADER *vih = (VIDEOINFOHEADER*)mediaType.pbFormat;
				SaveImage(name, buf, size, vih->bmiHeader);
				delete[] buf;
			}
		} while(currentPos <= duration);
	}
	CoUninitialize();

	return 0;
}