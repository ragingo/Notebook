#pragma once

#include "framework.h"

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
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")

#ifdef _DEBUG

#include <d3d12sdklayers.h>

#endif // _DEBUG
