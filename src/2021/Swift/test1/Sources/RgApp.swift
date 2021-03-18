import WinSDK

private let hInstance = GetModuleHandleW(nil)

private let WINDOW_TITLE = "Swift で Win32 API を使ってみた"
private let WINDOW_CLASS_NAME = "SwiftAppSampleForWindowsDesktop"

struct RgApp {
    static func run() {
        guard let hWnd = createMainWindow(title: WINDOW_TITLE) else {
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

    private static func createMainWindow(title: String) -> HWND? {
        let className = RgString(WINDOW_CLASS_NAME)

        var wcex = WNDCLASSEXW()
        wcex.cbSize = UINT(MemoryLayout<WNDCLASSEX>.size);
        wcex.style = UINT(CS_HREDRAW | CS_VREDRAW);
        wcex.lpfnWndProc = windowProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.lpszClassName = className.ptr;
        wcex.hbrBackground = unsafeBitCast(GetStockObject(WHITE_BRUSH), to: HBRUSH.self)
        if RegisterClassExW(&wcex) == 0 {
            return nil
        }

        let windowTitle = RgString(title)

        let hWnd = CreateWindowExW(
            0,
            className.ptr,
            windowTitle.ptr,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            nil,
            nil,
            hInstance,
            nil
        )

        return hWnd
    }
}

// TODO: Window クラスを作ってそっちに書く
func windowProc(hWnd: HWND?, msg: UINT, wParam: WPARAM, lParam: LPARAM) -> LRESULT {
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
