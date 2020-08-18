#pragma once

class CApp
{
public:
    static int Run(HINSTANCE hInstance, int nCmdShow);

private:
    static bool Initialize(HINSTANCE hInstance);

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    static constexpr const wchar_t* const WINDOW_CLASS_NAME = L"dx12 sample";
};
