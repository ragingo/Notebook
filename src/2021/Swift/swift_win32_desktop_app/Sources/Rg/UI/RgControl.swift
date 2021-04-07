import Foundation

class RgControl {
    private static var id: Int = 0
    private(set) var currentId: Int = 0

    var handle: HWND? = nil
    var parentHandle: HWND? = nil

    private var _text: String = ""
    var text: String {
        get { return _text }
        set (newValue) {
            if _text == newValue {
                return
            }
            let str = RgString(newValue)
            if SetWindowTextW(handle, str.ptr) {
                _text = newValue
            }
        }
    }

    private(set) var name: String
    private(set) var className: String

    required init(parentHandle: HWND? = nil, name: String = "", className: String) {
        Self.id += 1
        self.currentId = Self.id
        self.name = name
        self.className = className
        self.parentHandle = parentHandle

        if parentHandle == nil {
            handle = nil
        } else {
            handle = createControl(hWnd: parentHandle, name: name, className: className, hMenu: HMENU(bitPattern: Self.id))
            applySubClass()
        }
    }

    func setParent(_ parentHandle: HWND?) {
        self.parentHandle = parentHandle
        if handle == nil {
            let selfPtr = unsafeBitCast(self, to: UnsafeMutableRawPointer.self)
            handle = createControl(hWnd: parentHandle, name: name, className: className, hMenu: HMENU(bitPattern: Self.id))
            applySubClass()
        } else {
            SetParent(handle, parentHandle)
        }
    }

    func applySubClass() {
            print(handle)
        let windowProc: SUBCLASSPROC = { (hWnd, uMsg, wParam, lParam, uIdSubclass, dwRefData) -> LRESULT in
            var windowMessage = RgWindowMessage(uMsg: uMsg, wParam: wParam, lParam: lParam, handled: false)

            // let _selfPtr = unsafeBitCast(dwRefData, to: UnsafeMutableRawPointer.self)
            // print(_selfPtr)
            // let _self = unsafeBitCast(_selfPtr, to: RgControl.self)
            // print(_self, _selfPtr)

            // let _self = unsafeBitCast(dwRefData, to: RgControl.self)
            // print(_self)

            // _self.windowProc(hWnd, &windowMessage) // これを実行するとクラッシュ
            // print(_self.handle) // これでもクラッシュ。プロパティやメソッドをなにか参照するクラッシュ

            if windowMessage.handled {
                return 0
            }

            return DefSubclassProc(hWnd, uMsg, wParam, lParam)
        }

        // let selfPtr = unsafeBitCast(self, to: UnsafeMutableRawPointer.self)
        SetWindowSubclass(handle, windowProc, UINT_PTR(Self.id), unsafeBitCast(self, to: DWORD_PTR.self))
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
