import WinSDK

private let WINDOW_TITLE = "Swift で Win32 API を使ってみた"
private let WINDOW_CLASS_NAME = "SwiftAppSampleForWindowsDesktop"

class MainWindow {
    private var window = RgWindow()

    var handle: HWND? { window.handle }

    func create(_ hInstance: HINSTANCE?) {
        window.create(hInstance, WINDOW_CLASS_NAME, WINDOW_TITLE) { (hWnd, msg, wParam, lParam) -> LRESULT in
            switch msg {
            case UINT(WM_CREATE):
                onCreate(hWnd)
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

private func onCreate(_ hWnd: HWND?) {
    InitCommonControls();
    let tabClassName = RgString(WC_TABCONTROL)
    let hTab = CreateWindowExW(
        0,
        tabClassName.ptr,
        nil,
        UINT(WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE),
        10, 10, 500, 500,
        hWnd,
        nil,
        nil,
        nil
    )

    let tabText = RgString("タブ1")
    var tabItem = TCITEMW()
    tabItem.mask = UINT(TCIF_TEXT)
    tabItem.pszText = tabText.mutablePtr // TODO: 文字化け直す
    tabItem.cchTextMax = tabText.length
    rg_TabCtrl_InsertItem(hTab, 0, &tabItem)
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
    let hDC = GetDC(hWnd)
    let text = RgString("やっほー")
    TextOutW(hDC, 100, 100, text.ptr, text.length)
    ReleaseDC(hWnd, hDC)
}
