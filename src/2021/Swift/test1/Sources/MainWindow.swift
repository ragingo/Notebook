import WinSDK

private let WINDOW_TITLE = "Swift で Win32 API を使ってみた"
private let WINDOW_CLASS_NAME = "SwiftAppSampleForWindowsDesktop"

private let TAB_BUTTON_HIGHT: Int32 = 30

let GWL_USERDATA: Int32 = -21

final class MainWindow: RgWindow {
    func create(_ hInstance: HINSTANCE?) {
        self.create(hInstance, WINDOW_CLASS_NAME, WINDOW_TITLE)
    }

    private var hBmp: HBITMAP?

    override func onCreate(_ hWnd: HWND?, _ hInstance: HINSTANCE) {
        super.onCreate(hWnd, hInstance)

        loadCommonControls()

        guard let hWnd = hWnd, let hTab = createTabControl(hWnd, hInstance, "tab") else {
            return
        }

        addTabItem(hTab, 0, "タブ1")
        addTabItem(hTab, 1, "タブ2")
        addTabItem(hTab, 2, "タブ3")

        let path = RgString("./Resources/catman.bmp")
        let ptr = LoadImageW(nil, path.ptr, UINT(IMAGE_BITMAP), 0, 0, UINT(LR_LOADFROMFILE));
        self.hBmp = unsafeBitCast(ptr, to: HBITMAP.self)
    }

    override func onPaint(_ windowMessage: inout RgWindowMessage) {
        var ps = PAINTSTRUCT()
        guard let hDC = BeginPaint(hWnd, &ps) else {
            return
        }

        var rect = RECT()
        let tabName = RgString("tab")

        if let hTab = FindWindowExW(hWnd, nil, nil, tabName.ptr) {
            GetClientRect(hTab, &rect)
            switch rg_TabCtrl_GetCurSel(hTab) {
            case 0:
                onTabItem1Paint(hTab, hDC, rect)
            case 1:
                onTabItem2Paint(hTab, hDC, rect)
            case 2:
                onTabItem3Paint(hTab, hDC, rect)
            default:
                break
            }
        }

        EndPaint(hWnd, &ps)
    }

    private func onTabItem1Paint(_ hTab: HWND, _ hDC: HDC, _ rect: RECT) {
        Rectangle(hDC, 10, rect.bottom - rect.top, 30, rect.bottom - rect.top + 30)

        if let hBmp = hBmp {
            let hMemDC = CreateCompatibleDC(hDC)
            SelectObject(hMemDC, hBmp)
            BitBlt(hDC, 0, TAB_BUTTON_HIGHT, 32, 32, hMemDC, 0, 0, SRCCOPY)
            DeleteDC(hMemDC)
        }
    }

    private func onTabItem2Paint(_ hTab: HWND, _ hDC: HDC, _ rect: RECT) {
        Rectangle(hDC, 10, rect.bottom - rect.top, 50, rect.bottom - rect.top + 50)
    }

    private func onTabItem3Paint(_ hTab: HWND, _ hDC: HDC, _ rect: RECT) {
        Rectangle(hDC, 10, rect.bottom - rect.top, 70, rect.bottom - rect.top + 70)
    }

    override func onSize(_ windowMessage: inout RgWindowMessage) {
        let tabName = RgString("tab")
        if let hTab = FindWindowExW(hWnd, nil, nil, tabName.ptr) {
            MoveWindow(hTab, 0, 0, rg_LOWORD(windowMessage.lParam), TAB_BUTTON_HIGHT, true)
        }
    }

    override func onNotify(_ windowMessage: inout RgWindowMessage) {
        let msg = unsafeBitCast(windowMessage.lParam, to: LPNMHDR.self)
        if msg.pointee.code == TCN_SELCHANGE {
            InvalidateRect(hWnd, nil, true)
        }
    }

    override func onDestroy() {
        if let hBmp = hBmp {
            DeleteObject(hBmp)
        }
        super.onDestroy()
    }
}
