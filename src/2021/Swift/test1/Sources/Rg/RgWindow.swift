import WinSDK

func getInstance<T: RgWindow>(_ type: T.Type, _ hWnd: HWND?) -> T {
    let ptr = GetWindowLongPtrW(hWnd, GWL_USERDATA)
    return unsafeBitCast(ptr, to: T.self)
}

class RgWindow {
    private var hInstance: HINSTANCE?
    var handle: HWND?

    init() {
    }

    func create(_ hInstance: HINSTANCE?, _ windowClass: String, _ windowTitle: String) {
        self.hInstance = hInstance
        let windowClass = RgString(windowClass)

        let windowProc: WNDPROC = { (hWnd, msg, wParam, lParam) -> LRESULT in
            let lpCreateStruct = unsafeBitCast(lParam, to: LPCREATESTRUCT.self)

            switch msg {
            case UINT(WM_CREATE):
                let params = lpCreateStruct.pointee.lpCreateParams
                let ptr = unsafeBitCast(params, to: LONG_PTR.self)
                SetWindowLongPtrW(hWnd, GWL_USERDATA, ptr)
                getInstance(RgWindow.self, hWnd).onCreate(hWnd, lpCreateStruct.pointee.hInstance)
            case UINT(WM_DESTROY):
                getInstance(RgWindow.self, hWnd).onDestroy()
            case UINT(WM_PAINT):
                getInstance(RgWindow.self, hWnd).onPaint(hWnd)
            case UINT(WM_SIZE):
                getInstance(RgWindow.self, hWnd).onSize(hWnd, lParam)
            case UINT(WM_NOTIFY):
                getInstance(RgWindow.self, hWnd).onNotify(hWnd, lParam)
            case UINT(WM_LBUTTONDOWN):
                getInstance(RgWindow.self, hWnd).onMouseLButtonDown(hWnd)
            default:
                return DefWindowProcW(hWnd, msg, wParam, lParam)
            }
            return 0
        }

        guard registerClass(windowClass, windowProc) else {
            return
        }

        let windowTitle = RgString(windowTitle)
        let selfPtr = unsafeBitCast(self, to: UnsafeMutableRawPointer.self)

        handle = CreateWindowExW(
            0,
            windowClass.ptr,
            windowTitle.ptr,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            nil,
            nil,
            hInstance,
            selfPtr
        )
    }

    private func registerClass(_ windowClass: RgString, _ windowProc: @escaping WNDPROC) -> Bool {
        var wcex = WNDCLASSEXW()
        wcex.cbSize = UINT(MemoryLayout<WNDCLASSEX>.size);
        wcex.style = UINT(CS_HREDRAW | CS_VREDRAW);
        wcex.lpfnWndProc = windowProc
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.lpszClassName = windowClass.ptr;
        wcex.hbrBackground = unsafeBitCast(GetStockObject(WHITE_BRUSH), to: HBRUSH.self)
        return RegisterClassExW(&wcex) != 0
    }

    // func windowProc(_ hWnd: HWND, _ msg: UINT, _ wParam: WPARAM, _ lParam: LPARAM) {
    // }

    func onCreate(_ hWnd: HWND?, _ hInstance: HINSTANCE) {
    }

    func onPaint(_ hWnd: HWND?) {
    }

    func onSize(_ hWnd: HWND?, _ lParam: LPARAM) {
    }

    func onNotify(_ hWnd: HWND?, _ lParam: LPARAM) {
    }

    func onMouseLButtonDown(_ hWnd: HWND?) {
    }

    func onDestroy() {
        PostQuitMessage(0)
    }
}
