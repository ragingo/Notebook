#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string_view>
#include <thread>
#include <chrono>
#include <memory>
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <vector>

struct StartupInfo {
    int32_t processID;
    uint64_t threadID;
    int32_t readPipe;
    int32_t writePipe;
};

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

static uint64_t getCurrentThreadID() {
    uint64_t threadID;
    pthread_threadid_np(nullptr, &threadID);
    return threadID;
}

static bool createProcess(std::string name, std::vector<std::string> args, StartupInfo& si) {
    int fd[2] = { -1 };
    if (pipe(fd) < 0) {
        return false;
    }

    pid_t processID = fork();
    printf("pid: %d\n", processID);

    if (processID < 0) {
        return false;
    }
    if (processID == 0) {
        // memo: 子プロセスで実行されている
        ::close(fd[0]);

        ::dup2(fd[1], STDOUT_FILENO);
        ::close(fd[1]);

        execApp(name, args);
        _exit(EXIT_SUCCESS);
    }

    ::close(fd[1]);

    si.processID = processID;
    si.threadID = getCurrentThreadID();
    si.readPipe = fd[0];
    // si.writePipe = ;
    return true;
}

int main() {
    const auto appName = "ping";
    const std::vector<std::string> args = { "127.0.0.1" };
    StartupInfo si = {};

    if (!createProcess(appName, args, si)) {
        std::cout << "fork failed." << std::endl;
        return EXIT_FAILURE;
    }

    while(true) {
        char buf[256] = {};
        int len = read(si.readPipe, buf, sizeof(buf));
        if (len < 0) {
            printf("read error.\n");
        } else if (len == 0) {
            printf("eof.\n");
        } else {
            printf("%s\n", buf);
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return EXIT_SUCCESS;
}
