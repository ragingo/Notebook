import Foundation
import FoundationNetworking

private func createUrl(_ urlString: String, params: [String: Any]) -> URL? {
    guard var urlComponents = URLComponents(string: urlString) else { return nil }
    urlComponents.queryItems = params.map {
        URLQueryItem(name: $0.key, value: String(describing: $0.value))
    }
    return urlComponents.url
}

extension URLRequest {
    mutating func addHeaders(_ headers: [String: String]) {
        headers.forEach {
            addValue($0.value, forHTTPHeaderField: $0.key)
        }
    }

    mutating func setHeaders(_ headers: [String: String]) {
        headers.forEach {
            setValue($0.value, forHTTPHeaderField: $0.key)
        }
    }
}

enum HttpMethod: String {
    case get = "GET"
    case post = "POST"
    case put = "PUT"
    case delete = "DELETE"
}

final class HttpClient {
    typealias Headers = [String: String]
    typealias QueryParams = [String: Any]
    typealias CompletionHandler = (Data?, URLResponse?, Error?) -> Void

    static let shared = { HttpClient() }()

    private static let defaultHeaders: Headers = [
        "UserAgent": "SwiftHttpClient/0.0.1"
    ]

    private init() {
    }

    func send(
        _ urlString: String,
        method: HttpMethod = .get,
        headers: Headers = [:],
        params: QueryParams = [:],
        completionHandler: @escaping (Data?, URLResponse?, Error?) -> Void
    ) {
        guard let url = createUrl(urlString, params: params) else { return }
        var req = URLRequest(url: url)
        req.httpMethod = method.rawValue
        req.setHeaders(HttpClient.defaultHeaders)
        req.setHeaders(headers)

        let task = URLSession.shared.dataTask(with: req) { data, res, err in
            completionHandler(data, res, err)
        }
        task.resume()
    }

    func getDecodedObject<T: Decodable>(
        _ urlString: String,
        headers: Headers = [:],
        params: QueryParams = [:],
        completionHandler: @escaping (T?) -> Void
    ) {
        send(
            urlString,
            method: .get,
            headers: headers,
            params: params
        ) { data, res, err in
            if err != nil {
                print(err as Any)
                return
            }

            guard let data = data else { return }
            if res == nil { return }

            do {
                let decoder = JSONDecoder()
                let obj = try decoder.decode(T.self, from: data)
                completionHandler(obj)
            } catch (let e) {
                print(e)
            }
        }
    }
}
