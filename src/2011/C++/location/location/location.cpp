
#include "stdafx.h"

namespace std
{
	basic_ostream<TCHAR>& tcout =
#ifdef UNICODE
wcout;
#else
cout;
#endif
}

using namespace std;
using namespace ATL;

CAtlString ConvertGuidToString(const GUID& guid)
{
	CAtlString str;
	str.Format(_T("{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}"),
		guid.Data1,
		guid.Data2,
		guid.Data3,
		guid.Data4[0], guid.Data4[1],
		guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]
	);
	return str;
}

// WMIを使用して、認識しているセンサーを列挙
// PS> Get-WmiObject Win32_PNPEntity | ?{ $_.HardwareID -match "Sensors\\.*" }
int _tmain()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_tsetlocale(LC_ALL, _T(""));

	CoInitialize(NULL);
	{
		CComPtr<ILocation> pLocation;
		pLocation.CoCreateInstance(CLSID_Location);
		IID REPORT_TYPES[] = {
			IID_ILatLongReport,
			IID_ICivicAddressReport
		};
		// ↓ダイアログを見た事が無い・・・
		pLocation->RequestPermissions(NULL, REPORT_TYPES, ARRAYSIZE(REPORT_TYPES), TRUE);
		LOCATION_REPORT_STATUS status;
		pLocation->GetReportStatus(IID_ILatLongReport, &status);
		switch(status)
		{
		case REPORT_NOT_SUPPORTED:
			// デバイスマネージャーで位置センサーを無効にするとここに来る。
			return EXIT_FAILURE;
		}

		// 要求精度
		LOCATION_DESIRED_ACCURACY accuracy;
		pLocation->GetDesiredAccuracy(IID_ILatLongReport, &accuracy);
		tcout << _T("要求精度    : ");
		switch(accuracy)
		{
		case LOCATION_DESIRED_ACCURACY_DEFAULT:
			tcout << _T("DEFAULT") << endl;
			break;
		case LOCATION_DESIRED_ACCURACY_HIGH:
			tcout << _T("HIGH") << endl;
			break;
		}

		// 緯度経度取得
		CComPtr<ILocationReport> pLocationReport1;
		pLocation->GetReport(IID_ILatLongReport, &pLocationReport1);
		CComQIPtr<ILatLongReport> pLatLongReport(pLocationReport1);
		double latitude = 0.0;
		double longitude = 0.0;
		pLatLongReport->GetLatitude(&latitude);
		pLatLongReport->GetLongitude(&longitude);
		tcout << _T("緯度,経度   : ") << latitude << _T(", ") << longitude << endl;

		// 住所取得
		CComPtr<ILocationReport> pLocationReport2;
		pLocation->GetReport(IID_ICivicAddressReport, &pLocationReport2);
		CComQIPtr<ICivicAddressReport> pCivicAddressReport(pLocationReport2);

		typedef HRESULT (STDMETHODCALLTYPE ICivicAddressReport::*CivicAddressReportFunc)(BSTR*);
		auto BSTR2TSTR = [&](CivicAddressReportFunc func)->CAtlString {
			CComBSTR str;
			(pCivicAddressReport->*func)(&str);
			return
				str.Length() > 0 ?
				CAtlString(str) : _T("");
		};
		tcout << _T("郵便番号    : ") << BSTR2TSTR(&ICivicAddressReport::GetPostalCode).GetBuffer() << endl;
		tcout << _T("都道府県    : ") << BSTR2TSTR(&ICivicAddressReport::GetStateProvince).GetBuffer() << endl;
		tcout << _T("市区町村    : ") << BSTR2TSTR(&ICivicAddressReport::GetCity).GetBuffer() << endl;
		tcout << _T("住所1       : ") << BSTR2TSTR(&ICivicAddressReport::GetAddressLine1).GetBuffer() << endl;
		tcout << _T("住所2       : ") << BSTR2TSTR(&ICivicAddressReport::GetAddressLine2).GetBuffer() << endl;

		// センサーID
		SENSOR_ID id = GUID_NULL;
		pLatLongReport->GetSensorID(&id);
		tcout << _T("センサーID  : ") << ConvertGuidToString(id).GetBuffer() << endl;
	}
	CoUninitialize();
	return EXIT_SUCCESS;
}
