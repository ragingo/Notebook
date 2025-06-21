// swift-tools-version: 6.2

import PackageDescription

let package = Package(
    name: "CompileTimeValuesSampleLib",
    products: [
        .library(
            name: "CompileTimeValuesSampleLib",
            targets: ["CompileTimeValuesSampleLib"]
        ),
    ],
    targets: [
        .target(
            name: "CompileTimeValuesSampleLib",
            swiftSettings: [
                .unsafeFlags([
                    "-parse-as-library",
                    "-enable-experimental-feature", "CompileTimeValues",
                    "-enable-experimental-feature", "CompileTimeValuesPreview"
                ])
            ]
        ),
        .testTarget(
            name: "CompileTimeValuesSampleLibTests",
            dependencies: ["CompileTimeValuesSampleLib"]
        ),
    ]
)
