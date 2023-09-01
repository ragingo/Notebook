import SwiftUI
import PlaygroundSupport

struct ContentView: View {
    private let timer = Timer.publish(every: 1, on: .main, in: .common).autoconnect()
    @State private var isNormalAgumon = false

    var body: some View {
        grid(dots: isNormalAgumon ? agumonNormal : agumonAngry)
            .onReceive(timer) { _ in
                withAnimation(.easeInOut(duration: 1)) {
                    isNormalAgumon.toggle()
                }
            }
    }

    @ViewBuilder
    private func grid(dots: String) -> some View {
        let rows = dots.split(separator: "\n").map { String($0) }
        VStack(spacing: 0) {
            ForEach(rows.indices, id: \.self) { index in
                HStack(spacing: 0) {
                    let dots = Array(String(rows[index]).map { $0 })
                    ForEach(dots.indices, id: \.self) { index in
                        drawDot(String(dots[index]))
                    }
                }
            }
        }
    }

    @ViewBuilder
    private func drawDot(_ dot: String) -> some View {
        if dot == "⬜" {
            Color.white.frame(width: 10, height: 10)
        } else if dot == "⬛" {
            Color.black.frame(width: 10, height: 10)
        }
    }
}

let agumonNormal = """
    ⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜
    ⬜⬜⬜⬜⬜⬜⬛⬛⬛⬛⬛⬛⬜⬜⬜⬜
    ⬜⬜⬜⬜⬜⬛⬜⬜⬜⬜⬜⬜⬛⬜⬜⬜
    ⬜⬜⬛⬛⬛⬜⬜⬜⬜⬛⬛⬜⬜⬛⬜⬜
    ⬜⬛⬜⬜⬜⬜⬜⬜⬜⬜⬛⬛⬜⬛⬜⬜
    ⬜⬛⬜⬜⬜⬜⬜⬜⬜⬛⬛⬛⬜⬛⬜⬜
    ⬜⬛⬜⬜⬜⬜⬛⬛⬜⬜⬜⬜⬜⬛⬜⬜
    ⬜⬜⬛⬛⬛⬛⬜⬜⬜⬜⬜⬜⬜⬛⬜⬜
    ⬜⬜⬛⬜⬜⬜⬜⬜⬛⬛⬜⬜⬛⬜⬜⬜
    ⬜⬜⬜⬛⬛⬛⬛⬛⬜⬛⬛⬜⬛⬜⬜⬜
    ⬜⬜⬛⬜⬜⬛⬜⬜⬛⬜⬜⬜⬜⬛⬜⬜
    ⬜⬜⬛⬛⬛⬛⬜⬜⬛⬛⬛⬛⬜⬛⬜⬜
    ⬜⬜⬜⬜⬜⬛⬛⬜⬜⬜⬜⬛⬜⬜⬛⬜
    ⬜⬜⬜⬛⬛⬜⬜⬛⬛⬛⬛⬜⬜⬜⬛⬜
    ⬜⬜⬛⬜⬛⬜⬜⬛⬜⬛⬜⬛⬜⬛⬜⬛
    ⬜⬜⬛⬛⬛⬛⬛⬛⬜⬛⬛⬛⬛⬛⬛⬛
    """

let agumonAngry = """
    ⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜
    ⬜⬜⬜⬜⬜⬜⬛⬛⬛⬛⬛⬛⬜⬜⬜⬜
    ⬜⬜⬜⬜⬜⬛⬜⬜⬜⬜⬜⬜⬛⬜⬜⬜
    ⬜⬜⬛⬛⬛⬜⬜⬜⬜⬛⬛⬜⬜⬛⬜⬜
    ⬜⬛⬜⬜⬜⬜⬜⬜⬛⬛⬜⬜⬜⬛⬜⬜
    ⬜⬛⬛⬛⬛⬛⬜⬜⬜⬜⬜⬜⬜⬛⬜⬜
    ⬜⬜⬜⬜⬜⬜⬛⬜⬜⬜⬜⬜⬜⬛⬜⬜
    ⬜⬜⬛⬛⬛⬛⬜⬜⬜⬜⬜⬜⬜⬛⬜⬜
    ⬜⬜⬛⬜⬜⬜⬜⬛⬛⬜⬜⬛⬜⬜⬜⬜
    ⬜⬜⬜⬛⬛⬛⬛⬛⬜⬛⬛⬜⬛⬜⬜⬜
    ⬜⬜⬛⬜⬜⬛⬜⬜⬛⬜⬜⬜⬜⬛⬜⬜
    ⬜⬜⬛⬛⬛⬛⬜⬜⬛⬛⬛⬛⬜⬛⬜⬜
    ⬜⬜⬜⬜⬜⬛⬛⬜⬜⬜⬜⬛⬜⬜⬛⬜
    ⬜⬜⬜⬛⬛⬜⬜⬛⬛⬛⬛⬜⬜⬜⬛⬜
    ⬜⬜⬛⬜⬛⬜⬜⬛⬜⬛⬜⬛⬜⬛⬜⬛
    ⬜⬜⬛⬛⬛⬛⬛⬛⬜⬛⬛⬛⬛⬛⬛⬛
    """

PlaygroundPage.current.liveView = UIHostingController(rootView: ContentView())
