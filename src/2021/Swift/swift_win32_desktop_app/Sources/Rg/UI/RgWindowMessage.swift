import WinSDK

struct RgWindowMessage {
    let uMsg: UINT
    let wParam: WPARAM
    let lParam: LPARAM
    var handled: Bool
}
