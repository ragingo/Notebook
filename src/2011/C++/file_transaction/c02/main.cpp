
#include <iostream>
#include <conio.h>
#include <atlbase.h>
#include <atlcom.h>
#include <locationapi.h>

namespace std
{
	std::basic_ostream<TCHAR>& tcout = 
#ifdef UNICODE
wcout;
#else
cout;
#endif

#ifdef UNICODE
typedef wstring tstring;
#else
typedef string tstring;
#endif
}

using namespace std;
using namespace ATL;

int _tmain()
{
	CComPtr<ILocation> pLocation;
	pLocation.CoCreateInstance(CLSID_Location);

	LOCATION_DESIRED_ACCURACY desiredAccuracy;
	pLocation->GetDesiredAccuracy(IID_ILatLongReport, &desiredAccuracy);

	tcout << _T("LOCATION_DESIRED_ACCURACY : ");
	switch(desiredAccuracy)
	{
	case LOCATION_DESIRED_ACCURACY_DEFAULT:
		tcout << _T("DEFAULT") << endl;
		break;
	case LOCATION_DESIRED_ACCURACY_HIGH:
		tcout << _T("HIGH") << endl;
		break;
	}
}
