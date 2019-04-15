
#pragma once
#include <iostream>
#include <atlbase.h>
#include <atlstr.h>
#include <atlcoll.h>

#pragma comment(lib, "strmiids.lib")
#include <dshow.h>

// デバイスクラスを指定し、モニカを列挙
static HRESULT MonikerEnumerator(REFCLSID clsidDeviceClass, CAtlArray<CComPtr<IMoniker>>& pMonikers)
{
	HRESULT hr;
	CComPtr<ICreateDevEnum> pCreateDevEnum;
	CComPtr<IEnumMoniker>   pEnumMoniker;
	CComPtr<IMoniker>       pMoniker;

	hr = pCreateDevEnum.CoCreateInstance(CLSID_SystemDeviceEnum);
	if(FAILED(hr))
	{
		return hr;
	}

	hr = pCreateDevEnum->CreateClassEnumerator(clsidDeviceClass, &pEnumMoniker, 0);
	if(FAILED(hr))
	{
		return hr;
	}

	while(pEnumMoniker->Next(1, &pMoniker, NULL) == S_OK)
	{
		pMonikers.Add(pMoniker);
		pMoniker.Release();
	}
	return S_OK;
}

// モニカとプロパティ名を指定し、プロパティ値を取得
static HRESULT ReadPropertyBag(const CComPtr<IMoniker>& pMoniker, LPCOLESTR pszPropName, CComVariant& var)
{
	HRESULT hr;
	CComPtr<IPropertyBag> pPropertyBag;

	hr = pMoniker->BindToStorage(NULL, NULL, IID_IPropertyBag, reinterpret_cast<void**>(&pPropertyBag));
	if(FAILED(hr))
	{
		return hr;
	}

	hr = pPropertyBag->Read(pszPropName, &var, NULL);
	if(FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}

static void ShowVideoCompressors()
{
	CoInitialize(NULL);
	{
		CAtlArray<CComPtr<IMoniker>> monikers;
		MonikerEnumerator(CLSID_VideoCompressorCategory, monikers);

		// 圧縮フィルタ一覧
		for(size_t i=0;i < monikers.GetCount(); i++)
		{
			CComVariant v;
			if(ReadPropertyBag(monikers[i], L"FriendlyName", v) == S_OK)
			{
				CString str(v.bstrVal);
				std::wcout << i+1 << " : " << str.GetBuffer() << std::endl;
			}
		}
	}
	CoUninitialize();
}