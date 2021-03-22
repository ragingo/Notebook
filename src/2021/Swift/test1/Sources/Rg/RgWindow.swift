import WinSDK

func getInstance<T: RgWindow>(_ type: T.Type, _ hWnd: HWND?) -> T? {
    guard let hWnd = hWnd else {
        return nil
    }
    let ptr = GetWindowLongPtrW(hWnd, GWL_USERDATA)
    if ptr == 0 {
        return nil
    }
    return unsafeBitCast(ptr, to: T.self)
}

struct RgWindowMessage {
    let uMsg: UINT
    let wParam: WPARAM
    let lParam: LPARAM
}

class RgWindow {
    private var hInstance: HINSTANCE?
    private(set) var hWnd: HWND?

    init() {
    }

    func create(_ hInstance: HINSTANCE?, _ windowClass: String, _ windowTitle: String) {
        self.hInstance = hInstance
        let windowClass = RgString(windowClass)

        let windowProc: WNDPROC = { (hWnd, uMsg, wParam, lParam) -> LRESULT in
            let lpCreateStruct = unsafeBitCast(lParam, to: LPCREATESTRUCT.self)
            var windowMessage = RgWindowMessage(uMsg: uMsg, wParam: wParam, lParam: lParam)

            if let instance = getInstance(RgWindow.self, hWnd) {
                instance.windowProc(hWnd, &windowMessage)
            }

            switch uMsg {
            case UINT(WM_CREATE):
                let params = lpCreateStruct.pointee.lpCreateParams
                let ptr = unsafeBitCast(params, to: LONG_PTR.self)
                SetWindowLongPtrW(hWnd, GWL_USERDATA, ptr)
                getInstance(RgWindow.self, hWnd)?.onCreate(hWnd, lpCreateStruct.pointee.hInstance)
            case UINT(WM_DESTROY):
                getInstance(RgWindow.self, hWnd)?.onDestroy()
            case UINT(WM_PAINT):
                getInstance(RgWindow.self, hWnd)?.onPaint(&windowMessage)
            case UINT(WM_SIZE):
                getInstance(RgWindow.self, hWnd)?.onSize(&windowMessage)
            case UINT(WM_NOTIFY):
                getInstance(RgWindow.self, hWnd)?.onNotify(&windowMessage)
            case UINT(WM_LBUTTONDOWN):
                getInstance(RgWindow.self, hWnd)?.onMouseLButtonDown(&windowMessage)
            default:
                return DefWindowProcW(hWnd, uMsg, wParam, lParam)
            }
            return 0
        }

        guard registerClass(windowClass, windowProc) else {
            return
        }

        let windowTitle = RgString(windowTitle)
        let selfPtr = unsafeBitCast(self, to: UnsafeMutableRawPointer.self)

        let hWnd = CreateWindowExW(
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
        ShowWindow(hWnd, SW_SHOW);
        UpdateWindow(hWnd)
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

    func windowProc(_ hWnd: HWND?, _ windowMessage: inout RgWindowMessage) {
    }

    func onCreate(_ hWnd: HWND?, _ hInstance: HINSTANCE) {
        self.hWnd = hWnd
    }

    func onPaint(_ windowMessage: inout RgWindowMessage) {
    }

    func onSize(_ windowMessage: inout RgWindowMessage) {
    }

    func onNotify(_ windowMessage: inout RgWindowMessage) {
    }

    func onMouseLButtonDown(_ windowMessage: inout RgWindowMessage) {
    }

    func onDestroy() {
        PostQuitMessage(0)
    }
}
