#include <cstdio>
#include <Windows.h>

using namespace std;

int main()
{
    HANDLE readPipe = INVALID_HANDLE_VALUE;
    HANDLE writePipe = INVALID_HANDLE_VALUE;

    SECURITY_ATTRIBUTES sa = {};
    sa.bInheritHandle = TRUE;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);

    BOOL ret = CreatePipe(&readPipe, &writePipe, &sa, 0);

    if (!ret) {
        printf("create pipe error: %ld\n", GetLastError());
        return -1;
    }

    wchar_t args[] = L"tshark -i 5";

    STARTUPINFO si = {};
    si.cb = sizeof(STARTUPINFO);
    si.wShowWindow = SW_HIDE;
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdOutput = writePipe;
    si.hStdError = writePipe;

    PROCESS_INFORMATION pi = {};

    ret = CreateProcess(
        nullptr,
        args,
        nullptr,
        nullptr,
        TRUE,
        CREATE_NO_WINDOW,
        nullptr,
        nullptr,
        &si,
        &pi
    );

    if (!ret) {
        printf("create process error: %ld\n", GetLastError());
        return -1;
    }

    printf("pid: %d\n", pi.dwProcessId);

    SetConsoleOutputCP(CP_UTF8);

    char buf[1024] = { 0 };
    DWORD len = 0;

    while (true) {
        Sleep(500);
        ZeroMemory(buf, sizeof(buf));

        ret = ReadFile(readPipe, buf, sizeof(buf), &len, nullptr);
        if (!ret) {
            break;
        }
        if (len == 0) {
            continue;
        }
        printf("%s\n", buf);
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(readPipe);
    CloseHandle(writePipe);

    return 0;
}
