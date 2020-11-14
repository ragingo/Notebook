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
                print(err as Any)
                return
            }

            guard let data = data else { return }
            guard let _ = res else { return }

            do {
                let json = try JSONSerialization.jsonObject(with: data)
                handler(json)
            } catch (let e) {
                print(e)
            }
        }
    )
}

func getJson2<T: Codable>(string: String, params: HttpRequestQueryParams, handler: @escaping (T) -> Void) {
    get(
        string: string,
        params: params,
        completionHandler: { data, res, err in
            if err != nil {
                print(err as Any)
                return
            }

            guard let data = data else { return }
            guard let _ = res else { return }

            do {
                let json = try JSONDecoder().decode(T.self, from: data)
                handler(json)
            } catch (let e) {
                print(e)
            }
        }
    )
}

struct VideoItem: Codable {
    var contentId: String
}
struct ApiMeta: Codable {
    var status: Int
}
struct ApiResponse: Codable {
    var meta: ApiMeta
    var data: [VideoItem]
}

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

dispatchMain()
