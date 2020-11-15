import Foundation

func test_http_get() {
    getJson2(
        string: VIDEO_SEARCH_API_ENDPOINT,
        params: [
            "q": "game",
            "targets": "title",
            "fields": "contentId",
            "_sort": "-viewCounter",
            "_context": "swifttest",
        ]
    ) { (json: ApiResponse) in
        json.data.forEach {
            print($0.contentId);
        }
        exit(EXIT_SUCCESS)
    }
}

func main() {
    test_http_get()
    dispatchMain()
}

main()
