import Foundation
import FoundationNetworking

// niconico search api
// https://site.nicovideo.jp/search-api-docs/search.html

let SEARCH_API_ENDPOINT = "https://api.search.nicovideo.jp/api/v2/:service/contents/search"
let VIDEO_SEARCH_API_ENDPOINT = SEARCH_API_ENDPOINT.replacingOccurrences(of: ":service", with: "video")
let LIVE_SEARCH_API_ENDPOINT = SEARCH_API_ENDPOINT.replacingOccurrences(of: ":service", with: "live")

print(VIDEO_SEARCH_API_ENDPOINT)
print(LIVE_SEARCH_API_ENDPOINT)

typealias HttpRequestQueryParams = Dictionary<String, String>

typealias HttpRequestCompletionHandler = (Data?, URLResponse?, Error?) -> Void

func get(string: String, params: HttpRequestQueryParams, completionHandler: @escaping HttpRequestCompletionHandler) {
    guard var components = URLComponents(string: string) else { return }
    components.queryItems = params.map {
        URLQueryItem(name: $0.key, value: $0.value)
    }

    guard let url = components.url else { return }
    print(url)

    let task = URLSession.shared.dataTask(with: url) { data, res, err in
        completionHandler(data, res, err)
    }
    task.resume()
}

func getJson(string: String, params: HttpRequestQueryParams, handler: @escaping (Any) -> Void) {
    get(
        string: string,
        params: params,
        completionHandler: { data, res, err in
            if err != nil {
                print(err)
                return
            }

            guard let data = data else { return }
            guard let _ = res else { return }

            do {
                let json = try JSONSerialization.jsonObject(with: data)
                if json != nil {
                    handler(json)
                }
            } catch (let e) {
                print(e)
            }
        }
    )
}

getJson(
    string: VIDEO_SEARCH_API_ENDPOINT,
    params: [
        "q": "game",
        "targets": "title",
        "fields": "contentId",
        "_sort": "-viewCounter",
        "_context": "swifttest",
    ]
) { json in
    print(json)
    exit(EXIT_SUCCESS)
}

dispatchMain()
