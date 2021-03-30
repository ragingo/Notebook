import Foundation

func getInstance<T: RgControl>(_ type: T.Type, _ hWnd: HWND?) -> T? {
    guard let hWnd = hWnd else {
        return nil
    }
    let ptr = GetWindowLongPtrW(hWnd, GWL_USERDATA)
    if ptr == 0 {
        return nil
    }
    return unsafeBitCast(ptr, to: T.self)
}

class RgControl {
    private var hInstance: HINSTANCE?
    var handle: HWND? = nil
    var parentHandle: HWND? = nil
    private var defaultWndProc: WNDPROC? = nil

    required init(hWndParent: HWND?, name: String = "", hInstance: HINSTANCE? = nil, enableSubClass: Bool = false) {
        self.hInstance = hInstance
        parentHandle = hWndParent

        let selfPtr = unsafeBitCast(self, to: UnsafeMutableRawPointer.self)
        handle = createStaticControl(hWndParent, hInstance, selfPtr)

        if enableSubClass {
            let ptr = unsafeBitCast(selfPtr, to: LONG_PTR.self)
            SetWindowLongPtrW(handle, GWL_USERDATA, ptr)
            applySubClass()
        }
    }

    func applySubClass() {
        let ptr = GetWindowLongPtrW(handle, GWL_WNDPROC)
        defaultWndProc = unsafeBitCast(ptr, to: WNDPROC.self)

        let windowProc: WNDPROC = { (hWnd, uMsg, wParam, lParam) -> LRESULT in
            var windowMessage = RgWindowMessage(uMsg: uMsg, wParam: wParam, lParam: lParam)
            if let instance = getInstance(RgControl.self, hWnd) {
                instance.windowProc(hWnd, &windowMessage)
                return CallWindowProcW(instance.defaultWndProc, hWnd, uMsg, wParam, lParam)
            }
            return 0
        }
        SetWindowLongPtrW(handle, GWL_WNDPROC, unsafeBitCast(windowProc, to: LONG_PTR.self))
    }

    func windowProc(_ hWnd: HWND?, _ windowMessage: inout RgWindowMessage) {
        switch windowMessage.uMsg {
        case UINT(WM_PAINT):
            onPaint(&windowMessage)
        case UINT(WM_SIZE):
            onSize(&windowMessage)
        case UINT(WM_NOTIFY):
            onNotify(&windowMessage)
        default:
            break
        }
    }

    func onCreate(_ hWnd: HWND?) {
    }

    func onPaint(_ windowMessage: inout RgWindowMessage) {
    }

    func onSize(_ windowMessage: inout RgWindowMessage) {
        var rect = RECT()
        GetClientRect(parentHandle, &rect)
        adjustSize(&rect)
    }

    func onNotify(_ windowMessage: inout RgWindowMessage) {
    }

    func addChild<T: RgControl>(child: T) -> T {
        // SetParent(child.handle, nil)
        SetParent(child.handle, handle)
        return child
    }

    func addChild<T: RgControl>(type _: T.Type) -> T {
        let child = T(hWndParent: handle, name: String(describing: T.self), hInstance: hInstance)
        return addChild(child: child)
    }

    func adjustSize(_ rect: inout RECT) {
        SetWindowPos(handle, nil, 0, 0, rect.right - rect.left, rect.bottom - rect.top, 0)
    }

    func resize(width: Int32, height: Int32) {
        SetWindowPos(handle, nil, 0, 0, width, height, UINT(SWP_NOMOVE))
    }

    func position(x: Int32, y: Int32) {
        SetWindowPos(handle, nil, x, y, 0, 0, UINT(SWP_NOSIZE))
    }

    func show() {
        ShowWindow(handle, SW_SHOW)
        UpdateWindow(handle)
    }

    func hide() {
        ShowWindow(handle, SW_HIDE)
        UpdateWindow(handle)
    }
}
