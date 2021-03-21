
#pragma once

#define UNICODE

#include <Windows.h>
#include <CommCtrl.h>

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
