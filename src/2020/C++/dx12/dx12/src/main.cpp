#include "pch.h"
#include "framework.h"
#include "Resource.h"
#include "CDxApp.h"

ATOM MyRegisterClass(HINSTANCE hInstance, LPCWSTR);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

CDxApp g_DxApp;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int nCmdShow)
{
    WCHAR title[100] = {};
    WCHAR windowClassName[100] = {};

    LoadStringW(hInstance, IDS_APP_TITLE, title, _countof(title));
    LoadStringW(hInstance, IDC_DX12, windowClassName, _countof(windowClassName));
    MyRegisterClass(hInstance, windowClassName);

    HWND hWnd = CreateWindowW(windowClassName, title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
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

    g_DxApp.Initialize(hWnd, width, height);

    MSG msg = {};

    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    g_DxApp.Destroy();

    return static_cast<int>(msg.wParam);
}

ATOM MyRegisterClass(HINSTANCE hInstance, LPCWSTR windowClassName)
{
    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DX12));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_DX12);
    wcex.lpszClassName = windowClassName;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_COMMAND: {
        int wmId = LOWORD(wParam);
#pragma warning(push)
#pragma warning(disable : 4065)
        switch (wmId) {
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
#pragma warning(pop)
    } break;
    case WM_PAINT: {
        g_DxApp.Render();
    } break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
