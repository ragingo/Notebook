#include "lib.h"

#include <stdio.h>

extern "C" {

    int rg_TabCtrl_InsertItem(HWND hWnd, UINT iItem, const TCITEMW* pitem) {
        return (int)SendMessageW(hWnd, TCM_INSERTITEM, (WPARAM)(int)(iItem), (LPARAM)pitem);
    }

}
