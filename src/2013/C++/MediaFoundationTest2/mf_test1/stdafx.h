#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ShObjIdl.h>
#include <Shlwapi.h>

#include <mfapi.h>
#include <mfidl.h>
#include <evr.h>

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib,"Mfplat.lib")
#pragma comment(lib,"Mf.lib")
#pragma comment(lib,"Mfuuid.lib")
#pragma comment(lib,"Strmiids.lib")

#include <stdlib.h>
#include <tchar.h>
#include <memory>

#include <atlbase.h>
#include <atlcom.h>
#include <atlconv.h>
#include <atlwin.h>

#include "defs.h"
#include "AsyncCallback.h"
