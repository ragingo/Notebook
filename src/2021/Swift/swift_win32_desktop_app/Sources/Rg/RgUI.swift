import WinSDK

func loadCommonControls() {
    var initCommCtrl = INITCOMMONCONTROLSEX()
    initCommCtrl.dwSize = UINT(MemoryLayout<INITCOMMONCONTROLSEX>.size)
    initCommCtrl.dwICC = DWORD(ICC_TAB_CLASSES | ICC_LISTVIEW_CLASSES)
    InitCommonControlsEx(&initCommCtrl)
}

func createControl(hWnd: HWND?, name: String, className: String, styles: UINT = UINT(WS_CHILD | WS_VISIBLE), x: Int32 = 0, y: Int32 = 0, w: Int32 = 0, h: Int32 = 0, hInstance: HINSTANCE? = nil, lpParam: LPVOID? = nil) -> HWND? {
    let className = RgString(className)
    let name = RgString(name)
    let handle = CreateWindowExW(
        0,
        className.ptr,
        name.ptr,
        styles,
        x, y, w, h,
        hWnd,
        nil,
        hInstance,
        lpParam
    )
    return handle
}

func createStaticControl(_ hWnd: HWND?, _ hInstance: HINSTANCE? = nil, _ lpParam: LPVOID? = nil) -> HWND? {
    return createControl(hWnd: hWnd, name: "", className: WC_STATIC, hInstance: hInstance, lpParam: lpParam)
}

func createButton(_ hWnd: HWND?, _ text: String, _ hInstance: HINSTANCE? = nil) -> HWND? {
    return createControl(hWnd: hWnd, name: text, className: WC_BUTTON, hInstance: hInstance)
}

func createTabControl(_ hWnd: HWND?, _ name: String, _ hInstance: HINSTANCE? = nil, _ lpParam: LPVOID? = nil) -> HWND? {
    return createControl(hWnd: hWnd, name: name, className: WC_TABCONTROL, hInstance: hInstance, lpParam: lpParam)
}

func createListView(_ hWnd: HWND?, _ name: String, _ hInstance: HINSTANCE? = nil, _ lpParam: LPVOID? = nil) -> HWND? {
    return createControl(hWnd: hWnd, name: name, className: WC_LISTVIEW, hInstance: hInstance, lpParam: lpParam)
}

func addTabItem(_ hTab: HWND?, _ index: UINT, _ text: String) {
    let tabText = RgString(text)
    var tabItem = TCITEMW()
    tabItem.mask = UINT(TCIF_TEXT)
    tabItem.pszText = tabText.mutablePtr
    tabItem.cchTextMax = tabText.length
    rg_TabCtrl_InsertItem(hTab, index, &tabItem)
}

func addListViewColumn(_ hListView: HWND?, _ index: UINT, _ text: String) {
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