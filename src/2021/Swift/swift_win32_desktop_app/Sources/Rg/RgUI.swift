import WinSDK

func loadCommonControls() {
    var initCommCtrl = INITCOMMONCONTROLSEX()
    initCommCtrl.dwSize = UINT(MemoryLayout<INITCOMMONCONTROLSEX>.size)
    initCommCtrl.dwICC = DWORD(ICC_TAB_CLASSES | ICC_LISTVIEW_CLASSES)
    InitCommonControlsEx(&initCommCtrl)
}

func createControl(hWnd: HWND, hInstance: HINSTANCE, name: String, className: String, styles: UINT) -> HWND? {
    let className = RgString(className)
    let name = RgString(name)
    let handle = CreateWindowExW(
        0,
        className.ptr,
        name.ptr,
        styles,
        0, 0, 0, 0,
        hWnd,
        nil,
        hInstance,
        nil
    )
    return handle
}

func createTabControl(_ hWnd: HWND, _ hInstance: HINSTANCE, _ name: String) -> HWND? {
    return createControl(
        hWnd: hWnd,
        hInstance: hInstance,
        name: name,
        className: WC_TABCONTROL,
        styles: UINT(WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE)
    )
}

func addTabItem(_ hTab: HWND, _ index: UINT, _ text: String) {
    let tabText = RgString(text)
    var tabItem = TCITEMW()
    tabItem.mask = UINT(TCIF_TEXT)
    tabItem.pszText = tabText.mutablePtr
    tabItem.cchTextMax = tabText.length
    rg_TabCtrl_InsertItem(hTab, index, &tabItem)
}

func createListView(_ hWnd: HWND, _ hInstance: HINSTANCE, _ name: String) -> HWND? {
    let hListView = createControl(
        hWnd: hWnd,
        hInstance: hInstance,
        name: name,
        className: WC_LISTVIEW,
        styles: UINT(WS_CHILD | WS_VISIBLE | LVS_REPORT)
    )
    let appName = RgString("Explorer")
    SetWindowTheme(hListView, appName.ptr, nil)
    return hListView
}

func addListViewColumn(_ hListView: HWND, _ index: UINT, _ text: String) {
    let colText = RgString(text)
    var col = LVCOLUMNW()
    col.mask = UINT(LVCF_FMT | LVCF_TEXT | LVCF_WIDTH)
    col.fmt = Int32(LVCFMT_LEFT)
    col.cx = 100
    col.pszText = colText.mutablePtr
    rg_ListView_InsertColumn(hListView, index, &col)
}

func addListViewItem(_ hListView: HWND, _ index: Int32, _ text: String) {
    let itemText = RgString(text)
    var item = LVITEMW()
    item.mask = UINT(LVIF_TEXT)
    item.pszText = itemText.mutablePtr
    item.iItem = index
    rg_ListView_InsertItem(hListView, &item)
}