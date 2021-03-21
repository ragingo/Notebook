import WinSDK

private let WINDOW_TITLE = "Swift で Win32 API を使ってみた"
private let WINDOW_CLASS_NAME = "SwiftAppSampleForWindowsDesktop"

private let TAB_BUTTON_HIGHT: Int32 = 30

let GWL_USERDATA: Int32 = -21

private func getInstance<T: RgWindow>(_ type: T.Type, _ hWnd: HWND?) -> T {
    let ptr = GetWindowLongPtrW(hWnd, GWL_USERDATA)
    return unsafeBitCast(ptr, to: T.self)
}

class MainWindow: RgWindow {
    func create(_ hInstance: HINSTANCE?) {
        self.create(hInstance, WINDOW_CLASS_NAME, WINDOW_TITLE) { (hWnd, msg, wParam, lParam) -> LRESULT in
            let lpCreateStruct = unsafeBitCast(lParam, to: LPCREATESTRUCT.self)

            switch msg {
            case UINT(WM_CREATE):
                let params = lpCreateStruct.pointee.lpCreateParams
                let ptr = unsafeBitCast(params, to: LONG_PTR.self)
                SetWindowLongPtrW(hWnd, GWL_USERDATA, ptr)
                getInstance(MainWindow.self, hWnd).onCreate(hWnd, lpCreateStruct.pointee.hInstance)
            case UINT(WM_DESTROY):
                getInstance(MainWindow.self, hWnd).onDestroy()
            case UINT(WM_PAINT):
                getInstance(MainWindow.self, hWnd).onPaint(hWnd)
            case UINT(WM_SIZE):
                getInstance(MainWindow.self, hWnd).onSize(hWnd, lParam)
            case UINT(WM_NOTIFY):
                getInstance(MainWindow.self, hWnd).onNotify(hWnd, lParam)
            case UINT(WM_LBUTTONDOWN):
                getInstance(MainWindow.self, hWnd).onMouseLButtonDown(hWnd)
            default:
                return DefWindowProcW(hWnd, msg, wParam, lParam)
            }
            return 0
        }
    }

    private var hBmp: HBITMAP?

    private func onCreate(_ hWnd: HWND?, _ hInstance: HINSTANCE) {
        loadCommonControls()
        guard let hTab = createTabControl(hWnd, hInstance, "tab") else {
            return
        }
        addTabItem(hTab, 0, "タブ1")
        addTabItem(hTab, 1, "タブ2")
        addTabItem(hTab, 2, "タブ3")

        let path = RgString("./Resources/catman.bmp")
        let ptr = LoadImageW(nil, path.ptr, UINT(IMAGE_BITMAP), 0, 0, UINT(LR_LOADFROMFILE));
        self.hBmp = unsafeBitCast(ptr, to: HBITMAP.self)
    }

    private func onPaint(_ hWnd: HWND?) {
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

    private func onSize(_ hWnd: HWND?, _ lParam: LPARAM) {
        let tabName = RgString("tab")
        if let hTab = FindWindowExW(hWnd, nil, nil, tabName.ptr) {
            MoveWindow(hTab, 0, 0, rg_LOWORD(lParam), TAB_BUTTON_HIGHT, true)
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

    private func onDestroy() {
        if let hBmp = hBmp {
            DeleteObject(hBmp)
        }
        PostQuitMessage(0)
    }
}
