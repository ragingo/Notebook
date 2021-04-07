

typealias EventArgs = ()
typealias ClickedEventHandler = (RgControl, EventArgs) -> ()

class RgButton: RgControl {
    var onClicked: [ClickedEventHandler] = []

    override required init(parentHandle: HWND? = nil, name: String = "", className: String = WC_BUTTON) {
        super.init(parentHandle: parentHandle, name: "", className: className)
    }

    override func windowProc(_ hWnd: HWND?, _ windowMessage: inout RgWindowMessage) {
        switch windowMessage.uMsg {
        case UINT(WM_COMMAND):
            let targetId = rg_LOWORD(Int64(windowMessage.wParam))
            print(currentId, targetId)
            if currentId != targetId {
                return
            }
            onClicked.forEach { handler in
                handler(self, EventArgs())
            }
            windowMessage.handled = true
            return
        default:
            break
        }

        super.windowProc(hWnd, &windowMessage)
    }

}
