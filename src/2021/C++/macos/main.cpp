#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string_view>
#include <thread>
#include <chrono>
#include <deque>
#include <mutex>
#include <memory>
#include <functional>
#include <iostream>

static std::mutex outputs_mutex;

static std::thread createProcess(std::string name, std::deque<std::string>& outputs) {
    // memo: https://www.ipa.go.jp/security/awareness/vendor/programmingv2/contents/c909.html
    std::shared_ptr<FILE> pipe(
        popen(name.c_str(), "r"),
        [](FILE* fp) {
            pclose(fp);
        }
    );

    std::thread thread([pipe, &outputs]{
        char buf[1024] = {};
        while (!feof(pipe.get())) {
            if (fgets(buf, sizeof(buf), pipe.get()) != nullptr) {
                std::lock_guard<std::mutex> lock(outputs_mutex);
                outputs.push_back(buf);
            }
        }
    });

    return thread;
}

static void printOutputs(std::deque<std::string>& outputs) {
    if (outputs.empty()) {
        return;
    }
    std::lock_guard<std::mutex> lock(outputs_mutex);
    auto output = outputs.front();
    outputs.pop_front();
    std::cout << output << std::endl;
}

int main() {
    const auto appName = "ping 127.0.0.1";
    std::deque<std::string> outputs;
    auto thread = createProcess(appName, outputs);

    while(true) {
        printOutputs(outputs);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return EXIT_SUCCESS;
}
