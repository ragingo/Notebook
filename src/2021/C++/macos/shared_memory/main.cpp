#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

static int execApp(std::string name, std::vector<std::string> args) {
    std::vector<const char*> argv;
    argv.push_back(name.c_str());
    for (auto arg : args) {
        argv.push_back(arg.c_str());
    }
    argv.push_back(nullptr);
    argv.shrink_to_fit();
    return execvp(name.c_str(), (char**)argv.data());
}

static bool createProcess(std::string name, std::vector<std::string> args) {
    pid_t processID = fork();

    if (processID < 0) {
        return false;
    }
    if (processID == 0) {
        execApp(name, args);
        _exit(EXIT_SUCCESS);
    }

    return true;
}

int main() {
    const auto appName = "ping";
    const std::vector<std::string> args = { "127.0.0.1" };

    if (!createProcess(appName, args)) {
        std::cout << "fork failed." << std::endl;
        return EXIT_FAILURE;
    }

    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return EXIT_SUCCESS;
}
