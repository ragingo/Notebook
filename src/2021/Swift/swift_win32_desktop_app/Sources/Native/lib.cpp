#include "lib.h"

extern "C" {

    int rg_LOWORD(LPARAM lParam) {
        return LOWORD(lParam);
    }

    int rg_HIWORD(LPARAM lParam) {
        return HIWORD(lParam);
    }

    int rg_TabCtrl_InsertItem(HWND hWnd, UINT iItem, const TCITEMW* pitem) {
        return TabCtrl_InsertItem(hWnd, iItem, pitem);
    }

    int rg_TabCtrl_GetCurSel(HWND hWnd) {
        return TabCtrl_GetCurSel(hWnd);
    }

}

extern "C" {

    void rg_jpeg_create_decompress(j_decompress_ptr cinfo) {
        jpeg_create_decompress(cinfo);
    }

}
