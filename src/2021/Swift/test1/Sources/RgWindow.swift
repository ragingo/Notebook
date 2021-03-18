import WinSDK

class RgWindow {
    private var hInstance: HINSTANCE?
    var handle: HWND?

    init() {
    }

    func create(_ hInstance: HINSTANCE?, windowClass: String, windowTitle: String) {
        self.hInstance = hInstance
        let windowClass = RgString(windowClass)

        guard registerClass(windowClass) else {
            return
        }

        let windowTitle = RgString(windowTitle)

        handle = CreateWindowExW(
            0,
            windowClass.ptr,
            windowTitle.ptr,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            nil,
            nil,
            hInstance,
            nil
        )
    }

    private func registerClass(_ windowClass: RgString) -> Bool {
        var wcex = WNDCLASSEXW()
        wcex.cbSize = UINT(MemoryLayout<WNDCLASSEX>.size);
        wcex.style = UINT(CS_HREDRAW | CS_VREDRAW);
        wcex.lpfnWndProc = windowProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.lpszClassName = windowClass.ptr;
        wcex.hbrBackground = unsafeBitCast(GetStockObject(WHITE_BRUSH), to: HBRUSH.self)

        return RegisterClassExW(&wcex) != 0
    }
}

private func windowProc(hWnd: HWND?, msg: UINT, wParam: WPARAM, lParam: LPARAM) -> LRESULT {
    switch msg {
    case UINT(WM_DESTROY):
        PostQuitMessage(0)
    // case UINT(WM_PAINT):
        // TODO: 描画コードをここに記述
    default:
        return DefWindowProcW(hWnd, msg, wParam, lParam)
    }
    return 0
}
