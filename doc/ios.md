# iOS アプリ開発で便利なコマンドのまとめ

## ビルド

### シミュレーター用

#### xcodebuild

ℹ️　`-destination` の `OS=x.x` を省略すると、最新版が選択される  
ℹ️　`-derivedDataPath` で指定したディレクトリにビルド成果物を出力する  
ℹ️　build/Build/Products/`Config1`-iphonesimulator/`Sample`.app を、Finder から シミュレーターへ DnD すればインストールできる  

```sh
xcodebuild \
  -workspace "Sample.xcworkspace" \
  -scheme "Scheme1" \
  -configuration Config1 \
  -destination 'platform=iOS Simulator,name=iPhone X' \
  -derivedDataPath build
```

## デバイス一覧

### xctrace list devices

⚠　標準エラーを標準出力にリダイレクトしないと grep が効かないから要注意

```sh
# iPhone 14.x
xcrun xctrace list devices 2>&1 | grep -E "^iPhone .+ \(14.\d\)"
```

### simctl list devices

#### iOS 13.x or iOS 14.x

```sh
# input
xcrun simctl list devices -j | jq '.devices | to_entries | map(select(.key | test("iOS-(13|14)-\\d+"))) | map({ key: .key })'

# output
[
  {
    "key": "com.apple.CoreSimulator.SimRuntime.iOS-13-7"
  },
  {
    "key": "com.apple.CoreSimulator.SimRuntime.iOS-14-1"
  }
]
```

#### iPad Pro and (iOS 13.x or iOS 14.x) and isAvailable=true

```sh
# input
xcrun simctl list devices -j | \
  jq '.devices | to_entries | map(select(.key | test("iOS-(13|14)-\\d+"))) | .[] | .value[] | select(.isAvailable and (.name | startswith("iPad Pro")))'

# output
{
  "dataPath": "xxxxxxxxxx",
  "logPath": "xxxxxxxxxx",
  "udid": "xxxxxxxxxx",
  "isAvailable": true,
  "deviceTypeIdentifier": "com.apple.CoreSimulator.SimDeviceType.iPad-Pro-xxxxxxxxxx",
  "state": "Shutdown",
  "name": "iPad Pro xxxxxxxxxx"
}
```
