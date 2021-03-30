
class RgButton: RgControl {
    override required init(hWndParent: HWND?, name: String = "", hInstance: HINSTANCE? = nil, enableSubClass: Bool = false) {
        super.init(hWndParent: hWndParent, name: "", hInstance: hInstance, enableSubClass: enableSubClass)
        handle = createButton(handle, name, hInstance)
    }
}
