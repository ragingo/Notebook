
#pragma once

#define UNICODE

#include <Windows.h>
#include <CommCtrl.h>

#ifdef __cplusplus
extern "C" {
#endif

    int rg_TabCtrl_InsertItem(HWND, UINT, const TCITEMW*);

#ifdef __cplusplus
}
#endif
