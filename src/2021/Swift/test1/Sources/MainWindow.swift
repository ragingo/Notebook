import WinSDK

class MainWindow {
    private var window = RgWindow()

    var handle: HWND? { window.handle }

    func create(_ hInstance: HINSTANCE?, windowClass: String, windowTitle: String) {
        window.create(hInstance, windowClass, windowTitle) { (hWnd, msg, wParam, lParam) -> LRESULT in
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
    }
}
