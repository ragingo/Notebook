import WinSDK

private let hInstance = GetModuleHandleW(nil)

struct App {
    static func run() {
        let window = MainWindow()
        window.create(hInstance)

        var msg = MSG()
        while msg.message != WM_QUIT {
            if PeekMessageW(&msg, nil, 0, 0, UINT(PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }
        }
    }
}
