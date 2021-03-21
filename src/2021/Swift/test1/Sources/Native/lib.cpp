#include "lib.h"

#include <stdio.h>

extern "C" {

    int rg_TabCtrl_InsertItem(HWND hWnd, UINT iItem, const TCITEMW* pitem) {
        return TabCtrl_InsertItem(hWnd, iItem, pitem);
    }

}
