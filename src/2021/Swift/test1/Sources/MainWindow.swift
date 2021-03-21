import WinSDK

private let WINDOW_TITLE = "Swift で Win32 API を使ってみた"
private let WINDOW_CLASS_NAME = "SwiftAppSampleForWindowsDesktop"

class MainWindow {
    private var window = RgWindow()

    var handle: HWND? { window.handle }

    func create(_ hInstance: HINSTANCE?) {
        window.create(hInstance, WINDOW_CLASS_NAME, WINDOW_TITLE) { (hWnd, msg, wParam, lParam) -> LRESULT in
            let lpCreateStruct = unsafeBitCast(lParam, to: LPCREATESTRUCT.self)
            switch msg {
            case UINT(WM_CREATE):
                onCreate(hWnd, lpCreateStruct.pointee.hInstance)
            case UINT(WM_DESTROY):
                onDestroy()
            case UINT(WM_PAINT):
                onPaint(hWnd)
            case UINT(WM_LBUTTONDOWN):
                onMouseLButtonDown(hWnd)
            default:
                return DefWindowProcW(hWnd, msg, wParam, lParam)
            }
            return 0
        }
    }
}

private func loadCommonControls() {
    var initCommCtrl = INITCOMMONCONTROLSEX()
    initCommCtrl.dwSize = UINT(MemoryLayout<INITCOMMONCONTROLSEX>.size)
    initCommCtrl.dwICC = DWORD(ICC_TAB_CLASSES)
    InitCommonControlsEx(&initCommCtrl)
}

private func createTabControl(_ hWnd: HWND?, _ hInstance: HINSTANCE) -> HWND? {
    let tabClassName = RgString(WC_TABCONTROL)
    let hTab = CreateWindowExW(
        0,
        tabClassName.ptr,
        nil,
        UINT(WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE),
        10, 10, 500, 500,
        hWnd,
        nil,
        hInstance,
        nil
    )
    return hTab
}

private func addTabItem(_ hTab: HWND, _ index: UINT, _ text: String) {
    let tabText = RgString(text)
    var tabItem = TCITEMW()
    tabItem.mask = UINT(TCIF_TEXT)
    tabItem.pszText = tabText.mutablePtr
    tabItem.cchTextMax = tabText.length
    rg_TabCtrl_InsertItem(hTab, index, &tabItem)
}

private func onCreate(_ hWnd: HWND?, _ hInstance: HINSTANCE) {
    loadCommonControls()
    guard let hTab = createTabControl(hWnd, hInstance) else {
        return
    }
    addTabItem(hTab, 0, "タブ1")
    addTabItem(hTab, 1, "タブ2")
    addTabItem(hTab, 2, "タブ3")
}

private func onDestroy() {
    PostQuitMessage(0)
}

private func onPaint(_ hWnd: HWND?) {
    var ps = PAINTSTRUCT()
    let hDC = BeginPaint(hWnd, &ps)
    let text = RgString("やっほー!!!")
    TextOutW(hDC, 0, 0, text.ptr, text.length)
    EndPaint(hWnd, &ps)
}

private func onMouseLButtonDown(_ hWnd: HWND?) {
}
