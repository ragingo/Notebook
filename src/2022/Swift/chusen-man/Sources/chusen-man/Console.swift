import Foundation

struct Console {
    private enum Command {
        case save
        case restore
        case clear
        case move(position: Position)

        var escapeSequence: String {
            switch self {
            case .save:
                return "\u{1B}7"
            case .restore:
                return "\u{1B}8"
            case .clear:
                return "\u{1B}[2J"
            case .move(let position):
                return "\u{1B}[\(position.y);\(position.x)H"
            }
        }
    }

    static func clear() {
        execute(command: .clear)
    }

    static func moveCursor(position: Position) {
        execute(command: .move(position: position))
    }

    static func write(_ items: Any..., separator: String = " ", terminator: String = "") {
        let value = items.map { "\($0)" }.joined(separator: separator)
        print(value, terminator: terminator)
    }

    // https://bugs.swift.org/browse/SR-128
    static func writeLine(_ items: Any..., separator: String = " ") {
        let value = items.map { "\($0)" }.joined(separator: separator)
        print(value)
    }

    private static func execute(command: Command) {
        write(command.escapeSequence)
    }
}
