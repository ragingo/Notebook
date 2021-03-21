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
            case UINT(WM_SIZE):
                onSize(hWnd, lParam)
            case UINT(WM_NOTIFY):
                onNotify(hWnd, lParam)
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

private func createTabControl(_ hWnd: HWND?, _ hInstance: HINSTANCE, _ name: String) -> HWND? {
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
    guard let hTab = createTabControl(hWnd, hInstance, "tab") else {
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

    var rect = RECT()
    let tabName = RgString("tab")

    if let hTab = FindWindowExW(hWnd, nil, nil, tabName.ptr) {
        GetClientRect(hTab, &rect)
        if rg_TabCtrl_GetCurSel(hTab) == 0 {
            Rectangle(hDC, 0, rect.bottom - rect.top, 30, rect.bottom - rect.top + 30)
        }
    }

    EndPaint(hWnd, &ps)
}

private func onSize(_ hWnd: HWND?, _ lParam: LPARAM) {
    let tabName = RgString("tab")
    if let hTab = FindWindowExW(hWnd, nil, nil, tabName.ptr) {
        MoveWindow(hTab, 0, 0, rg_LOWORD(lParam), 30, true)
    }
}

private func onNotify(_ hWnd: HWND?, _ lParam: LPARAM) {
    let msg = unsafeBitCast(lParam, to: LPNMHDR.self)
    if msg.pointee.code == TCN_SELCHANGE {
        InvalidateRect(hWnd, nil, true)
    }
}

private func onMouseLButtonDown(_ hWnd: HWND?) {
}
