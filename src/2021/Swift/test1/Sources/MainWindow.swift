import WinSDK

private let WINDOW_TITLE = "Swift で Win32 API を使ってみた"
private let WINDOW_CLASS_NAME = "SwiftAppSampleForWindowsDesktop"

private let TAB_BUTTON_HIGHT: Int32 = 30

final class MainWindow: RgWindow {
    func create(_ hInstance: HINSTANCE?) {
        self.create(hInstance, WINDOW_CLASS_NAME, WINDOW_TITLE)
    }

    private var hBmp1: HBITMAP?
    private var imageInfo: ImageInfo?

    override func onCreate(_ hWnd: HWND?, _ hInstance: HINSTANCE) {
        super.onCreate(hWnd, hInstance)

        loadCommonControls()

        guard let hWnd = hWnd, let hTab = createTabControl(hWnd, hInstance, "tab") else {
            return
        }

        addTabItem(hTab, 0, "タブ1")
        addTabItem(hTab, 1, "タブ2")
        addTabItem(hTab, 2, "タブ3")

        onTabItem1Create()
        onTabItem2Create()
    }

    private func onTabItem1Create() {
        let path = RgString("./Resources/catman.bmp")
        let ptr = LoadImageW(nil, path.ptr, UINT(IMAGE_BITMAP), 0, 0, UINT(LR_LOADFROMFILE));
        self.hBmp1 = unsafeBitCast(ptr, to: HBITMAP.self)
    }

    private func onTabItem2Create() {
        let params: [String: Any] = [
            "q": "game",
            "targets": "title",
            "fields": "contentId,thumbnailUrl",
            "_sort": "-viewCounter",
            "_context": "swifttest",
        ]

        HttpClient.shared.getDecodedObject(videoSearchApiEndpoint, params: params) { (res: ApiResponse?) in
            guard let res = res else {
                return
            }
            res.data.forEach {
                print($0);
                // 今はとりあえず1枚だけ処理する
                if $0.contentId != "sm23867118" {
                    return
                }
                HttpClient.shared.send($0.thumbnailUrl) { [unowned self] data, res, _ in
                    print("thumbnail downloaded")
                    guard let data = data else { return }
                    decodeJpegFromMemory(data) { info in
                        self.imageInfo = info
                    }
                }
            }
        }
    }

    private static func createBitmapInfo(_ imageInfo: ImageInfo, _ bitmapInfo: inout BITMAPINFO) {
        bitmapInfo.bmiHeader.biSize = DWORD(MemoryLayout<BITMAPINFOHEADER>.size)
        bitmapInfo.bmiHeader.biWidth = imageInfo.width
        bitmapInfo.bmiHeader.biHeight = imageInfo.height
        bitmapInfo.bmiHeader.biPlanes = 1
        bitmapInfo.bmiHeader.biBitCount = WORD(imageInfo.bitCount)
        bitmapInfo.bmiHeader.biCompression = DWORD(BI_RGB)
        bitmapInfo.bmiHeader.biSizeImage = DWORD(imageInfo.width * imageInfo.height * (imageInfo.bitCount / 8))
        bitmapInfo.bmiHeader.biXPelsPerMeter = 0
        bitmapInfo.bmiHeader.biYPelsPerMeter = 0
        bitmapInfo.bmiHeader.biClrUsed = 0
        bitmapInfo.bmiHeader.biClrImportant = 0
    }

    override func onPaint(_ windowMessage: inout RgWindowMessage) {
        var ps = PAINTSTRUCT()
        guard let hDC = BeginPaint(hWnd, &ps) else {
            return
        }

        var rect = RECT()
        let tabName = RgString("tab")

        if let hTab = FindWindowExW(hWnd, nil, nil, tabName.ptr) {
            GetClientRect(hTab, &rect)
            switch rg_TabCtrl_GetCurSel(hTab) {
            case 0:
                onTabItem1Paint(hTab, hDC, rect)
            case 1:
                onTabItem2Paint(hTab, hDC, rect)
            case 2:
                onTabItem3Paint(hTab, hDC, rect)
            default:
                break
            }
        }

        EndPaint(hWnd, &ps)
    }

    private func onTabItem1Paint(_ hTab: HWND, _ hDC: HDC, _ rect: RECT) {
        if let bmp = hBmp1 {
            let hMemDC = CreateCompatibleDC(hDC)
            SelectObject(hMemDC, bmp)
            BitBlt(hDC, 0, TAB_BUTTON_HIGHT, 32, 32, hMemDC, 0, 0, SRCCOPY)
            DeleteDC(hMemDC)
        }
    }

    private func onTabItem2Paint(_ hTab: HWND, _ hDC: HDC, _ rect: RECT) {
        guard let imageInfo = imageInfo else {
            return
        }

        var bi = BITMAPINFO()
        Self.createBitmapInfo(imageInfo, &bi)
        StretchDIBits(
            hDC,
            0, TAB_BUTTON_HIGHT, imageInfo.width, imageInfo.height,
            0, 0, imageInfo.width, imageInfo.height,
            imageInfo.image,
            &bi,
            UINT(DIB_RGB_COLORS),
            SRCCOPY
        )
    }

    private func onTabItem3Paint(_ hTab: HWND, _ hDC: HDC, _ rect: RECT) {
        Rectangle(hDC, 10, rect.bottom - rect.top, 70, rect.bottom - rect.top + 70)
    }

    override func onSize(_ windowMessage: inout RgWindowMessage) {
        let tabName = RgString("tab")
        if let hTab = FindWindowExW(hWnd, nil, nil, tabName.ptr) {
            MoveWindow(hTab, 0, 0, rg_LOWORD(windowMessage.lParam), TAB_BUTTON_HIGHT, true)
        }
    }

    override func onNotify(_ windowMessage: inout RgWindowMessage) {
        let msg = unsafeBitCast(windowMessage.lParam, to: LPNMHDR.self)
        if msg.pointee.code == TCN_SELCHANGE {
            InvalidateRect(hWnd, nil, true)
        }
    }

    override func onDestroy() {
        if let bmp = hBmp1 {
            DeleteObject(bmp)
        }
        if let info = imageInfo {
            info.image.deallocate()
        }
        super.onDestroy()
    }
}


// TODO: 廃止されるらしいからこっちに移行する https://site.nicovideo.jp/search-api-docs/snapshot
let searchApiEndpoint = "https://api.search.nicovideo.jp/api/v2/:service/contents/search"
let videoSearchApiEndpoint = searchApiEndpoint.replacingOccurrences(of: ":service", with: "video")

struct VideoItem: Decodable {
    var contentId: String
    var thumbnailUrl: String
}

struct ApiMeta: Decodable {
    var status: Int
}

struct ApiResponse: Decodable {
    var meta: ApiMeta
    var data: [VideoItem]
}

