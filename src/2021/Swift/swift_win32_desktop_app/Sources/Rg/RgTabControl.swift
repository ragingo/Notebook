
class RgTabControl: RgControl {
    private var tabCount: UInt32 = 0
    private var buttonArea: RgTabButtonArea? = nil
    private var pages: [RgTabPage] = []

    required init(hWndParent: HWND?, name: String = String(describing: RgTabControl.self), hInstance: HINSTANCE? = nil, enableSubClass: Bool = true) {
        super.init(hWndParent: hWndParent, name: name, hInstance: hInstance, enableSubClass: true)

        buttonArea = RgTabButtonArea(hWndParent: handle, hInstance: hInstance)
        addChild(child: buttonArea!)
    }

    func addPage(text: String) -> RgTabPage {
        tabCount += 1

        addTabItem(buttonArea?.handle, tabCount - 1, text)

        let page = RgTabPage(hWndParent: handle)
        addChild(child: page)
        pages.append(page)

        return page
    }

    func getPage(index: Int) -> RgTabPage? {
        pages[index]
    }

    override func adjustSize(_ rect: inout RECT) {
        super.adjustSize(&rect)

        let width = rect.right - rect.left
        let height = rect.bottom - rect.top

        buttonArea?.adjustSize(&rect)

        for page in pages {
            page.adjustSize(&rect)
        }
    }

    var selectedTabIndex: Int32 {
        return rg_TabCtrl_GetCurSel(buttonArea?.handle)
    }

    override func onPaint(_ windowMessage: inout RgWindowMessage) {
        for i in 0 ..< pages.count {
            let page = pages[i]
            if i != selectedTabIndex {
                page.hide()
                continue
            }
            page.show()
        }
    }

    override func onSize(_ windowMessage: inout RgWindowMessage) {
        var rect = RECT()
        GetClientRect(parentHandle, &rect)
        adjustSize(&rect)
    }

    override func onNotify(_ windowMessage: inout RgWindowMessage) {
        let msg = unsafeBitCast(windowMessage.lParam, to: LPNMHDR.self)
        if msg.pointee.code == TCN_SELCHANGE {
            InvalidateRect(handle, nil, true)
        }
    }
}

class RgTabButtonArea: RgControl {
    static let defaultHeight: Int32 = 30
    required init(hWndParent: HWND?, name: String = String(describing: RgTabButtonArea.self), hInstance: HINSTANCE? = nil, enableSubClass: Bool = false) {
        super.init(hWndParent: hWndParent, name: name, hInstance: hInstance, enableSubClass: false)
        handle = createTabControl(handle, name, hInstance)
    }

    override func resize(width: Int32, height: Int32 = defaultHeight) {
        super.resize(width: width, height: height)
    }

    override func adjustSize(_ rect: inout RECT) {
        position(x: 0, y: 0)
        resize(width: rect.right - rect.left)
    }
}

class RgTabPage: RgControl {
    required init(hWndParent: HWND?, name: String = String(describing: RgTabPage.self), hInstance: HINSTANCE? = nil, enableSubClass: Bool = true) {
        super.init(hWndParent: hWndParent, name: name, hInstance: hInstance, enableSubClass: enableSubClass)
    }

    override func position(x: Int32 = 0, y: Int32 = RgTabButtonArea.defaultHeight) {
        super.position(x: x, y: y)
    }

    override func adjustSize(_ rect: inout RECT) {
        position()
        resize(width: rect.right - rect.left, height: rect.bottom - rect.top)
    }
}
