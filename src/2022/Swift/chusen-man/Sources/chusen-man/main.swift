import Foundation

enum DayOfWeek: Int {
    case sunDay = 1
    case monDay
    case tuesDay
    case wednesDay
    case thursDay
    case friDay
    case saturDay
}

struct Member {
    let name: String
    let schedule: [DayOfWeek]
}

func currentWeekDay() -> DayOfWeek? {
    guard let timeZone = TimeZone(abbreviation: "JST") else {
        return nil
    }
    let component = Calendar.current.dateComponents(in: timeZone, from: Date())
    guard let weekday = component.weekday else {
        return nil
    }
    return .init(rawValue: weekday)
}

func makeMembers() -> [Member] {
    [
        .init(name: "ichiro", schedule: [.tuesDay]),
        .init(name: "jiro", schedule: [.monDay, .tuesDay, .wednesDay, .thursDay, .friDay]),
        .init(name: "saburo", schedule: [.wednesDay, .thursDay]),
        .init(name: "shiro", schedule: [.monDay, .tuesDay, .wednesDay, .thursDay, .friDay]),
        .init(name: "goro", schedule: [.monDay, .tuesDay, .wednesDay, .thursDay, .friDay]),
        .init(name: "rokuro", schedule: [.tuesDay, .wednesDay, .thursDay]),
        .init(name: "nanaro", schedule: [.monDay, .tuesDay, .wednesDay, .thursDay, .friDay])
    ]
}

func sandstorm(body: (Int, Int, Int) -> Void) {
    (0...50).forEach { index in
        let positionX = Int.random(in: 0..<100)
        let positionY = Int.random(in: 0..<30)
        Console.moveCursor(position: .init(x: positionX, y: positionY))
        body(index, positionX, positionX)
        Thread.sleep(forTimeInterval: 0.1)
    }
}

func main() {
    guard let weekday = currentWeekDay() else {
        print("どこかバグってるみたい")
        return
    }

    Console.clear()
    Console.moveCursor(position: .zero)

    let names = makeMembers().compactMap {
        $0.schedule.contains(weekday) ? $0.name : nil
    }

    sandstorm { index, _, _ in
        Console.writeLine(names[index % names.count])
    }

    Console.clear()
    Console.moveCursor(position: .zero)
    print("\(names.shuffled().first!)さん、当選しました！")
}

main()
