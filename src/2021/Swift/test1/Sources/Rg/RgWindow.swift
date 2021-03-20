import WinSDK

class RgWindow {
    private var hInstance: HINSTANCE?
    var handle: HWND?

    init() {
    }

    func create(_ hInstance: HINSTANCE?, _ windowClass: String, _ windowTitle: String, _ windowProc: @escaping WNDPROC) {
        self.hInstance = hInstance
        let windowClass = RgString(windowClass)

        guard registerClass(windowClass, windowProc) else {
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
}
