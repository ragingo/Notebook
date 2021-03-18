import WinSDK

private let hInstance = GetModuleHandleW(nil)

private let WINDOW_TITLE = "Swift で Win32 API を使ってみた"
private let WINDOW_CLASS_NAME = "SwiftAppSampleForWindowsDesktop"

struct RgApp {
    static func run() {
        let window = RgWindow()
        window.create(hInstance, windowClass: WINDOW_CLASS_NAME, windowTitle: WINDOW_TITLE)

        guard let hWnd = window.handle else {
            return
        }

        ShowWindow(hWnd, SW_SHOW);
        UpdateWindow(hWnd)

        var msg = MSG()
        while msg.message != WM_QUIT {
            if PeekMessageW(&msg, nil, 0, 0, UINT(PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }
        }
    }
}
