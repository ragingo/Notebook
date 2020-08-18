#include "framework.h"
#include "resource.h"
#include "src/CApp.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int nCmdShow)
{
    return CApp::Run(hInstance, nCmdShow);
}
