
#pragma once

#define UNICODE

#include <stdio.h>
#include <Windows.h>
#include <CommCtrl.h>
#include <Uxtheme.h>
// #include <jpeglib.h> // エラーになる
#include "../../Tools/vcpkg/installed/x64-windows-static/include/jpeglib.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#ifdef __cplusplus
extern "C" {
#endif

    // windows api
    int rg_LOWORD(LPARAM);
    int rg_HIWORD(LPARAM);
    int rg_TabCtrl_InsertItem(HWND, UINT, const TCITEMW*);
    int rg_TabCtrl_GetCurSel(HWND);
    int rg_ListView_InsertColumn(HWND, UINT, const LVCOLUMNW*);
    int rg_ListView_InsertItem(HWND, const LVITEMW*);

    // ligjpeg-turbo
    void rg_jpeg_create_decompress(j_decompress_ptr);

#ifdef __cplusplus
}
#endif
