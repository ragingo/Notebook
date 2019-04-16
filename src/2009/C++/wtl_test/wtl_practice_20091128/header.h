
#pragma once
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

#pragma warning(push)
#pragma warning(disable:4996)
#include <atlbase.h>
#include <atlstr.h>
#include <atlapp.h>
#include <atlwin.h>
#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlcrack.h>
#include <atltheme.h>

#include <tchar.h>
#include <windows.h>

#pragma comment(lib, "dwmapi.lib")
#include <dwmapi.h>
#pragma comment(lib, "uxtheme.lib")
#include <uxtheme.h>

extern CAppModule _Module;

#include "resource.h"

//#define WINVER         0x0600
//#define _WIN32_WINNT   0x0600
//#define _WIN32_IE      0x0700
//
//#define _WTL_NO_WTYPES
// 
//#include <atlbase.h>
//#include <atltypes.h>
//#include <atlapp.h>
//extern CAppModule _Module;
//#include <atlwin.h>
//#include <atlframe.h>
//#include <atlmisc.h>
//#include <atlcrack.h>
//#include <atltheme.h>
//#include <dwmapi.h>



#pragma warning(pop)
