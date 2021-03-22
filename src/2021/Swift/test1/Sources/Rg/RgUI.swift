import WinSDK

func loadCommonControls() {
    var initCommCtrl = INITCOMMONCONTROLSEX()
    initCommCtrl.dwSize = UINT(MemoryLayout<INITCOMMONCONTROLSEX>.size)
    initCommCtrl.dwICC = DWORD(ICC_TAB_CLASSES)
    InitCommonControlsEx(&initCommCtrl)
}

func createTabControl(_ hWnd: HWND, _ hInstance: HINSTANCE, _ name: String) -> HWND? {
    let tabClassName = RgString(WC_TABCONTROL)
    let name = RgString(name)
    let hTab = CreateWindowExW(
        0,
        tabClassName.ptr,
        name.ptr,
        UINT(WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE),
        0, 0, 0, 0,
        hWnd,
        nil,
        hInstance,
        nil
    )
    return hTab
}

func addTabItem(_ hTab: HWND, _ index: UINT, _ text: String) {
    let tabText = RgString(text)
    var tabItem = TCITEMW()
    tabItem.mask = UINT(TCIF_TEXT)
    tabItem.pszText = tabText.mutablePtr
    tabItem.cchTextMax = tabText.length
    rg_TabCtrl_InsertItem(hTab, index, &tabItem)
}
