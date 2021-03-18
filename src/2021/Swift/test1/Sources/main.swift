import WinSDK

let hInstance = GetModuleHandleW(nil)

struct App {
	static func run() {
		guard let hWnd = createMainWindow(title: "やほーーーー") else {
			return
		}
		ShowWindow(hWnd, SW_SHOW);

		var msg = MSG()
		while msg.message != WM_QUIT {
			if PeekMessageW(&msg, nil, 0, 0, UINT(PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
		}
	}

	private static func createMainWindow(title: String) -> HWND? {
		func windowProc(hWnd: HWND?, msg: UINT, wParam: WPARAM, lParam: LPARAM) -> LRESULT {
			switch msg {
			case UINT(WM_DESTROY):
				PostQuitMessage(0)
				return 0
			default:
				return DefWindowProcW(hWnd, msg, wParam, lParam)
			}
		}

		let className = RgString("SwiftAppSampleForWindowsDesktop")

		var windowClass = WNDCLASSW()
		windowClass.hInstance = hInstance
		windowClass.lpfnWndProc = windowProc
		windowClass.lpszClassName = className.ptr
		RegisterClassW(&windowClass)

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

App.run()
