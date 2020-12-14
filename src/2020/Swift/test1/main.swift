import Foundation

func test_http_get() {
    // niconico search api
    // https://site.nicovideo.jp/search-api-docs/search.html

    let searchApiEndpoint = "https://api.search.nicovideo.jp/api/v2/:service/contents/search"
    let videoSearchApiEndpoint = searchApiEndpoint.replacingOccurrences(of: ":service", with: "video")

    struct VideoItem: Decodable {
        var contentId: String
    }

    struct ApiMeta: Decodable {
        var status: Int
    }

    struct ApiResponse: Decodable {
        var meta: ApiMeta
        var data: [VideoItem]
    }

    HttpClient.shared.getDecodedObject(
        videoSearchApiEndpoint,
        params: [
            "q": "game",
            "targets": "title",
            "fields": "contentId",
            "_sort": "-viewCounter",
            "_context": "swifttest",
        ]
    ) { (obj: ApiResponse?) in
        obj?.data.forEach {
            print($0.contentId);
        }
        exit(EXIT_SUCCESS)
    }

    dispatchMain()
}

func test_cpp() {
    print(ret1000())
    print(add(1, 2))
}

func main() {
    // test_http_get()
    test_cpp()
}

main()
