import WinSDK

private let WINDOW_TITLE = "Swift で Win32 API を使ってみた"
private let WINDOW_CLASS_NAME = "SwiftAppSampleForWindowsDesktop"

private let TAB_BUTTON_HEIGHT: Int32 = 30

final class MainWindow: RgWindow {
    // private var tabControl: RgTabControl? = nil

    func create() {
        self.create(WINDOW_CLASS_NAME, WINDOW_TITLE)
    }

    private var hBmp1: HBITMAP?
    private var imageInfos: [ImageInfo] = []

    override func onCreate(_ hWnd: HWND?) {
        super.onCreate(hWnd)

        loadCommonControls()

        let button = RgButton()
        addChild(child: button)
        button.moveTo(x: 100, y: 100)
        button.resize(width: 300, height: 300)
        button.text = "ボタン1"
        button.onClicked += [
            { [hWnd] (_, _) in
                print("aaaa")
                let text = RgString("hello!")
                MessageBoxW(hWnd, nil, text.ptr, UINT(MB_OK))
            }
        ]

        // tabControl = RgTabControl(hWndParent: hWnd, name: "tab")
        // let page1 = tabControl!.addPage(text: "a")
        // let button1 = page1.addChild(type: RgButton.self)
        // button1.resize(width: 100, height: 30)
        // let page2 = tabControl!.addPage(text: "b")
        // let page3 = tabControl!.addPage(text: "c")

        // if let lv = createListView(hWnd, "VideoListView") {
        //     addListViewColumn(lv, 0, "col1")
        //     addListViewColumn(lv, 1, "col2")
        //     addListViewColumn(lv, 2, "col3")
        //     addListViewItem(lv, 0, "item1")
        //     addListViewItem(lv, 1, "item2")
        //     addListViewItem(lv, 2, "item3")
        //     var rect = RECT()
        //     GetClientRect(hWnd, &rect)
        //     SetWindowPos(lv, nil, 0, TAB_BUTTON_HEIGHT, rect.right - rect.left, (rect.bottom - rect.top) - TAB_BUTTON_HEIGHT, UINT(SWP_NOMOVE))
        // }
    }

    override func onShown() {
        onTabItem1Create()
        onTabItem2Create()
    }

    private func onTabItem1Create() {
        let path = RgString("./Resources/catman.bmp")
        let ptr = LoadImageW(nil, path.ptr, UINT(IMAGE_BITMAP), 0, 0, UINT(LR_LOADFROMFILE));
        self.hBmp1 = unsafeBitCast(ptr, to: HBITMAP.self)
    }

    private func onTabItem2Create() {
        // let params: [String: Any] = [
        //     "q": "game",
        //     "targets": "title",
        //     "fields": "contentId,thumbnailUrl",
        //     "_sort": "-viewCounter",
        //     "_context": "swifttest",
        // ]

        // HttpClient.shared.getDecodedObject(videoSearchApiEndpoint, params: params) { (res: ApiResponse?) in
        //     guard let res = res else {
        //         return
        //     }
        //     res.data.forEach {
        //         print($0);
        //         HttpClient.shared.send($0.thumbnailUrl) { [unowned self] data, res, _ in
        //             print("thumbnail downloaded")
        //             guard let data = data else { return }
        //             decodeJpegFromMemory(data) { info in
        //                 self.imageInfos.append(info)
        //             }
        //         }
        //     }
        // }
    }

    override func onPaint(_ windowMessage: inout RgWindowMessage) {
        super.onPaint(&windowMessage)

        var ps = PAINTSTRUCT()
        guard let hDC = BeginPaint(hWnd, &ps) else {
            return
        }
        defer {
            EndPaint(hWnd, &ps)
        }

        // TODO: ここに描画コードを書く
    }

    override func onSize(_ windowMessage: inout RgWindowMessage) {
        // 消したい
        // var rect = RECT()
        // GetClientRect(hWnd, &rect)
        // tabControl?.adjustSize(&rect)
    }

    private func onTabItem1Paint(_ hTab: HWND, _ hDC: HDC, _ rect: RECT) {
        if let bmp = hBmp1 {
            let hMemDC = CreateCompatibleDC(hDC)
            SelectObject(hMemDC, bmp)
            BitBlt(hDC, 0, TAB_BUTTON_HEIGHT, 32, 32, hMemDC, 0, 0, SRCCOPY)
            DeleteDC(hMemDC)
        }
    }

    private func onTabItem2Paint(_ hTab: HWND, _ hDC: HDC, _ rect: RECT) {
        var y = TAB_BUTTON_HEIGHT
        for info in imageInfos {
            var bi = BITMAPINFO()
            createBitmapInfo(info, &bi)
            StretchDIBits(
                hDC,
                0, y, info.width, info.height,
                0, 0, info.width, info.height,
                info.image,
                &bi,
                UINT(DIB_RGB_COLORS),
                SRCCOPY
            )
            y += info.height
        }
    }

    private func onTabItem3Paint(_ hTab: HWND, _ hDC: HDC, _ rect: RECT) {
        Rectangle(hDC, 10, rect.bottom - rect.top, 70, rect.bottom - rect.top + 70)
    }

    override func onDestroy() {
        if let bmp = hBmp1 {
            DeleteObject(bmp)
        }
        for info in imageInfos {
            info.image.deallocate()
        }
        super.onDestroy()
    }
}

private func createBitmapInfo(_ imageInfo: ImageInfo, _ bitmapInfo: inout BITMAPINFO) {
    bitmapInfo.bmiHeader.biSize = DWORD(MemoryLayout<BITMAPINFOHEADER>.size)
    bitmapInfo.bmiHeader.biWidth = imageInfo.width
    bitmapInfo.bmiHeader.biHeight = -imageInfo.height
    bitmapInfo.bmiHeader.biPlanes = 1
    bitmapInfo.bmiHeader.biBitCount = WORD(imageInfo.bitCount)
    bitmapInfo.bmiHeader.biCompression = DWORD(BI_RGB)
    bitmapInfo.bmiHeader.biSizeImage = 0
    bitmapInfo.bmiHeader.biXPelsPerMeter = 0
    bitmapInfo.bmiHeader.biYPelsPerMeter = 0
    bitmapInfo.bmiHeader.biClrUsed = 0
    bitmapInfo.bmiHeader.biClrImportant = 0
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
