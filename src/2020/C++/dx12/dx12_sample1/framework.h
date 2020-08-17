#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


#include <cinttypes>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include <wrl.h>
#include <dxgi1_6.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <DirectXTex.h>
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")

#include "lib/d3dx12.h"

#ifdef _DEBUG

#include <d3d12sdklayers.h>

#endif // _DEBUG
