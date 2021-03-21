
#pragma once

#define UNICODE

#include <Windows.h>
#include <CommCtrl.h>

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#ifdef __cplusplus
extern "C" {
#endif

    int rg_LOWORD(LPARAM);
    int rg_HIWORD(LPARAM);

    int rg_TabCtrl_InsertItem(HWND, UINT, const TCITEMW*);
    int rg_TabCtrl_GetCurSel(HWND);

#ifdef __cplusplus
}
#endif
