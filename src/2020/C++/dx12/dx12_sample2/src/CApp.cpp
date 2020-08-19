#include "CApp.h"
#include "gfx/CGfx.h"

CGfx g_Gfx;

int CApp::Run(HINSTANCE hInstance, int nCmdShow)
{
    if (!Initialize(hInstance)) {
        return FALSE;
    }

    HWND hWnd = CreateWindowW(WINDOW_CLASS_NAME, L"", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
    if (!hWnd) {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    RECT rect = {};
    if (!GetWindowRect(hWnd, &rect)) {
        return FALSE;
    }

    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    if (!g_Gfx.Initialize(hWnd, width, height)) {
        return FALSE;
    }


    MSG msg = {};

    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return static_cast<int>(msg.wParam);
}

bool CApp::Initialize(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = &CApp::WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = WINDOW_CLASS_NAME;

    if (RegisterClassExW(&wcex) == 0) {
        return false;
    }

    return true;
}

LRESULT CALLBACK CApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
#pragma warning(push)
#pragma warning(disable : 4065)
        switch (wmId) {
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
#pragma warning(pop)
    } break;

    case WM_PAINT:
        g_Gfx.Update();
        g_Gfx.Render();
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}
