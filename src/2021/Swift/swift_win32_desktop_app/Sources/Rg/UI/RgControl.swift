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
    var handle: HWND? = nil
    var parentHandle: HWND? = nil

    private(set) var name: String
    private(set) var className: String

    required init(parentHandle: HWND? = nil, name: String = "", className: String) {
        self.name = name
        self.className = className
        self.parentHandle = parentHandle

        let selfPtr = unsafeBitCast(self, to: UnsafeMutableRawPointer.self)

        if parentHandle == nil {
            handle = nil
        } else {
            handle = createControl(hWnd: parentHandle, name: name, className: className, hInstance: GetModuleHandleW(nil), lpParam: selfPtr)
            applySubClass()
        }
    }

    func setParent(_ parentHandle: HWND?) {
        self.parentHandle = parentHandle
        if handle == nil {
            let selfPtr = unsafeBitCast(self, to: UnsafeMutableRawPointer.self)
            handle = createControl(hWnd: parentHandle, name: name, className: className, hInstance: GetModuleHandleW(nil), lpParam: selfPtr)
            applySubClass()
        } else {
            SetParent(handle, parentHandle)
        }
    }

    func applySubClass() {
        let selfPtr = unsafeBitCast(self, to: UnsafeMutableRawPointer.self)
        SetWindowLongPtrW(handle, GWL_USERDATA, unsafeBitCast(selfPtr, to: LONG_PTR.self))

        let windowProc: SUBCLASSPROC = { (hWnd, uMsg, wParam, lParam, uIdSubclass, dwRefData) -> LRESULT in
            var windowMessage = RgWindowMessage(uMsg: uMsg, wParam: wParam, lParam: lParam, handled: false)
            if let instance = getInstance(RgControl.self, hWnd) {
                instance.windowProc(hWnd, &windowMessage)
                if windowMessage.handled {
                    return 0
                }
            }
            return DefSubclassProc(hWnd, uMsg, wParam, lParam)
        }
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
        SetParent(child.handle, handle)
        return child
    }

    func adjustSize(_ rect: inout RECT) {
        SetWindowPos(handle, nil, 0, 0, rect.right - rect.left, rect.bottom - rect.top, 0)
    }

    func resize(width: Int32, height: Int32) {
        SetWindowPos(handle, nil, 0, 0, width, height, UINT(SWP_NOMOVE))
    }

    func moveTo(x: Int32, y: Int32) {
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
